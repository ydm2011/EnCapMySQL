/*
 * =====================================================================================
 *
 *       Filename:  connectinterface.cpp
 *
 *    Description:  this file is defination of the mysqlinterface;
 *
 *        Version:  1.0
 *        Created:  08/24/2014 12:55:02 PM
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Yu Daoming (mn), yudaoming@360.cn
 *        Company:  Qihoo 360 LTD.
 *
 * =====================================================================================
 */
#include "connectinterface.h"
using namespace mysql_interface;
using namespace std;
//get the connector from the connectPool
SqlInterface::SqlInterface(ConnectPool *connectpool):no(ONE),connectPool(connectpool),\
    query_status(0)
{
    if(!connectpool)
        Throw(SqlException,"The ConnectPool is not defined!");
    if(!connectpool->checkPool())
        Throw(SqlException,"The connection of the pool have exhausted!");
    connectorPtr = connectPool->getConnector();
    log.init(("./mysql_interface.log"));
}
//release the connector, do nothing!
SqlInterface::~SqlInterface()
{
    if(connectorPtr)
        connectPool->freeConnection(connectorPtr);
    connectorPtr = (Connector*)0;
}
void SqlInterface::query(const string &sql_query, GetNo getno,Resultmode mode)throw(SqlException)
{
    if(sql_query=="")
    {
        Throw(SqlException,"__SqlInterface__:query:__line__:37:Invalid query!");
        log.write_log(confmgr::LEVEL_WARNING,"__SqlInterface__:query:__line__:37");
    }
    if(results.size())
        log.write_log(confmgr::LEVEL_WARNING,"__SqlInterface__:query:__line__:41");
    results.clear();
    no = getno;
    //that means don't get the result!
    if(no==NONE)
        return;
    try{
        if(no==ONE&&mode==USE_RESULT_MODE)
            fetch_one(sql_query,mode);
        else fetch_all(sql_query,mode);
    }catch(SqlException)
    {
        Throw(SqlException,"");
    }
}
//the fetch_one interface,return a copy of the response;
//if the sql mode is limits 1 .it return the result copy;
//if the sql mode is many,it should return  the last element of the results;
int SqlInterface::get_one(string& result)
{
    if(no != NONE )
    {
        //this return the last element of the fetch_all result;
        if(!results.size())
            return -1;

        result = results.back();
        results.pop_back();
    }
    return 0;
}
//the interface of send data to the users
const vector<string>& SqlInterface::get_all()
{
    if(no==NONE)
        results.clear();

    return results;
}
unsigned long SqlInterface::get_status()
{
    return connectorPtr->mysql_affected();
}

//get one string of the result;
void SqlInterface:: fetch_one(const std::string& query,Resultmode mode)throw(SqlException)
{
    if(!connectorPtr->is_connect())
    {
        log.write_log(confmgr::LEVEL_ERROR,"__SqlInterface__:fetch_one:__line__:36");
        Throw(SqlException,"SqlInterface:: fetch_one:__LINE__:93");
        //std::cout<<"Not connect,please get another one!"<<std::endl;
        return ;
    }
    if(query.size()==0)
    {
        log.write_log(confmgr::LEVEL_ERROR,"__SqlInterface__:fetch_one:__line__:45");
        return ;
    }
    results.clear();
    mode = USE_RESULT_MODE;//this can cover the bug of the connector;next version will fix this;
    connectorPtr->query(query.c_str());

    connectorPtr->fetch_one(mode);
    connectorPtr->parseRows(results);
}

//get all the response data from the database
void SqlInterface::fetch_all(const string& query, Resultmode mode)throw(SqlException)
{
    results.clear();
    if(query=="")
    {
        std::cout<<"Invalid query!"<<std::endl;
    }
    mode = STORE_RESULT_MODE;//this will cover the bug of the connector;next version will fix this;
    connectorPtr->query(query.c_str());
    connectorPtr->fetch_all(mode);
    connectorPtr->parseRows(results);
    std::cout<<"\n"<<results.size()<<std::endl;
}
