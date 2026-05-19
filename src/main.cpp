#include "RenderTexture.hpp"
#include "console.hpp"
#include "game_renderer.hpp"
#include "game_resources.hpp"
#include "keybinds.hpp"
#include "raylib-cpp.hpp"
#include "raylib.h"
#include "scene.hpp"
#include "scene_manager.hpp"
#include <vector>

int main() {
  raylib::Window window = raylib::Window(1280, 720, "Game ");

  window.SetTargetFPS(60);

  GameRenderer::Init(1280, 720);
  GameResources::LoadResources();

#ifdef RELEASE
  sceneManager.SetScene(std::make_unique<MainMenu>());
#endif

  sceneManager.SetScene(std::make_unique<MainMenu>());
  while (!window.ShouldClose() && !sceneManager.shouldExit) {
    // Update Logic Here ========================
    if (!Console::Get().enabled)
      sceneManager.Update();
    Console::Get().Update();

    // Drawing Logic Here =======================
    GameRenderer::Begin();

    sceneManager.Draw();

    GameRenderer::End();

    window.BeginDrawing();
    window.ClearBackground(BLACK);

    if (Console::Get().enabled) {
      GameRenderer::Flip({&GameResources::Blur});
      DrawRectangleRec(
          Rectangle(0, 0, GameRenderer::GetWidth(), GameRenderer::GetHeight()),
          Color(0, 0, 0, 180));
      Console::Get().Draw();
    } else {
      GameRenderer::Flip({});
    }

    window.EndDrawing();
  }

  CloseWindow();

  return 0;
}