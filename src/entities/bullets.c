#include "bullets.h"
#include "macros.h"

t_bullet* bullet_create(vector2D location, vector2D velocity)
{
	t_bullet* bullet = malloc(sizeof(t_bullet));
	bullet->collision.center = location;
	bullet->collision.length = 10.0;

    bullet->velocity = scaleVector(velocity, 600);

    return bullet;
}

void bullet_render(SDL_Renderer* renderer, t_bullet* bullet, unsigned int nbLines)
{
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
    bullet->collision.center = addVectors(bullet->collision.center, scaleVector(bullet->velocity, deltaTime));
}