//
// Created by kolya on 8/18/2021.
//
#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <cstdlib>

class Application {

public:
    void run()
    {
        initVulkan();
        initWindow();
        mainLoop();
        cleanup();
    }

private:
    void initVulkan();

    void initWindow();

    void mainLoop();

    void cleanup();

    GLFWwindow* m_Window = nullptr;

    const uint32_t WIDTH = 800;
    const uint32_t HEIGHT = 600;
};

