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
SqlInterface::SqlInterface(Connector* connectHandle):no(ONE),connectorPtr(connectHandle),\
    query_status(0)

{
    log.init(("./mysql_interface.log"));
}
//release the connector, do nothing!
SqlInterface::~SqlInterface()
{
    connectorPtr = (Connector*)0;
}
void SqlInterface::query(const string &sql_query, GetNo getno,Resultmode mode)throw(SqlException)
{
    if(sql_query=="")
    {
        Throw(SqlException,"__SqlInterface__:query:__line__:37:Invalid query!");
        log.write_log(confmgr::LEVEL_WARNING,"__SqlInterface__:query:__line__:37");
    }
    if(result.size()&&results.size())
        log.write_log(confmgr::LEVEL_WARNING,"__SqlInterface__:query:__line__:41");
    no = getno;
    try{
        if(no==ONE)
            fetch_one(sql_query,mode);
        else fetch_all(sql_query,mode);
    }catch(SqlException)
    {
        Throw(SqlException,"");
    }
}

string SqlInterface::get_one()
{
    string tmp;
    if(no==ONE)
    {
        tmp = result;
        result="";
    }
    else
    {
        //this return the last element of the fetch_all result;
        if(!results.size())
            return "";

        tmp = results.back();
        results.pop_back();
    }
    return tmp;
}
const vector<string>& SqlInterface::get_all()
{
    if(no==ONE)
    {
        results.clear();
        results.push_back(result);
        result="";
    }
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

    vector<string> tmp;
    connectorPtr->query(query.c_str());

    connectorPtr->fetch_one(mode);
    connectorPtr->parseRows(tmp);
    result = tmp[0];
}


void SqlInterface::fetch_all(const string& query, Resultmode mode)throw(SqlException)
{
    results.clear();
    if(query=="")
    {
        std::cout<<"Invalid query!"<<std::endl;
    }
    connectorPtr->query(query.c_str());
    connectorPtr->fetch_all(mode);
    connectorPtr->parseRows(results);
}
