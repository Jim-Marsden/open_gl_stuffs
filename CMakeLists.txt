cmake_minimum_required(VERSION 3.20)
project(open_gl_stuffs)

set(CMAKE_CXX_STANDARD 20)


include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

set(imgui_backend
        imgui_backend/imgui_impl_glfw.cpp
        imgui_backend/imgui_impl_glfw.h

        imgui_backend/imgui_impl_opengl3.cpp
        imgui_backend/imgui_impl_opengl3.h
        )

add_executable(open_gl_stuffs main.cpp imgui_mgr.cpp imgui_mgr.h ${imgui_backend})
target_link_libraries(open_gl_stuffs ${CONAN_LIBS})
