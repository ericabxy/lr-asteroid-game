#include <stdint.h>
#include <stdbool.h>

#ifndef __G_GAME__
#define __G_GAME__

#define DELTATIME 0.033333333333333
#define PI 3.14159265358979323846
// Object limits.
#define MAXASTEROIDS 10
#define MAXBULLETS 20
#define MAXSHIPS 2
// Bullet properties.
#define BULLETSIZE 5
#define BULLETSPEED 500
#define BULLETTIMERLIMIT 0.5
// Ship properties.
#define TURNSPEED 10
#define SHIPCIRCLEDISTANCE 20
// Joystick directions.
#define STICKUP 1
#define STICKRIGHT 2
#define STICKDOWN 4
#define STICKLEFT 8

typedef struct
{
    float x, y;
    int width, height;
} rectangle_t;

typedef struct
{
    float x, y, angle, speed;
    int radius;
    uint32_t color;
    bool active;
} asteroid_t;

typedef struct
{
    float x, y, angle;
    float speed, timeleft;
    int radius;
    uint32_t color;
    bool active;
} bullet_t;

typedef struct
{
    float x, y, angle;
    float speedX, speedY;
    float bulletTimer;
    int radius;
    uint32_t color;
} ship_t;

typedef struct
{
    ship_t ship;
    bullet_t bullets[MAXBULLETS];
    asteroid_t asteroids[MAXASTEROIDS];
} game_t;

void G_SpawnAsteroid(game_t *game, int x, int y, int radius, float angle, float speed);
void G_SpawnBullet(game_t *game, int x, int y, float angle, float speed);
void G_InitGame(game_t *game);
void G_FireBullet(game_t *game);
void G_Joystick(game_t *game, int direction);
bool G_CheckTouching(int x0, int y0, int r0, int x1, int y1, int r1);
void G_UpdateGame(game_t *game);
void G_RenderGame(game_t *game, uint32_t *buf);

#endif
