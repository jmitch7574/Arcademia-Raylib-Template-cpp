#include "inspector.hpp"
#include "game_renderer.hpp"
#include "imgui.h"
#include "raylib.h"
#include "rlImGui.h"
#include <time.h>
#include <vector>

void Inspector::Init() {
  rlImGuiSetup(true);
  frametimes = std::vector<float>(maxFrametimeHistory, 0.0f);
}

void Inspector::Shutdown() { rlImGuiShutdown(); }

void Inspector::BeginFrame() { rlImGuiBegin(); }

void Inspector::EndFrame() { rlImGuiEnd(); }

void Inspector::RegisterInspector(IInspector *inspector) {
  for (auto &item : activeInspectors) {
    if (item == inspector) {
      return;
    }
  }
  activeInspectors.push_back(inspector);
}

void Inspector::UnregisterInspector(IInspector *inspector) {
  for (auto it = activeInspectors.begin(); it != activeInspectors.end(); ++it) {
    if (*it == inspector) {
      activeInspectors.erase(it);
      return;
    }
  }
}

void Inspector::DrawInspector() {
  BeginFrame();

  int width  = GetScreenWidth();
  int height = GetScreenHeight();

  const int inspectorWidth = 400;
  const int consoleHeight  = 200;

  // Inspector goes right hand side
  ImGui::SetNextWindowPos(ImVec2(width - inspectorWidth, 0));
  ImGui::SetNextWindowSize(ImVec2((float)inspectorWidth, (float)(height)));

  // Initialise Window and Tabs
  ImGui::Begin("Inspector");

  if (ImGui::BeginTabBar("SystemTabs")) {
    // Draw Engine Tab
    DrawEngineTab();

    // Draw Individual Systems
    for (auto *sys : activeInspectors) {
      if (ImGui::BeginTabItem(sys->GetName())) {
        sys->DrawInspector();
        ImGui::EndTabItem();
      }
    }
    ImGui::EndTabBar();
  }

  ImGui::End();

  // Console goes all the way across the bottom

  // Inspector goes right hand side
  ImGui::SetNextWindowPos(ImVec2(0, (float)(height - consoleHeight)));
  ImGui::SetNextWindowSize(
      ImVec2((float)(width - inspectorWidth), (float)(consoleHeight)));

  ImGui::Begin("Console");

  if (ImGui::Button("Clear")) {
    messages = std::vector<Message>();
  }

  // Scroll Field
  ImGui::BeginChild("Scrolling Region", ImVec2((float)0, (float)0), true,
                    ImGuiWindowFlags_HorizontalScrollbar);

  for (auto &message : messages) {
    struct tm *time_info;
    char timeString[9]; // space for "HH:MM:SS\0"

    time_info = localtime(&message.timestamp);

    strftime(timeString, sizeof(timeString), "%H:%M:%S", time_info);

    std::string fullMessage =
        TextFormat("[%s] : %s", timeString, message.string.c_str());

    ImVec4 textColor;

    switch (message.level) {
    case ERROR:
      textColor = ImVec4(1.0f, 0, 0, 1.0f);
      break;
    case WARNING:
      textColor = ImVec4(1.0f, 1.0f, 0, 1.0f);
      break;
    case LOG:
      textColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
      break;
    }

    ImGui::TextColored(textColor, fullMessage.c_str());
  }

  // Auto Scrolling Logic
  if (ImGui::GetScrollY() < ImGui::GetScrollMaxY())
    autoScroll = false;
  else
    autoScroll = true;

  if (autoScroll)
    ImGui::SetScrollHereY(1.0f);

  ImGui::EndChild();

  ImGui::End();

  EndFrame();
}

void Inspector::DrawEngineTab() {
  if (ImGui::BeginTabItem("Engine")) {
    ImGui::Checkbox("Pause when Open", &pauseWhenOpen);

    time_t c_time;
    time(&c_time);

    ImGui::Text("%s", ctime(&c_time));

    ImGui::SeparatorText("Arcademia Crossplatform Raylib Engine (ACRE)");
    ImGui::Text("Raylib v%s", RAYLIB_VERSION);
    ImGui::Text("DearImgui v%s", IMGUI_VERSION);

    ImGui::SeparatorText("Details");
    ImGui::Text("Mouse Position (Window): %f %f", GetMousePosition().x,
                GetMousePosition().y);

    ImGui::Text("Mouse Position (Internal): %f %f",
                GameRenderer::GetScaledMousePosition().x,
                GameRenderer::GetScaledMousePosition().y);

    // Performance Graphs
    frametimes.push_back(GetFrameTime() * 1000);
    if (frametimes.size() > maxFrametimeHistory)
      frametimes.erase(frametimes.begin());

    float arr[maxFrametimeHistory];
    copy(frametimes.begin(), frametimes.end(), arr);

    ImGui::PlotLines("Frame Times", arr, IM_COUNTOF(arr), 0, nullptr, 0.0f,
                     60.0f, ImVec2(0, 50));

    // Basic Renderer Settings
    if (ImGui::CollapsingHeader("Renderer")) {
      // Resolution State
      int width  = GetScreenWidth();
      int height = GetScreenHeight();

      ImGui::PushItemWidth(75.0f);
      ImGui::InputScalar("Width", ImGuiDataType_S16, &width);
      if (ImGui::IsItemDeactivatedAfterEdit())
        SetWindowSize(width, height);

      ImGui::SameLine();

      ImGui::PushItemWidth(75.0f);
      ImGui::InputScalar("Height", ImGuiDataType_S16, &height);
      if (ImGui::IsItemDeactivatedAfterEdit())
        SetWindowSize(width, height);

      bool isFullscreen = IsWindowFullscreen();

      if (ImGui::Checkbox("Fullscreen", &isFullscreen)) {
        if (isFullscreen) {
          SetWindowState(FLAG_FULLSCREEN_MODE);
        } else {
          ClearWindowState(FLAG_FULLSCREEN_MODE);
        }
      }
    }

    ImGui::EndTabItem();
  }
}

void Inspector::Log(const char *message) {
  messages.push_back({std::string(message), LOG, GetTimestamp()});
}

void Inspector::Warn(const char *message) {
  messages.push_back({std::string(message), WARNING, GetTimestamp()});
}

void Inspector::Error(const char *message) {
  messages.push_back({std::string(message), ERROR, GetTimestamp()});
}

time_t Inspector::GetTimestamp() {
  time_t currentTime;
  time(&currentTime);

  return currentTime;
}

bool Inspector::IsOpen() { return isOpen; }

bool Inspector::ShouldPauseGame() { return isOpen && pauseWhenOpen; }

void Inspector::Toggle() { isOpen = !IsOpen(); }