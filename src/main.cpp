#include "keybinds.hpp"
#include "raylib-cpp.hpp"
#include "raylib.h"
#include "scene.hpp"
#include "scene_manager.hpp"

int main() {
  raylib::Window window = raylib::Window(1280, 720, "Game ");

  window.SetTargetFPS(60);

  sceneManager.SetScene(std::make_unique<MainMenu>());

  while (!window.ShouldClose() && !sceneManager.shouldExit) {
    // Update Logic Here ========================

    sceneManager.Update();

    // Drawing Logic Here =======================

    window.BeginDrawing();
    window.ClearBackground(BLACK);

    sceneManager.Draw();

    window.EndDrawing();
  }

  CloseWindow();

  return 0;
}