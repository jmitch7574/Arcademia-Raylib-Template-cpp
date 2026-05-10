#pragma once
#include "raylib.h"

struct Keybind {
  int key;
  const char *string;
};

struct ExampleKeybinds {
  Keybind moveLeft;
  Keybind moveRight;
  Keybind moveUp;
  Keybind moveDown;
  Keybind primary;
  Keybind secondary;
};

#ifdef ARCADEMIA

// ==================================================
// PLAYER ONE
// ==================================================

// Joystick
inline constexpr Keybind P1_JOYSTICK_UP    = {KEY_UP, "P1 Joystick UP"};
inline constexpr Keybind P1_JOYSTICK_DOWN  = {KEY_DOWN, "P1 Joystick DOWN"};
inline constexpr Keybind P1_JOYSTICK_LEFT  = {KEY_LEFT, "P1 Joystick LEFT"};
inline constexpr Keybind P1_JOYSTICK_RIGHT = {KEY_RIGHT, "P1 Joystick RIGHT"};

// Buttons
inline constexpr Keybind P1_A = {KEY_Z, "P1 A"};
inline constexpr Keybind P1_B = {KEY_X, "P1 B"};
inline constexpr Keybind P1_C = {KEY_C, "P1 C"};
inline constexpr Keybind P1_D = {KEY_V, "P1 D"};
inline constexpr Keybind P1_E = {KEY_B, "P1 E"};
inline constexpr Keybind P1_F = {KEY_N, "P1 F"};

// Misc
inline constexpr Keybind P1_START = {KEY_ENTER, "P1 Start"};
inline constexpr Keybind P1_EXIT  = {KEY_ESCAPE, "P1 Exit"};

// ==================================================
// PLAYER TWO
// ==================================================

// Joystick
inline constexpr Keybind P2_JOYSTICK_UP    = {KEY_W, "P2 Joystick UP"};
inline constexpr Keybind P2_JOYSTICK_DOWN  = {KEY_A, "P2 Joystick DOWN"};
inline constexpr Keybind P2_JOYSTICK_LEFT  = {KEY_S, "P2 Joystick LEFT"};
inline constexpr Keybind P2_JOYSTICK_RIGHT = {KEY_D, "P2 Joystick RIGHT"};

// Buttons
inline constexpr Keybind P2_A = {KEY_F, "P2 A"};
inline constexpr Keybind P2_B = {KEY_G, "P2 B"};
inline constexpr Keybind P2_C = {KEY_H, "P2 C"};
inline constexpr Keybind P2_D = {KEY_J, "P2 D"};
inline constexpr Keybind P2_E = {KEY_K, "P2 E"};
inline constexpr Keybind P2_F = {KEY_L, "P2 F"};

// Misc
inline constexpr Keybind P2_START = {KEY_BACKSPACE, "P2 Start"};
inline constexpr Keybind P2_EXIT  = {KEY_Q, "P2 Exit"};

#endif

#ifdef ARCADEMIA

// =================================================
// ARCADEMIA BINDS
// =================================================

inline constexpr ExampleKeybinds KEYBINDS = {.moveLeft  = P1_JOYSTICK_LEFT,
                                             .moveRight = P1_JOYSTICK_RIGHT,
                                             .moveUp    = P1_JOYSTICK_UP,
                                             .moveDown  = P1_JOYSTICK_DOWN,
                                             .primary   = P1_A,
                                             .secondary = P1_B};

#else

// =================================================
// DESKTOP BINDS
// =================================================

inline constexpr ExampleKeybinds KEYBINDS = {
    .moveLeft  = {KEY_A, "A"},
    .moveRight = {KEY_D, "D"},
    .moveUp    = {KEY_W, "W"},
    .moveDown  = {KEY_S, "S"},
    .primary   = {KEY_ENTER, "Enter"},
    .secondary = {KEY_BACKSPACE, "Backspace"}};

#endif