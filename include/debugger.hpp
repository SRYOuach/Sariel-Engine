#pragma once

#include "glad.h"
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <iostream>

#include "global.hpp"
#include "engine_key.hpp"

namespace sariel
{
    class Debugger
    {
    public:
        Debugger(const std::string &path, const std::string &font);
        Debugger(const Debugger &) = delete;
        Debugger &operator=(const Debugger &) = delete;
        ~Debugger();

        void linkEngine(GLFWwindow *window, EngineKey);

        void render(f32 fps);

        GLFWwindow *window;
        f32 cameraSpeed = 1.0f;
        f32 cameraSensitivity = 0.2f;
        f32 cameraFOV = 90.0f;
        bool closeEngine = false;
    };
}