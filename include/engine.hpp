#pragma once

#include "glad.h"
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <iostream>
#include <unordered_map>

#include "global.hpp"
#include "shader.hpp"
#include "renderer.hpp"
#include "camera.hpp"
#include "input.hpp"
#include "debugger.hpp"

namespace sariel
{
    class Engine
    {
    public:
        Engine(int width, int height, const char *title);
        Engine(const Engine &) = delete;
        Engine &operator=(const Engine &) = delete;
        ~Engine();

        void selectCamera(Camera &camera);
        void selectRenderer(Renderer &renderer);
        void selectInput(InputManager &input);
        void selectDebugger(Debugger &debugger);

        void beginFrame();
        void endFrame();

        static void framebuffer_size_callback(GLFWwindow *window, int width, int height);
        static void mouse_callback(GLFWwindow *window, double xpos, double ypos);

        f32 calculateFPS();

        bool isRunning();

        int WIDTH;
        int HEIGHT;

        bool renderDebugger = false;

    private:
        GLFWwindow *window;
        Camera *activeCamera = nullptr;
        Renderer *activeRenderer = nullptr;
        InputManager *activeInput = nullptr;
        Debugger *activeDebugger = nullptr;

        listener engineListener;
        void processInput();

        f32 lastFrame = 0.0f;
        f32 deltaTime = 0.0f;
    };
}