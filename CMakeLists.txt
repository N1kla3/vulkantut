cmake_minimum_required(VERSION 3.19)
project(vulk)

set(CMAKE_CXX_STANDARD 17)

find_package(Vulkan REQUIRED)

add_subdirectory(glfw-3.3.4)
add_subdirectory(glm)

add_executable(vulk main.cpp Application.cpp Application.h)
target_compile_definitions(vulk PRIVATE VK_USE_PLATFORM_WIN32_KHR)
target_link_libraries(vulk Vulkan::Vulkan glfw glm::glm)