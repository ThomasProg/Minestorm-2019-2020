#include "bullets.h"
#include "worldField.h"
#include "macros.h"

t_bullet* bullet_create()
{
	t_bullet* bullet = malloc(sizeof(t_bullet));
    bullet->isAlive = false;

    return bullet;
}

void bullet_init(t_bullet* bullet, vector2D location, vector2D relativeVelocity, vector2D shootDirection)
{
    bullet->isAlive = true;
	bullet->collision.center = location;
	bullet->collision.length = BULLETS_SIZE;
    bullet->timeAlive = 0.f;

    bullet->velocity = addVectors(scaleVector(shootDirection, 600), relativeVelocity); //add velocity at shooting
}

void bullet_render(SDL_Renderer* renderer, t_bullet* bullet, unsigned int nbLines)
{
    if (! bullet->isAlive)
        return;

    vector2D origin = bullet->collision.center;
    float l = bullet->collision.length;
    float toAdd = 2 * PI / nbLines;
    float loc = 0.f;
    for (unsigned int i = 0; i < nbLines; i++)
    {  
        SDL_RenderDrawLine(renderer, origin.x + l * cosf(loc + toAdd), origin.y + l * sinf(loc + toAdd), origin.x + l * cosf(loc), origin.y + l * sinf(loc));
        loc += toAdd;
    }
}

void bullet_tick(t_bullet* bullet, float deltaTime)
{
    bullet->timeAlive += deltaTime;
    bullet->collision.center = addVectors(bullet->collision.center, scaleVector(bullet->velocity, deltaTime));
    border_teleportation(&bullet->collision.center);

    if (bullet->timeAlive > BULLETS_TIME_ALIVE)
        bullet->isAlive = false;
}