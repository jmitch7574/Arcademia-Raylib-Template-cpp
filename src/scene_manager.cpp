#include "scene_manager.hpp"
#include "imgui.h"
#include "scene.hpp"
#include <functional>

SceneManager sceneManager = SceneManager();

struct SceneRegistry {
  std::string name;
  std::function<std::unique_ptr<Scene>()> getScene;
};

std::vector<SceneRegistry> sceneList = {
    {"Main Menu", []() { return std::make_unique<MainMenu>(); }},
    {"Play Scene", []() { return std::make_unique<PlayScene>(); }}};

void SceneManager::DrawInspector() {
  ImGui::Text("Current Scene: %s", currentScene->GetName());
  ImGui::SeparatorText("Load Scene");

  for (auto &scene : sceneList) {
    if (ImGui::Button(scene.name.c_str())) {
      SetScene(scene.getScene());
    }
  }
}