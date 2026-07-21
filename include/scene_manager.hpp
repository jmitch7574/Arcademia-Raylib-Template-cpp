#pragma once
#include "inspector.hpp"
#include "raylib.h"
#include "scene.hpp"
#include <memory>

class SceneManager : IInspector {
private:
  std::unique_ptr<Scene> currentScene;
  std::unique_ptr<Scene> nextScene = nullptr;

public:
  bool shouldExit = false;

  SceneManager() {};

  ~SceneManager() {}

  void SetScene(std::unique_ptr<Scene> scene) {
    Inspector::Log(TextFormat("Loaded Scene: %s", scene->GetName()));
    nextScene = std::move(scene);
  }

  void LateUpdate() {
    if (nextScene != nullptr) {
      currentScene = std::move(nextScene);
      nextScene    = nullptr;
    }
  }

  void Update() {
    if (currentScene)
      currentScene->Update();
  }

  void Draw() {
    if (currentScene)
      currentScene->Draw();
  }

  void DrawInspector() override;
  const char *GetName() const override { return "Scene"; }

  void CloseGame() { shouldExit = true; }
};

extern SceneManager sceneManager;