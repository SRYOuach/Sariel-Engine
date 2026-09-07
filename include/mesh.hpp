#pragma once

#include "glad.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include "global.hpp"

#define SARIEL_VOID 0
#define SARIEL_TRI 1
#define SARIEL_QUAD 2
#define SARIEL_CIRCLE 3
#define SARIEL_CUBE 4
#define SARIEL_SPHERE 5

#define SARIEL_PI glm::pi<f32>()

namespace sariel
{
    struct vertix
    {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 texture;
        vertix(f32 px, f32 py, f32 pz, f32 nx, f32 ny, f32 nz, f32 tu, f32 tv)
        {
            position = glm::vec3(px, py, pz);
            normal = glm::vec3(nx, ny, nz);
            texture = glm::vec2(tu, tv);
        }
    };

    class Mesh
    {
    public:
        explicit Mesh(uint preset = SARIEL_VOID);
        Mesh(const Mesh &) = delete;
        Mesh &operator=(const Mesh &) = delete;
        Mesh(Mesh &&other) noexcept;
        Mesh &operator=(Mesh &&other) noexcept;
        ~Mesh();

        void generateData(uint preset);

        void transform(glm::mat4 transformation);

        uint getID() const;
        uint indiceCount() const;

    private:
        uint VAO, VBO, EBO;
        std::vector<vertix> vertices;
        std::vector<uint> indices;
    };
}