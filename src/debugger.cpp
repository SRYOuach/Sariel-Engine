#include "debugger.hpp"
#include "camera.hpp"

sariel::Debugger::Debugger(const std::string &path, const std::string &font)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    ImGui::GetStyle().ScaleAllSizes(1.5f);
    ImFont *fontLoaded = io.Fonts->AddFontFromFileTTF((path + font).c_str(), 27.0f);
    if (!fontLoaded)
    {
        ImFontConfig cfg;
        cfg.SizePixels = 27.0f;
        io.Fonts->AddFontDefault(&cfg);
    }

    ImGui::StyleColorsClassic();
}

sariel::Debugger::~Debugger()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void sariel::Debugger::linkEngine(GLFWwindow *window, EngineKey)
{
    this->window = window;

    ImGui_ImplGlfw_InitForOpenGL(this->window, true);
    ImGui_ImplOpenGL3_Init("#version 130");
}

void sariel::Debugger::render(f32 fps)
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Sariel Debugger", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::Text("FPS : %f", fps);
    ImGui::SliderFloat("movement speed", &cameraSpeed, 0.1f, 10.0f);
    ImGui::SliderFloat("mouse sensitivity", &cameraSensitivity, 0.1f, 1.0f);
    ImGui::SliderFloat("FOV", &cameraFOV, 1.0f, 120.0f);

    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}