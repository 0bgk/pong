#include "raylib.h"
#include <stdio.h>
#include <math.h>

// CONSTANTS
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 450;
const float PLAYER_VELOCITY = 300.f;
#define MAX_POINTS 10000

struct Vector
{
  Vector2 position;
  Vector2 size;
  Color color;
};

struct Vector CreatePlayer(int SCREEN_WIDTH, int SCREEN_HEIGHT, Color color, short number)
{
  struct Vector player;
  player.size = (Vector2){12, 60};
  player.position.x = number == 2 ? (float)(SCREEN_WIDTH - player.size.x) : 0.f;
  player.position.y = (float)((SCREEN_HEIGHT / 2) - (player.size.y / 2));
  player.color = color;

  return player;
}

struct Vector CreateBall()
{
  struct Vector ball;
  ball.size = (Vector2){12, 12};
  ball.position.y = (float)((SCREEN_HEIGHT / 2) - (ball.size.y / 2));
  ball.position.x = (float)((SCREEN_WIDTH / 2) - (ball.size.x / 2));
  ball.color = BLACK;

  return ball;
}

void DrawVector(struct Vector player)
{
  DrawRectangleV(player.position, player.size, player.color);
}

float CalcMRU(float v, float t)
{
  return v * t;
}

float CalcMRUA(float v, float t, float a)
{
  return v * t + 1 / 2 * a * pow(t, 2);
}

void PayerMotion(struct Vector *player, int upKey, int downKey, float deltaTime)
{
  if (IsKeyDown(upKey) && player->position.y > 0)
    player->position.y -= CalcMRU(PLAYER_VELOCITY, deltaTime);
  if (IsKeyDown(downKey) && (player->position.y + player->size.y) < SCREEN_HEIGHT)
    player->position.y += CalcMRU(PLAYER_VELOCITY, deltaTime);
}

// Program main entry point
int main(void)
{
  // Create Vectors
  struct Vector ball = CreateBall();
  struct Vector berserk = CreatePlayer(SCREEN_WIDTH, SCREEN_HEIGHT, BLACK, 1);
  struct Vector griffith = CreatePlayer(SCREEN_WIDTH, SCREEN_HEIGHT, BLACK, 2);
  Vector2 points[MAX_POINTS] = {0};

  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "raylib [core] example - basic window");
  SetTargetFPS(120); // Set our game to run at 60 frames-per-second

  float velocity = 300.f;
  float degrees = 0.f;
  float acceleration = 0;
  // bool randomBool = (int)(GetTime() * 1000) % 2;
  bool rigth = true;
  bool top = true;
  int pointCount = 0;

  // Main game loop
  // Detect window close button or ESC key
  while (!WindowShouldClose())
  {
    // Calc delta time
    float deltaTime = GetFrameTime();

    PayerMotion(&berserk, KEY_W, KEY_S, deltaTime);
    PayerMotion(&griffith, KEY_UP, KEY_DOWN, deltaTime);

    // Vector Ball values
    // Pythagoras
    double radians = degrees * M_PI / 180.0;

    // Calc velocity and acceleration of axles
    float voy = velocity * sin(radians);
    float vox = velocity * cos(radians);
    float ay = acceleration * sin(radians);
    float ax = acceleration * cos(radians);

    float x = vox * deltaTime + 0.5f * ax * pow(deltaTime, 2);
    float y = voy * deltaTime + 0.5f * ay * pow(deltaTime, 2);

    float centerVectorBallY = (ball.position.y + (ball.size.y / 2));
    float centerVectorBallX = (ball.position.x + (ball.size.x / 2));
    // Almacena la posición actual del vector
    if (pointCount < MAX_POINTS)
    {
      points[pointCount++] = (Vector2){
          centerVectorBallX,
          centerVectorBallY,
      };
    }

    // collision with the lower and upper window hegith
    if ((ball.position.y < 0) || (ball.position.y > (SCREEN_HEIGHT - ball.size.y)))
    {
      ball.position.y = (ball.position.y < 0) ? 0 : (SCREEN_HEIGHT - ball.size.y);
      top = !top;
    }

    // Test
    if (ball.position.x < 0)
    {
      rigth = !rigth;
    }

    // Ball motion
    ball.position.x += rigth ? -x : x;
    ball.position.y += top ? -y : y;

    // Player collition
    if (rigth == true)
    {
      // Check comparations
      //   for (size_t i = 0; i < berserk.size.y; i++)
      //   {
      //     // ball.position collides with the paddle
      //     // adjust ball.position.x comparative
      //     if (((int)berserk.position.y + i) == centerVectorBallY + (ball.size.y / 2) && centerVectorBallX <= berserk.position.x)
      //     {
      //       float centerPosition = (i - (berserk.size.y / 2));
      //       degrees = centerPosition * 180.f / berserk.size.y;
      //       top = centerPosition > 30;
      //       rigth = false;
      //     }
      //   }
      // }
      // else
      // {
      //   for (size_t i = 0; i < griffith.size.y; i++)
      //   {
      //     // ball.position collides with the paddle
      //     // adjust ball.position.x comparative
      //     if (((int)griffith.position.y + i) == centerVectorBallY + (ball.size.y / 2) && centerVectorBallX >= griffith.position.x)
      //     {
      //       float centerPosition = (i - (griffith.size.y / 2));
      //       degrees = centerPosition * 180.f / griffith.size.y;
      //       top = centerPosition > 29;
      //       rigth = true;
      //     }
      //   }
      // }

      acceleration += 0;

      BeginDrawing();
      ClearBackground(RAYWHITE);
      DrawText("Move player one with arrow keys", 10, 10, 20, DARKGRAY);
      char accelerationText[50];
      sprintf(accelerationText, "Acceleration: %.2f", acceleration);
      DrawText(accelerationText, 10, 30, 20, BLACK);

      char berserkCoords[50];
      for (int i = 1; i < pointCount; i++)
      {
        DrawLineV(points[i - 1], points[i], RED);
      }
      DrawVector(ball);
      DrawVector(berserk);
      DrawVector(griffith);
      EndDrawing();
    }

    // De-Initialization
    CloseWindow(); // Close window and OpenGL context

    return 0;
  }