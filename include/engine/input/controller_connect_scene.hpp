#include "engine/game_renderer.hpp"
#include "inspector.hpp"
#include "raylib.h"
#include "scene.hpp"
#include <functional>
#include <memory>

struct ControllerConnectConfig {
  int minPlayers;
  int maxPlayers;
  std::function<std::unique_ptr<Scene>()> createNextScene;
};

class ControllerConnectScene : public Scene, IInspector {
public:
  ControllerConnectScene(ControllerConnectConfig config);
  ~ControllerConnectScene();
  const char *GetName() const override { return "Controller Connect"; }
  void Update() override;
  void Draw() override;
  void DrawInspector() override;

private:
  ControllerConnectConfig config;
  Renderer::Viewport viewport;
  Texture2D controllerMicro;
  Texture2D keyboardMicro;
};