/*
 *
 * main.cc
 *
 * Created by Ruibin.Chow on 2022/01/22.
 * Copyright (c) 2022年 Ruibin.Chow All rights reserved.
 */

#include <lua/lua.hpp>
#include "app.h"

int main()
{
    OatppServer::Application::GetInstance()
        .SetHost("localhost")
        .SetPort(8000)
        .SetTcpPort(9806)
        .SetUdpPort(5566)
        .Run();
    return 0;
}



