//
// Created by kolya on 8/18/2021.
//
#include "Application.h"
#include <vector>

#ifdef NDEBUG
    const bool enableValidationLayers = false;
#else
    const bool enableValidationLayers = true;
#endif

namespace
{
    const std::vector<const char*> ValidationLayers{ "VK_LAYER_KHRONOS_validation" };
}

void Application::initVulkan()
{
    createInstance();
    setupDebugMessenger();
}

void Application::mainLoop()
{
    while (!glfwWindowShouldClose(m_Window))
    {
        glfwPollEvents();
    }
}

void Application::cleanup()
{
    if (enableValidationLayers)
    {
        DestroyDebugUtilsMessengerEXT(m_Instance, debugMessenger, nullptr);
    }

    vkDestroyInstance(m_Instance, nullptr);

    glfwDestroyWindow(m_Window);

    glfwTerminate();
}

void Application::initWindow()
{
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    m_Window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
}

void Application::createInstance()
{
    VkApplicationInfo app_info{};
    app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app_info.pApplicationName = "Triangle";
    app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    app_info.pEngineName = "No Engine";
    app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    app_info.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo create_info{};
    create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    create_info.pApplicationInfo = &app_info;
    if (enableValidationLayers)
    {
        create_info.enabledLayerCount = static_cast<uint32_t>(ValidationLayers.size());
        create_info.ppEnabledLayerNames = ValidationLayers.data();
    }
    else
    {
        create_info.enabledLayerCount = 0;
    }
    auto extensions_ = getRequiredExtensions();
    create_info.enabledExtensionCount = static_cast<uint32_t>(extensions_.size());
    create_info.ppEnabledExtensionNames = extensions_.data();

    create_info.enabledLayerCount = 0;

    VkResult result = vkCreateInstance(&create_info, nullptr, &m_Instance);
    if (result != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to craete vk instance");
    }

    uint32_t ext_count = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &ext_count, nullptr);

    std::vector<VkExtensionProperties> extensions(ext_count);
    vkEnumerateInstanceExtensionProperties(nullptr, &ext_count, extensions.data());
    std::cout << "Available extensions \n";
    for (const auto& ext : extensions)
    {
        std::cout << '\t' << ext.extensionName << '\n';
    }

    if (enableValidationLayers && !checkValidationLayerSupport())
    {
        throw  std::runtime_error("validation layers requested, but not available");
    }
}

bool Application::checkValidationLayerSupport()
{
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    bool layer_found = false;

    for (const char* layerName : ValidationLayers)
    {
        for (const auto& layerProperty : availableLayers)
        {
            if (strcmp(layerName, layerProperty.layerName) == 0)
            {
                layer_found = true;
                break;
            }
        }
        if (!layer_found)
        {
            return false;
        }
    }
    return true;
}

std::vector<const char *> Application::getRequiredExtensions() {
    uint32_t glfwExtensionsCount = 0;
    const char** glfwExtensions;

    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionsCount);
    std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionsCount);
    if (enableValidationLayers)
    {
        extensions.push_back("VK_EXT_debug_utils");
    }
    return extensions;
}

void Application::setupDebugMessenger()
{
    if (!enableValidationLayers) return;

    VkDebugUtilsMessengerCreateInfoEXT create_info{};
    create_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    create_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT;
    create_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT;
    create_info.pfnUserCallback = debugCallback;
    create_info.pUserData = nullptr;

    if (CreateDebugUtilsMessengerEXT(m_Instance, &create_info, nullptr, &debugMessenger) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to setup debug messenger");
    }
}
