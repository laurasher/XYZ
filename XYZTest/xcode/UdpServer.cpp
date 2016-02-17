//
//  UdpServer.cpp
//  XYZTest
//
//  Created by Laura Asher on 2/17/16.
//
//

#include "Cinder/Log.h"
#include "UdpServer.h"

using namespace ci;
using namespace std;
using namespace soso;
using asio::ip::udp;

using UdpServerRef = std::shared_ptr<UdpServer>;

UdpServer::UdpServer(asio::io_service &io)
    : _service(&io)
{
}

UdpServerRef UdpServer::instance(asio::io_service &io_service)
{
    auto server = UdpServerRef(new UdpServer(io_service));

    // Do we need to use shared from this?  Why?
    return server->shared_from_this();
}

void UdpServer::open(int port)
{
    _socket = make_shared<udp::socket>(*_service);

    receiverEndpoint = make_shared<udp::endpoint>(udp::v4(), port);
    asio::error_code errCode;

    // This is blocking.  TODO: Make this asynchronous
    _socket->open(udp::v4(), errCode);
    CI_LOG_I("Opened socket.");

    if (errCode) {
        CI_LOG_E("Problem opening socket.");
    }
    else {
        _socket->set_option(asio::socket_base::reuse_address(true));
        _socket->bind(udp::endpoint(udp::v4(), port), errCode);

        waitForData();
    }
}

void UdpServer::waitForData()
{
    CI_LOG_I("Waiting for data");

    _socket->async_receive_from(asio::buffer(recv_buffer), *receiverEndpoint, [this](asio::error_code ec, std::size_t bytes) {

        if (!ec) {
            CI_LOG_I("Got some data " << bytes);

            onDataReceived.emit(string(recv_buffer));
            waitForData();
        }
        else {
            CI_LOG_E("Problem getting data " << ec.message());
        }
    });
}