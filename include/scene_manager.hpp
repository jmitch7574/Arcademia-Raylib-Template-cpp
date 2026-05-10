#pragma once
#include "scene.hpp"
#include <memory>

class SceneManager {
private:
  std::unique_ptr<Scene> currentScene;

public:
  bool shouldExit = false;

  SceneManager() {

  };

  void SetScene(std::unique_ptr<Scene> scene) {
    currentScene = std::move(scene);
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