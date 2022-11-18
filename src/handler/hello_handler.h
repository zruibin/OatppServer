/*
 * hello_handler.h
 *
 * Created by Ruibin.Chow on 2022/01/23.
 * Copyright (c) 2022年 Ruibin.Chow All rights reserved.
 */

#ifndef HELLO_HANDLER_H
#define HELLO_HANDLER_H

#include <oatpp/web/server/HttpConnectionHandler.hpp>
#include <string>
#include "log/logging.h"
#include "util/util.h"

class HelloHandler : public oatpp::web::server::HttpRequestHandler
{
public:
    std::shared_ptr<OutgoingResponse> handle(const std::shared_ptr<IncomingRequest>& request) override {
        Log(INFO) << &request;
        
        std::string echoStr("<html><body><h2><center>");
        echoStr.append("Hello World! From Oatpp -> Ruibin.Chow");
        echoStr.append("<br/>Now: ");
        echoStr.append(OatppServer::util::get_current_time_string());
        echoStr.append("</center></h2></body></html>");
        
        return ResponseFactory::createResponse(Status::CODE_200, echoStr);
    }
};


#endif /* !HELLO_HANDLER_H */
