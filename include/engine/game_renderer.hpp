#pragma once
#include "raylib.h"
#include <vector>

namespace Renderer {

  // Viewport Definition
  struct Viewport {
    RenderTexture2D target;
    std::vector<Shader> shaders;
  };

  // Viewport registration
  Viewport CreateViewport(int width, int height,
                          std::vector<Shader> shaders = {});
  void DestroyViewport(Viewport &viewport);

  // Lifecycle
  void BeginFrame();
  void BeginViewport(Viewport &viewport);
  void EndViewport();
  void DrawViewport(Viewport &viewport,
                    Rectangle destination = Rectangle(0, 0, 0, 0));
  void EndFrame();

  // Misc.
  Vector2 GetMousePositionInViewport();

} // namespace Renderer