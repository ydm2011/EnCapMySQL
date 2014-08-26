/**
 * @file util.cpp
 * @author dongjiang(@360.cn)
 * @date 2014/08/19 14:24:52
 * @brief 
 *  
 **/


#include <execinfo.h>
#include <stdlib.h> // use backtrace_symbols backtrace
#include <cxxabi.h> // 使用 abi::__cxa_demangle
#include <iostream> // 使用 cont
#include <sstream> //使用 stringstream
#include "CException.h"

using namespace std;

ExceptionBase::ExceptionBase(const string& msg) throw() 
    : mMsg(msg),
      mFile("<unknown file>"),
      mFunc("<unknown func>"),
      mLine(-1),
      mStackTraceSize(0) {
}


ExceptionBase::~ExceptionBase() throw() {
}
    

void ExceptionBase::init(const char* file, const char* func, int line) {
    mFile = file;
    mFunc = func;
    mLine = line;
    mStackTraceSize = backtrace(mStackTrace, MAX_STACK_TRACE_SIZE);
}

string ExceptionBase::GetClassName() const {

    return "ExceptionBase"; 
}

const char* ExceptionBase::what() const throw() {
    return ToString().c_str();
}

const string& ExceptionBase::ToString() const {
    if (mWhat.empty())
    {
        stringstream sstr("");
        if (mLine > 0)
        {
            sstr << mFile << "(" << mLine << ")";
        }
        sstr <<  ": " << GetClassName();
        if (!GetMessage().empty())
        {
            sstr << ": " << GetMessage();
        }
        sstr << "\nStack Trace:\n";
        sstr << GetStackTrace();
        mWhat = sstr.str();
    }
    return mWhat;
}

string ExceptionBase::GetMessage() const {
        return mMsg;
}

string ExceptionBase::GetStackTrace() const {
    if ( mStackTraceSize == 0 )
        return "<no stack Trace>\n";

    char** strings = backtrace_symbols(mStackTrace, 10);
    if (strings == NULL)
        return "<Unknown error: backtrace_symbols return NULL>\n";

    string result;
    for (size_t i = 0; i < mStackTraceSize; ++i)
    {
        string mangledName = strings[i];
        string::size_type begin = mangledName.find('(');
        string::size_type end = mangledName.find('+', begin);
        if (begin == string::npos || end == string::npos )
        {
            result += mangledName;
            result += '\n';
            continue;
        }
        ++begin;
        int status;
        char* s = abi::__cxa_demangle(mangledName.substr(begin, end-begin).c_str(),
                                      NULL, 0, &status);

        if (status != 0)
        {
            result += mangledName;
            result += '\n';
            continue;
        }
        string demangledName(s);
        free(s);
        // 增加 #0 的首栈信息
        result += mangledName.substr(0, begin);
        result += demangledName;
        result += mangledName.substr(end);
        result += '\n';
    }
    free(strings);
    return result;
}









/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
