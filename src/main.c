#include "raylib.h"
#include <stdio.h>

// Program main entry point
int main(void)
{
  // Initialization
  const int screenWidth = 800;
  const int screenHeight = 450;

  InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");
  SetTargetFPS(60); // Set our game to run at 60 frames-per-second

  // Main game loop
  while (!WindowShouldClose()) // Detect window close button or ESC key
  {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawText("Move player one with arrow keys", 10, 10, 20, DARKGRAY);
    EndDrawing();
  }

  // De-Initialization
  CloseWindow(); // Close window and OpenGL context

  return 0;
}