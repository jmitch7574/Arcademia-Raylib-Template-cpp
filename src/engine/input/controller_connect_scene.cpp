#include "engine/input/controller_connect_scene.hpp"
#include "engine/game_renderer.hpp"
#include "engine/input/input_manager.hpp"
#include "engine/raylib_extensions.hpp"
#include "imgui.h"
#include "inspector.hpp"
#include "raygui.h"
#include "raylib.h"
#include "scene.hpp"
#include "scene_manager.hpp"
#include <algorithm>
#include <cmath>
#include <functional>
#include <memory>

// Layouts go as following
// 1-5 Max Players - One Row
// 6 - 3, 3
// 7 - 4, 3
// 8 - 4, 4

namespace {
  // Scene constants
  constexpr int sceneWidth  = 640;
  constexpr int sceneHeight = 360;

  const int usableWidth  = 480;
  const int usableHeight = 200;
  const int padding      = 25;

  // Scene variables
  int rowOneCount = 0;
  int rowTwoCount = 0;
} // namespace

ControllerConnectScene::ControllerConnectScene(
    ControllerConnectConfig _config) {
  config = _config;

  viewport = Renderer::CreateViewport(sceneWidth, sceneHeight);

  if (config.maxPlayers > MAX_PLAYERS) {
    Inspector::Error("Cannot load Controller Connect scene with more players "
                     "than engine permits...");
    // sceneManager.SetScene(std::make_unique<MainMenu>());
  }

  keyboardMicro   = LoadTexture("resources/engine/input/keyboard_micro.png");
  controllerMicro = LoadTexture("resources/engine/input/controller_micro.png");
}

ControllerConnectScene::~ControllerConnectScene() {
  Renderer::DestroyViewport(viewport);
}

void ControllerConnectScene::Update() {
#if DEBUG
  if (IsKeyPressed(KEY_RIGHT))
    config.maxPlayers++;
  if (IsKeyPressed(KEY_LEFT))
    config.maxPlayers--;

  config.maxPlayers = std::clamp(config.maxPlayers, 1, 8);
#endif
}

void ControllerConnectScene::Draw() {
  Renderer::BeginViewport(viewport);
  ClearBackground(Color(34, 32, 52, 255));

  // Top Text
  DrawTextAligned("Who's Playing...", 30, Vector2(320, 25), TEXT_ALIGN_CENTER,
                  TEXT_ALIGN_TOP, WHITE);

  // Bottom Text
  std::string bottomMessage =
      InputManager::GetTotalPlayerCount() < config.minPlayers
          ? TextFormat("Minimum %d Players required", config.minPlayers)
          : "Player 1, Press START to begin...";

  DrawTextAligned(bottomMessage.c_str(), 30, Vector2(320, 350),
                  TEXT_ALIGN_CENTER, TEXT_ALIGN_BOTTOM, WHITE);

  if (config.maxPlayers < 4) {
    rowOneCount = config.maxPlayers;
    rowTwoCount = 0;
  } else {
    rowOneCount = std::ceil((float)config.maxPlayers / 2);
    rowTwoCount = std::floor((float)config.maxPlayers / 2);
  }

  bool isThereSecondRow = rowTwoCount > 0;

  for (int i = 0; i < config.maxPlayers; i++) {
    bool isInSecondRow = i >= rowOneCount;

    int rowIndex = i;
    if (isInSecondRow)
      rowIndex -= rowOneCount;

    int neighbours;
    if (!isInSecondRow)
      neighbours = rowOneCount;
    else
      neighbours = rowTwoCount;

    float width  = ((float)usableWidth - (padding * neighbours)) / (neighbours);
    float height = (float)usableHeight / (isThereSecondRow ? 2 : 1);

    float x = rowIndex * (width + padding);
    float y = isInSecondRow ? (height + padding) : 0;

    // Center

    x += 80;
    y += 80;

    float centerX = x + width / 2;
    float centerY = y + height / 2;

    DrawTextAligned(TextFormat("%d", i + 1), 40, Vector2(centerX, centerY),
                    TEXT_ALIGN_CENTER, TEXT_ALIGN_MIDDLE,
                    Color(63, 63, 116, 255));

    // Draw Player Info if player exists
    InputManager::PlayerInput player = InputManager::GetPlayerInfo(i);
    if (player.isActive) {
      float rectangleCompletion =
          std::clamp(player.lifetime / 2.0f, 0.0f, 1.0f);
      rectangleCompletion = rectangleCompletion < 0.5
                                ? 8 * rectangleCompletion *
                                      rectangleCompletion *
                                      rectangleCompletion * rectangleCompletion
                                : 1 - pow(-2 * rectangleCompletion + 2, 4) / 2;

      DrawRectangle(x, y, width, height * rectangleCompletion,
                    ColorBrightness(InputManager::GetPlayerColor(i), 0.5f));

      float alpha = std::clamp(player.lifetime - 2.0f, 0.0f, 1.0f) * 80;

      BeginBlendMode(BLEND_ADDITIVE);
      DrawTextAligned(TextFormat("P%d", i + 1), 20, Vector2(centerX, y + 15),
                      TEXT_ALIGN_CENTER, TEXT_ALIGN_TOP,
                      Color(255, 255, 255, alpha));

      std::string friendlyName = InputManager::GetFriendlyName(i);
      if (friendlyName.length() > 10) {
        friendlyName = friendlyName.substr(0, 10) + "...";
      }

      DrawTextAligned(friendlyName.c_str(), 10, Vector2(centerX, y + 35),
                      TEXT_ALIGN_CENTER, TEXT_ALIGN_TOP,
                      Color(255, 255, 255, alpha));

      Texture2D targetTex = player.isKeyboard ? keyboardMicro : controllerMicro;

      float scale = 1.5f - std::clamp(player.timeSinceIdentifyingInput * 2.0f,
                                      0.0f, 0.5f);

      Vector2 position =
          Vector2(centerX - (32 * scale), centerY - (16 * scale) + 16);

      DrawTextureEx(targetTex, position, 0, scale, Color(255, 255, 255, alpha));

      EndBlendMode();
    }

    // Draw Outline
    DrawRectangleLinesEx(Rectangle(x, y, width, height), 2,
                         Color(63, 63, 116, 255));

    // DrawCircle(x, y, 10, RED);
    // DrawCircle(x + width, y + height, 10, GREEN);
  }

  Renderer::EndViewport();
  Renderer::DrawViewport(viewport);
}

void ControllerConnectScene::DrawInspector() {
  ImGui::SeparatorText("Config");
  ImGui::Text("Minimum Players: %d", config.minPlayers);
  ImGui::Text("Maximum Players: %d", config.maxPlayers);
  ImGui::SeparatorText("Calculated Layout");
  ImGui::Text("Row One Elements: %d", rowOneCount);
  ImGui::Text("Row Two Elements: %d", rowTwoCount);
}