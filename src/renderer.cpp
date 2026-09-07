#include "renderer.hpp"
#include "camera.hpp"

sariel::Drawable::Drawable(Mesh *mesh, std::initializer_list<Texture *> textures, f32 x, f32 y, f32 z, f32 scalex,
                           f32 scaley, f32 scalez)
{
    selectMesh(mesh);
    selectTextures(textures);
    setModel(x, y, z, scalex, scaley, scalez);
}

sariel::Drawable::~Drawable()
{
    mesh = nullptr;
}

void sariel::Drawable::selectMesh(Mesh *mesh)
{
    if (mesh)
        this->mesh = mesh;
}

void sariel::Drawable::selectTextures(std::initializer_list<Texture *> textures)
{
    this->textures.reserve(textures.size());
    for (Texture *texture : textures)
    {
        if (texture)
            this->textures.push_back(texture);
    }
}

void sariel::Drawable::setModel(f32 x, f32 y, f32 z, f32 scalex, f32 scaley, f32 scalez)
{
    glm::mat4 transformation = glm::mat4(1.0f);
    transformation = glm::scale(transformation, glm::vec3(scalex, scaley, scalez));
    transformation = glm::translate(transformation, glm::vec3(x, y, z));

    model = transformation;
}

sariel::Renderer::Renderer()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
}

// --- select a shader program to render with ---
void sariel::Renderer::selectShader(sariel::Shader &shader)
{
    this->shader = &shader;
    glUseProgram(shader.getProgram());
    this->shader->setUniform("diffuseLightPos", glm::vec3(0.0f, 10.0f, 0.0f));
    this->shader->setUniform("diffuseLightIntensity", 1.0f);
    this->shader->setUniform("diffuseLightColor", glm::vec3(1.0f, 1.0f, 1.0f));
}

// --- set the View Matrix used by the renderer (retrieved from a camera) ---
void sariel::Renderer::setView(glm::mat4 transMatrix)
{
    shader->setUniform("view", transMatrix);
}
// --- set the Projection Matrix used by the renderer (retrieved from a camera)
// ---
void sariel::Renderer::setProjection(glm::mat4 transMatrix)
{
    assert(shader && "setProjection() requires selectShader() to have been "
                     "called and given a correct shader pointer");
    shader->setUniform("projection", transMatrix);
}
// --- update the view and projection matrices based on the new camera position,
// orientation and perspective ---
void sariel::Renderer::updatePerspective(Camera *camera)
{
    setProjection(camera->getProjection());
    setView(camera->getCamView());
}

// --- draw a given Drawable object, using its model Matrix, mesh object
// reference and texture objects references
void sariel::Renderer::draw(sariel::Drawable &model)
{
    assert(shader && "draw() requires selectShader() to have been called and "
                     "given a correct shader pointer");

    // activate the texture units and bind the texture objects of the currently
    // active textures
    uint i = 0;
    for (auto &tex : model.textures)
    {
        tex->bind(i);
        i++;
    }
    shader->setUniform("activeTextureCount", static_cast<int>(model.textures.size()));

    for (uint i = 0; i < model.textures.size(); i++)
        shader->setUniform(("textures[" + std::to_string(i) + "]").c_str(), static_cast<int>(i));

    glBindVertexArray(model.mesh->getID());
    shader->setUniform("model", model.model);

    glDrawElements(GL_TRIANGLES, model.mesh->indiceCount(), GL_UNSIGNED_INT, nullptr);
}
