#pragma once
#include "scene.hpp"
#include <raylib.h>
#include <vector>

#define MAX_PLAYERS 8 // The engine's max limit, ideally, don't change this
#define MAX_KEYBOARD_PLAYERS 1 // Change if you want two players on one keyboard
#define MAX_CONTROLLER_LISTENING                                               \
  128 // How many of raylib's controller indexes we
      // listen to

namespace InputManager {

  inline bool listening;

  // Player Colours
  constexpr Color playerColours[MAX_PLAYERS] = {
      Color(245, 46, 46, 255),  Color(84, 99, 255, 255),
      Color(255, 199, 23, 255), Color(31, 158, 64, 255),
      Color(255, 102, 25, 255), Color(36, 212, 196, 255),
      Color(212, 28, 229, 255), Color(74, 69, 89, 255)};

  enum EngineButton {
    BUTTON_INVALID       = -1,
    BUTTON_NORTH         = GAMEPAD_BUTTON_RIGHT_FACE_UP,
    BUTTON_SOUTH         = GAMEPAD_BUTTON_RIGHT_FACE_DOWN,
    BUTTON_WEST          = GAMEPAD_BUTTON_RIGHT_FACE_LEFT,
    BUTTON_EAST          = GAMEPAD_BUTTON_RIGHT_FACE_RIGHT,
    BUTTON_LEFT_BUMPER   = GAMEPAD_BUTTON_LEFT_TRIGGER_1,
    BUTTON_RIGHT_BUMPER  = GAMEPAD_BUTTON_RIGHT_TRIGGER_1,
    BUTTON_LEFT_TRIGGER  = GAMEPAD_BUTTON_LEFT_TRIGGER_2,
    BUTTON_RIGHT_TRIGGER = GAMEPAD_BUTTON_RIGHT_TRIGGER_2,
    BUTTON_START         = GAMEPAD_BUTTON_MIDDLE_RIGHT,
    BUTTON_BACK          = GAMEPAD_BUTTON_MIDDLE_LEFT,
    BUTTON_DPAD_UP       = GAMEPAD_BUTTON_LEFT_FACE_UP,
    BUTTON_DPAD_DOWN     = GAMEPAD_BUTTON_LEFT_FACE_DOWN,
    BUTTON_DPAD_LEFT     = GAMEPAD_BUTTON_LEFT_FACE_LEFT,
    BUTTON_DPAD_RIGHT    = GAMEPAD_BUTTON_LEFT_FACE_RIGHT
  };

  enum EngineAxis {
    AXIS_INVALID       = -1,
    AXIS_LEFT_STICK_X  = GAMEPAD_AXIS_LEFT_X,
    AXIS_LEFT_STICK_Y  = GAMEPAD_AXIS_LEFT_Y,
    AXIS_RIGHT_STICK_X = GAMEPAD_AXIS_RIGHT_X,
    AXIS_RIGHT_STICK_Y = GAMEPAD_AXIS_RIGHT_Y,
    AXIS_LEFT_TRIGGER  = GAMEPAD_AXIS_LEFT_TRIGGER,
    AXIS_RIGHT_TRIGGER = GAMEPAD_AXIS_RIGHT_TRIGGER
  };

  struct ButtonAction {
    int keyboardButton[MAX_KEYBOARD_PLAYERS];
    int arcademiaButtons[2];
    EngineButton controllerButton;
  };

  struct AxisAction {
    int negativeKeyboardInput[MAX_KEYBOARD_PLAYERS];
    int positiveKeyboardInput[MAX_KEYBOARD_PLAYERS];
    int negativeArcademiaInput[2];
    int positiveArcademiaInput[2];
    EngineAxis controllerAxis;
  };

  struct PlayerInput {
    bool isKeyboard;
    int inputIdx;

    bool isActive;    // Whether or not this player slot is active in game
    bool isConnected; // Whether or not this player has a connected input

    float lifetime                  = 0; // Time since this input became active
    float timeSinceIdentifyingInput = 0; // Time since a repeating join press
  };

  // System Lifecycle
  void Initialise();
  void Update();

  // System State
  void BeginListening();           // Start allowing players to join
  void EndListening();             // Stop allowing players to join
  void ClearDisconnectedPlayers(); // Deactivate any active player slots without
                                   // a connected input
  void CheckPlayerJoins();
  void CheckControllerDisconnects();
  void CheckPlayerDrops();

  // System Stats
  int GetKeyboardPlayerCount();
  int GetControllerPlayerCount();
  int GetTotalPlayerCount();
  bool IsThereAvailablePlayerSlot();
  int GetNextInactivePlayerSlot();
  int GetNextDisconnectedPlayerSlot();
  PlayerInput GetPlayerInfo(int playerIdx);
  const char *GetFriendlyName(int playerIdx);
  Color GetPlayerColor(int playerIdx);

  class GamepadDebugScene : public Scene {
  public:
    GamepadDebugScene();
    ~GamepadDebugScene();
    const char *GetName() const override { return "GamepadDebug"; }

    void Update() override;
    void Draw() override;

  private:
    Texture2D xboxTex;
    int gamepad;
  };
} // namespace InputManager