#include "engine/input/action_map.hpp"
#include "engine/input/arcademia_keybinds.hpp"
#include "engine/input/input_manager.hpp"
#include "inspector.hpp"
#include <cstddef>
#include <raylib.h>
#include <vector>

static InputManager::PlayerInput players[MAX_PLAYERS];

void InputManager::Initialise() {
  for (int i = 0; i < MAX_PLAYERS; i++) {
    players[i] = {false, -1, false, false};
  }
}

void InputManager::Update() {

  // Update Lifetimes
  for (auto &player : players) {
    if (player.isActive) {
      player.lifetime += GetFrameTime();
      player.timeSinceIdentifyingInput += GetFrameTime();
    }
  }

  CheckPlayerJoins();
  CheckPlayerDrops();
  CheckControllerDisconnects();

  if (listening) {
    ClearDisconnectedPlayers();
  }
}

void InputManager::BeginListening() { listening = true; }

void InputManager::EndListening() { listening = false; }

void InputManager::ClearDisconnectedPlayers() {
  for (int i = 0; i < MAX_PLAYERS; i++) {
    if (players[i].isActive && !players[i].isKeyboard &&
        !players[i].isConnected) {
      players[i].isActive = false;
    }
  }
}

void InputManager::CheckPlayerJoins() {
  // Look for new players

#ifdef ARCADEMIA
  if (IsKeyPressed(ArcademiaKeybinds::P1_START)) {
    if (players[0].isActive)
      players[0].timeSinceIdentifyingInput = 0;
    else if (listening)
      players[0] = {true, 0, true, true};
  }
  if (IsKeyPressed(ArcademiaKeybinds::P2_START)) {
    if (players[1].isActive)
      players[1].timeSinceIdentifyingInput = 0;
    else if (listening)
      players[1] = {true, 1, true, true};
  }
#else

  // New Keyboard Player
  if (GetKeyboardPlayerCount() < MAX_KEYBOARD_PLAYERS) {
    // Look for next keyboard player's input map
    if (IsKeyPressed(
            ActionMap::JoinGame.keyboardButton[GetKeyboardPlayerCount()])) {
      if (listening && IsThereAvailablePlayerSlot())
        players[GetNextInactivePlayerSlot()] = {true, GetKeyboardPlayerCount(),
                                                true, true};
      if (!listening && GetNextDisconnectedPlayerSlot() != -1)
        players[GetNextDisconnectedPlayerSlot()] = {
            true, GetKeyboardPlayerCount(), true, true};
    }
  }

  for (auto &player : players) {
    if (player.isActive && player.isKeyboard &&
        IsKeyPressed(ActionMap::JoinGame.keyboardButton[player.inputIdx])) {
      player.timeSinceIdentifyingInput = 0;
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
          players[j].inputIdx == gamepadIdx) {
        controllerTaken = true;
        if (IsGamepadButtonPressed(gamepadIdx,
                                   ActionMap::JoinGame.controllerButton))
          players[j].timeSinceIdentifyingInput = 0;
      }
    }
    if (controllerTaken)
      continue;

    // Check if the start button of this controller is pressed
    if (IsGamepadButtonPressed(gamepadIdx,
                               ActionMap::JoinGame.controllerButton)) {
      if (listening && IsThereAvailablePlayerSlot())
        players[GetNextInactivePlayerSlot()] = {false, gamepadIdx, true, true};
      if (!listening && GetNextDisconnectedPlayerSlot() != -1)
        players[GetNextDisconnectedPlayerSlot()] = {
            false, gamepadIdx, true, true,
            players[GetNextDisconnectedPlayerSlot()].lifetime};
    }
  }

#endif
}

void InputManager::CheckControllerDisconnects() {
  // Look for Unavailable Controllers
  for (int playerIdx = 0; playerIdx < MAX_PLAYERS; playerIdx++) {
    if (players[playerIdx].isActive && players[playerIdx].isConnected &&
        !players[playerIdx].isKeyboard) {
      if (!IsGamepadAvailable(players[playerIdx].inputIdx)) {
        players[playerIdx].isConnected = false;
      }
    }
  }
}

void InputManager::CheckPlayerDrops() {
  // Drop out Controllers
  for (int playerIdx = 0; playerIdx < MAX_PLAYERS; playerIdx++) {
    if (players[playerIdx].isActive && players[playerIdx].isConnected &&
        ActionMap::IsActionPressed(playerIdx, ActionMap::DropOut)) {
      players[playerIdx].isActive = false;
    }
  }
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
    if (!players[i].isKeyboard && players[i].isActive && players[i].isConnected)
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

// Returns the index of the next inactive player slot
// Returns -1 if all slots are filled
int InputManager::GetNextInactivePlayerSlot() {

  for (int i = 0; i < MAX_PLAYERS; i++) {
    if (!players[i].isActive)
      return i;
  }

  return -1;
}

// Returns the index of the next active player slot with no device
// Returns -1 if all slots are filled
int InputManager::GetNextDisconnectedPlayerSlot() {

  for (int i = 0; i < MAX_PLAYERS; i++) {
    if (players[i].isActive && !players[i].isConnected)
      return i;
  }

  return -1;
}

InputManager::PlayerInput InputManager::GetPlayerInfo(int playerIdx) {
  if (playerIdx > MAX_PLAYERS) {
    Inspector::Error(
        "Caught query trying to get player index outside of alotted bounds");

    return {};
  }
  return players[playerIdx];
}

const char *InputManager::GetFriendlyName(int playerIdx) {
  PlayerInput *player = &players[playerIdx];

  if (player->isKeyboard) {
    return TextFormat("Keyboard %d", player->inputIdx);
  } else {
    return GetGamepadName(player->inputIdx);
  }
}

Color InputManager::GetPlayerColor(int playerIdx) {
  if (playerIdx > MAX_PLAYERS) {
    Inspector::Error(
        "Caught query trying to get player index outside of alotted bounds");

    return RED;
  }

  return playerColours[playerIdx];
}