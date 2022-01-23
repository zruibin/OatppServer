/*
 * hello_handler.h
 *
 * Created by Ruibin.Chow on 2022/01/23.
 * Copyright (c) 2022年 Ruibin.Chow All rights reserved.
 */

#ifndef HELLO_HANDLER_H
#define HELLO_HANDLER_H

#include <oatpp/web/server/HttpConnectionHandler.hpp>
#include "log/log_manager.h"

class HelloHandler : public oatpp::web::server::HttpRequestHandler
{
public:
    std::shared_ptr<OutgoingResponse> handle(const std::shared_ptr<IncomingRequest>& request) override {
//        Log(INFO) << &request;
        return ResponseFactory::createResponse(Status::CODE_200, "Hello World! From Oatpp -> Ruibin.Chow");
    }
};


#endif /* !HELLO_HANDLER_H */
