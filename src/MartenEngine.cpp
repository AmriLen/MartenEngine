#include "MartenUtils/MartenLogger.h"
#include "MartenData.h"
#include <iostream>
#include <fstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

int main() {
    std::cout << "Hello, " << MartenData::NAME << "!\n" << std::endl;
    std::cout << "Version: " << MartenData::VERSION_PREFIX << ": " << MartenData::VERSION << std::endl;
    std::cout << "Author: " << MartenData::AUTHOR << std::endl;

    std::cout << std::endl;
    std::cout << "MartenLogger test:" << std::endl;
    MartenLogger::Initialize();
    MartenLogger::SECS("This is a successful security message.");
    MartenLogger::INFO(MartenData::NAME);
    MartenLogger::WARN("This is a warning message.");
    MartenLogger::ERR("This is an error message.");
    MartenLogger::CRIT("This is a critical message.");
    std::cout << std::endl;
    std::cout << "LOG:" << std::endl;


    if (glfwInit()) {
        MartenLogger::SECS("GLFW initialized successfully");
    } else {
        MartenLogger::CRIT("Failed to initialize GLFW");
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window;
    window = glfwCreateWindow(1280, 720, "Marten Engine", NULL, NULL);
    if (window != NULL) {
        MartenLogger::SECS("GLFW window created successfully");
    } else {
        MartenLogger::CRIT("Failed to open GLFW window");
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        MartenLogger::SECS("GLAD initialized successfully");
    } else {
        MartenLogger::CRIT("Failed to initialize GLAD");
        return -1;
    }

    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);
    });

    while(!glfwWindowShouldClose(window)) {
        glfwSwapBuffers(window);
        glfwPollEvents();    
    }

    glfwTerminate();

    MartenLogger::Shutdown();
    system("pause");
    return 0;
}