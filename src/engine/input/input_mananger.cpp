#include "engine/input/action_map.hpp"
#include "engine/input/arcademia_keybinds.hpp"
#include "engine/input/input_manager.hpp"
#include <raylib.h>
#include <vector>

static InputManager::PlayerInput players[MAX_PLAYERS];

static bool listening = false;

void InputManager::Initialise() {
  for (int i = 0; i < MAX_PLAYERS; i++) {
    players[i] = {false, -1, false};
  }
}

void InputManager::Update() {

  // Look for new players

#ifdef ARCADEMIA
  if (IsKeyPressed(P1_START)) {
    players[0] = {true, 0, true};
  }
  if (IsKeyPressed(P2_START)) {
    players[1] = {true, 1, true};
  }
#else

  // New Keyboard Player
  if (GetKeyboardPlayerCount() < MAX_KEYBOARD_PLAYERS) {
    // Look for next keyboard player's input map
    if (IsKeyPressed(
            ActionMap::JoinGame.keyboardButton[GetKeyboardPlayerCount()])) {
      if (IsThereAvailablePlayerSlot())
        players[GetNextEmptyPlayerSlot()] = {true, GetKeyboardPlayerCount(),
                                             true};
    }
  }

  // New Controller Player
  for (int gamepadIdx = 0; gamepadIdx < MAX_CONTROLLER_LISTENING;
       gamepadIdx++) {

    // Is Gamepad Valid and Available
    if (!IsGamepadAvailable(gamepadIdx))
      continue;

    // If this controller is already registered, skip it
    bool controllerTaken = false;
    for (int j = 0; j < MAX_PLAYERS; j++) {
      if (players[j].isActive && !players[j].isKeyboard &&
          players[j].inputIdx == gamepadIdx)
        controllerTaken = true;
    }
    if (controllerTaken)
      continue;

    // Check if the start button of this controller is pressed
    if (IsGamepadButtonPressed(gamepadIdx,
                               ActionMap::JoinGame.controllerButton)) {
      if (IsThereAvailablePlayerSlot())
        players[GetNextEmptyPlayerSlot()] = {false, gamepadIdx, true};
    }
  }

  // Look for Unavailable Controllers
  for (int playerIdx = 0; playerIdx < MAX_PLAYERS; playerIdx++) {
    if (players[playerIdx].isActive && !players[playerIdx].isKeyboard) {
      if (!IsGamepadAvailable(players[playerIdx].inputIdx)) {
        players[playerIdx].isActive = false;
      }
    }
  }
#endif
}

int InputManager::GetKeyboardPlayerCount() {
  int kpc = 0;
  for (int i = 0; i < MAX_PLAYERS; i++) {
    if (players[i].isKeyboard && players[i].isActive)
      kpc++;
  }

  return kpc;
}

int InputManager::GetControllerPlayerCount() {
  int cpc = 0;
  for (int i = 0; i < MAX_PLAYERS; i++) {
    if (!players[i].isKeyboard && players[i].isActive)
      cpc++;
  }

  return cpc;
}

int InputManager::GetTotalPlayerCount() {

  int pc = 0;
  for (int i = 0; i < MAX_PLAYERS; i++) {
    if (players[i].isActive)
      pc++;
  }

  return pc;
}

bool InputManager::IsThereAvailablePlayerSlot() {
  for (int i = 0; i < MAX_PLAYERS; i++) {
    if (!players[i].isActive)
      return true;
  }

  return false;
}

// Returns the index of the next available player slot
// Returns -1 if all slots are filled
int InputManager::GetNextEmptyPlayerSlot() {

  for (int i = 0; i < MAX_PLAYERS; i++) {
    if (!players[i].isActive)
      return i;
  }

  return -1;
}

InputManager::PlayerInput InputManager::GetPlayerInfo(int playerIdx) {
  return players[playerIdx];
}