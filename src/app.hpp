/*
 * app.hpp
 *
 * Created by Ruibin.Chow on 2022/01/23.
 * Copyright (c) 2022年 Ruibin.Chow All rights reserved.
 */

#ifndef APP_HPP
#define APP_HPP

#include <memory>
#include <mutex>
#include <thread>
#include <string>
#include <asio/asio.hpp>
#include <oatpp/web/server/HttpConnectionHandler.hpp>
#include <oatpp/network/Server.hpp>
#include <oatpp/network/tcp/server/ConnectionProvider.hpp>

#include "log/log_manager.h"
#include "handler/hello_handler.h"

namespace OatppServer {

class Application
{
public:
    explicit Application() = default;
    ~Application() = default;

    static Application& GetInstance() {
        static Application *application = nullptr;
        static std::once_flag flag;
        std::call_once(flag, []() {
            if (application == nullptr) {
                application = new Application();
                application->router_ = oatpp::web::server::HttpRouter::createShared();
                application->Initialize();
            }
        });
        
        return *application;
    }
    
public:
    Application& SetHost(const std::string& host) {
        host_ = host;
        return *this;
    }
    
    Application& SetPort(unsigned int port) {
        port_ = port;
        return *this;
    }
    
    void Run() {
        oatpp::base::Environment::init();
        auto connectionHandler = oatpp::web::server::HttpConnectionHandler::createShared(router_);
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
    
    void RegisterRouteForGetMethod(const std::string& routeString,
                                   std::shared_ptr<oatpp::web::server::HttpRequestHandler> handler) {
        router_->route("GET", routeString, handler);
    }
    
    void RegisterRouteForPostMethod(const std::string& routeString,
                                    std::shared_ptr<oatpp::web::server::HttpRequestHandler> handler) {
        router_->route("POST", routeString, handler);
    }
    
private:
    void Initialize() {
        std::shared_ptr<HelloHandler> helloHandler = std::make_shared<HelloHandler>();
        this->RegisterRouteForGetMethod("/", helloHandler);
        this->RegisterRouteForGetMethod("/hello", helloHandler);
    }
    
private:
    std::string host_;
    unsigned int port_;
    std::shared_ptr<oatpp::web::server::HttpRouter> router_;
};

}
#endif /* !APP_HPP */
