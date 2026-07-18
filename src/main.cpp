#include "engine/input/input_manager.hpp"
#include "game_renderer.hpp"
#include "game_resources.hpp"
#include "imgui.h"
#include "inspector.hpp"
#include "raylib.h"
#include "rlImGui.h"
#include "scene.hpp"
#include "scene_manager.hpp"
#include <vector>

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define WINDOW_NAME "Arcademia Game"
#define TARGET_FPS 60

#define INTERNAL_WIDTH 1280
#define INTERNAL_HEIGHT 720

#define GetMousePosition GameRenderer::GetScaledMousePosition
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#undef GetMousePosition

int main() {
  Inspector::Warn("Example Warning...");
  Inspector::Error("Example Error...");
  Inspector::Log("Starting Game...");

  InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME);

  SetTargetFPS(TARGET_FPS);

  Inspector::Log(TextFormat("Game Window created at %dx%d@%dfps", WINDOW_WIDTH,
                            WINDOW_HEIGHT, TARGET_FPS));

  GameRenderer::Init(INTERNAL_WIDTH, INTERNAL_HEIGHT);

  Inspector::Log(TextFormat("Internal Render Texture initialised at %dx%d",
                            INTERNAL_WIDTH, INTERNAL_HEIGHT));

  GameResources::LoadResources();

#ifdef RELEASE
  sceneManager.SetScene(std::make_unique<MainMenu>());
#endif

  Inspector::Init();

  sceneManager.SetScene(std::make_unique<MainMenu>());

  while (!WindowShouldClose() && !sceneManager.shouldExit) {
    // Update Logic Here ========================

    if (IsKeyPressed(KEY_GRAVE))
      Inspector::Toggle();

    if (!Inspector::ShouldPauseGame()) {
      InputManager::Update();
      sceneManager.Update();
    }

    // Drawing Logic Here =======================
    GameRenderer::Begin();

    sceneManager.Draw();

    GameRenderer::End();

    BeginDrawing();
    ClearBackground(BLACK);

    if (Inspector::ShouldPauseGame()) {
      GameRenderer::Flip({&GameResources::Blur});
      DrawRectangleRec(Rectangle(0, 0, GetScreenWidth(), -GetScreenHeight()),
                       Color(0, 0, 0, 180));
    } else {
      GameRenderer::Flip({});
    }

    if (Inspector::IsOpen())
      Inspector::DrawInspector();

    EndDrawing();
  }

  Inspector::Shutdown();

  CloseWindow();

  return 0;
}