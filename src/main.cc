/*
 *
 * main.cc
 *
 * Created by Ruibin.Chow on 2022/01/22.
 * Copyright (c) 2022年 Ruibin.Chow All rights reserved.
 */


#include <memory>
#include <iostream>

extern "C" {
#include <kcp/ikcp.h>
#include <lua/lua.h>
}
#include <spawn.h>
#include <pthread.h>

#include <asio/asio.hpp>
#include <oatpp/web/server/HttpConnectionHandler.hpp>
#include <oatpp/network/Server.hpp>
#include <oatpp/network/tcp/server/ConnectionProvider.hpp>


extern "C" {

void forkDeamon()
{
    pid_t  nPid;
    posix_spawnattr_t X;
    posix_spawn_file_actions_t Y;
    char* argv[] = {"/bin/ps", "-lf", NULL};
    char* envp[] = {"PROCESS=2"};
  
    posix_spawnattr_init(&X);
    posix_spawn_file_actions_init(&Y);
  
    posix_spawn(&nPid, "/bin/ps", &Y, &X, argv, envp);
  
    std::cout << "spawned PID" << nPid << std::endl;
}

}

class Handler : public oatpp::web::server::HttpRequestHandler
{
public:
    std::shared_ptr<OutgoingResponse> handle(const std::shared_ptr<IncomingRequest>& request) override {
        std::cout << request << std::endl;
        return ResponseFactory::createResponse(Status::CODE_200, "Hello World! From Oatpp -> Ruibin.Chow");
    }
};

void run()
{
    auto router = oatpp::web::server::HttpRouter::createShared();

    std::shared_ptr<Handler> handler = std::make_shared<Handler>();
    router->route("GET", "/hello", handler);
    router->route("GET", "/", handler);
    
    auto connectionHandler = oatpp::web::server::HttpConnectionHandler::createShared(router);
    auto connectionProvider = oatpp::network::tcp::server::ConnectionProvider::createShared({"localhost", 8000, oatpp::network::Address::IP_4});
    oatpp::network::Server server(connectionProvider, connectionHandler);
    
    OATPP_LOGI("MyApp", "Server running on port %s", connectionProvider->getProperty("port").getData());

    server.run();
}

int main()
{
    // forkDeamon();
    oatpp::base::Environment::init();
    run();
    oatpp::base::Environment::destroy();
    
    return 0;
}





