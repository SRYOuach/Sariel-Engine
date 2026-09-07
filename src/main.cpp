#include "assets.hpp"
#include "engine.hpp"
#include "global.hpp"
#include "renderer.hpp"
#include "shader.hpp"

#include <string>

struct CelestialBody {
  f32 x, y, z;
  f32 xscale, yscale, zscale;
  glm::vec3 acceleration = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::vec3 velocity = glm::vec3(0.0f, 0.0f, 0.0f);
  sariel::Drawable drawable;
  void accelerate(f32 x, f32 y, f32 z) { acceleration += glm::vec3(x, y, z); }
  void update() {}
};

int main() {
  const std::string resourceDirectory = "resources/";
  // initialize the engine
  sariel::Engine engine(1280, 720, "sariel");

  // initialize an asset manager and load assets
  sariel::AssetManager assets;
  assets.loadTextureFolder(resourceDirectory + "textures/");

  // compile the shaders and link them into a program
  sariel::Shader shader("basicVertex.glsl", "basicFragment.glsl",
                        resourceDirectory + "shaders/");
  shader.CompileShaders();
  shader.LinkProgram();

  // initialize a renderer
  sariel::Renderer renderer;
  renderer.selectShader(shader);

  // initialize an input manager
  sariel::InputManager input;

  // initialize a camera
  sariel::Camera camera;

  // initialize a debugging window
  sariel::Debugger debugger(resourceDirectory + "fonts/", "open_sans.ttf");

  // link engine modules
  engine.selectCamera(camera);
  engine.selectRenderer(renderer);
  engine.selectInput(input);
  engine.selectDebugger(debugger);

  // link input listeners to the input manager
  input.addListener(camera.getListener());

  sariel::Drawable earth(assets.loadMesh(SARIEL_SPHERE),
                         {assets.loadTexture("earth.jpg")});
  sariel::Drawable crate(assets.loadMesh(SARIEL_CUBE),
                         {assets.loadTexture("metal.jpg")}, 3);

  // render loop
  while (engine.isRunning()) {
    engine.beginFrame();

    renderer.updatePerspective(&camera);

    renderer.draw(earth);
    renderer.draw(crate);

    if (engine.renderDebugger)
      debugger.render(engine.calculateFPS());

    input.forwardToListeners();
    engine.endFrame();
  }

  return 0;
}
