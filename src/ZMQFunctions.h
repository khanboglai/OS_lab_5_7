#pragma once
#include <bits/stdc++.h>
#include <zmq.hpp>


const int MAIN_PORT = 4040;

void send_message(zmq::socket_t &socket, const std::string &msg) {
    zmq::message_t message(msg.size());
    memcpy(message.data(), msg.c_str(), msg.size());
    socket.send(message, zmq::send_flags::none);
}

std::string receive_message(zmq::socket_t &socket) { // получить сообщение
    zmq::message_t message;
    zmq::recv_flags flags = zmq::recv_flags::none;

    int chars_read;
    try {
        (void)socket.recv(message, zmq::recv_flags::none);
        chars_read = message.size();
    }
    catch (...) {
        chars_read = 0;
    }
    if (chars_read == 0) {
        throw -1;
    }
    std::string received_msg(static_cast<char*>(message.data()), message.size());
    return received_msg;
}

void connect(zmq::socket_t &socket, int port) {
    std::string address = "tcp://127.0.0.1:" + std::to_string(port);
    socket.connect(address);
}

void disconnect(zmq::socket_t &socket, int port) {
    std::string address = "tcp://127.0.0.1:" + std::to_string(port);
    socket.disconnect(address);
}

int bind(zmq::socket_t &socket, int id) { // привязать
    int port = MAIN_PORT + id;
    std::string address = "tcp://127.0.0.1:" + std::to_string(port);
    
    while(1){
        try{
            socket.bind(address);
            break;
        }
        catch(...){
            port++;
        }
    }
    return port;
}

void unbind(zmq::socket_t &socket, int port) { // отвязать
    std::string address = "tcp://127.0.0.1:" + std::to_string(port);
    socket.unbind(address);
}
