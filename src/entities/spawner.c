#include "spawner.h"
#include "macros.h"

void spawner_render(SDL_Renderer* renderer, t_spawner* spawner1)
{
    //draw spawner
    int x1, y1, x2, y2;
    x1 = spawner1->location.x - SPAWNER_SIZE;
    y1 = spawner1->location.y - SPAWNER_SIZE;
    x2 = spawner1->location.x + SPAWNER_SIZE;
    y2 = spawner1->location.y + SPAWNER_SIZE;
    SDL_SetRenderDrawColor(renderer, SPAWNER_COLOR);
    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
    SDL_RenderDrawLine(renderer, x1, y2, x2, y1);
}