cmake_minimum_required(VERSION 3.10)
project(SarielEngine)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)

# --- GLAD ---
add_library(glad STATIC external/glad/glad.c)
target_include_directories(glad SYSTEM PUBLIC external/glad)

# --- ImGui ---
file(GLOB IMGUI_SOURCES
    "external/imgui/*.cpp"
    "external/imgui/backends/imgui_impl_glfw.cpp"
    "external/imgui/backends/imgui_impl_opengl3.cpp"
)
add_library(imgui STATIC ${IMGUI_SOURCES})
target_include_directories(imgui SYSTEM PUBLIC external/imgui external/imgui/backends)
target_link_libraries(imgui PRIVATE glfw OpenGL::GL)

# --- Sariel ---

file(GLOB_RECURSE SOURCES
    "${CMAKE_SOURCE_DIR}/src/*.c"
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
)

add_executable(sariel ${SOURCES})

set_property(TARGET sariel PROPERTY CXX_STANDARD 23)

target_include_directories(sariel PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(sariel SYSTEM PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/external)

target_link_libraries(sariel PRIVATE glfw OpenGL::GL dl pthread glad imgui)
