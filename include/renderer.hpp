#pragma once

#include "glad.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_access.hpp>

#include <initializer_list>
#include <vector>

#include "global.hpp"
#include "mesh.hpp"
#include "shader.hpp"
#include "texture.hpp"

namespace sariel
{
  class Camera;

  struct Light
  {
    f32 lightIntensity;
    glm::vec3 lightColor;
    glm::vec3 lightPos;
  };

  struct Drawable
  {

    bool isLight;
    Light light;

    Mesh *mesh;
    std::vector<Texture *> textures;
    glm::mat4 model;

    Drawable(Mesh *mesh, std::initializer_list<Texture *> textures, f32 x = 0,
             f32 y = 0, f32 z = 0, f32 scalex = 1.0f, f32 scaley = 1.0f,
             f32 scalez = 1.0f);
    ~Drawable();

    void selectTextures(std::initializer_list<Texture *> textures);
    void selectMesh(Mesh *mesh);
    void setModel(f32 x, f32 y, f32 z, f32 scalex, f32 scaley, f32 scalez);
    void setLight(Light &&light);
  };
  class Renderer
  {
  private:
    sariel::Shader *shader;

  public:
    Renderer();

    void selectShader(sariel::Shader &shader);

    void updatePerspective(Camera *camera);
    void setView(glm::mat4 transMatrix);
    void setProjection(glm::mat4 transMatrix);

    void draw(Drawable &model);
  };
}
