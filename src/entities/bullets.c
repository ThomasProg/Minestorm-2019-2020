#include "bullets.h"
#include "worldField.h"
#include "macros.h"

// t_bullet* bullet_create()
// {
// 	t_bullet* bullet = malloc(sizeof(t_bullet));

//     return bullet;
// }

void bullet_init(t_bullet* bullet, vector2D location, 
                 vector2D relativeVelocity, vector2D shootDirection, bool isBullet)
{
	bullet->collision.center = location;
	bullet->collision.length = BULLETS_SIZE;
    bullet->timeAlive = 0.f;
    bullet->isBullet = isBullet;

    bullet->velocity = scaleVector(shootDirection, BULLET_SPEED); //add velocity at shooting
}

void bullet_destroy(t_bullet* bullet)
{
	//free(bullet);
}

void bullet_render(SDL_Renderer* renderer, t_bullet* bullet, unsigned int nbLines)
{
    vector2D origin = bullet->collision.center;
    float radius = bullet->collision.length;//radius
    float toAdd = 2 * PI / nbLines;
    float loc = 0.f;
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    for (unsigned int i = 0; i < nbLines; i++)
    {  
        SDL_RenderDrawLine(renderer, origin.x + radius * cosf(loc + toAdd), origin.y + radius * sinf(loc + toAdd), 
            origin.x + radius * cosf(loc), origin.y + radius * sinf(loc));
        loc += toAdd;
    }
}

void bullet_tick(t_bullet* bullet, float deltaTime)
{
    bullet->timeAlive += deltaTime;
    bullet->collision.center = addVectors(bullet->collision.center, scaleVector(bullet->velocity, deltaTime));
    border_teleportation(&bullet->collision.center);
}
