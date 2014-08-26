/*
 * =====================================================================================
 *
 *       Filename:  connectorinterface.h
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

#ifndef CONNECTOR_CPP_H
#define CONNECTOR_CPP_H
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "mysql.h"
#include "util.h"
#include "CException.h"
namespace mysql_interface{
enum Resultmode{STORE_RESULT_MODE,USE_RESULT_MODE};
enum Fetchmode{FETCH_ONE,FETCH_MANY,FETCH_ALL};
enum DataLocation{SERVER,LOCAL};
class SqlException : public ExceptionBase
{
public:
    DEFINE_EXCEPTION(SqlException, ExceptionBase)
};
struct Testhandle{
    Testhandle()
    {
        host = "localhost";
        user = "root";
        password = "09";
        db_name = "daoming_test";
        sock_name = (const char*)0;
        port_number = 0;
        flags = 0;
    }

    std::string host;
    std::string user;
    std::string password;
    std::string db_name;
    unsigned int port_number;
    const char* sock_name;
    unsigned int flags;
};
//this class is cpp interface of the mysql;
class Connector
{
    public:
        Connector();//the local database;
        Connector(const Testhandle& _info);//pass the connection info
        bool connect();//connect the database
        int query(const char* sql_query);//query the database
        int fetch_one(Resultmode mode = USE_RESULT_MODE);//get one result from the res_result
        int fetch_all(Resultmode mode = STORE_RESULT_MODE);//get all the result;

        bool is_connect();//whether the db is connect;
        void closeConnect();//close the  connect
        void parseRows(std::vector<std::string>& result);//parse the sql result to string
        ~Connector();//clean
    protected:
        std::string parseRow(MYSQL_ROW sql_row);//parse each row
        //the connectinfo which can short the parametes of the function
        struct Dbconnectinfo
        {
            Dbconnectinfo()
            {
                host = "localhost";
                user = "root";
                password = "09";
                db_name = "daoming_test";
                port_number = 0;
                sock_name = (char*)0;
                flags = 0;
            }
            std::string host;
            std::string user;
            std::string password;
            std::string db_name;
            unsigned int port_number;
            const char* sock_name;
            unsigned int flags;
        };

    private:
        //std::ofstream* log;
        confmgr::Log log;
        MYSQL db;//the handle of mysql connection
        pthread_mutex_t lock;//lock not use in here
        bool connectOrNot;//status
        Dbconnectinfo info;
        MYSQL_RES* res;//the result buffer of the mysql
        MYSQL_ROW sqlRow;//point one row of the res
        std::vector<MYSQL_ROW> rows;//save fetching result;
};
}
#endif
