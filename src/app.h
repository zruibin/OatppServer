/*
 * app.h
 *
 * Created by Ruibin.Chow on 2022/01/23.
 * Copyright (c) 2022年 Ruibin.Chow All rights reserved.
 */

#ifndef APP_HPP
#define APP_HPP

#include <string>

namespace OatppServer {

class ApplicationImpl;

class Application
{
public:
    explicit Application() = default;
    ~Application() = default;

    static Application& GetInstance();
    
public:
    Application& SetHost(const std::string& host);
    Application& SetPort(unsigned int port);
    Application& SetTcpPort(unsigned int port);
    Application& SetUdpPort(unsigned int port);
    void Run();
private:
    void Initialize();
    
private:
    std::string host_;
    unsigned int port_;
    unsigned int tcp_port_;
    unsigned int udp_port_;
    ApplicationImpl *impl;
};

}
#endif /* !APP_HPP */
