#pragma once

#include <GLFW/glfw3.h>

#include "global.hpp"
#include "engine_key.hpp"

#include <iostream>
#include <array>
#include <unordered_map>
#include <vector>
#include <algorithm>

#define SARIEL_MOUSE 0
#define SARIEL_KEYBOARD 1

namespace sariel
{
    struct listener
    {
        bool isFocusedMouse;
        bool isFocusedKeyboard;
        bool isUsingMouse;
        bool isUsingMouseOffset;

        double mouseX;
        double mouseY;

        float mouseOffsetX;
        float mouseOffsetY;

        std::unordered_map<uint, bool> boundToggleKeys;
        std::unordered_map<uint, bool> boundHoldKeys;

        listener(bool mouse = false, bool mouseOffset = true);
        void setFocus(uint medium, bool foc);
        void bindKey(uint key, bool toggle);
    };

    class InputManager
    {
    public:
        InputManager() = default;
        ~InputManager();

        void addListener(listener *lis);
        void removeListener(listener *lis);
        void setListenerFocus(listener *lis, uint medium, bool foc);

        void forwardToListeners();

        // methods accessible only to an Engine object with an EngineKey
        void engineRetrieveMouse(double x, double y, EngineKey);
        void setWindow(GLFWwindow *window, EngineKey);

    private:
        GLFWwindow *window;
        std::unordered_map<int, bool> toggleStates;
        std::vector<listener *> listeners;

        bool getKeyToggle(uint key);
        bool getKeyHold(uint key);

        double getMouseX();
        double getMouseY();

        f32 getMouseOffsetX();
        f32 getMouseOffsetY();

        double currentMouseX;
        double currentMouseY;

        bool isLinked();
    };
}