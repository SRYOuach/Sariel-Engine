#pragma once

#include "glad.h"
#include <GLFW/glfw3.h>
#include <iostream>

#include "debugger.hpp"

namespace sariel
{
    class Texture
    {
    public:
        explicit Texture(const std::string &path);

        Texture(const Texture &) = delete;
        Texture &operator=(const Texture &) = delete;

        Texture(Texture &&other) noexcept;
        Texture &operator=(Texture &&other) noexcept;

        ~Texture();

        uint getID() const;
        void bind(uint unit) const;

    private:
        uint TexID;
    };
}
