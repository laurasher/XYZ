#include "asio/asio.hpp"
#include "cinder/Log.h"
#include "cinder/System.h"
#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/params/Params.h"

#include "UdpServer.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using asio::ip::udp;
using namespace soso;

class XYZTestApp : public App {
public:
    void setup() override;
    void update() override;
    void draw() override;
    //    udp::socket *socket;
    //    udp::endpoint *receiver_endpoint;
    //    char recv_buffer[1024];

private:
    ci::Font mFont;
    std::vector<std::string> mText;
    ci::gl::TextureRef mTexture;

    float mFrameRate;
    bool mFullScreen;
    std::shared_ptr<UdpServer> _udpServer;
};

void XYZTestApp::setup()
{
    _udpServer = UdpServer::instance(io_service());

    _udpServer->open(10552);

    _udpServer->getOnDataReceivedSignal().connect([](std::string buff) {

        CI_LOG_I("Data signal emitted");

        cout << buff;

    });
}

void XYZTestApp::update()
{
}

void XYZTestApp::draw()
{
    gl::clear(Colorf::black());
}

CINDER_APP(XYZTestApp, RendererGl)
