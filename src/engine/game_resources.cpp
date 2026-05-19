#include "game_resources.hpp"
#include "ShaderUnmanaged.hpp"
#include "game_renderer.hpp"

raylib::Shader GameResources::Blur;

void GameResources::LoadResources() {
  Blur          = LoadShader(0, "resources/shaders/blur.fs");
  int widthLoc  = Blur.GetLocation("renderWidth");
  int heightLoc = Blur.GetLocation("renderHeight");

  int width  = GameRenderer::GetWidth();
  int height = GameRenderer::GetWidth();

  Blur.SetValue(widthLoc, &width, RL_SHADER_UNIFORM_INT);
  Blur.SetValue(heightLoc, &height, RL_SHADER_UNIFORM_INT);
}