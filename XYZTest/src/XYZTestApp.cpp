
#include "CinderImGui.h"
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

//std::vector<string> xyz(3); //should probably convert to float
//std::vector<float> xyz(3); //should probably convert to float
std::vector<float> xyz(3);
float dx = 0.0f;
float vx = 0.0f;
float dy = 0.0f;
float vy = 0.0f;
float dz = 0.0f;
float vz = 0.0f;

class XYZTestApp : public App {
public:
    void setup() override;
    void update() override;
    void draw() override;
    void prepareSettings(Settings *settings);

private:
    ci::Font mFont;
    std::vector<std::string> mText;
    ci::gl::TextureRef mTexture;

    float mFrameRate;
    bool mFullScreen;
    std::shared_ptr<UdpServer> _udpServer;
};

void XYZTestApp::prepareSettings(Settings *settings)
{
    settings->setWindowSize(800, 600);
    settings->setFrameRate(60);
}

void XYZTestApp::setup()
{
    ui::initialize();
    _udpServer = UdpServer::instance(io_service());

    _udpServer->open(10552);

    _udpServer->getOnDataReceivedSignal().connect([](std::string buff) {
        CI_LOG_I("Data signal emitted");

        //Populate xyz vector
        std::vector<std::string> substrings = split(buff, ",");
        for (int i = 0; i < 3; i++) {
            //            cout << substrings.back();
            xyz[i] = std::stof(substrings.back());
            substrings.pop_back();
        }

        //Turn xyz accel into position data
        //        for (int i = 1; i < n; i++)
    });
}

void XYZTestApp::update()
{
    // Print xyz vector
    for (auto i : xyz) {
        cout << i << endl;
    }
    //draw gui
    ui::ScopedWindow window("Accelerometer Measurements");

    stringstream ss;
    ss << std::fixed << std::setprecision(3) << xyz.front() << "m/s^2";
    std::vector<float> x(1);
    x[0] = xyz[0];
    std::vector<float> y(1);
    y[0] = xyz[1];
    std::vector<float> z(1);
    z[0] = xyz[2];
    ui::PlotLines("X Accel", x.data(), x.size(), 0, ss.str().c_str(), -3.0f, 3.0f);
    ui::PlotLines("Y Accel", y.data(), y.size(), 0, ss.str().c_str(), -3.0f, 3.0f);
    ui::PlotLines("Z Accel", z.data(), z.size(), 0, ss.str().c_str(), -3.0f, 3.0f);
}

void XYZTestApp::draw()
{
    gl::clear(Colorf::black());
    //Put measurements on screen
    //Simple accel vector plot
    gl::color(Color(0.0f, 1.0f, 0.0f));
    float headLength = 8.0f;
    float headRadius = 6.0f;

    int mag = 500;
    int xorigin = 600;
    int yorigin = 600;
    gl::drawVector(ivec3(xorigin, yorigin, 0), ivec3(xorigin + mag * xyz[2], yorigin + mag * xyz[1], 0), headLength, headRadius);
}

CINDER_APP(XYZTestApp, RendererGl)
