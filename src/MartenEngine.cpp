#include "MartenCore/Application.hpp"
#include "MartenData.h"
#include <iostream>
#include <string>

class MyApp : public MartenEngine::Application {
    virtual void on_update() override {
        // Application update logic here
    }
    int frameCount = 0;
};

int main() {

    std::cout << 
    R"(   __    __   ______   ______  ______  ______   __   __     )" << "\n" <<
    R"(  /\ "-./  \ /\  __ \ /\  == \/\__  _\/\  ___\ /\ "-.\ \    )" << "\n" <<
    R"(  \ \ \-./\ \\ \  __ \\ \  __<\/_/\ \/\ \  __\ \ \ \-.  \   )" << "\n" <<
    R"(   \ \_\ \ \_\\ \_\ \_\\ \_\ \_\ \ \_\ \ \_____\\ \_\\"\_\  )" << "\n" <<
    R"(    \/_/  \/_/ \/_/\/_/ \/_/ /_/  \/_/  \/_____/ \/_/ \/_/  )" << "\n" <<
    R"(   ______   __   __   ______   __   __   __   ______        )" << "\n" <<
    R"(  /\  ___\ /\ "-.\ \ /\  ___\ /\ \ /\ "-.\ \ /\  ___\       )" << "\n" <<
    R"(  \ \  __\ \ \ \-.  \\ \ \__ \\ \ \\ \ \-.  \\ \  __\       )" << "\n" <<
    R"(   \ \_____\\ \_\\"\_\\ \_____\\ \_\\ \_\\"\_\\ \_____\     )" << "\n" <<
    R"(    \/_____/ \/_/ \/_/ \/_____/ \/_/ \/_/ \/_/ \/_____/     )" << "\n" <<
    std::endl;

    std::cout << "By: " << ME_AUTHOR << std::endl << std::endl;

    std::cout << "Version: \t" << ME_VERSION_PREFIX << " : " << ME_VERSION << std::endl;
    std::cout << "Build Date: \t" << ME_BUILD_DATE << std::endl << std::endl;
    
    auto myApp = std::make_unique<MyApp>();
    int returnCode = myApp->start(1280, 720, "Marten Engine");

    std::cin.get();
    return returnCode;
}