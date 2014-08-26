/**
 * @file util.h
 * @author dongjiang(@360.cn)
 * @date 2014/08/22 17:16:34
 * @brief 异常基类
 *  
 **/

#ifndef __CEXCEPTION_H_
#define __CEXCEPTION_H_
#include <exception>
#include <string>
#include <stdarg.h>

using namespace std;

#define Throw(Exclass, args, ...)  \
    do\
    {\
        Exclass e(args);  \
        e.init(__FILE__, __func__, __LINE__);\
        throw e;\
    }\
    while(false)

#define DEFINE_EXCEPTION(ExClass, Base) \
    ExClass(const string& msg = "") throw()    \
            : Base(msg)\
    {}\
    \
    ~ExClass() throw() \
    {}\
    \
    string GetClassName() const\
    {\
        return #ExClass;\
    }

/**
 * @brief log类；完成日志打印
 **/
class ExceptionBase : public exception
{

public:
    /**
    * @brief 构造函数
    **/
    ExceptionBase(const string& msg = "") throw();

    /**
    * @brief 析构函数
    **/
    virtual ~ExceptionBase() throw();

    /**
    * @brief 初始化函数
    **/
    void init(const char* file, const char* func, int line);

    /**
    * @brief 获取类名
    **/
    virtual string GetClassName() const;

    /**
    * @brief 获取获取异常信息
    **/
    virtual string GetMessage() const;

    /**
    * @brief 得到 traceback 内容
    **/
    const char* what() const throw();

    /**
    * @brief char To String
    **/
    const string& ToString() const;

    /**
    * @brief 获得指定行数的栈信息
    **/
    string GetStackTrace() const;

public:

    string mMsg;  //异常信息
    const char* mFile;  // 内部存储 __FILE__ 宏
    const char* mFunc;  // 内部存储 __FUNC__ 宏
    int mLine;          // 内部存储 __LINE__ 宏

private:
    enum { MAX_STACK_TRACE_SIZE = 50 };   // 指定栈的行数
    void* mStackTrace[MAX_STACK_TRACE_SIZE];  // 保存每一条栈的指证
    size_t mStackTraceSize; // 实际的栈的条数，计数用
    mutable string mWhat;  // traceback 内容对象
};



#endif //__CEXCEPTION_H_




/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
