/**
 * @file util.h
 * @author dongjiang(@360.cn)
 * @date 2014/08/17 19:46:34
 * @brief log�ࣻ�����־��ӡ
 *  
 **/

#ifndef  __UTIL_H_
#define  __UTIL_H_

#include <assert.h>
#include <stdio.h>  
#include <iostream>  
#include <stdarg.h>  
#include <pthread.h>

namespace confmgr
{

typedef enum
    {
        LEVEL_FATAL     = -1,
        LEVEL_NONE      = 0, /* �������־ */
        LEVEL_ERROR     = 1,
        LEVEL_INFO      = 4,
        LEVEL_WARNING   = 8,
        LEVEL_DEBUG     = 16,
    } LEVEL_TYPE;

/**
 * @brief log�ࣻ�����־��ӡ
 */
class Log
{
public:
    /**
     * @brief ���캯��
    **/
    Log();

    /**
     * @brief ��������
     */
    ~Log();

    /**
     * @brief init��ʼ������
    **/
    bool init(const char* file_name);

    /**
     * @brief д��־
    **/
    void write_log(LEVEL_TYPE level, const char* format, ...);

    /**
     * @brief ˢ�¾��
    **/
    void flush(void);

private:
    static const int MAX_SIZE         = 1024; 
    static const unsigned int MAX_BUF = 4096;
    char *dir_name;  // �ļ�·��
    char *log_name;  // �ļ���
    int m_time;
    FILE *m_fp;
};

} //end confmgr

#define WARNINGLOG(format, ...)\
{\
    confmgr::Log::write_log(confmgr::LEVEL_TYPE::LEVEL_WARNING, format, ...);\
}

#define INFOLOG(format, ...)\
{\
    confmgr::Log::write_log(confmgr::LEVEL_TYPE::LEVEL_INFO, format, ...);\
}

#define DEBUGLOG(format, ...)\
{\
    confmgr::Log::write_log(confmgr::LEVEL_TYPE::LEVEL_DEBUG, format, ...);\
}

#define FATALLOG(format, ...)\
{\
    confmgr::Log::write_log(confmgr::LEVEL_TYPE::LEVEL_FATAL, format, ...);\
}

#define ERRORLOG(format, ...)\
{\
    confmgr::Log::write_log(confmgr::LEVEL_TYPE::LEVEL_ERROR, format, ...);\
}

#endif  //__UTIL_H_
