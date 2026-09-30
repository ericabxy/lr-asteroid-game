#include <stdint.h>
#include <stdlib.h>
#include <math.h>

#include "libretro.h"
#include "gamedef.h"
#include "g_game.h"
#include "r_draw.h"

void G_SpawnAsteroid(game_t *game, int x, int y, int radius, float angle, float speed)
{
    for (int i = 0; i < MAXASTEROIDS; i++)
    {
        asteroid_t *asteroid = &game->asteroids[i];

        if (!asteroid->active)
        {
            asteroid->active = true;
            asteroid->x = x;
            asteroid->y = x;
            asteroid->radius = radius;
            asteroid->angle = angle;
            asteroid->speed = speed;
            break;
        }
    }
}

void G_SpawnBullet(game_t *game, int x, int y, float angle, float speed)
{
    for (int i = 0; i < MAXBULLETS; i++)
    {
        bullet_t *bullet = &game->bullets[i];

        if (!bullet->active)
        {
            bullet->active = true;
            bullet->x = x;
            bullet->y = y;
            bullet->angle = angle;
            bullet->timeleft = 4;
            break;
        }
    }
}

void G_InitGame(game_t *game)
{
    game->ship.x = WINDOW_WIDTH / 2 + (rand() % 10);
    game->ship.y = WINDOW_HEIGHT / 2;
    game->ship.radius = 30;
    game->ship.angle = 0;
    game->ship.speedX = 0;
    game->ship.speedY = 0;
    game->ship.bulletTimer = 0;

    // Initialize asteroids.
    for (int i = 0; i < MAXASTEROIDS; i++)
        game->asteroids[i].active = false;
    game->asteroids[0].x = 100;
    game->asteroids[0].y = 100;
    game->asteroids[0].active = true;
    game->asteroids[0].angle = rand() % (int) (2 * PI);
    game->asteroids[0].radius = 80;
    game->asteroids[0].speed = 20;
    game->asteroids[1].x = WINDOW_WIDTH - 100;
    game->asteroids[1].y = 100;
    game->asteroids[1].active = true;
    game->asteroids[1].angle = rand() % (int) (2 * PI);
    game->asteroids[1].radius = 80;
    game->asteroids[1].speed = 20;
    game->asteroids[2].x = WINDOW_WIDTH / 2;
    game->asteroids[2].y = WINDOW_HEIGHT - 100;
    game->asteroids[2].active = true;
    game->asteroids[2].angle = rand() % (int) (2 * PI);
    game->asteroids[2].radius = 80;
    game->asteroids[2].speed = 20;

    // Initialize bullets.
    for (int i = 0; i < MAXBULLETS; i++)
    {
        game->bullets[i].active = false;
        game->bullets[i].speed = BULLETSPEED;
        game->bullets[i].radius = BULLETSIZE;
    }
}

void G_Joystick(game_t *game, int direction)
{
    int shipSpeed = 100;
    int turnSpeed = 5;

    switch (direction)
    {
    case STICKRIGHT:
        game->ship.angle += turnSpeed * DELTATIME;
        break;

    case STICKLEFT:
        game->ship.angle -= turnSpeed * DELTATIME;
        break;

    case STICKUP:
        game->ship.speedX += cos(game->ship.angle) * shipSpeed * DELTATIME;
        game->ship.speedY += sin(game->ship.angle) * shipSpeed * DELTATIME;
        break;
    }
}

void G_FireBullet(game_t *game)
{
    if (game->ship.bulletTimer >= BULLETTIMERLIMIT)
    {
        game->ship.bulletTimer = 0;

        for (int i = 0; i < MAXBULLETS; i++)
        {
            if (!game->bullets[i].active)
            {
                game->bullets[i].active = true;
                game->bullets[i].x = game->ship.x + cos(game->ship.angle) * game->ship.radius;
                game->bullets[i].y = game->ship.y + sin(game->ship.angle) * game->ship.radius;
                game->bullets[i].angle = game->ship.angle;
                game->bullets[i].timeleft = 4;
                break;
            }
        }
    }
}

bool G_CheckTouching(int x0, int y0, int r0, int x1, int y1, int r1)
{
    return pow((x0 - x1), 2) + pow((y0 - y1), 2) <= pow((r0 + r1), 2);
}

void G_UpdateGame(game_t *game)
{
    game->ship.angle = fmod(game->ship.angle, (2 * PI));
    game->ship.bulletTimer += DELTATIME;
    game->ship.x += game->ship.speedX * DELTATIME;
    game->ship.y += game->ship.speedY * DELTATIME;
    if (game->ship.x > WINDOW_WIDTH)
        game->ship.x = game->ship.x - WINDOW_WIDTH;
    else if (game->ship.x < 0)
        game->ship.x = WINDOW_WIDTH + game->ship.x;
    if (game->ship.y > WINDOW_HEIGHT)
        game->ship.y = game->ship.y - WINDOW_HEIGHT;
    else if (game->ship.y < 0)
        game->ship.y = WINDOW_HEIGHT + game->ship.y;

    // Update asteroids.
    for (int i = 0; i < MAXASTEROIDS; i++)
    {
        asteroid_t *asteroid = &game->asteroids[i];

        if (asteroid->active)
        {
            asteroid->x += cos(asteroid->angle) * asteroid->speed * DELTATIME;
            asteroid->y += sin(asteroid->angle) * asteroid->speed * DELTATIME;
            if (asteroid->x > WINDOW_WIDTH)
                asteroid->x = asteroid->x - WINDOW_WIDTH;
            else if (asteroid->x < 0)
                asteroid->x = WINDOW_WIDTH + asteroid->x;
            if (asteroid->y > WINDOW_HEIGHT)
                asteroid->y = asteroid->y - WINDOW_HEIGHT;
            else if (asteroid->y < 0)
                asteroid->y = WINDOW_HEIGHT + asteroid->y;
            if (G_CheckTouching(game->ship.x, game->ship.y, game->ship.radius, asteroid->x, asteroid->y, asteroid->radius))
                G_InitGame(game);
        }
    }

    // Update bullets.
    for (int i = 0; i < MAXBULLETS; i++)
    {
        bullet_t *bullet = &game->bullets[i];

        if (bullet->active)
        {
            bullet->timeleft -= DELTATIME;
            if (bullet->timeleft <= 0)
                bullet->active = false;
            else
            {
                bullet->x += cos(bullet->angle) * bullet->speed * DELTATIME;
                bullet->y += sin(bullet->angle) * bullet->speed * DELTATIME;
                if (bullet->x > WINDOW_WIDTH)
                    bullet->x = bullet->x - WINDOW_WIDTH;
                else if (bullet->x < 0)
                    bullet->x = WINDOW_WIDTH + bullet->x;
                if (bullet->y > WINDOW_HEIGHT)
                    bullet->y = bullet->y - WINDOW_HEIGHT;
                else if (bullet->y < 0)
                    bullet->y = WINDOW_HEIGHT + bullet->y;
            }

            // Check to hit asteroids.
            for (int i = 0; i < MAXASTEROIDS; i++)
            {
                asteroid_t *asteroid = &game->asteroids[i];

                if (asteroid->active)
                {
                    if
                    (
                        G_CheckTouching(
                            bullet->x, bullet->y, bullet->radius,
                            asteroid->x, asteroid->y, asteroid->radius
                        )
                    )
                    {
                        bullet->active = false;
                        if (asteroid->radius > 15)
                        {
                            int angle1 = rand() % (int)(2 * PI);
                            int angle2 = (int)(angle1 - PI) % (int)(2 * PI);
                            int radius = asteroid->radius == 80 ? 50 : asteroid->radius == 50 ? 30 : 15;
                            int speed = radius == 50 ? 50 : radius == 30 ? 70 : 120;
                            G_SpawnAsteroid(game, asteroid->x, asteroid->y, radius, angle1, speed);
                            G_SpawnAsteroid(game, asteroid->x, asteroid->y, radius, angle2, speed);
                        }
                        asteroid->active = false;
                        break;
                    }
                }
            }
        }
    }
}

void G_RenderGame(game_t *game, uint32_t *buf)
{
    R_FillRect(buf, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0x000000);
    R_FillCircle(buf, game->ship.x, game->ship.y, game->ship.radius, 0x0000ff);
    R_FillCircle(
        buf,
        game->ship.x + cos(game->ship.angle) * SHIPCIRCLEDISTANCE,
        game->ship.y + sin(game->ship.angle) * SHIPCIRCLEDISTANCE,
        5,
        0x00ffff
    );
    for (int i = 0; i < MAXBULLETS; i++)
    {
        if (game->bullets[i].active)
            R_FillCircle(buf, game->bullets[i].x, game->bullets[i].y, game->bullets[i].radius, 0x00ff00);
    }
    for (int i = 0; i < MAXASTEROIDS; i++)
    {
        if (game->asteroids[i].active)
            R_FillCircle(buf, game->asteroids[i].x, game->asteroids[i].y, game->asteroids[i].radius, 0xffff00);
    }
}
