//
//  UdpServer.h
//  XYZTest
//
//  Created by Laura Asher on 2/17/16.
//
//

#pragma once

#include "asio/asio.hpp"
#include "signals.h"

namespace soso {

class UdpServer : public std::enable_shared_from_this<UdpServer> {
    using UdpServerRef = std::shared_ptr<UdpServer>;
    using socketRef = std::shared_ptr<asio::io_service>;

public:
    static UdpServerRef instance(asio::io_service &io_service);

    void open(int port);

    Signal &getOnConnectSignal() { return onConnect; }
    BufferSignal &getOnDataReceivedSignal() { return onDataReceived; }
protected:
    UdpServer(asio::io_service &io);

    void waitForData();

    Signal onConnect;
    BufferSignal onDataReceived;
    Signal onError;

    std::shared_ptr<asio::io_service> _service;
    std::shared_ptr<asio::ip::udp::socket> _socket;

    char recv_buffer[1024];
    std::shared_ptr<asio::ip::udp::endpoint> receiverEndpoint;
};
}