// Default
#include <iostream>
#include <array>
// Engine
#include "Engine/Window.h"
#include "Engine/Graphics.h"
#include "Engine/Keyboard.h"
#include "Engine/Utils/Timer.h"
// Game
#include "Game/Map.h"
#include "Game/Player.h"

int main()
{
  Window window("Demo game", Size(800, 600));
  Graphics gfx(window);
  // initial data
  Map gameMap;
  Position2D screenOffset(0.0f, 0.0f);
  float zoom = 0.1f;
  // Player
  Player player(Position2D(3.0f, 5.0f));
  // Camera
  Position2D cameraPos(player.position);
  // Timer
  Timer timer;
  // onUpdate
  while (window.isOpen())
  {
    float timeElapsed = timer.elapsed();
    float tileSizeY = zoom;
    float tileSizeX = zoom / window.screenProportion;
    // Stop player
    player.velocity.x = 0.0f;
    player.velocity.y = 0.0f;
    /* Handle Input
    ----------------*/
    // Player movement
    if (window.hasFocus)
    {
      if (KeyBoard::isKeyPressed(Key::Up))
      {
        player.velocity.y = -6.0f;
      }
      else if (KeyBoard::isKeyPressed(Key::Down))
      {
        player.velocity.y = 6.0f;
      }
      if (KeyBoard::isKeyPressed(Key::Right))
      {
        player.velocity.x = 6.0f;
      }
      else if (KeyBoard::isKeyPressed(Key::Left))
      {
        player.velocity.x = -6.0f;
      }
    }
    Position2D newPlayerPos(
        player.position.x + player.velocity.x * timeElapsed,
        player.position.y + player.velocity.y * timeElapsed);
    /* Collision
    -------------*/
    // Horizontal Collision
    if (player.velocity.x < 0) // Moving Left
    {
      if (newPlayerPos.x < 0.0f)
        newPlayerPos.x = 0;
      if (
          gameMap.getTile(newPlayerPos.x, player.position.y) != '.' ||
          gameMap.getTile(newPlayerPos.x, player.position.y + 0.9f) != '.')
      {
        newPlayerPos.x = (int)newPlayerPos.x + 1;
        player.velocity.x = 0;
      }
    }
    else if (player.velocity.x > 0) // Moving Right
    {
      if (
          gameMap.getTile(newPlayerPos.x + 1.0f, player.position.y) != '.' ||
          gameMap.getTile(newPlayerPos.x + 1.0f, player.position.y + 0.9f) != '.')
      {
        newPlayerPos.x = (int)newPlayerPos.x;
        player.velocity.x = 0;
      }
    }
    // Vertical Collision
    if (player.velocity.y < 0) // Moving Up
    {
      if (newPlayerPos.y < 0.0f)
        newPlayerPos.y = 0;
      if (gameMap.getTile(newPlayerPos.x, newPlayerPos.y) != '.' ||
          gameMap.getTile(newPlayerPos.x + 0.9f, newPlayerPos.y) != '.')
      {
        newPlayerPos.y = (int)newPlayerPos.y + 1;
        player.velocity.y = 0;
      }
    }
    else if (player.velocity.y > 0) // Moving Down
    {
      if (
          gameMap.getTile(newPlayerPos.x, newPlayerPos.y + 1.0f) != '.' ||
          gameMap.getTile(newPlayerPos.x + 0.9f, newPlayerPos.y + 1.0f) != '.')
      {
        newPlayerPos.y = (int)newPlayerPos.y;
        player.velocity.y = 0;
        player.isOnGround = true; // Player has a solid surface underfoot
      }
    }
    // Apply player move
    player.position.x = newPlayerPos.x;
    player.position.y = newPlayerPos.y;
    /* Set camera
    --------------*/
    // Focus on player
    cameraPos.x = player.position.x;
    cameraPos.y = player.position.y;
    // Calculate Top-Leftmost visible tile
    screenOffset.x = cameraPos.x * tileSizeX - tileSizeX * 1 / tileSizeX;
    screenOffset.y = cameraPos.y * tileSizeY - tileSizeY * 1 / tileSizeY;
    // Stop camera in map limit
    float maxOffsetX = gameMap.current[0].size() * tileSizeX - 2;
    if (screenOffset.x < 0)
      screenOffset.x = 0;
    else if (screenOffset.x > maxOffsetX)
      screenOffset.x = maxOffsetX;
    if (screenOffset.y < 0)
      screenOffset.y = 0;
    /* Clear GFX buffers
    ---------------------*/
    gfx.clear();
    gfx.backgroundColor(fColor::Black);
    /* Draw Map
    ------------*/
    float tilesOffsetY = screenOffset.y / tileSizeY;
    float tilesTopHalf = (tilesOffsetY - (int)tilesOffsetY) * tileSizeY;

    float tilesOffsetX = screenOffset.x / tileSizeX;
    float tilesLeftHalf = (tilesOffsetX - (int)tilesOffsetX) * tileSizeX;

    auto drawXTiles = [&](float &oY, float fY) {
      float x = -1.0f;
      short tileX = tilesOffsetX;
      gfx.rectangle(
          x,
          tileSizeX - tilesLeftHalf,
          oY,
          fY,
          gameMap.getTileColor(tileX++, tilesOffsetY));
      x += tileSizeX - tilesLeftHalf;
      do
      {
        gfx.rectangle(
            x,
            x + tileSizeX,
            oY,
            fY,
            gameMap.getTileColor(tileX++, tilesOffsetY));
        x += tileSizeX;
      } while (x < 0.9f);
      gfx.rectangle(
          x,
          1.0f,
          oY,
          fY,
          gameMap.getTileColor(tileX++, tilesOffsetY++));
    };
    // Draw upside consider Y offset
    float y = 1.0f;
    drawXTiles(y, tileSizeY - tilesTopHalf);
    y -= tileSizeY - tilesTopHalf;
    do
    { // Draw Between Up and Down Consider just X Offset
      drawXTiles(y, y - tileSizeY);
      y -= tileSizeY;
    } while (y > -0.9f);
    // Draw downside consider Y offset
    drawXTiles(y, -1.0f);
    /* Draw player
    ---------------*/
    float oPlayerX = (player.position.x * tileSizeX - screenOffset.x) - 1.0f;
    float fPlayerX = oPlayerX + tileSizeX;
    float oPlayerY = (-player.position.y * tileSizeY + screenOffset.y) + 1.0f;
    float fPlayerY = oPlayerY - tileSizeY;
    gfx.rectangle(
        oPlayerX > -1.0f ? oPlayerX : -1.0f,
        fPlayerX < 1.0f ? fPlayerX : 1.0f,
        oPlayerY < 1.0f ? oPlayerY : 1.0f,
        fPlayerY > -1.0f ? fPlayerY : -1.0f,
        Color::Red);
    /* Render
    ----------*/
    gfx.render();
  }
  return 0;
}