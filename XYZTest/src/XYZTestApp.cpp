#include "UdpServer.h"
#include "array"
#include "asio/asio.hpp"
#include "cinder/Log.h"
#include "cinder/System.h"
#include "cinder/Utilities.h"
#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/params/Params.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using asio::ip::udp;
using namespace soso;

std::vector<string> xyz(3); //should probably convert to float

class XYZTestApp : public App {
public:
    void setup() override;
    void update() override;
    void draw() override;

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

        //Populate xyz vector
        std::vector<std::string> substrings = split(buff, ",");
        for (int i = 0; i < 3; i++) {
            xyz[i] = substrings.back();
            substrings.pop_back();
        }
    });
}

void XYZTestApp::update()
{
    // Print xyz vector
    for (auto i : xyz) {
        cout << i << endl;
    }
}

void XYZTestApp::draw()
{
    gl::clear(Colorf::black());
}

CINDER_APP(XYZTestApp, RendererGl)
