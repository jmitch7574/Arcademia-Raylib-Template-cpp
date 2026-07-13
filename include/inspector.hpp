#pragma once
#include <queue>
#include <string>
#include <time.h>
#include <vector>

struct IInspector;

namespace Inspector {
  // System Initialisation and shut down
  void Init();
  void Shutdown();

  // Frame Initialisation and Ending
  void BeginFrame();
  void EndFrame();

  // Inspector Registry
  void RegisterInspector(IInspector *inspector);
  void UnregisterInspector(IInspector *inspector);

  // Drawing
  void DrawInspector();
  void DrawEngineTab();

  // Debug Console
  void Log(const char *message);
  void Warn(const char *message);
  void Error(const char *message);
  time_t GetTimestamp();

  // Inspector State
  bool IsOpen();
  bool ShouldPauseGame();
  void Toggle();
} // namespace Inspector

struct IInspector {

  IInspector() { Inspector::RegisterInspector(this); }
  virtual ~IInspector() { Inspector::UnregisterInspector(this); }

  virtual void DrawInspector()        = 0;
  virtual const char *GetName() const = 0;
};

namespace {
  bool isOpen;
  bool pauseWhenOpen = true;
  bool autoScroll    = true;

  // Perforamnce Graph Data
  std::vector<float> frametimes;
  const int maxFrametimeHistory = 600;

  enum MessageLevel { LOG, WARNING, ERROR };

  struct Message {
    std::string string;
    MessageLevel level;
    time_t timestamp;
  };

  std::vector<Message> messages;

  //   struct InspectorRegistry {
  //     std::string key;
  //     IInspector *inspector;
  //   };

  std::vector<IInspector *> activeInspectors;
} // namespace
