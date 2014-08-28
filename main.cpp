#include <iostream>
#include "connector.h"
#include "connectpool.h"
using namespace std;
using namespace mysql_interface;
int main()
{
    /*
     * test connector!
     */
    //Testhandle addr;
    //ConnectPool* connectpool = ConnectPool::getInstance(addr,5);
    Connector connector;
    connector.connect();
    connector.query(" select user_id, user_name,user_money from user where user_money > 1000 limit 1;");
    connector.fetch_one(USE_RESULT_MODE);
    vector<string> test;
    connector.parseRows(test);
    std::cout<<test[0]<<std::endl;
    if(connector.is_connect())
        std::cout<<"connect !"<<std::endl;
    connector.query(" select user_id, user_name,user_money from user where user_money > 1000;");
    connector.fetch_all(STORE_RESULT_MODE);
    connector.parseRows(test);
    std::cout<<test[1]<<std::endl;
    std::cout<<test[2]<<std::endl;
    //test the affected rows
    std::cout<<connector.mysql_affected()<<std::endl;
    connector.closeConnect();
    if(!connector.is_connect())
        std::cout<<"Connector is closed!"<<std::endl;
    //connector.~Connector();

    /*
     * test ConnectPool!
     */
    Testhandle test_info;//use default;
    ConnectPool* pool;
    pool = ConnectPool::getInstance(test_info,10);//test the constructor

    Connector* test_pool_connector;
    test_pool_connector=pool->getConnector();//get the connector from the pool
    if(test_pool_connector->is_connect())
        std::cout<<"connector have connected!"<<endl;
    pool->freeConnection(test_pool_connector);

    /*
     *
     */

    std::cout<<"finsh"<<std::endl;
    return 0;
}

