#include <iostream>
#include <array>
#include "Engine/Window.h"
#include "Engine/Graphics.h"
#include "Engine/Keyboard.h"
#include "Engine/Utils/Size.h"
#include "Engine/Utils/Timer.h"

#include "Map.h"

int main()
{
  Window window("Demo game", Size(800, 600));
  Graphics gfx(window);
  // Game Data
  Map gameMap(16);
  gameMap.loadLvL1();
  // Player
  Position2D playerPos(1.0f, 1.0f);
  Position2D playerVel(0.0f, 0.0f);
  bool playerOnGround = false;
  //
  Position2D cameraPos(0.0f, 0.0f);
  Position2D screenOffSet(0.0f, 0.0f);
  Timer timer;
  // onUpdate
  while (window.isOpen())
  {
    float timeElapsed = timer.elapsed();
    playerVel.x = 0.0f;
    playerVel.y = 0.0f;
    // pixels to cartesian distance
    float value = 0.009f;
    Position2D pixelDistance(
        1.6f / window.screen.width * gameMap.tileSize,
        1.6f / window.screen.height * gameMap.tileSize);
    // Handle Input
    if (window.hasFocus)
    {
      if (KeyBoard::isKeyPressed(Key::Up))
      {
        playerVel.y = -6.0f;
      }
      if (KeyBoard::isKeyPressed(Key::Down))
      {
        playerVel.y = 6.0f;
      }
      if (KeyBoard::isKeyPressed(Key::Right))
      {
        playerVel.x = 6.0f;
      }
      if (KeyBoard::isKeyPressed(Key::Left))
      {
        playerVel.x = -6.0f;
      }
    }
    // player move
    Position2D newPlayerPos(
        playerPos.x + playerVel.x * timeElapsed,
        playerPos.y + playerVel.y * timeElapsed);
    // Horizontal Collision
    if (playerVel.x < 0) // Moving Left
    {
      if (
          gameMap.getTile(newPlayerPos.x, playerPos.y) != '.' ||
          gameMap.getTile(newPlayerPos.x, playerPos.y + 0.9f) != '.')
      {
        newPlayerPos.x = (int)newPlayerPos.x + 1;
        playerVel.x = 0;
      }
    }
    else if (playerVel.x > 0) // Moving Right
    {
      if (
          gameMap.getTile(newPlayerPos.x + 1.0f, playerPos.y) != '.' ||
          gameMap.getTile(newPlayerPos.x + 1.0f, playerPos.y + 0.9f) != '.')
      {
        newPlayerPos.x = (int)newPlayerPos.x;
        playerVel.x = 0;
      }
    }
    // Vertical Collision
    if (playerVel.y < 0) // Moving Up
    {
      if (gameMap.getTile(newPlayerPos.x, newPlayerPos.y) != '.' ||
          gameMap.getTile(newPlayerPos.x + 0.9f, newPlayerPos.y) != '.')
      {
        newPlayerPos.y = (int)newPlayerPos.y + 1;
        playerVel.y = 0;
      }
    }
    else if (playerVel.y > 0) // Moving Down
    {
      if (
          gameMap.getTile(newPlayerPos.x, newPlayerPos.y + 1.0f) != '.' ||
          gameMap.getTile(newPlayerPos.x + 0.9f, newPlayerPos.y + 1.0f) != '.')
      {
        newPlayerPos.y = (int)newPlayerPos.y;
        playerVel.y = 0;
        playerOnGround = true; // Player has a solid surface underfoot
      }
    }
    // Apply player move
    playerPos.x = newPlayerPos.x;
    playerPos.y = newPlayerPos.y;
    // Focus on player
    cameraPos.x = playerPos.x;
    cameraPos.y = playerPos.y;
    // Draw Level
    int nVisibleTilesX = window.screen.width / gameMap.tileSize;
    int nVisibleTilesY = window.screen.height / gameMap.tileSize;
    // Calculate Top-Leftmost visible tile
    screenOffSet.x = cameraPos.x - (nVisibleTilesX / 2.0f);
    screenOffSet.y = cameraPos.y - nVisibleTilesY / 2.0f;
    // Clamp camera to game boundaries
    if (screenOffSet.x < 0)
      screenOffSet.x = 0;
    if (screenOffSet.y < 0)
      screenOffSet.y = 0;
    // get offsets for smooth movement
    float tileOffSetX = 2.0f / window.screen.width * (screenOffSet.x - (int)screenOffSet.x) * gameMap.tileSize;
    float tileOffSetY = 2.0f / window.screen.height * (screenOffSet.y - (int)screenOffSet.y) * gameMap.tileSize;
    // Square Schema
    Position2D tilePos;
    // Draw visible tile map
    gfx.clear();
    gfx.backgroundColor(fColor::Black);
    // nVisibleTilesX + 1
    for (int x = -1; x < nVisibleTilesX + 1; x++)
    {
      for (int y = -1; y < nVisibleTilesY + 1; y++)
      {
        RGBA color = Color::Black;
        char tileId = gameMap.getTile(x + screenOffSet.x, y + screenOffSet.y);
        switch (tileId)
        {
        case '.':
          color = {52, 235, 213};
          break;
        case '#':
          color = {5, 140, 0};
          break;
        case 'o':
          color = Color::Yellow;
          break;
        case '?':
          color = {184, 162, 6};
          break;
        case 'G':
          color = {130, 63, 12};
          break;
        case 'B':
          color = {130, 63, 12};
          break;
        default:
          continue;
        }
        // tileOffsetX
        tilePos.x = (x * pixelDistance.x - 1.0f) - tileOffSetX;
        tilePos.y = (1.0f - pixelDistance.y * y) + tileOffSetY;
        gfx.square(tilePos, pixelDistance, color);
      }
    }
    // Draw Player
    tilePos.x = -1.0f + pixelDistance.x * (playerPos.x - screenOffSet.x);
    tilePos.y = 1.0f - pixelDistance.y * (playerPos.y - screenOffSet.y);
    gfx.square(tilePos, pixelDistance, Color::Red);
    gfx.render();
  }
  return 0;
}