#include "engine/input/action_map.hpp"
#include "engine/input/input_manager.hpp"
#include "raylib.h"
#include <algorithm>

bool ActionMap::IsActionPressed(int playerIdx,
                                const InputManager::ButtonAction &action) {
  InputManager::PlayerInput player = InputManager::GetPlayerInfo(playerIdx);

  if (player.isKeyboard) {
#if ARCADEMIA
    return IsKeyPressed(action.arcademiaButtons[player.inputIdx]);
#else
    return IsKeyPressed(action.keyboardButton[player.inputIdx]);
#endif
  } else {
    return IsGamepadButtonPressed(player.inputIdx, action.controllerButton);
  }
}

bool ActionMap::IsActionDown(int playerIdx,
                             const InputManager::ButtonAction &action) {
  InputManager::PlayerInput player = InputManager::GetPlayerInfo(playerIdx);

  if (player.isKeyboard) {
#if ARCADEMIA
    return IsKeyDown(action.arcademiaButtons[player.inputIdx]);
#else
    return IsKeyDown(action.keyboardButton[player.inputIdx]);
#endif
  } else {
    return IsGamepadButtonDown(player.inputIdx, action.controllerButton);
  }
}

bool ActionMap::IsActionReleased(int playerIdx,
                                 const InputManager::ButtonAction &action) {
  InputManager::PlayerInput player = InputManager::GetPlayerInfo(playerIdx);

  if (player.isKeyboard) {
#if ARCADEMIA
    return IsKeyReleased(action.arcademiaButtons[player.inputIdx]);
#else
    return IsKeyReleased(action.keyboardButton[player.inputIdx]);
#endif
  } else {
    return IsGamepadButtonReleased(player.inputIdx, action.controllerButton);
  }
}

float ActionMap::GetAxis(int playerIdx,
                         const InputManager::AxisAction &action) {
  InputManager::PlayerInput player = InputManager::GetPlayerInfo(playerIdx);

  if (player.isKeyboard) {
#if ARCADEMIA
    return IsKeyDown(action.positiveArcademiaInput[player.inputIdx]) -
           IsKeyDown(action.negativeArcademiaInput[player.inputIdx]);
#else
    return IsKeyDown(action.positiveKeyboardInput[player.inputIdx]) -
           IsKeyDown(action.negativeKeyboardInput[player.inputIdx]);
#endif
  } else {
    return GetGamepadAxisMovement(player.inputIdx, action.controllerAxis);
  }
}

bool ActionMap::IsActionPressedGlobal(
    const InputManager::ButtonAction &action) {

#if ARCADEMIA
  return IsKeyPressed(action.arcademiaButtons[0]) ||
         IsKeyPressed(action.arcademiaButtons[1]);
#endif

  for (int k = 0; k < MAX_KEYBOARD_PLAYERS; k++) {
    if (IsKeyPressed(action.keyboardButton[k]))
      return true;
  }

  for (int c = 0; c < MAX_CONTROLLER_LISTENING; c++) {
    if (IsGamepadButtonPressed(c, action.controllerButton))
      return true;
  }

  return false;
}

bool ActionMap::IsActionDownGlobal(const InputManager::ButtonAction &action) {

#if ARCADEMIA
  return IsKeyDown(action.arcademiaButtons[0]) ||
         IsKeyDown(action.arcademiaButtons[1]);
#endif

  for (int k = 0; k < MAX_KEYBOARD_PLAYERS; k++) {
    if (IsKeyDown(action.keyboardButton[k]))
      return true;
  }

  for (int c = 0; c < MAX_CONTROLLER_LISTENING; c++) {
    if (IsGamepadButtonDown(c, action.controllerButton))
      return true;
  }

  return false;
}

bool ActionMap::IsActionReleasedGlobal(
    const InputManager::ButtonAction &action) {

#if ARCADEMIA
  return IsKeyReleased(action.arcademiaButtons[0]) ||
         IsKeyReleased(action.arcademiaButtons[1]);
#endif

  for (int k = 0; k < MAX_KEYBOARD_PLAYERS; k++) {
    if (IsKeyReleased(action.keyboardButton[k]))
      return true;
  }

  for (int c = 0; c < MAX_CONTROLLER_LISTENING; c++) {
    if (IsGamepadButtonReleased(c, action.controllerButton))
      return true;
  }

  return false;
}

float ActionMap::GetAxisGlobal(const InputManager::AxisAction &action) {
#if ARCADEMIA
  return std::clamp(
      (IsKeyDown(action.positiveArcademiaInput[0]) +
       IsKeyDown(action.positiveArcademiaInput[1])) -
          (IsKeyDown(action.negativeArcademiaInput[player.inputIdx]) +
           IsKeyDown(action.negativeArcademiaInput[player.inputIdx])),
      0, 1);
#endif

  float axis = 0;

  for (int k = 0; k < MAX_KEYBOARD_PLAYERS; k++) {
    axis += (IsKeyDown(action.positiveKeyboardInput[k]) -
             IsKeyDown(action.positiveKeyboardInput[k]));
  }

  for (int c = 0; c < MAX_KEYBOARD_PLAYERS; c++) {
    axis += GetGamepadAxisMovement(c, action.controllerAxis);
  }

  return std::clamp(axis, 0.0f, 1.0f);
}