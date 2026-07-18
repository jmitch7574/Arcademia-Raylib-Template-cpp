#pragma once
#include "arcademia_keybinds.hpp"
#include "input_manager.hpp"
#include <raylib.h>

namespace ActionMap {
  // Define Actions Here

  // Join Controller
  inline const InputManager::ButtonAction JoinGame = {
      {KEY_ENTER},
      {ArcademiaKeybinds::P1_START, ArcademiaKeybinds::P2_START},
      InputManager::BUTTON_START};

} // namespace ActionMap