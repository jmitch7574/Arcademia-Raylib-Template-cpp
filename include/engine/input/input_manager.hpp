#pragma once
#include <raylib.h>

#define MAX_PLAYERS 4
#define MAX_KEYBOARD_PLAYERS 1 // Change if you want two players on one keyboard
#define MAX_CONTROLLER_PLAYERS 3
#define MAX_CONTROLLER_LISTENING                                               \
  128 // How many of raylib's controller indexes we
      // listen to

namespace InputManager {

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
    AXIS_LEFTSTICK_X   = GAMEPAD_AXIS_LEFT_X,
    AXIS_LEFTSTICK_Y   = GAMEPAD_AXIS_LEFT_Y,
    AXIS_RIGHTSTICK_X  = GAMEPAD_AXIS_RIGHT_X,
    AXIS_RIGHTSTICK_Y  = GAMEPAD_AXIS_RIGHT_Y,
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

    bool isActive;
  };

  // System Lifecycle
  void Initialise();
  void Update();

  // System State
  void BeginListening(); // Start allowing players to join
  void EndListening();   // Stop allowing players to join

  // System Stats
  int GetKeyboardPlayerCount();
  int GetControllerPlayerCount();
  int GetTotalPlayerCount();
  bool IsThereAvailablePlayerSlot();
  int GetNextEmptyPlayerSlot();
  PlayerInput GetPlayerInfo(int playerIdx);

  // Reading Data
  bool IsActionPressed(int playerIdx, const ButtonAction &action);
  bool IsActionDown(int playerIdx, const ButtonAction &action);
  bool IsActionReleased(int playerIdx, const ButtonAction &action);
  float GetAxis(int playerIdx, const AxisAction &action);

  void BindKeyboard(int playerIdx, int keyboardIdx);

  bool IsButtonDown(int playerIdx, EngineButton btn);
  float GetAxis(int playerIdx, EngineAxis axis);
} // namespace InputManager