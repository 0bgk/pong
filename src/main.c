#include "raylib.h"
#include <stdio.h>
#include <math.h>

#define MAX_POINTS 10000
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 450;
const float PLAYER_VELOCITY = 300.f;

struct GameMetrics
{
  float velocity;
  float acceleration;
  bool right;
  bool top;
  float degrees;
  int pointCount;
};

struct Vector
{
  Vector2 position;
  Vector2 size;
  Color color;
};

struct Vector CreatePlayer(short playerNum)
{
  struct Vector player;
  player.size = (Vector2){12, 60};
  player.position.x = playerNum == 2 ? (float)(SCREEN_WIDTH - player.size.x) : 0.f;
  player.position.y = (float)((SCREEN_HEIGHT / 2) - (player.size.y / 2));
  player.color = BLACK;
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

void PlayerMotion(struct Vector *player, int upKey, int downKey, float deltaTime)
{
  if (IsKeyDown(upKey) && player->position.y > 0)
    player->position.y -= PLAYER_VELOCITY * deltaTime;
  if (IsKeyDown(downKey) && (player->position.y + player->size.y) < SCREEN_HEIGHT)
    player->position.y += PLAYER_VELOCITY * deltaTime;
}

void BallMotion(struct Vector *ball, bool right, bool top, float x, float y)
{
  ball->position.x += right ? -x : x;
  ball->position.y += top ? -y : y;
}

void PayerCollision(struct Vector *player, float centerBallY, float *degrees, bool *top, bool *right)
{
  float vectorColides = centerBallY - player->position.y;
  float centerPosition = (vectorColides - (player->size.y / 2));

  *degrees = centerPosition * 180.f / player->size.y;
  *top = centerPosition > 30;
  *right = !(*right);
}

void ResetGame(struct GameMetrics *gameMetrics, struct Vector *ball, struct Vector *player, struct Vector *player2)
{
  gameMetrics->velocity = 300.f;
  gameMetrics->degrees = 0.f;
  gameMetrics->acceleration = 0.f;
  gameMetrics->right = (int)(GetTime() * 1000) % 2;
  gameMetrics->top = true;
  *ball = CreateBall();
  *player = CreatePlayer(1);
  *player2 = CreatePlayer(2);
}

int main(void)
{
  // Create Vectors
  struct Vector ball;
  struct Vector berserk;
  struct Vector griffith;

  // Vector for draw ball direction
  // Vector2 points[MAX_POINTS] = {0};

  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "PONG");
  // Set our game to run at 60 frames-per-second
  SetTargetFPS(120);

  // Create game metrics
  struct GameMetrics gameMetrics;
  // Reset game metrics when game starts
  ResetGame(&gameMetrics, &ball, &berserk, &griffith);

  // Main game loop
  // Detect window close button or ESC key
  while (!WindowShouldClose())
  {
    // Calc delta time
    float deltaTime = GetFrameTime();

    // Player motions
    PlayerMotion(&berserk, KEY_W, KEY_S, deltaTime);
    PlayerMotion(&griffith, KEY_UP, KEY_DOWN, deltaTime);

    // Convert degrees to radians
    double radians = gameMetrics.degrees * M_PI / 180.0;

    // Calc velocity and acceleration of axles with Pythagoras and Cinematics
    float voy = gameMetrics.velocity * sin(radians);
    float vox = gameMetrics.velocity * cos(radians);
    float ay = gameMetrics.acceleration * sin(radians);
    float ax = gameMetrics.acceleration * cos(radians);

    float x = vox * deltaTime + 0.5f * ax * pow(deltaTime, 2);
    float y = voy * deltaTime + 0.5f * ay * pow(deltaTime, 2);

    float centerBallY = (ball.position.y + (ball.size.y / 2));
    float centerBallX = gameMetrics.right == true ? (ball.position.x - ball.size.x) : (ball.position.x + ball.size.x);

    // Save actual position of vectors for draw it
    // if (gameMetrics.pointCount < MAX_POINTS)
    // {
    //   points[gameMetrics.pointCount++] = (Vector2){
    //       centerBallX,
    //       centerBallY,
    //   };
    // }

    // Ball collision with upper and lower window height
    if ((ball.position.y < 0) || (ball.position.y > (SCREEN_HEIGHT - ball.size.y)))
    {
      ball.position.y = (ball.position.y < 0) ? 0 : (SCREEN_HEIGHT - ball.size.y);
      gameMetrics.top = !gameMetrics.top;
    }

    // Ball motion
    BallMotion(&ball, gameMetrics.right, gameMetrics.top, x, y);

    // Player collition
    struct Vector *currentPlayer = gameMetrics.right ? &berserk : &griffith;
    float playerPosX = currentPlayer->position.x;
    float playerPosY = currentPlayer->position.y;
    float playerSizeY = currentPlayer->size.y;

    bool includesInsidePlayer = (centerBallY >= playerPosY) && (centerBallY <= (playerPosY + playerSizeY));
    bool colidesWithPlayer = gameMetrics.right ? (playerPosX >= centerBallX) : (playerPosX <= centerBallX);

    if (includesInsidePlayer && colidesWithPlayer)
    {
      PayerCollision(currentPlayer, centerBallY, &gameMetrics.degrees, &gameMetrics.top, &gameMetrics.right);
      gameMetrics.acceleration += 10000;
    }

    // Reset the game when the ball goes out of bounds
    if ((centerBallX < -50) || (centerBallX > (SCREEN_WIDTH + 50)))
    {
      ResetGame(&gameMetrics, &ball, &berserk, &griffith);
    }

    // Draw items
    BeginDrawing();
    // Draw vector position
    //  for (int i = 1; i < gameMetrics.pointCount; i++)
    //  {
    //    DrawLineV(points[i - 1], points[i], RED);
    //  }
    ClearBackground(RAYWHITE);
    DrawVector(ball);
    DrawVector(berserk);
    DrawVector(griffith);
    EndDrawing();
  }

  // Close window and OpenGL context
  CloseWindow();

  return 0;
}