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
//initial the staic var defination
ConnectPool* ConnectPool::connectPool= (ConnectPool*)0;
//testhandle needed by the mysql connection function;maxSize is the max
//connection no of the pool
ConnectPool::ConnectPool(const Testhandle& info, int maxSize):currSize(0)
{
    this->info = info;
    this->maxSize = maxSize;
    pthread_mutex_init(&lock,NULL);
}
//close all the connection and delete the connectPool
ConnectPool::~ConnectPool()
{
    connectPool->destoryConnectPool();
    delete connectPool;
    connectPool = (ConnectPool*)0;
    pthread_mutex_destory(&lock); 
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

    }
}
//get the instansce for singleton
ConnectPool* ConnectPool::getInstance(const Testhandle& info, int maxSize)
{
    if(maxSize<0||maxSize==0)
        throw 0;
    if(maxSize>50)
        maxSize = 50;
    if(connectPool==0)
        connectPool = new ConnectPool(info,maxSize);
    return connectPool;
}
//if there are no connection in the pool ,this will be called to create a new connection
Connector* ConnectPool::createConnector()
{
    Connector* connector_ptr = new Connector(info);
    //try{
    if(!connector_ptr->connect()){
        std::cout<<"Connect failed!"<<std::endl;
        return (Connector*)0;
    }    
    return connector_ptr;
}

//initial the pool after get the pool;
void ConnectPool::initConnectPool(int connectSize)
{
    Connector* connector_ptr;
    currSize = 0;
    if(connectSize > maxSize)
    {
        std::cout<<"_FUNC__ ConnectPool::initConnectionPool="
            <<"connectSize larger than the maxSize,set the default!"<<std::endl;
        connectSize = maxSize;
    }
    pthread_mutex_lock(&lock);
    for(int i=0;i<connectSize;i++)
    {
        connector_ptr = createConnector();
        if(!connector_ptr)
        {
            connectorList.push_back(connector_ptr);
            currSize++;
        }
    }
    pthread_mutex_unlock(&lock);
}
//this method free all the connections in the pool;
void ConnectPool::destoryConnectPool()
{
    std::list<Connector*>::iterator iter;
    pthread_mutex_lock(&lock);
    Connector conn_tr;
    for(iter=connectorList.begin();iter!=connectorList.end();iter++)
    {
        if(!destoryConnection((*iter)))
            std::cout<<"destory connect pool failed!";
    }
    connectorList.clear();   
    currSize=0;
    pthread_mutex_unlock(&lock);
}

//destory connection
bool ConnectPool:: destoryConnection(Connector* connector)
{
    connector->closeConnect();
    delete connector;
    return true;
}

//get the connector from the poor
Connector* ConnectPool::getConnector()
{
    Connector* connPtr;
    pthread_mutex_lock(&lock);
    if(connectorList.size()>0)
    {
        connPtr = connectorList.front();
        connectorList.pop_front();
        if(connPtr==0)
            currSize--;
        else if(!connPtr->is_connect())
            connPtr = createConnector();
        pthread_mutex_unlock(&lock);
        return connPtr;
    }
    if(currSize < maxSize)
    {
        connPtr = createConnector();
        if(connPtr)
        {
            currSize++;
            pthread_mutex_unlock(&lock);
        }else
            connPtr = (Connector*)0;
    }else
    {
        pthread_mutex_unlock(&lock);
        connPtr =  (Connector*)0;
    }
    return connPtr;
}
