
#include <imgui.h>
#include "imgui_backend/imgui_impl_glfw.h"
#include "imgui_backend/imgui_impl_opengl3.h"
#include "imgui_mgr.h"
//#include "imgui_impl_glfw.h"
//#include "imgui_impl_opengl3.h"

#include <GL/glew.h> // included by imgui_mgr
#include <GLFW/glfw3.h> // included by imgui_mgr

//#include <imgui.h>
#include <iostream>
#include <string>

#include <format>

#include <array>

GLFWwindow* create_opengl();
void destroy_opengl(GLFWwindow* window);
int main(){
    struct glfw_lifetime_manager_cls{GLFWwindow* window; glfw_lifetime_manager_cls() : window{create_opengl()}{} ~glfw_lifetime_manager_cls(){destroy_opengl(window);}} glfw_lifetime_manager;

    std::array vertices{
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.0f,  0.5f, 0.0f
    };

    auto const vert_shader = R"VertShader(

#version 330 core
layout (location = 0) in vec3 aPos;

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
)VertShader";

    unsigned VBO{};
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size(), vertices.data(), GL_STATIC_DRAW);


    while(!glfwWindowShouldClose(glfw_lifetime_manager.window)){
        glfwPollEvents();

    }
}

void PrintOpenGLErrors(char const* const Function, char const* const File, int const Line)
{
    bool Succeeded = true;

    GLenum Error = glGetError();
    if (Error!=GL_NO_ERROR) {
        char const* ErrorString = (char const*) gluErrorString(Error);
        if (ErrorString)
            std::cerr << ("OpenGL Error in %s at line %d calling function %s: '%s'", File, Line, Function, ErrorString)
                      << std::endl;
        else
            std::cerr
                    << ("OpenGL Error in %s at line %d calling function %s: '%d 0x%X'", File, Line, Function, Error, Error)
                    << std::endl;
    }
}

void PrintShaderInfoLog(GLint const Shader)
{
    int InfoLogLength = 0;
    int CharsWritten = 0;

    glGetShaderiv(Shader, GL_INFO_LOG_LENGTH, &InfoLogLength);

    if (InfoLogLength>0) {
        GLchar* InfoLog = new GLchar[InfoLogLength];
        glGetShaderInfoLog(Shader, InfoLogLength, &CharsWritten, InfoLog);
        std::cout << "Shader Info Log:" << std::endl << InfoLog << std::endl;
        delete[] InfoLog;
    }
}

GLFWwindow* create_opengl(){
    GLFWwindow* window{nullptr};
    if (!glfwInit())
        return nullptr;

    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return nullptr;
    }
    glfwMakeContextCurrent(window);

    GLenum err = glewInit();
    if (GLEW_OK!=err) {
        std::cerr << "Error: " << glewGetErrorString(err) << std::endl;
        glfwTerminate();
        return nullptr;
    }
    return window;
}
void destroy_opengl(GLFWwindow* window){
    glfwDestroyWindow(window);
    glfwTerminate();
}

int main2()
{
    GLFWwindow* window;

    if (!glfwInit())
        return -1;

    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    GLenum err = glewInit();
    if (GLEW_OK!=err) {
        std::cerr << "Error: " << glewGetErrorString(err) << std::endl;
        glfwTerminate();
        return -1;
    }

    char const* VertexShaderSource = R"GLSL(
		#version 150
		in vec2 position;
		void main()
		{
			gl_Position = vec4(position, 0.0, 1.0);
		}
	)GLSL";

    char const* FragmentShaderSource = R"GLSL(
		#version 150
		out vec4 outColor;
		void main()
		{
			outColor = vec4(1.0, 1.0, 1.0, 1.0);
		}
	)GLSL";

    GLfloat const Vertices[] = {
            0.0f, 0.5f,
            0.5f, -0.5f,
            -0.5f, -0.5f
    };

    GLuint const Elements[] = {
            0, 1, 2
    };

    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    GLuint EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Elements), Elements, GL_STATIC_DRAW);

    GLint Compiled;
    GLuint VertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(VertexShader, 1, &VertexShaderSource, NULL);
    glCompileShader(VertexShader);
    glGetShaderiv(VertexShader, GL_COMPILE_STATUS, &Compiled);
    if (!Compiled) {
        std::cerr << "Failed to compile vertex shader!" << std::endl;
        PrintShaderInfoLog(VertexShader);
    }

    GLuint FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(FragmentShader, 1, &FragmentShaderSource, NULL);
    glCompileShader(FragmentShader);
    glGetShaderiv(FragmentShader, GL_COMPILE_STATUS, &Compiled);
    if (!Compiled) {
        std::cerr << "Failed to compile fragment shader!" << std::endl;
        PrintShaderInfoLog(FragmentShader);
    }

    GLuint ShaderProgram = glCreateProgram();
    glAttachShader(ShaderProgram, VertexShader);
    glAttachShader(ShaderProgram, FragmentShader);
    glBindFragDataLocation(ShaderProgram, 0, "outColor");
    glLinkProgram(ShaderProgram);
    glUseProgram(ShaderProgram);

    GLint PositionAttribute = glGetAttribLocation(ShaderProgram, "position");
    glEnableVertexAttribArray(PositionAttribute);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(PositionAttribute, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        switch (action) {
        case 0:
            std::cout << "Released " << key << "\n";
            return;
        case 1:
            std::cout << "Pressed " << key << "\n";
            return;
        case 2:
            return;
        default:
            std::cout << "???\n";
            return;
        }
        //    std::cout << "key: " << key << "scancode: " << scancode << "action:" << action << "mods: " << mods << '\n';

    });


    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    int display_w{}, display_h{};
    ogll::imgui_mgr mgr(window, "#version 130", false);



    while (!glfwWindowShouldClose(window))
    {
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        glfwPollEvents();

        mgr();
        //glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // Cleanup
//    ImGui_ImplOpenGL3_Shutdown();
//    ImGui_ImplGlfw_Shutdown();
//    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;


    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

        // feed inputs to dear imgui, start new frame

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteProgram(ShaderProgram);
    glDeleteShader(FragmentShader);
    glDeleteShader(VertexShader);

    glDeleteBuffers(1, &EBO);
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);

    glfwTerminate();
    return 0;
}
