//
// Created by james on 12/4/2021.
//

#ifndef OPEN_GL_STUFFS_IMGUI_MGR_H
#define OPEN_GL_STUFFS_IMGUI_MGR_H


#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string_view>

namespace ogll {
    class imgui_mgr {
    public:
        ~imgui_mgr();
        imgui_mgr() = delete;
        imgui_mgr(imgui_mgr const &) = delete; // Should be moved, not copied? Can't think of why coping this is a good idea.
        imgui_mgr(imgui_mgr &&) = default;

        imgui_mgr(GLFWwindow *window_in, std::string_view shader_version ,bool install_callback=true);

        imgui_mgr & operator=(imgui_mgr const &) = delete;
        imgui_mgr & operator=(imgui_mgr &&) = default;

        void operator()();

    private:
        GLFWwindow *window = nullptr;
        bool show_demo_window = false;
        bool show_another_window = false;
        ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

        int display_w{}, display_h{};

    };
} // namespace ogll
#endif //OPEN_GL_STUFFS_IMGUI_MGR_H
