#pragma once
#include "arcademia_keybinds.hpp"
#include "input_manager.hpp"
#include <map>
#include <raylib.h>

namespace ActionMap {
  // Getting Action Functions
  bool IsActionPressed(int playerIdx, const InputManager::ButtonAction &action);
  bool IsActionDown(int playerIdx, const InputManager::ButtonAction &action);
  bool IsActionReleased(int playerIdx,
                        const InputManager::ButtonAction &action);
  float GetAxis(int playerIdx, const InputManager::AxisAction &action);

  // Define Actions Here

  // Join Controller
  inline const InputManager::ButtonAction JoinGame = {
      {KEY_ENTER},
      {ArcademiaKeybinds::P1_START, ArcademiaKeybinds::P2_START},
      InputManager::BUTTON_START};

  // Example Movement Actions
  inline const InputManager::AxisAction Movement = {
      {KEY_LEFT},
      {KEY_RIGHT},
      {ArcademiaKeybinds::P1_JOYSTICK_LEFT,
       ArcademiaKeybinds::P2_JOYSTICK_LEFT},
      {ArcademiaKeybinds::P1_JOYSTICK_RIGHT,
       ArcademiaKeybinds::P2_JOYSTICK_RIGHT},
      InputManager::AXIS_LEFT_STICK_X};

  inline const InputManager::ButtonAction Jump = {
      {KEY_SPACE},
      {ArcademiaKeybinds::P1_A, ArcademiaKeybinds::P2_A},
      InputManager::BUTTON_SOUTH};

  // Dictionary definitions, these are purely used in the ActionMap debugger and
  // do not affect real gamemplay
  inline const std::map<const char *, InputManager::ButtonAction> ButtonDict = {
      {"Join Game", JoinGame}, {"Jump", Jump}};

  inline const std::map<const char *, InputManager::AxisAction> AxisDict = {
      {"Horizontal Movement", Movement}};

  // Scene Definition for the ActionMap debugger
  class ActionDebugScene : public Scene {
  public:
    ActionDebugScene();
    ~ActionDebugScene();
    const char *GetName() const override { return "ActionDebug"; }

    void Update() override;
    void Draw() override;
  };

} // namespace ActionMap