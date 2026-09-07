#include "engine.hpp"

// --- Engine Initialization ---
sariel::Engine::Engine(int width, int height, const char *title) : WIDTH(width), HEIGHT(height)
{
    // --- GLFW Initialization ---
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // --- Window Creation ---
    window = glfwCreateWindow(WIDTH, HEIGHT, title, NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);

    // --- Glad Initialization ---
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        exit(EXIT_FAILURE);
    }

    glViewport(0, 0, WIDTH, HEIGHT);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwSetWindowUserPointer(window, this);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);

    engineListener = listener(false, false);
    engineListener.boundToggleKeys[GLFW_KEY_ESCAPE];
    engineListener.boundToggleKeys[GLFW_KEY_GRAVE_ACCENT];
}

// --- Destruct Engine and Clear Resources ---
sariel::Engine::~Engine()
{
    glfwTerminate();
}

// --- Select a Renderer, Camera, Debugger and InputManager Objects to Use ---
void sariel::Engine::selectRenderer(sariel::Renderer &renderer)
{
    activeRenderer = &renderer;
}
void sariel::Engine::selectCamera(sariel::Camera &camera)
{
    activeCamera = &camera;
    activeCamera->onViewportResize(WIDTH, HEIGHT, EngineKey{});
}
void sariel::Engine::selectDebugger(sariel::Debugger &debugger)
{
    assert(activeCamera && "selectDebugger() requires selectCamera() to have been called");

    activeDebugger = &debugger;
    activeDebugger->linkEngine(window, EngineKey{});
}
void sariel::Engine::selectInput(sariel::InputManager &input)
{
    activeInput = &input;
    activeInput->setWindow(window, EngineKey{});
    activeInput->addListener(&engineListener);
}

// --- Engine Behavior During Render Loop ---
void sariel::Engine::beginFrame()
{
    float CurrentTime = static_cast<float>(glfwGetTime());
    deltaTime = CurrentTime - lastFrame;
    lastFrame = CurrentTime;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void sariel::Engine::endFrame()
{
    if (activeCamera)
    {
        activeCamera->camUpdate();
        if (activeDebugger && renderDebugger)
            activeCamera->parameterUpdate(activeDebugger->cameraFOV,
                                          activeDebugger->cameraSpeed,
                                          activeDebugger->cameraSensitivity);
    }
    processInput();

    glfwSwapBuffers(window);
    glfwPollEvents();
}

// --- Window Resize Callback ---
void sariel::Engine::framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);

    // get the pointer to the Engine currently running
    auto *engine = static_cast<Engine *>(glfwGetWindowUserPointer(window));

    // update engine's width and height attributes and update active camera
    engine->WIDTH = width;
    engine->HEIGHT = height;

    if (engine->activeCamera)
        engine->activeCamera->onViewportResize(engine->WIDTH, engine->HEIGHT, EngineKey{});
}

// --- mouse callback ---
void sariel::Engine::mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
    // get the pointer to the Engine currently running
    auto *engine = static_cast<Engine *>(glfwGetWindowUserPointer(window));

    // update the mouse coordinates for the active input manager
    if (engine->activeInput)
        engine->activeInput->engineRetrieveMouse(xpos, ypos, EngineKey{});
}

// --- Calculate the engine's Frames Per Second every second ---
float sariel::Engine::calculateFPS()
{
    static double lastFrame = glfwGetTime();
    static int frameCount = 0;
    static f32 currentFPS = 0.0f;

    double currentTime = glfwGetTime();
    frameCount++;

    // set currentFPS to the number of frames that elapsed in the duration of 1 second
    if (currentTime - lastFrame >= 1)
    {
        currentFPS = static_cast<float>(frameCount);
        frameCount = 0;
        lastFrame += 1.0f;
    }
    return currentFPS;
}

// --- Process engine input ---
void sariel::Engine::processInput()
{
    assert(activeInput && "processInput() requires selectInput() to have been called");
    assert(activeCamera && "processInput() requires selectCamera() to have been called");

    if (engineListener.boundToggleKeys[GLFW_KEY_ESCAPE])
        glfwSetWindowShouldClose(window, true);

    if (engineListener.boundToggleKeys[GLFW_KEY_GRAVE_ACCENT])
    {
        renderDebugger = !renderDebugger;
        activeInput->setListenerFocus(activeCamera->getListener(), SARIEL_MOUSE, !renderDebugger);
        activeInput->setListenerFocus(activeCamera->getListener(), SARIEL_KEYBOARD, !renderDebugger);
        glfwSetInputMode(window, GLFW_CURSOR, renderDebugger ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
    }
}

// --- Confirm that the Engine is still running ---
bool sariel::Engine::isRunning()
{
    assert(activeDebugger && "isRunning() requires selectDebugger() to have been called");
    if (glfwWindowShouldClose(window))
        return false;
    if (activeDebugger->closeEngine)
        return false;
    return true;
}