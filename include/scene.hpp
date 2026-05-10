#pragma once
#include "raylib-cpp.hpp"

class Scene {
public:
  Scene()          = default;
  virtual ~Scene() = default;

  virtual void Update() = 0;
  virtual void Draw()   = 0;
};

class MainMenu : public Scene {
public:
  MainMenu();
  ~MainMenu(void);

  void Update() override;
  void Draw() override;

private:
  bool acceptPressed = false;
  enum class MenuOption { Play, Quit };
  MenuOption selectedOption = MenuOption::Play;
  raylib::Texture arcademiaTex;
};

class PlayScene : public Scene {
public:
  PlayScene();
  ~PlayScene(void);

  void Update() override;
  void Draw() override;
};