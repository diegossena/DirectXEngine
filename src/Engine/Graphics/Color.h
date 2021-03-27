#pragma once
#include <cstdint>

class RGBA
{
public:
  uint8_t r = 0, g = 0, b = 0;
  uint8_t a = 255; // Alpha (opacity) component
  RGBA(uint8_t r_, uint8_t g_, uint8_t b_) : r(r_), g(g_), b(b_) {}
};

struct fRGBA
{
  float r, g, b;  // Red, Green, Blue components
  float a = 1.0f; // Alpha (opacity) component
};

namespace Color
{
  const RGBA Black = {0, 0, 0};       // Black predefined color
  const RGBA White = {255, 255, 255}; // White predefined color
  const RGBA Red = {255, 0, 0};       // Red predefined color
  const RGBA Green = {0, 255, 0};     // Green predefined color
  const RGBA Blue = {0, 0, 255};      // Blue predefined color
  const RGBA Yellow = {255, 255, 0};  // Yellow predefined color
  const RGBA Magenta = {255, 0, 255}; // Magenta predefined color
  const RGBA Cyan = {0, 255, 255};    // Cyan predefined color
};

namespace fColor
{
  const fRGBA Black = {0.0f, 0.0f, 0.0f};   // Black predefined color
  const fRGBA White = {1.0f, 1.0f, 1.0f};   // White predefined color
  const fRGBA Red = {1.0f, 0.0f, 0};        // Red predefined color
  const fRGBA Green = {0.0f, 1.0f, 0};      // Green predefined color
  const fRGBA Blue = {0.0f, 0.0f, 1.0f};    // Blue predefined color
  const fRGBA Yellow = {1.0f, 1.0f, 0};     // Yellow predefined color
  const fRGBA Magenta = {1.0f, 0.0f, 1.0f}; // Magenta predefined color
  const fRGBA Cyan = {0.0f, 1.0f, 1.0f};    // Cyan predefined color
};