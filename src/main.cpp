#include "engine/game_renderer.hpp"
#include "engine/input/input_manager.hpp"
#include "game_resources.hpp"
#include "imgui.h"
#include "inspector.hpp"
#include "raylib.h"
#include "raymath.h"
#include "rlImGui.h"
#include "scene.hpp"
#include "scene_manager.hpp"
#include <vector>

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080
#define WINDOW_NAME "Arcademia Game"
#define TARGET_FPS 60

#define GetMousePosition Renderer::GetMousePositionInViewport
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#undef GetMousePosition

int main() {
  Inspector::Warn("Example Warning...");
  Inspector::Error("Example Error...");
  Inspector::Log("Starting Game...");

  InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME);

  SetExitKey(KEY_NULL);

  SetTargetFPS(TARGET_FPS);

  Inspector::Log(TextFormat("Game Window created at %dx%d@%dfps", WINDOW_WIDTH,
                            WINDOW_HEIGHT, TARGET_FPS));

  Renderer::Viewport root =
      Renderer::CreateViewport(WINDOW_WIDTH, WINDOW_HEIGHT);

  Inspector::Log(TextFormat("Root Render Texture initialised at %dx%d",
                            WINDOW_HEIGHT, TARGET_FPS));

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

    // Drawing Systems to Root Viewport =======================
    Renderer::BeginViewport(root);

    sceneManager.Draw();

    Renderer::EndViewport();

    // Draw root viewport to screen ===============
    BeginDrawing();
    ClearBackground(BLACK);

    if (Inspector::IsOpen()) {
      Vector2 screenSizeMinusViewport = Vector2Subtract(
          Vector2(GetScreenWidth(), GetScreenHeight()),
          Vector2(Inspector::GetDeadZoneX(), Inspector::GetDeadZoneY()));

      Rectangle screenMinusViewport =
          Rectangle(0, 0, screenSizeMinusViewport.x, screenSizeMinusViewport.y);

      Renderer::DrawViewport(root, screenMinusViewport);
      if (Inspector::ShouldPauseGame()) {
        DrawRectangleRec(screenMinusViewport, Color(180, 180, 180, 128));
      }

      Inspector::DrawInspector();
    } else {
      Renderer::DrawViewport(root);
    }

    EndDrawing();

    sceneManager.LateUpdate();
  }

  Inspector::Shutdown();

  CloseWindow();

  return 0;
}