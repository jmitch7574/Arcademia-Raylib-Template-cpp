#include "game_renderer.hpp"
#include "inspector.hpp"
#include "raylib.h"
#include "raymath.h"

RenderTexture2D GameRenderer::tex;

void GameRenderer::Init(int width, int height) {
  tex = LoadRenderTexture(width, height);
}

void GameRenderer::Begin() {
  BeginTextureMode(tex);
  ClearBackground(BLACK);
}

void GameRenderer::End() { EndTextureMode(); }

void GameRenderer::Flip(std::vector<Shader *> shaders) {
  for (int i = 0; i < shaders.size(); i++) {
    BeginShaderMode(*shaders[i]);
  }

  Rectangle destination = Rectangle(0, 0, GetScreenWidth(), GetScreenHeight());

  if (Inspector::IsOpen()) {
    destination.height -= Inspector::GetDeadZoneY();
    destination.width -= Inspector::GetDeadZoneX();
  }

  DrawTexturePro(tex.texture, Rectangle(0, 0, GetWidth(), -GetHeight()),
                 destination, Vector2(0, 0), 0, WHITE);

  for (int j = 0; j < shaders.size(); j++) {
    EndShaderMode();
  }
}

int GameRenderer::GetWidth() { return tex.texture.width; }

int GameRenderer::GetHeight() { return tex.texture.height; }

Vector2 GameRenderer::GetScaledMousePosition() {
  return Vector2Multiply(
      GetMousePosition(),
      Vector2Divide(Vector2(tex.texture.width, tex.texture.height),
                    Vector2(GetScreenWidth(), GetScreenHeight())));
}