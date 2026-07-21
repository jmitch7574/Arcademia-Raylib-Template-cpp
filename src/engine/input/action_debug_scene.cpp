#include "engine/game_renderer.hpp"
#include "engine/input/action_map.hpp"
#include "engine/input/arcademia_keybinds.hpp"
#include "engine/input/input_manager.hpp"
#include <raylib.h>
#include <vector>

#define SCENE_WIDTH 640.0
#define SCENE_HEIGHT 360.0

namespace {
  Renderer::Viewport viewport;
  Font tiny5;
} // namespace

ActionMap::ActionDebugScene::ActionDebugScene() {
  viewport = Renderer::CreateViewport(SCENE_WIDTH, SCENE_HEIGHT);
  tiny5    = LoadFont("resources/fonts/arcademia.fnt");
  SetTextureFilter(tiny5.texture, TEXTURE_FILTER_POINT);
}

ActionMap::ActionDebugScene::~ActionDebugScene() {
  Renderer::DestroyViewport(viewport);
}

void ActionMap::ActionDebugScene::Update() {}

void ActionMap::ActionDebugScene::Draw() {
  Renderer::BeginViewport(viewport);

  float columnWidth = SCENE_WIDTH / MAX_PLAYERS;

  // Draw Column Separators
  for (int i = 1; i < MAX_PLAYERS; i++) {
    DrawLine(columnWidth * i, 0, columnWidth * i, SCENE_HEIGHT, GRAY);
  }

  // Draw Columns
  for (int i = 0; i < MAX_PLAYERS; i++) {
    InputManager::PlayerInput player = InputManager::GetPlayerInfo(i);

    float xStart  = i * columnWidth;
    float xMax    = (i + 1) * columnWidth;
    float xWidth  = (xMax - xStart);
    float xCenter = xStart + (xWidth / 2);

    std::string titleText = TextFormat("Player %d", i + 1);
    float titleWidth      = MeasureText(titleText.c_str(), 20);

    Color col = WHITE;

    if (!player.isActive)
      col = GRAY;

    DrawText(titleText.c_str(), xStart + (xWidth - titleWidth) / 2, 25, 20,
             col);

    if (!player.isActive)
      continue;

    std::string nameText = InputManager::GetFriendlyName(i);
    if (nameText.length() > 20)
      nameText = TextFormat("%s...", TextSubtext(nameText.c_str(), 0, 20));
    float nameWidth = MeasureText(nameText.c_str(), 10);

    DrawText(nameText.c_str(), xStart + (xWidth - nameWidth) / 2, 50, 5, WHITE);

    int yOffset = 100;
    for (auto &buttonAction : ActionMap::ButtonDict) {
      Color col = ActionMap::IsActionDown(i, buttonAction.second) ? RED : GRAY;

      DrawCircle(xStart + 15, yOffset, 5, col);
      DrawText(buttonAction.first, xStart + 30, yOffset - 5, 10, WHITE);

      yOffset += 35;
    }

    constexpr int axisWidth = 20;
    for (auto &axisAction : ActionMap::AxisDict) {
      float axis       = ActionMap::GetAxis(i, axisAction.second);
      float lineOffset = (axisWidth / 2.0) + (axis * (axisWidth / 2.0));

      DrawRectangle(xStart + 5, yOffset - 10, axisWidth, 5, GRAY);
      DrawRectangle(xStart + 5 + (lineOffset - 1), yOffset - 10, 2, 5, RED);
      DrawText(axisAction.first, xStart + 30, yOffset - 12, 10, WHITE);

      yOffset += 35;
    }
  }

  Renderer::EndViewport();
  Renderer::DrawViewport(viewport);
}