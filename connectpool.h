/*
 * =====================================================================================
 *
 *       Filename:  connectpool.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  08/22/2014 12:55:02 AM
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Yu Daoming (mn), yudaoming@360.cn
 *        Company:  Qihoo 360 LTD.
 *
 * =====================================================================================
 */
#ifndef CONNECT_POOL_H
#define CONNECT_POOL_H
#include <pthread.h>
#include "connector.h"
#include <list>
namespace mysql_interface{
     /*
      * this function just use for test!
      */
    class MysqlPoolException : public confmgr::ExceptionBase
    {
        public:
            DEFINE_EXCEPTION(MysqlPoolException,ExceptionBase)
    };
    /*
     * the connector pool ,all the connector come from this pool;
     */
    class ConnectPool{
        public:
            ~ConnectPool();
            Connector* getConnector();
            void freeConnection(Connector* connector);
            static ConnectPool* getInstance(const Testhandle& info, int maxSize);

        private:
            //create a connector 
            Connector* createConnector();
            //initial the connectpool 
            void initConnectPool(int connectSize);
            //destory connect pool
            void destoryConnectPool();
            //free the connnection 
            bool destoryConnection(Connector* connector);
            //private construction
            ConnectPool(const Testhandle& info,int maxSize);


            confmgr::Log log;
            int currSize;
            int maxSize;
            Testhandle info;
            std::list<Connector*> connectorList;
            pthread_mutex_t lock;
            static ConnectPool *connectPool;
    };
}
#endif