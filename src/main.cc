/*
 *
 * main.cc
 *
 * Created by Ruibin.Chow on 2022/01/22.
 * Copyright (c) 2022年 Ruibin.Chow All rights reserved.
 */


#include <memory>

extern "C" {
#include <kcp/ikcp.h>
#include <lua/lua.h>
}
#include <asio/asio.hpp>
#include "app.hpp"

int main()
{
    OatppServer::Application::GetInstance()
        .SetHost("localhost")
        .SetPort(8000)
        .Run();
    return 0;
}



