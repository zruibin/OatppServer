/*
 *
 * log_manager.cc
 *
 * Created by Ruibin.Chow on 2022/01/23.
 * Copyright (c) 2022年 Ruibin.Chow All rights reserved.
 */

#include "logging.h"
#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>
#include "platform/platform.h"

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

static const std::string loggingNow()
{
    auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
//    std::stringstream ss;
//    ss << std::put_time(std::localtime(&now), "%Y-%m-%d %H.%M.%S");
//    return std::move(ss.str());
    struct tm* ptm = localtime(&now);
    char date[60] = {0};
    sprintf(date, "%d-%02d-%02d %02d:%02d:%02d",
            (int)ptm->tm_year + 1900,(int)ptm->tm_mon + 1,(int)ptm->tm_mday,
            (int)ptm->tm_hour,(int)ptm->tm_min,(int)ptm->tm_sec);
    return std::string(date);
}

LogMessage::LogMessage(const char* file, int line, LoggingSeverity severity)
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
    stringBuffer_->append("[");
    stringBuffer_->append(std::to_string(platform::thread_get_current_id()));
    stringBuffer_->append("]");
    stringBuffer_->append(" ");
}

LogMessage::~LogMessage()
{
    stringBuffer_->append("\n");
    std::cout << stringBuffer_->c_str();
    delete stringBuffer_;
}


}
}
