#pragma once

#include "glad.h"

#include <unordered_map>
#include <filesystem>
#include <string>
#include <array>

#include "global.hpp"
#include "texture.hpp"
#include "mesh.hpp"

namespace sariel
{
    class AssetManager
    {
    private:
        std::unordered_map<std::string, Mesh> meshes;
        std::unordered_map<std::string, Texture> textures;
        std::string fileName(const std::string &path);

        std::array<std::string, 3> extensions = {".jpg", ".png", ".jpeg"};

    public:
        AssetManager() = default;
        AssetManager(const AssetManager &) = delete;
        AssetManager &operator=(const AssetManager &) = delete;

        Texture *loadTexture(const std::string &path);
        bool loadTextureFolder(const std::string &path);

        Mesh *loadMesh(const std::string &path);
        Mesh *loadMesh(uint preset);

        bool unloadTexture(const std::string &path);
        bool unloadMesh(const std::string &path);
        bool unloadMesh(uint preset);
    };
}