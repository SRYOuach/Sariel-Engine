#include "texture.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// --- initialize and generate a texture from a given image file ---
sariel::Texture::Texture(const std::string &path)
{
    glActiveTexture(GL_TEXTURE0);

    // generate and bind a texture
    glGenTextures(1, &TexID);
    glBindTexture(GL_TEXTURE_2D, TexID);

    // configure texture wrapping & filtering options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    GL_CHECK("after texture Generation | Binding | Configuration");

    // load and generate texture data
    stbi_set_flip_vertically_on_load(true);
    int width, height, nrChannels;
    unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        GLenum format;
        switch (nrChannels)
        {
        case 1:
            format = GL_RED;
            // make single-channel textures sample as (r, r, r, 1) instead of (r, 0, 0, 1)
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_R, GL_RED);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_G, GL_RED);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_B, GL_RED);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_A, GL_ONE);
            break;
        case 2:
            format = GL_RG;
            // properly organize the two channels to create greyscale textures
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_R, GL_RED);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_G, GL_RED);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_B, GL_RED);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_A, GL_GREEN);
            break;
        case 3:
            format = GL_RGB;
            break;
        case 4:
            format = GL_RGBA;
            break;
        default:
            stbi_image_free(data);
            throw std::runtime_error("Unsupported channel count (" + std::to_string(nrChannels) + ") in " + path);
        }
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        GL_CHECK("after 2D image texture setup");
    }
    else
        std::cerr << "Failed to load texture " << path << std::endl;

    // free the now redundant data memory
    stbi_image_free(data);
}

// --- move texture buffer's ownerhsip from one texture object to the other while freeing the sender's held texture ID ---
sariel::Texture::Texture(Texture &&other) noexcept : TexID(other.TexID)
{
    other.TexID = 0;
}

sariel::Texture &sariel::Texture::operator=(Texture &&other) noexcept
{
    if (this != &other)
    {
        glDeleteTextures(1, &TexID);

        TexID = other.TexID;

        other.TexID = 0;
    }
    return *this;
}

// --- destruct a texture object and free the corresponding allocated memory on the VRAM---
sariel::Texture::~Texture()
{
    glDeleteTextures(1, &TexID);
}

// --- return the Texture Buffer's identifier ---
uint sariel::Texture::getID() const
{
    return TexID;
}

// --- bind this texture object to a given texture unit ---
void sariel::Texture::bind(uint unit) const
{
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, TexID);

    GL_CHECK("after Texture::bind");
}