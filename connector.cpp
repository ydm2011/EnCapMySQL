/*
 * =====================================================================================
 *
 *       Filename:  connector.cpp
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
#include "connector.h"
using namespace std;
using namespace mysql_interface;
//
Connector::Connector(const Testhandle& _info)
{
    info.host = _info.host;
    info.user = _info.user;
    info.password = _info.password;
    info.db_name = _info.db_name;
    info.port_number = _info.port_number;
    info.sock_name = _info.sock_name;
    info.flags = _info.flags;
    //use the log
    log.init(("./mysql_connector.log"));
    pthread_mutex_init(&lock,NULL);
    connectOrNot = false;
    res=(MYSQL_RES*)0;
    mysql_init(&db);
}

//
Connector::Connector()
{
    log.init(("./mysql_connector.log"));
    pthread_mutex_init(&lock,NULL);
    connectOrNot = false;
    mysql_init(&db);
}

//
Connector::~Connector()
{
    if(is_connect())
        closeConnect();
    pthread_mutex_destroy(&lock);
    if(res!=(MYSQL_RES*)0)
        mysql_free_result(res);
}
//connect the sqlserver
bool Connector:: connect()throw(SqlException)
{
    MYSQL* conn_ptr;
    conn_ptr = mysql_real_connect(&db,info.host.c_str(),info.user.c_str(),
            info.password.c_str(),info.db_name.c_str(),info.port_number,info.sock_name,info.flags);
    if(!conn_ptr)
    {
        log.write_log(confmgr::LEVEL_ERROR,mysql_error(&db));
        Throw(SqlException,"Connector: connect error");
    }
    log.write_log(confmgr::LEVEL_INFO,"Connector: connect success!");
    connectOrNot = true;
    return true;
}
//query the mysql;
int Connector:: query(const char* sql_query)
{
    int res_no;
    //pthread_mutex_lock(&lock);
    res_no = mysql_query(&db,sql_query);
    //pthread_mutex_unlock(&lock);
    if(!res_no)
       log.write_log(confmgr::LEVEL_INFO,"Connector: query success!");
    else Throw(SqlException,"query error");
    return 0;
}
//get one row from the mysql
int  Connector::fetch_one(Resultmode mode)
{
    if(mode == USE_RESULT_MODE)
        res = mysql_use_result(&db);
    else res = mysql_store_result(&db);
    if(res)
    {
        log.write_log(confmgr::LEVEL_INFO,"Connector: fetch_one success!");
    }else{
        log.write_log(confmgr::LEVEL_ERROR,"Connector: fetch_one failed!");
        Throw(SqlException,"fetch_one error");
    }
    sqlRow = mysql_fetch_row(res);
    if(mysql_errno(&db))
    {
        log.write_log(confmgr::LEVEL_ERROR,mysql_error(&db));
        Throw(SqlException,"fetch_one error");
    }
    rows.clear();
    rows.push_back(sqlRow);
    //mysql_free_result(res);
    return 0;
}

int Connector::fetch_all(Resultmode mode)
{
    if(mode == USE_RESULT_MODE)
        res = mysql_use_result(&db);
    else res = mysql_store_result(&db);

    if(res)
        log.write_log(confmgr::LEVEL_INFO,"Connector: fetch_all success!");
    else{
        log.write_log(confmgr::LEVEL_ERROR,mysql_error(&db));
        Throw(SqlException,"fetch_all error");
    }
    rows.clear();
    while((sqlRow = mysql_fetch_row(res)))
    {
        rows.push_back(sqlRow);
    }
    if(mysql_errno(&db))
    {
        log.write_log(confmgr::LEVEL_ERROR,mysql_error(&db));
        Throw(SqlException,"fetch_all error");
    }
    //mysql_free_result(res);
    return 0;
}
//parse each row ,in the protected scope
string Connector::parseRow(MYSQL_ROW sql_row)
{
    int fields;
    fields = mysql_field_count(&db);
    string row;
    row += sql_row[0];
    int field=1;
    while (field < fields) {
        row += " ";
        row += sql_row[field];
        field++;
    }
    return row;

}
//this will release the cache of the mysql;
void Connector::parseRows(vector<string>& result)
{
    if(!rows.size())
    {
        log.write_log(confmgr::LEVEL_ERROR,"Connector:parseRows rows empty!");
        return ;
    }
    vector<MYSQL_ROW>::iterator iter = this->rows.begin();
    for( ; iter != rows.end(); iter++)
        result.push_back(parseRow(*iter));
    mysql_free_result(res);
    res = (MYSQL_RES*)0;
}
//close the connnect
void Connector::closeConnect()
{
    mysql_close(&db);
    connectOrNot = false;
}
//return the affected no of the mysql;
unsigned long long Connector::mysql_affected()
{
    return mysql_affected_rows(&db);
}
//check the connect status;
bool Connector::is_connect()
{
    return connectOrNot;
}
