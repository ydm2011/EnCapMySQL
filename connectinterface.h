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
enum GetNo{ONE,ALL};
class SqlInterface{
public:
    SqlInterface(ConnectPool* connectpool);
    void query(const std::string& sql_query,GetNo getno=ONE,Resultmode mode = USE_RESULT_MODE)throw(SqlException);
    std::string get_one();
    const std::vector<std::string>& get_all();
    unsigned long get_status();
    ~SqlInterface();
protected:
    void fetch_one(const std::string& query,Resultmode mode=USE_RESULT_MODE)throw(SqlException);
    void fetch_all(const std::string& query,Resultmode mode=USE_RESULT_MODE)throw(SqlException);
private:

    GetNo no;
    ConnectPool* connectPool;


    unsigned long  query_status;
    Connector* connectorPtr;

    //store the query result;
    std::string result;
    std::vector<std::string> results;

    confmgr::Log log;
};

}
#endif // CONNECTINTERFACE_H
