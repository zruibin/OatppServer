/*
 * app.cc
 *
 * Created by Ruibin.Chow on 2022/01/23.
 * Copyright (c) 2022年 Ruibin.Chow All rights reserved.
 */

#include "app.h"
#include <memory>
#include <mutex>
#include <thread>
#include <asio/asio.hpp>
#include <oatpp/web/server/HttpConnectionHandler.hpp>
#include <oatpp/network/Server.hpp>
#include <oatpp/network/tcp/server/ConnectionProvider.hpp>

#include "log/log_manager.h"
#include "handler/hello_handler.h"
#include "socket/web_socket.h"

namespace OatppServer {

class ApplicationImpl
{
public:
    void RegisterRouteForGetMethod(const std::string& routeString,
                                   std::shared_ptr<oatpp::web::server::HttpRequestHandler> handler);
    void RegisterRouteForPostMethod(const std::string& routeString,
                                    std::shared_ptr<oatpp::web::server::HttpRequestHandler> handler);
public:
    std::shared_ptr<oatpp::web::server::HttpRouter> router_;
};

Application& Application::GetInstance() {
    static Application *application = nullptr;
    static std::once_flag flag;
    std::call_once(flag, []() {
        if (application == nullptr) {
            Log(INFO) << "Application Init.";
            application = new Application();
            application->impl = new ApplicationImpl();
            application->Initialize();
        }
    });
    
    return *application;
}

Application& Application::SetHost(const std::string& host) {
    host_ = host;
    return *this;
}

Application& Application::SetPort(unsigned int port) {
    port_ = port;
    return *this;
}

Application& Application::SetTcpPort(unsigned int port)
{
    tcp_port_ = port;
    return *this;
}

Application& Application::SetUdpPort(unsigned int port)
{
    udp_port_ = port;
    return *this;
}

void Application::Run()
{
    socket::runAsynWebSocket(this->tcp_port_);
    
    oatpp::base::Environment::init();
    auto connectionHandler = oatpp::web::server::HttpConnectionHandler::createShared(this->impl->router_);
    auto connectionProvider = oatpp::network::tcp::server::ConnectionProvider::createShared({host_.c_str(),
        static_cast<v_uint16>(port_),
        oatpp::network::Address::IP_4});
    oatpp::network::Server server(connectionProvider, connectionHandler);
    
    const void* p = connectionProvider->getProperty("port").getData();
    Log(INFO) << "OatppServer " << "running on port "
    << static_cast<const char*>(connectionProvider->getProperty("port").getData());
    Log(DEBUG) << "DEBUG...";
    server.run();
    
    oatpp::base::Environment::destroy();
}

void Application::Initialize()
{
    this->impl->router_ = oatpp::web::server::HttpRouter::createShared();
    std::shared_ptr<HelloHandler> helloHandler = std::make_shared<HelloHandler>();
    this->impl->RegisterRouteForGetMethod("/", helloHandler);
    this->impl->RegisterRouteForGetMethod("/hello", helloHandler);
}


void ApplicationImpl::RegisterRouteForGetMethod(const std::string& routeString,
                                                std::shared_ptr<oatpp::web::server::HttpRequestHandler> handler)
{
    router_->route("GET", routeString, handler);
}

void ApplicationImpl::RegisterRouteForPostMethod(const std::string& routeString,
                                                 std::shared_ptr<oatpp::web::server::HttpRequestHandler> handler)
{
    router_->route("POST", routeString, handler);
}

}

