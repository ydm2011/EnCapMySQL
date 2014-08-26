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
SqlInterface::SqlInterface(Connector* connectHandle)
{
    connectorPtr = connectHandle;
    //pthread_mutex_init(&lock,NULL);
}
//release the connector, do nothing!
SqlInterface::~SqlInterface()
{
    connectorPtr = (Connector*)0;
}
//get one string of the
void SqlInterface:: fetch_one(const std::string& query,std::string& result,Resultmode mode)
{
    if(!connectorPtr->is_connect())
    {
        std::cout<<"Not connect,please get another one!"<<std::endl;
        return ;
    }
    if(query.size()==0)
    {
        std::cout<<"Invalid query!"<<std::endl;
        return ;
    }

    vector<string> tmp;
    connectorPtr->query(query.c_str());

    connectorPtr->fetch_one(mode);
    connectorPtr->parseRows(tmp);
    result = tmp[0];
}
void SqlInterface::fetch_all(const string& query, std::vector<string>& result, Resultmode mode)
{
    if(query=="")
    {
        std::cout<<"Invalid query!"<<std::endl;
    }
    connectorPtr->query(query.c_str());
    connectorPtr->fetch_all(mode);
    connectorPtr->parseRows(result);
}
