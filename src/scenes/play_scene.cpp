#include "keybinds.hpp"
#include "raylib-cpp.hpp"
#include "scene.hpp"
#include "scene_manager.hpp"

PlayScene::PlayScene() {
  // Load Arcademia Logo
  raylib::Texture arcademiaTex =
      raylib::Texture("resources/Arcademia_Logo.png");
}

PlayScene::~PlayScene(void) {}

void PlayScene::Update() {
  bool acceptPressed = IsKeyPressed(KEYBINDS.secondary.key);

  if (acceptPressed)
    sceneManager.SetScene(std::make_unique<MainMenu>());
}

void PlayScene::Draw() {
  ClearBackground(BLACK);

  const char *text = TextFormat("Press %s to return to the main menu",
                                KEYBINDS.secondary.string);

  int x = (1280 - MeasureText(text, 40)) / 2;

  DrawText(text, x, 400, 40, WHITE);
}