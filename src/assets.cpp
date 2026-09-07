#include "assets.hpp"

// --- create a texture object and load it into the map of textures ---
sariel::Texture *sariel::AssetManager::loadTexture(const std::string &path)
{
    std::string name = fileName(path);

    auto exists = textures.find(name);

    if (exists != textures.end())
        return &exists->second;

    textures.emplace(name, Texture(path));

    return &textures.at(name);
}

// --- create multiple texture objects from a folder and load them into the map of textures ---
bool sariel::AssetManager::loadTextureFolder(const std::string &path)
{
    namespace fs = std::filesystem;

    for (const auto &entry : fs::directory_iterator(path))
    {
        if (!entry.is_regular_file())
            continue;

        std::string ext = entry.path().extension().string();
        for (auto extension : extensions)
        {
            if (ext == extension)
            {
                std::string name = fileName(entry.path().string());

                auto exists = textures.find(name);
                if (exists != textures.end())
                    break;

                textures.emplace(name, Texture(entry.path().string()));
                break;
            }
        }
    }
    return true;
}

// --- create a mesh object and load it into the map of meshes ---
sariel::Mesh *sariel::AssetManager::loadMesh(uint preset)
{
    std::string name = std::to_string(preset);

    auto exists = meshes.find(name);

    if (exists != meshes.end())
        return &exists->second;

    meshes.emplace(name, Mesh(preset));

    return &meshes.at(name);
}

// --- unload a texture object from the map of meshes ---
bool sariel::AssetManager::unloadTexture(const std::string &path)
{
    std::string name = fileName(path);
    auto exists = textures.find(name);

    if (exists != textures.end())
    {
        textures.erase(exists);
        return true;
    }
    return false;
}

// --- unload a texture object from the map of meshes ---
bool sariel::AssetManager::unloadMesh(uint preset)
{
    std::string name = std::to_string(preset);
    auto exists = meshes.find(name);

    if (exists != meshes.end())
    {
        meshes.erase(exists);
        return true;
    }

    return false;
}

std::string sariel::AssetManager::fileName(const std::string &path)
{
    size_t index = path.find_last_of('/');

    if (index != std::string::npos)
        return path.substr(index + 1);
    else
        return path;
}