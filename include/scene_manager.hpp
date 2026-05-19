#pragma once
#include "console.hpp"
#include "scene.hpp"
#include <memory>

class SceneManager {
private:
  std::unique_ptr<Scene> currentScene;

public:
  bool shouldExit = false;

  SceneManager() {
    Console::Get().RegisterCommand(
        "ls", [this](const std::vector<std::string> &args) {
          if (args.empty())
            return;

          if (args[0] == "main" || args[0] == "menu")
            this->SetScene(std::make_unique<MainMenu>());
          if (args[0] == "play")
            this->SetScene(std::make_unique<PlayScene>());
        });
  };

  ~SceneManager() {}

  void SetScene(std::unique_ptr<Scene> scene) {
    currentScene = std::move(scene);
    Console::Get().Log(
        TextFormat("Loaded Scene: %s", currentScene->GetName().c_str()));
  }

  void Update() {
    if (currentScene)
      currentScene->Update();
  }

  void Draw() {
    if (currentScene)
      currentScene->Draw();
  }

  void CloseGame() { shouldExit = true; }
};

extern SceneManager sceneManager;