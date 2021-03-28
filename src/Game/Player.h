#include "Engine/Graphics/Position.h"

class Player
{
public:
  Position2D position;
  Position2D velocity = {0.0f, 0.0f};
  bool isOnGround = false;
  Player(Position2D pos) : position(pos) {}
};