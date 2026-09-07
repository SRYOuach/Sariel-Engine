#pragma once

#include "glad.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <fstream>

#include "global.hpp"

namespace sariel
{
    class Shader
    {
    public:
        Shader(const std::string &vertpth, const std::string &fragpth, const std::string &path);
        Shader(const Shader &) = delete;
        Shader &operator=(const Shader &) = delete;
        ~Shader();

        bool CompileShaders();
        bool LinkProgram();

        template <typename T> 
        void setUniform(const char *uniform, const T &value);

        uint getProgram() const;

    private:
        std::string vertShaderSrc;
        std::string fragShaderSrc;

        uint vertexShader = 0;
        uint fragmentShader = 0;
        uint shaderProgram = 0;

        template <typename>
        static constexpr bool always_false_v = false;
    };
}