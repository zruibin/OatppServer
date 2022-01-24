/*
 *
 * udp_socket.cc
 *
 * Created by Ruibin.Chow on 2022/01/23.
 * Copyright (c) 2022年 Ruibin.Chow All rights reserved.
 */

#include "udp_socket.h"
#include <pthread.h>
#include <thread>
#include <future>
#include <string>
#include <asio/asio.hpp>
#include "log/log_manager.h"

namespace OatppServer {
namespace socket {

using asio::ip::udp;

class UDPSocket
{
public:
    UDPSocket(asio::io_context& io_context, short port)
    : socket_(io_context, udp::endpoint(udp::v4(), port)) {
        do_receive();
    }
    
    void do_receive() {
        socket_.async_receive_from(asio::buffer(data_, max_length),
                                   sender_endpoint_,
                                   [this](asio::system_error ec, std::size_t bytes_recvd)
                                   {
            if (!ec.code() && bytes_recvd > 0) {
                do_send(bytes_recvd);
            } else {
                do_receive();
            }
        });
        Log(INFO) << "UDPSocket receive_from data:" << std::string(data_);
    }
    
    void do_send(std::size_t length) {
        Log(INFO) << "UDPSocket send_to data:" << std::string(data_);
        socket_.async_send_to(
                              asio::buffer(data_, length), sender_endpoint_,
                              [this](asio::system_error /*ec*/, std::size_t /*bytes_sent*/) {
                                  do_receive();
                                  memset(data_,'\0',sizeof(data_));
                              });
    }
    
private:
    udp::socket socket_;
    udp::endpoint sender_endpoint_;
    enum { max_length = 4096 }; //长度需要关注
    char data_[max_length];
};

static void runUDPSocket(short port)
{
    pthread_setname_np("UDPSocket Threading.");
    Log(INFO) << "UDPSocket run on port:" << port;
    try {
        asio::io_context io_context;
        UDPSocket s(io_context, port);
        io_context.run();
        Log(INFO) << "UDPSocket run after.";
    } catch (std::exception& e) {
        Log(ERROR) << "UDPSocket Exception: " << e.what();
    }
}

void runAsynUDPSocket(short port)
{
    Log(INFO) << "UDPSocket run detach.";
    std::thread t(runUDPSocket, port);
    t.detach();
    Log(INFO) << "UDPSocket run detach end.";
}

}
}


/*
 python测试脚本：

# -*- coding: utf-8 -*-

import socket
import time

#client 发送端
client_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
PORT = 5566

while True:
      start = time.time()  #获取当前时间
      print(time.strftime('%Y-%m-%d %H:%M:%S',time.localtime(start)))  #以指定格式显示当前时间
      msg=raw_input("请输入要发送的内容：")
      server_address = ("localhost", PORT)  # 接收方 服务器的ip地址和端口号
      client_socket.sendto(msg, server_address) #将msg内容发送给指定接收方
      now = time.time() #获取当前时间
      run_time = now-start #计算时间差，即运行时间
      print(time.strftime('%Y-%m-%d %H:%M:%S',time.localtime(now)))
      print("run_time: %d seconds\n" %run_time)

      receive_data, client = client_socket.recvfrom(1024)
      print(time.strftime('%Y-%m-%d %H:%M:%S',time.localtime(now))) #以指定格式显示时间
      print("接收到来自服务端%s,发送：%s\n" % (client, receive_data))  #打印接收的内容
 
*/
