#include "input.hpp"

// construct a simple listener,
sariel::listener::listener(bool mouse, bool mouseOffset) : isUsingMouse(mouse), isUsingMouseOffset(mouseOffset)
{
    isFocusedMouse = true;
    isFocusedKeyboard = true;
}

// set whether the listener is in focus by the input manager or not
void sariel::listener::setFocus(uint medium, bool foc)
{
    if (medium == SARIEL_KEYBOARD)
        isFocusedKeyboard = foc;
    else if (medium == SARIEL_MOUSE)
        isFocusedMouse = foc;
}

// bind a keyboard key as toggle or hold, use GLFW key values
void sariel::listener::bindKey(uint key, bool toggle)
{
    if (toggle)
    {
        boundToggleKeys[key];
    }
    else
    {
        boundHoldKeys[key];
    }
}

// destruct an input manager
sariel::InputManager::~InputManager()
{
    window = nullptr;
}

// check and return whether a given key is being held or not
bool sariel::InputManager::getKeyToggle(uint key)
{
    if (!isLinked())
        return false;
    bool ispressed = glfwGetKey(window, key) == GLFW_PRESS;
    bool waspressed = toggleStates[key];

    toggleStates[key] = ispressed;
    return ispressed && !waspressed;
}

// check and return whether a given key is being toggled or not
bool sariel::InputManager::getKeyHold(uint key)
{
    if (!isLinked())
        return false;
    if (glfwGetKey(window, key) == GLFW_PRESS)
        return true;

    return false;
}

// return current mouse x coordinate
double sariel::InputManager::getMouseX()
{
    if (!isLinked())
        return 0.0;
    return currentMouseX;
}

// return current mouse y coordinate
double sariel::InputManager::getMouseY()
{
    if (!isLinked())
        return 0.0;
    return currentMouseY;
}

// return last mouse movement offset on the X axis
f32 sariel::InputManager::getMouseOffsetX()
{
    if (!isLinked())
        return 0.0f;

    static f32 lastX = 0.0f;
    static bool firstMouse = true;

    // check if this is the first frame and there is  no last coodinate
    if (firstMouse)
    {
        lastX = static_cast<f32>(currentMouseX);
        firstMouse = false;
    }

    // calculate the delta between the last coordinate & the current coordinate
    f32 offset = static_cast<f32>(currentMouseX) - lastX;
    lastX = static_cast<f32>(currentMouseX);

    return offset;
}

// return last mosue movement offset on the Y axis
f32 sariel::InputManager::getMouseOffsetY()
{
    if (!isLinked())
        return 0.0f;

    static f32 lastY = 0.0f;
    static bool firstMouse = true;

    // check if this is the first frame and there is no last coordinate
    if (firstMouse)
    {
        lastY = static_cast<f32>(currentMouseY);
        firstMouse = false;
    }

    // calculate the delta between the last coordinate & the current coordinate
    f32 offset = lastY - static_cast<f32>(currentMouseY);
    lastY = static_cast<f32>(currentMouseY);

    return offset;
}

// add an input listener to the list of listeners
void sariel::InputManager::addListener(listener *lis)
{
    listeners.push_back(lis);
}

// remove a listener from the list of listeners
void sariel::InputManager::removeListener(listener *lis)
{
    auto it = std::find(listeners.begin(), listeners.end(), lis);
    if (it != listeners.end())
    {
        listeners.erase(it);
    }
}

// focus or unfocus a certain listener from the input manager
void sariel::InputManager::setListenerFocus(listener *lis, uint medium, bool foc)
{
    auto it = std::find(listeners.begin(), listeners.end(), lis);
    if (it != listeners.end())
    {
        (*it)->setFocus(medium, foc);
    }
}

// forward user input from the manager to added & focused listeners
void sariel::InputManager::forwardToListeners()
{
    f32 offsetX = getMouseOffsetX();
    f32 offsetY = getMouseOffsetY();

    for (size_t i{0}; i < listeners.size(); i++)
    {
        if (!listeners[i]) // skip dangling pointers
            continue;

        if (listeners[i]->isFocusedMouse) // only forward to focused listeners
        {
            if (listeners[i]->isUsingMouse)
            {
                listeners[i]->mouseX = getMouseX();
                listeners[i]->mouseY = getMouseY();
            }
            if (listeners[i]->isUsingMouseOffset)
            {
                listeners[i]->mouseOffsetX = offsetX;
                listeners[i]->mouseOffsetY = offsetY;
            }
        }
        if (listeners[i]->isFocusedKeyboard) // only forward to focused listeners
        {
            for (auto &[key, held] : listeners[i]->boundHoldKeys)
            {
                held = getKeyHold(key);
            }
            for (auto &[key, toggled] : listeners[i]->boundToggleKeys)
            {
                toggled = getKeyToggle(key);
            }
        }
    }
}

// a method only accessible by an Engine object, sends mouse coordinates from the current GLFW context to the input manager
void sariel::InputManager::engineRetrieveMouse(double x, double y, EngineKey)
{
    currentMouseX = x;
    currentMouseY = y;
}

// a method only accessible by an Engine object, allows the engine to forward its GLFW window to the input manager
void sariel::InputManager::setWindow(GLFWwindow *window, EngineKey)
{
    this->window = window;
}

// check whether a GLFW window is given or not
bool sariel::InputManager::isLinked()
{
    if (!window)
    {
        std::cerr << "Error : Input Manger is not linked to an engine" << std::endl;
        return false;
    }
    return true;
}