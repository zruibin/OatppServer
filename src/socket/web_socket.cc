/*
 *
 * web_socket.cc
 *
 * Created by Ruibin.Chow on 2022/01/23.
 * Copyright (c) 2022年 Ruibin.Chow All rights reserved.
 */

#include "web_socket.h"
#include <thread>
#include <future>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include "log/logging.h"
#include "platform/platform.h"

namespace OatppServer {
namespace socket {


typedef websocketpp::server<websocketpp::config::asio> server;
using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;
using ErrorCode = websocketpp::lib::error_code;

// pull out the type of messages sent by our config
typedef server::message_ptr message_ptr;

// Define a callback to handle incoming messages
void onMessage(server* s, websocketpp::connection_hdl hdl, message_ptr msg)
{
//    Log(INFO) << "on_message called with hdl: " << hdl.lock().get();
    Log(INFO) << "message: " << msg->get_payload();

    // check for a special command to instruct the server to stop listening so
    // it can be cleanly exited.
    if (msg->get_payload() == "stop-listening") {
        s->stop_listening();
        return;
    }

    try {
        s->send(hdl, msg->get_payload(), msg->get_opcode());
    } catch (websocketpp::exception const & e) {
        Log(INFO) << "Echo failed because: "
                  << "(" << e.what() << ")";
    }
}

void onOpen(server* s, websocketpp::connection_hdl hdl) {
    server::connection_ptr con = s->get_con_from_hdl(hdl);
    std::string client_ip_port = con->get_remote_endpoint();
    Log(INFO) << "onOpen clinet port: " << client_ip_port;
}

void onClose(server* s, websocketpp::connection_hdl hdl) {
    server::connection_ptr con = s->get_con_from_hdl(hdl);
    std::string client_ip_port = con->get_remote_endpoint();
    Log(INFO) << "onClose clinet port: " << client_ip_port;
}

static void runWebSocket(short port)
{
    platform::thread_set_name("WebSocket Threading.");
    // Create a server endpoint
    server echo_server;
    Log(INFO) << "WebSocket run on port:" << port;
    try {
        // Set logging settings
        echo_server.set_access_channels(websocketpp::log::alevel::all);
        echo_server.clear_access_channels(websocketpp::log::alevel::frame_payload);

        // Initialize Asio
        echo_server.init_asio();

        // Register our message handler
        echo_server.set_message_handler(bind(&onMessage, &echo_server, _1, _2));
        echo_server.set_open_handler(bind(&onOpen, &echo_server, _1));
        echo_server.set_close_handler(bind(&onClose, &echo_server, _1));
        

        // Listen on port 9002
        echo_server.listen(port);

        // Start the server accept loop
        echo_server.start_accept();

        // Start the ASIO io_service run loop
        echo_server.run();
    } catch (websocketpp::exception const & e) {
        Log(ERROR) << e.what();
    } catch (...) {
        Log(ERROR) << "other exception";
    }
}

void runAsynWebSocket(short port)
{
    Log(INFO) << "WebSocket run detach.";
//    std::async(runWebSocket, port);
    std::thread t(runWebSocket, port);
    t.detach();
    Log(INFO) << "WebSocket run detach end.";
}


}
}
