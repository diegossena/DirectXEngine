#pragma once
class Position2D
{
public:
  float x, y;
  Position2D(float _x, float _y) : x(_x), y(_y) {}
  Position2D() {}
};
class Position3D
{
public:
  float x, y, z = 0;
  Position3D(float _x, float _y, float _z = 0) : x(_x), y(_y), z(_z) {}
};