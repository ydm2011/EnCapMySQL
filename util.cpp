/**
 * @file util.cpp
 * @author dongjiang(@360.cn)
 * @date 2014/08/19 14:24:52
 * @brief 
 *  
 **/

#include <stdio.h>   //使用 snprintf
#include <iostream>  //使用 
#include <string.h>  //使用 strrchr
#include <time.h>    //使用 time_t, localtime
#include <stdarg.h>  //使用 ...
#include <pthread.h> //使用 pthread_mutex_t, pthread_mutex_init, pthread_mutex_destroy
#include <new>       //使用 new, delelte
#include "util.h"    //声明文件

namespace confmgr
{

using namespace std;

static pthread_mutex_t g_mutex;

Log::Log()
{
    pthread_mutex_init(&g_mutex, NULL);
    dir_name = new char[MAX_SIZE/2];
    log_name = new char[MAX_SIZE/2];
}

Log::~Log()  
{   
    if (dir_name != NULL)
        delete dir_name;
    if (log_name != NULL)
        delete log_name;
    if (m_fp != NULL)  
    {  
        fclose(m_fp);  
    }  
    pthread_mutex_destroy(&g_mutex);  
} 

bool Log::init(const char* file_name) 
{
    time_t t = time(NULL);
    struct tm* sys_tm = localtime(&t);
    struct tm my_tm = *sys_tm;
    char *p = strrchr(const_cast<char*>(file_name), '/');
    char log_full_name[MAX_SIZE] = {0};
    if (p == NULL)
    {
        snprintf(log_full_name, MAX_SIZE-1, "%d_%02d_%02d_%s",my_tm.tm_year+1900, my_tm.tm_mon+1, my_tm.tm_mday, file_name);
    }
    else
    {
        strcpy(log_name, p+1);
        strncpy(dir_name, file_name, p-file_name+1);
        snprintf(log_full_name, MAX_SIZE-1, "%s%d_%02d_%02d_%s", dir_name, my_tm.tm_year+1900, my_tm.tm_mon+1, my_tm.tm_mday, log_name);
    }

    m_time = my_tm.tm_mday;

    m_fp = fopen(log_full_name, "a");
    if (m_fp == NULL)
    {
        return false;
    }
    return true;
}

void Log::write_log(LEVEL_TYPE level, const char* format, ...)
{
    char buf[MAX_BUF] = {0};
    char input_buf[MAX_BUF/2] = {0};
    char new_log[MAX_SIZE/2] = {0};

    time_t t = time(NULL);  
    struct tm* sys_tm = localtime(&t);  
    struct tm my_tm = *sys_tm;  
    char s[32] = {0}; 

    switch(level)
    {
        case LEVEL_FATAL:
            strcpy(s, "[FATAL]:"); 
            break;
        case LEVEL_ERROR:
            strcpy(s, "[ERROR]:"); 
            break;
        case LEVEL_INFO:
            strcpy(s, "[INFO]:");
            break;
        case LEVEL_WARNING:
            strcpy(s, "[WARNING]:");
            break;
        case LEVEL_DEBUG:
            strcpy(s, "[DEBUG]:");
            break;
        default:
            strcpy(s, "[NONE]:");
    }
    
    snprintf(buf, MAX_BUF-1, "%d-%02d-%02d %02d:%02d:%02d %s ", my_tm.tm_year+1900, my_tm.tm_mon+1, my_tm.tm_mday,my_tm.tm_hour, my_tm.tm_min, my_tm.tm_sec, s); 

    if (m_time != my_tm.tm_mday)
    {
        pthread_mutex_lock(&g_mutex);
        fflush(m_fp);
        fclose(m_fp);
        char tail[MAX_SIZE/2] = {0};
        snprintf( tail, MAX_SIZE/2 -1,  "%d_%02d_%02d_", my_tm.tm_year+1900, my_tm.tm_mon+1, my_tm.tm_mday);
        snprintf(new_log, MAX_SIZE-1, "%s%s%s", dir_name, tail, log_name);
        m_fp = fopen(new_log, "a");
        m_time = my_tm.tm_mday;
        pthread_mutex_unlock(&g_mutex);
    }

    va_list valst;
    va_start(valst, format);
    vsprintf(input_buf, format, valst);  
    strncat(buf, input_buf, MAX_BUF-1);  
    buf[strlen(buf)-1] = '\n';  
    pthread_mutex_lock(&g_mutex);  
    fputs(buf, m_fp);  
    pthread_mutex_unlock(&g_mutex);  
    va_end(valst); 
}

void Log::flush(void)  
{  
    pthread_mutex_lock(&g_mutex);  
    fflush(m_fp);  
    pthread_mutex_unlock(&g_mutex);  
}

} //end confmgr





















/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
