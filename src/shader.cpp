#include "shader.hpp"

// --- Read Shader from Source Code Files ---
sariel::Shader::Shader(const std::string &vertpth, const std::string &fragpth, const std::string &path)
{
    std::ifstream vert(path + vertpth);
    std::ifstream frag(path + fragpth);

    if (!(vert.is_open() && frag.is_open()))
    {
        throw std::runtime_error("ERROR: FAILED TO OPEN SHADER FILES AT: " + (path + vertpth) + " OR " + (path + fragpth));
    }

    // Read file contents directly into std::string members
    vertShaderSrc = std::string((std::istreambuf_iterator<char>(vert)), std::istreambuf_iterator<char>());
    fragShaderSrc = std::string((std::istreambuf_iterator<char>(frag)), std::istreambuf_iterator<char>());
}

// --- Destroy this object and Free its resources ---
sariel::Shader::~Shader()
{
    if (glIsProgram(shaderProgram))
        glDeleteProgram(shaderProgram);

    if (glIsShader(vertexShader))
        glDeleteShader(vertexShader);

    if (glIsShader(fragmentShader))
        glDeleteShader(fragmentShader);
}

// --- Compile Shader ---
bool sariel::Shader::CompileShaders()
{
    int success;
    char infoLog[512];

    // --- Vertex Shader ---
    const char *vCode = vertShaderSrc.c_str(); // Create local pointer
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vCode, nullptr);
    glCompileShader(vertexShader);

    // Check For Compilation Errors
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::cerr << "ERROR: VERTEX SHADER COMPILATION FAILED\n"
                  << infoLog << std::endl; // Printed infoLog
        return false;
    }

    // --- Fragment Shader ---
    const char *fCode = fragShaderSrc.c_str(); // Create local pointer
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fCode, nullptr);
    glCompileShader(fragmentShader);

    // Check For Compilation Errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::cerr << "ERROR: FRAGMENT SHADER COMPILATION FAILED\n"
                  << infoLog << std::endl; // Printed infoLog
        return false;
    }
    return true;
}

// --- Link Compiled Shader into a Shader Program & Return its ID---
bool sariel::Shader::LinkProgram()
{
    int success;
    char infoLog[512];
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Check For Linking Errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cerr << "ERROR: SHADER PROGRAM LINKING FAILED\n"
                  << infoLog << std::endl; // Printed infoLog

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        return false;
    }

    // Deleting the No Longer Useful Shader Objects
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return true;
}

// --- return the shader program's ID ---
uint sariel::Shader::getProgram() const
{
    return shaderProgram;
}

// --- reach into a given uniform in the shader program and assign it a template value ---
template <typename T>
void sariel::Shader::setUniform(const char *uniform, const T &value)
{
    uint location = glGetUniformLocation(shaderProgram, uniform);
    if (location == -1)
        return;

    if constexpr (std::is_same_v<T, bool> || std::is_same_v<T, int> || std::is_same_v<T, uint>)
    {
        glUniform1i(location, static_cast<int>(value));
    }
    else if constexpr (std::is_same_v<T, f32>)
    {
        glUniform1f(location, value);
    }
    else if constexpr (std::is_same_v<T, glm::vec2>)
    {
        glUniform2fv(location, 1, glm::value_ptr(value));
    }
    else if constexpr (std::is_same_v<T, glm::vec3>)
    {
        glUniform3fv(location, 1, glm::value_ptr(value));
    }
    else if constexpr (std::is_same_v<T, glm::vec4>)
    {
        glUniform4fv(location, 1, glm::value_ptr(value));
    }
    else if constexpr (std::is_same_v<T, glm::mat4>)
    {
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
    }
    else
    {
        static_assert(always_false_v<T>, "Unsupported uniform type!");
    }
    GL_CHECK("after setUniform");
}

// --- instansiate all possible uniform template types ---
template void sariel::Shader::setUniform(const char *, const f32 &);
template void sariel::Shader::setUniform(const char *, const int &);
template void sariel::Shader::setUniform(const char *, const uint &);
template void sariel::Shader::setUniform(const char *, const bool &);
template void sariel::Shader::setUniform(const char *, const glm::vec2 &);
template void sariel::Shader::setUniform(const char *, const glm::vec3 &);
template void sariel::Shader::setUniform(const char *, const glm::vec4 &);
template void sariel::Shader::setUniform(const char *, const glm::mat4 &);