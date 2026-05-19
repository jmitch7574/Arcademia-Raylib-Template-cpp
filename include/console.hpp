#pragma once

#include "raylib.h"
#include <functional>
#include <map>
#include <string>
#include <vector>

#define MAX_CONSOLE_CHARS 32

using CommandCallback = std::function<void(const std::vector<std::string> &)>;

enum MessageLevel { LOG, WARNING, ERROR };

struct Message {
  std::string string;
  MessageLevel level;
};

class Console {
public:
  static Console &Get() {
    static Console instance;
    return instance;
  }
  Console(Console const &) = delete;
  void Update();
  void Draw();
  void RegisterCommand(const std::string &name, CommandCallback callback);
  void UnregisterCommand(const std::string &name);
  void Log(const std::string &message);
  void Warn(const std::string &message);
  void Error(const std::string &message);
  static Color GetMessageColor(MessageLevel level);
  bool enabled;

private:
  Console();
  ~Console(void);
  std::map<std::string, CommandCallback> commandRegistry;
  std::vector<Message> messages;
  char currentInput[MAX_CONSOLE_CHARS + 1] = "\0";
  void ProcessCommand(char command[]);
  void ResetInput();
  int letterCount;
};