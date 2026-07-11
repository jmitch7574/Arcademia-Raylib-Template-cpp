#include "keybinds.hpp"
#include "raylib.h"
#include "scene.hpp"
#include "scene_manager.hpp"

#include "raygui.h"
#include <iostream>

MainMenu::MainMenu() {
  GuiSetStyle(DEFAULT, TEXT_SIZE, 64);

  // Load Arcademia Logo
  arcademiaTex = LoadTexture("resources/Arcademia_Logo.png");
}

MainMenu::~MainMenu(void) {
  acceptPressed = false;
  UnloadTexture(arcademiaTex);
}

void MainMenu::Update() {
  acceptPressed = IsKeyPressed(KEYBINDS.primary.key);

  if (IsKeyPressed(KEYBINDS.moveRight.key)) {
    selectedOption = MenuOption::Quit;
  }
  if (IsKeyPressed(KEYBINDS.moveLeft.key)) {
    selectedOption = MenuOption::Play;
  }
}

void MainMenu::Draw() {
  ClearBackground(BLACK);

  DrawTexture(arcademiaTex, (1280 - arcademiaTex.width) / 2, 200, WHITE);

  GuiSetState(selectedOption == MenuOption::Play ? STATE_FOCUSED
                                                 : STATE_NORMAL);

  if (GuiButton(Rectangle(200, 400, 200, 100), "PLAY") ||
      (selectedOption == MenuOption::Play && acceptPressed)) {
    sceneManager.SetScene(std::make_unique<PlayScene>());
  }

  GuiSetState(selectedOption == MenuOption::Quit ? STATE_FOCUSED
                                                 : STATE_NORMAL);

  if (GuiButton(Rectangle(880, 400, 200, 100), "QUIT") ||
      (selectedOption == MenuOption::Quit && acceptPressed)) {
    sceneManager.CloseGame();
  }

  if (acceptPressed) {
    std::cout << "Accept Pressed";
  }
}