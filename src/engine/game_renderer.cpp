#include "engine/game_renderer.hpp"
#include "inspector.hpp"
#include "raylib.h"
#include "raymath.h"
#include <cstdlib>
#include <stack>
#include <vector>

std::stack<Renderer::Viewport *> activeViewports = {};

Renderer::Viewport Renderer::CreateViewport(int width, int height,
                                            std::vector<Shader> shaders) {
  return Renderer::Viewport{LoadRenderTexture(width, height), shaders};
}

void Renderer::DestroyViewport(Viewport &viewport) {
  UnloadRenderTexture(viewport.target);
}

void Renderer::BeginFrame() {
  while (!activeViewports.empty())
    activeViewports.pop();
}

void Renderer::BeginViewport(Viewport &viewport) {
  activeViewports.push(&viewport);
  BeginTextureMode(viewport.target);
  ClearBackground(BLANK);
}

void Renderer::EndViewport() {

  EndTextureMode();

  if (!activeViewports.empty()) {
    activeViewports.pop();
  }

  // Restore previous item's texture mode
  if (!activeViewports.empty()) {
    BeginTextureMode(activeViewports.top()->target);
  }
}

void Renderer::DrawViewport(Viewport &viewport, Rectangle destination) {
  if (destination.width == 0 && destination.height == 0)
    destination = Rectangle(0, 0, GetScreenWidth(), GetScreenHeight());

  for (int i = 0; i < viewport.shaders.size(); i++) {
    BeginShaderMode(viewport.shaders[i]);
  }

  DrawTexturePro(viewport.target.texture,
                 Rectangle(0, 0, viewport.target.texture.width,
                           -viewport.target.texture.height),
                 destination, Vector2(0, 0), 0, WHITE);

  for (int i = 0; i < viewport.shaders.size(); i++) {
    EndShaderMode();
  }
}

Vector2 Renderer::GetMousePositionInViewport() {
  Vector2 mousePosition = GetMousePosition();

  float screenW = (float)GetScreenWidth();
  float screenH = (float)GetScreenHeight();

  if (screenW <= 0.0f || screenH <= 0.0f || activeViewports.empty())
    return Vector2{0, 0};

  float viewW = (float)activeViewports.top()->target.texture.width;
  float viewH = (float)activeViewports.top()->target.texture.height;

  float effectiveW = screenW;
  float effectiveH = screenH;

  if (Inspector::IsOpen()) {
    effectiveW -= (float)Inspector::GetDeadZoneX();
    effectiveH -= (float)Inspector::GetDeadZoneY();

    if (effectiveW <= 0.0f || effectiveH <= 0.0f)
      return Vector2{0, 0};
  }

  mousePosition.x *= (viewW / effectiveW);
  mousePosition.y *= (viewH / effectiveH);

  return mousePosition;
}