#pragma once
// (short width, short height)
class Size
{
public:
  // Props
  unsigned short width, height;
  // Methods
  Size(unsigned short width_, unsigned short height_) : width(width_), height(height_)
  {
  }
};