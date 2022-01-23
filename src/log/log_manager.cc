/*
 *
 * log_manager.cc
 *
 * Created by Ruibin.Chow on 2022/01/23.
 * Copyright (c) 2022年 Ruibin.Chow All rights reserved.
 */

#include "log_manager.h"
#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>

namespace OatppServer {
namespace log {

static const char* loggingSeverityCover(LoggingSeverity severity)
{
    static const char* severityList[] = {
        [VERBOSE] = "[V]",
        [INFO] = "[I]",
        [DEBUG] = "[D]",
        [WARNING] = "[W]",
        [ERROR] = "[E]",
        [NONE] = "[N]",
    };
    
    return severityList[severity];
}

static const std::string&& loggingNow()
{
    auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::stringstream ss;
    ss << std::put_time(std::localtime(&now), "%Y-%m-%d %H.%M.%S");
    return std::move(ss.str());
}

LogManager::LogManager(const char* file, int line, LoggingSeverity severity)
        : stringBuffer_(new std::string)
{
    std::string fileStr(file);
    stringBuffer_->append(loggingNow());
    stringBuffer_->append(" [");
    stringBuffer_->append(fileStr.substr(fileStr.find_last_of("/")+1));
    stringBuffer_->append(":");
    stringBuffer_->append(std::to_string(line));
    stringBuffer_->append("]");
    stringBuffer_->append(loggingSeverityCover(severity));
    stringBuffer_->append(" ");
}

LogManager::~LogManager()
{
    stringBuffer_->append("\n");
    std::cout << stringBuffer_->c_str();
    delete stringBuffer_;
}


}
}
