#pragma once

#include "raylib.h"
#include <iostream>

class Scene {
public:
  Scene()                             = default;
  virtual ~Scene()                    = default;
  virtual const char *GetName() const = 0;

  virtual void Update() = 0;
  virtual void Draw()   = 0;
};

class MainMenu : public Scene {
public:
  MainMenu();
  ~MainMenu(void);
  const char *GetName() const override { return "MainMenu"; }

  void Update() override;
  void Draw() override;

private:
  bool acceptPressed = false;
  enum class MenuOption { Play, Quit };
  MenuOption selectedOption = MenuOption::Play;
  Texture arcademiaTex;
};

class PlayScene : public Scene {
public:
  PlayScene();
  ~PlayScene(void);
  const char *GetName() const override { return "PlayScene"; }

  void Update() override;
  void Draw() override;
};