/*
 * =====================================================================================
 *
 *       Filename:  connectinterface.h
 *
 *    Description:
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
#ifndef CONNECTINTERFACE_H
#define CONNECTINTERFACE_H
#include "connector.h"
#include "connectpool.h"
namespace mysql_interface{

class SqlInterface{
public:
    SqlInterface(Connector* connectHandle);
    //bool query(const char* sql_query);
    void fetch_one(const std::string& query,std::string& result,Resultmode mode=USE_RESULT_MODE);
    void fetch_all(const std::string& query,std::vector<std::string>& result,Resultmode mode=STORE_RESULT_MODE);
    //void read_field(std::string& result);
    //void read_field(std::vector<std::string>& result);
    ~SqlInterface();
private:

    Connector* connectorPtr;

    //pthread_mutex_t lock;
    //MYSQL_RES* res;
    //MYSQL_ROW sqlRow;
    //std::vector<MYSQL_ROW> rowVector;
};

}
#endif // CONNECTINTERFACE_H
