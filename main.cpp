#include <iostream>
#include "connector.h"
#include "connectpool.h"
using namespace std;
using namespace mysql_interface;
int main()
{
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
    cout << "Hello World!" << endl;
    return 0;

}

