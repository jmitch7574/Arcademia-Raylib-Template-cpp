#include "console.hpp"
#include "Functions.hpp"
#include "game_renderer.hpp"
#include "keybinds.hpp"
#include "raylib.h"
#include <cstring>

Console::Console() {
  messages = std::vector<Message>();
#ifdef DEBUG
  enabled = true;
#else
  enabled = false;
#endif
}

Console::~Console(void) {};

void Console::Update() {

#ifdef DEBUG
  if (IsKeyPressed(KEYBINDS.console.key))
    enabled = !enabled;
#endif

  if (!enabled)
    return;

  // Get char pressed (unicode character) on the queue
  int key = GetCharPressed();

  // Adding Characters
  while (key > 0) {
    // NOTE: Only allow keys in range [32..125]
    if ((key >= 32) && (key <= 125) && (letterCount < MAX_CONSOLE_CHARS) &&
        (key != KEY_GRAVE)) {
      currentInput[letterCount] = (char)key;
      currentInput[letterCount + 1] =
          '\0'; // Add null terminator at the end of the string
      letterCount++;
    }

    key = GetCharPressed(); // Check next character in the queue
  }

  // Removing characters
  if (IsKeyPressed(KEY_BACKSPACE)) {
    letterCount--;
    if (letterCount < 0)
      letterCount = 0;
    currentInput[letterCount] = '\0';
  }

  // Submitting Commands
  if (IsKeyPressed(KEY_ENTER)) {
    ProcessCommand(currentInput);
  }
}

void Console::Draw() {
  raylib::DrawText(currentInput, 20, GameRenderer::GetHeight() - 30, 20, WHITE);

  DrawLineEx(Vector2(0, GameRenderer::GetHeight() - 40),
             Vector2(GameRenderer::GetWidth(), GameRenderer::GetHeight() - 40),
             5, RAYWHITE);

  for (int i = messages.size() - 1; i >= 0; i--) {
    raylib::DrawText(messages[i].string, 20,
                     GameRenderer::GetHeight() -
                         (30 * ((messages.size() - i) + 2)),
                     20, Console::GetMessageColor(messages[i].level));
  }
}

void Console::RegisterCommand(const std::string &name,
                              CommandCallback callback) {

  if (commandRegistry.contains(name)) {
    Error(TextFormat("Duplicate Command Being Registered: %s", name.c_str()));
  }

  commandRegistry[name] = callback;
}

void Console::UnregisterCommand(const std::string &name) {
  commandRegistry.erase(name);
}

void Console::Log(const std::string &message) {
  messages.push_back({message, LOG});
}

void Console::Warn(const std::string &message) {
  messages.push_back({message, WARNING});
}

void Console::Error(const std::string &message) {
  messages.push_back({message, ERROR});
}

Color Console::GetMessageColor(MessageLevel level) {
  switch (level) {
  case ERROR:
    return RED;
  case WARNING:
    return GOLD;
  default:
    return RAYWHITE;
  }
}

void Console::ProcessCommand(char command[]) {
  std::vector<std::string> args;

  char *token = strtok(command, " ");

  while (token != NULL) {
    args.push_back(token);
    token = strtok(NULL, " ");
  }

  std::string commandName = args[0];
  args.erase(args.begin());

  auto it = commandRegistry.find(commandName);
  if (it != commandRegistry.end()) {
    it->second(args);
  } else {
    messages.push_back({"Unknown command: " + commandName, ERROR});
  }

  ResetInput();
}

void Console::ResetInput() {
  letterCount               = 0;
  currentInput[letterCount] = '\0';
}
