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

void PayerMotion(struct Vector *player, int upKey, int downKey, float deltaTime)
{
  if (IsKeyDown(upKey) && player->position.y > 0)
    player->position.y -= CalcMRU(PLAYER_VELOCITY, deltaTime);
  if (IsKeyDown(downKey) && (player->position.y + player->size.y) < SCREEN_HEIGHT)
    player->position.y += CalcMRU(PLAYER_VELOCITY, deltaTime);
}

void PayerCollision(struct Vector *player, float centerBallY, float *degrees, bool *top, bool *right)
{
  float vectorColides = centerBallY - player->position.y;
  float centerPosition = (vectorColides - (player->size.y / 2));

  *degrees = centerPosition * 180.f / player->size.y;
  *top = centerPosition > 30;
  *right = !(*right);
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

  float velocity = 400.f;
  float degrees = 0.f;
  float acceleration = 0.f;
  // bool randomBool = (int)(GetTime() * 1000) % 2;
  bool right = true;
  bool top = true;
  int pointCount = 0;

  // Main game loop
  // Detect window close button or ESC key
  while (!WindowShouldClose())
  {
    // Calc delta time
    float deltaTime = GetFrameTime();

    // Player motions
    PayerMotion(&berserk, KEY_W, KEY_S, deltaTime);
    PayerMotion(&griffith, KEY_UP, KEY_DOWN, deltaTime);

    // Convert degrees to radians
    double radians = degrees * M_PI / 180.0;

    // Calc velocity and acceleration of axles
    float voy = velocity * sin(radians);
    float vox = velocity * cos(radians);
    float ay = acceleration * sin(radians);
    float ax = acceleration * cos(radians);

    float x = vox * deltaTime + 0.5f * ax * pow(deltaTime, 2);
    float y = voy * deltaTime + 0.5f * ay * pow(deltaTime, 2);

    float centerBallY = (ball.position.y + (ball.size.y / 2));
    float centerBallX = (ball.position.x + (ball.size.x / 2));
    // Almacena la posición actual del vector
    if (pointCount < MAX_POINTS)
    {
      points[pointCount++] = (Vector2){
          centerBallX,
          centerBallY,
      };
    }

    // collision with the lower and upper window hegith
    if ((ball.position.y < 0) || (ball.position.y > (SCREEN_HEIGHT - ball.size.y)))
    {
      ball.position.y = (ball.position.y < 0) ? 0 : (SCREEN_HEIGHT - ball.size.y);
      top = !top;
    }

    // Ball motion
    ball.position.x += right ? -x : x;
    ball.position.y += top ? -y : y;

    // Player collition
    struct Vector *currentPlayer = right ? &berserk : &griffith;
    float playerPosX = currentPlayer->position.x;
    float playerPosY = currentPlayer->position.y;
    float playerSizeY = currentPlayer->size.y;

    bool includesInsidePlayer = (centerBallY >= playerPosY) && (centerBallY <= (playerPosY + playerSizeY));
    bool colidesWithPlayer = right ? (playerPosX >= centerBallX) : (playerPosX <= centerBallX);

    if (includesInsidePlayer && colidesWithPlayer)
      PayerCollision(currentPlayer, centerBallY, &degrees, &top, &right);

    acceleration += 10;

    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawText("Move player one with arrow keys", 10, 10, 20, DARKGRAY);
    char accelerationText[50];
    sprintf(accelerationText, "Acceleration: %.2f", centerBallY - berserk.position.y);
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