/*
 * log_manager.h
 *
 * Created by Ruibin.Chow on 2022/01/23.
 * Copyright (c) 2022年 Ruibin.Chow All rights reserved.
 */

#ifndef LOG_MANAGER_H
#define LOG_MANAGER_H

#include <cstdio>
#include <string>

#define Log(severity) OatppServer::log::LogManager(__FILE__, __LINE__, OatppServer::log::severity)

namespace OatppServer {
namespace log {

enum LoggingSeverity {
    VERBOSE,
    INFO,
    DEBUG,
    WARNING,
    ERROR,
    NONE,
};

class LogManager
{
public:
    explicit LogManager(const char* file, int line, LoggingSeverity severity);
    ~LogManager();
    
public:
    LogManager& operator<<(char c) = delete;
    
    LogManager& operator<<(void* pVoid) {
        stringBuffer_->append(*static_cast<std::string*>(pVoid));
        return *this;
    }
    
    LogManager& operator<<(const void* pConstVoid) {
        void* pChar = const_cast<void*>(pConstVoid);
        const char* p = static_cast<const char*>(pChar);
        stringBuffer_->append(p);
        return *this;
    }
    
    LogManager& operator<<(const char* charStr) {
        stringBuffer_->append(charStr);
        return *this;
    }
    
    LogManager& operator<<(const std::string_view str) {
        stringBuffer_->append(str.data(), str.length());
        return *this;
    }
    
    LogManager& operator<<(int i) {
        stringBuffer_->append(std::to_string(i));
        return *this;
    }
    
    LogManager& operator<<(unsigned i) {
        stringBuffer_->append(std::to_string(i));
        return *this;
    }
    
    LogManager& operator<<(long i) {
        stringBuffer_->append(std::to_string(i));
        return *this;
    }
    
    LogManager& operator<<(long long i) {
        stringBuffer_->append(std::to_string(i));
        return *this;
    }
    
    LogManager& operator<<(unsigned long i) {
        stringBuffer_->append(std::to_string(i));
        return *this;
    }
    
    LogManager& operator<<(unsigned long long i) {
        stringBuffer_->append(std::to_string(i));
        return *this;
    }
    
    LogManager& operator<<(float f) {
        stringBuffer_->append(std::to_string(f));
        return *this;
    }
    
    LogManager& operator<<(double f) {
        stringBuffer_->append(std::to_string(f));
        return *this;
    }
    
    LogManager& operator<<(long double f) {
        stringBuffer_->append(std::to_string(f));
        return *this;
    }
    

private:
    std::string* stringBuffer_;
};


}
}

#endif /* !LOG_MANAGER_H */
