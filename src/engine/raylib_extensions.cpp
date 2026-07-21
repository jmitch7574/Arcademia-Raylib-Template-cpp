#include "engine/raylib_extensions.hpp"
#include "raygui.h"
#include "raylib.h"
#include <cmath>

void DrawTextAligned(const char *text, int fontSize, Vector2 position,
                     GuiTextAlignment horizontalAlignment,
                     GuiTextAlignmentVertical verticalAlignment, Color color) {

  float spacing = floor(fontSize / 10);

  const Vector2 textSize =
      MeasureTextEx(GetFontDefault(), text, fontSize, spacing);

  float targetX;
  float targetY;

  switch (horizontalAlignment) {
  case (TEXT_ALIGN_LEFT):
    targetX = position.x;
    break;
  case (TEXT_ALIGN_CENTER):
    targetX = position.x - (textSize.x / 2);
    break;
  case (TEXT_ALIGN_RIGHT):
    targetX = position.x - textSize.x;
    break;
  }

  switch (verticalAlignment) {
  case (TEXT_ALIGN_TOP):
    targetY = position.y;
    break;
  case (TEXT_ALIGN_MIDDLE):
    targetY = position.y - (textSize.y / 2);
    break;
  case (TEXT_ALIGN_BOTTOM):
    targetY = position.y - textSize.y;
    break;
  }

  DrawText(text, targetX, targetY, fontSize, color);
}