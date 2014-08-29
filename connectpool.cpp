/*
 * =====================================================================================
 *
 *       Filename:  connectpool.cpp
 *
 *    Description: this file is the defination of the connectpool 
 *
 *        Version:  1.0
 *        Created:  08/22/2014 02:47:57 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Yu Daoming (mn), yudaoming@360.cn
 *        Company:  Qihoo 360 LTD.
 *
 * =====================================================================================
 */

#include "connectpool.h"
using namespace mysql_interface;
ConnectPool* ConnectPool::connectPoolInstance= (ConnectPool*)0;
//testhandle needed by the mysql connection function;maxSize is the max
//connection no of the pool
ConnectPool::ConnectPool(const Testhandle& info, int maxSize):realSize(0)
{
    static CleanPool cleanpool;
    this->info = info;
    this->maxSize = maxSize;
    log.init(("./mysql_connectpool.log"));
    pthread_mutex_init(&lock,NULL);
}
//check whether the pool have enough connections!
bool ConnectPool::checkPool()
{
    pthread_mutex_lock(&lock);
    bool status;
    if(!connectorList.size()&&realSize==maxSize)
        status = false;
    else status = true;
    pthread_mutex_unlock(&lock);
    return status;
}

//close all the connection and delete the connectPool
ConnectPool::~ConnectPool()
{
    connectPoolInstance ->destoryConnectPool();
    connectPoolInstance = (ConnectPool*)0;
    pthread_mutex_destroy(&lock);
}
//put the connection back  to the  Connectpool;
void ConnectPool::freeConnection(Connector* connector)
{
    if(connector)
    {
        pthread_mutex_lock(&lock);
        connectorList.push_back(connector);
        pthread_mutex_unlock(&lock);
    }else{
        log.write_log(confmgr::LEVEL_WARNING,"__ConnectPool__:__freeConnection__:__line__:42");
        //Throw(SqlException,"__ConnectPool__:__free__Connection__:__line__48");
    }
}
//get the instansce for singleton
ConnectPool* ConnectPool::getInstance(const Testhandle& info, int maxSize)
{
    if(maxSize<0||maxSize==0)
        throw 0;
    if(maxSize>50)
        maxSize = 50;
    if(connectPoolInstance==0)
        connectPoolInstance = new ConnectPool(info,maxSize);
    connectPoolInstance->initConnectPool(maxSize);
    return connectPoolInstance;
}
//if there are no connection in the pool ,this will be called to create a new connection
Connector* ConnectPool::createConnector()
{
    Connector* connector_ptr = new Connector(info);
    try{
        connector_ptr->connect();
    }catch(...)
    {
        log.write_log(confmgr::LEVEL_ERROR,"__ConnectPool__:__createConnector__:__line__:68");
        return (Connector*)0;;
    }
    return connector_ptr;
}

//initial the pool after get the pool;
void ConnectPool::initConnectPool(int connectorSize)
{
    Connector* connector_ptr;
    connectorSize /= 2;
    pthread_mutex_lock(&lock);
    for(int i=0;i<connectorSize;i++)
    {
        connector_ptr = createConnector();
        if(connector_ptr)
        {
            connectorList.push_back(connector_ptr);
            realSize++;
        }
    }
    pthread_mutex_unlock(&lock);
}
//this method free all the connections in the pool;
void ConnectPool::destoryConnectPool()
{
    std::list<Connector*>::iterator iter;
    //Connector conn_tr;
    pthread_mutex_lock(&lock);  
    for(iter=connectorList.begin();iter!=connectorList.end();iter++)
    {
        if(!destoryConnection((*iter)))
            log.write_log(confmgr::LEVEL_WARNING,"__ConnectPool__:destoryConnectPool:__line__:107");
    }
    connectorList.clear();   
    realSize=0;
    pthread_mutex_unlock(&lock);
}

//destory connection
bool ConnectPool:: destoryConnection(Connector* connector)
{
    //connector->closeConnect();
    delete connector;
    return true;
}

//get the connector from the poor
Connector* ConnectPool::getConnector()
{
    Connector* connPtr;
    pthread_mutex_lock(&lock);
    //connectorList have connections
    if(connectorList.size()>0)
    {
        connPtr = connectorList.front();
        connectorList.pop_front();
        //try to fix this invalid connector;
        if(connPtr==0)
        {
            connPtr = createConnector();
        }else if(!connPtr->is_connect())
        {
            try{
                connPtr->connect();
            }catch(SqlException)
            {
                delete connPtr;
                connPtr = createConnector();
            }
        }
        //if the effort that take to fix this  connector, change the realSize!
        if(!connPtr)
        {
            realSize--;
            log.write_log(confmgr::LEVEL_FATAL,"__ConnectPool__:__getConnector__::__LINE__:156:Get connector failed!");
        }
        pthread_mutex_unlock(&lock);
        return connPtr;
    }
    //connector is empty!
    if(realSize < maxSize)
    {
        connPtr = createConnector();
        if(connPtr)
        {
            realSize++;
        }else
        {
            log.write_log(confmgr::LEVEL_WARNING,"__ConnectPool__:getConnector:__line__:141");
            connPtr = (Connector*)0;
        }
    }else
    {
        log.write_log(confmgr::LEVEL_WARNING,"__ConnectPool__:getConnector:__line__:139:The Connections of connection have Exhausted!");
        connPtr =  (Connector*)0;
    }
    pthread_mutex_unlock(&lock);
    return connPtr;
}
