#include "mines.h"
#include "mines_subtype/minelayer.h"
#include "macros.h"

void mine_collisionBox_init(polygon* collision, 
    unsigned int nbPolygons /*SHALL NOT BE 0*/, float size1, float size2)
{
    collision->nbConvexPolygons = nbPolygons;
    collision->convexPolygons   = malloc(nbPolygons * sizeof(convexPolygon));

    //set each polygon of the star
    for (unsigned int i = 0; i < nbPolygons; i++)
    {
        vector2D* points = malloc(4 * sizeof(point2D));

        vector2D temp = {size1, 0.f};
        temp = rotateVector(temp, - PI/nbPolygons + 2 * PI / nbPolygons);

        points[0] = temp;
        points[1] = (point2D) {size2, 0.0};
        temp.y   *= -1;
        points[2] = temp;
        points[3] = (point2D) {0.0, -0.0};

        collision->convexPolygons[i].size = 4;

        float angle = i * 2 * PI / nbPolygons;

        for (unsigned int j = 0; j < collision->convexPolygons[i].size; j++)
        {
            //RotatePoint is less peformant.
            //Since the center of rotation is (0;0) , we can use rotateVector instead.
            points[j] = rotateVector(points[j], angle);
        }

        collision->convexPolygons[i].points = points;
    }
}

// void mine_spawn(t_dynamicArray* mines)
// {
//     world->spawnDelay = 0.3f;

//     unsigned int mineType = mines_readyToSpawn(&world->minesBySize[2]);
//     E_SIZE size = BIG;
//     if (mineType == 5)
//     {
//         mineType = mines_readyToSpawn(&world->minesBySize[1]);
//         size = MEDIUM;
//     }	
//     if (mineType == 5)
//     {
//         mineType = mines_readyToSpawn(&world->minesBySize[0]);
//         size = SMALL;
//     }	
//     if (mineType != 5)
//     {
//         t_mine* mine = dynamicArray_AddItem(mines);
//         mine_init(mine, mineType, spawner->location, size);
//         dynamicArray_RemoveItem(&world->spawners, i);
//     }
// }

void mine_init(t_mine* mine, unsigned int type, vector2D location, E_SIZE sizeType)
{
    entity_init(&mine->entity);

    mine->isMineLayer = false;
    mine->sizeType = sizeType;
    mine->mineLayerDelay = 5.f;

    mine->entity.collision = malloc(sizeof(polygon));

    //floating mine
    switch (type)
    {
        case 0 :
            mine_collisionBox_init(mine->entity.collision, 3, 25, 100);
            mine->followPlayer = false;
            mine->throwFireballs = false;
            switch (sizeType)
            {
                case SMALL:
                    mine->givenScore = 200;
                    break;
                case MEDIUM:
                    mine->givenScore = 135;
                    break;
                case BIG:
                    mine->givenScore = 100;
                    break;
                
                default:
                    break;
            }
            break;

        case 1 :
            mine_collisionBox_init(mine->entity.collision, 4, 25, 100);
            mine->followPlayer = true;
            mine->throwFireballs = false;
            switch (sizeType)
            {
                case SMALL:
                    mine->givenScore = 600;
                    break;
                case MEDIUM:
                    mine->givenScore = 535;
                    break;
                case BIG:
                    mine->givenScore = 500;
                    break;
                
                default:
                    break;
            }
            break;
        
        case 2 :
            mine_collisionBox_init(mine->entity.collision, 4, 50, 100);
            mine->followPlayer = false;
            mine->throwFireballs = true;
            switch (sizeType)
            {
                case SMALL:
                    mine->givenScore = 325;
                    break;
                case MEDIUM:
                    mine->givenScore = 360;
                    break;
                case BIG:
                    mine->givenScore = 425;
                    break;
                
                default:
                    break;
            }
            break;

        case 3 :
            mine_collisionBox_init(mine->entity.collision, 4, 10, 100);
            mine->followPlayer = true;
            mine->throwFireballs = true;
            switch (sizeType)
            {
                case SMALL:
                    mine->givenScore = 850;
                    break;
                case MEDIUM:
                    mine->givenScore = 585;
                    break;
                case BIG:
                    mine->givenScore = 750;
                    break;
                
                default:
                    break;
            }
            break;
        default :
            //assert(false); //no mine of this id exists
            break;
        case 4 :
            minelayer_collisionBox_init(mine->entity.collision);
            //mine_collisionBox_init(mine->entity.collision, 10, 10, 100);
            mine->followPlayer = false;
            mine->throwFireballs = false;
            mine->isMineLayer = true;
            mine->sizeType = BIG;
            mine->givenScore = 1000;
            mine->entity.isTeleportingAtBorder = false;
            break;
    }

    if (sizeType == SMALL)
        mine->size = SMALL_SIZE;
    if (sizeType == MEDIUM)
        mine->size = MEDIUM_SIZE;
    if (sizeType == BIG)
        mine->size = BIG_SIZE;

    polygon* collision = mine->entity.collision;
    //TODO : resize Polygon (because same as player's one)
	for (unsigned int i = 0; i < collision->nbConvexPolygons; i++)
	{	
		for (unsigned int j = 0; j < collision->convexPolygons[i].size; j++)
		{
			collision->convexPolygons[i].points[j] = scaleVector(collision->convexPolygons[i].points[j], mine->size * WINDOW_SCALE);
		}
	}

    if (!mine->followPlayer && FLOATING_MINE_SPEED != 0)
    {
        vector2D velocity = {rand() % 100 - 50, rand() % 100 - 50};
        if (velocity.x == 0.f && velocity.y == 0.f)
            velocity.x = 1.f;
        velocity = unitVector(velocity);
            mine->entity.velocity = scaleVector(velocity, 1 / mine->size * FLOATING_MINE_SPEED);
    }

    mine->entity.ref.origin = location;
	mine->entity.collisionType = E_MINE;
    mine->target = NULL;
}

void mine_destroy(t_mine* mine)
{
    polygon_free(mine->entity.collision);
    free(mine->entity.collision);
    polygon_free(&mine->entity.worldCollider);
}

void mine_render(t_mine* mine, t_render* render, bool renderDebug)
{
    entity_render(&mine->entity, render, renderDebug);

    polygon_aabb_generate(&mine->entity.worldCollider);

    SDL_SetRenderDrawColor(render->renderer, 255, 255, 0, 255);
    polygon_render(render->renderer, &mine->entity.worldCollider, renderDebug);
}

vector2D getShortestPath(vector2D a, vector2D b)
{
    if (vectorSquareLength(a) > vectorSquareLength(b)) //for optimization
        return b;
    else
        return a;
}

void mine_tick(t_mine* mine, float deltaTime)
{
    entity_tick(&mine->entity, deltaTime);

    //Magnetic mines
    if (mine->followPlayer)
    {
        //search for player
        if (mine->target != NULL)
        {
            vector2D mineLoc = mine->target->entity.ref.origin; 
            vector2D targetLoc = mine->entity.ref.origin;
            vector2D targetXP = addVectors(targetLoc, (vector2D) {WINDOW_SIZE_X, 0.f});
            vector2D targetXM = addVectors(targetLoc, (vector2D) {- WINDOW_SIZE_X, 0.f});
            vector2D targetYP = addVectors(targetLoc, (vector2D) {0.f, WINDOW_SIZE_Y});
            vector2D targetYM = addVectors(targetLoc, (vector2D) {0.f, - WINDOW_SIZE_Y});
            
            vector2D shortestPath = substractVectors(mineLoc, targetLoc);
            shortestPath = getShortestPath(shortestPath, substractVectors(mineLoc, targetXP));
            shortestPath = getShortestPath(shortestPath, substractVectors(mineLoc, targetXM));
            shortestPath = getShortestPath(shortestPath, substractVectors(mineLoc, targetYP));
            shortestPath = getShortestPath(shortestPath, substractVectors(mineLoc, targetYM));

            vector2D direction = nullVector();
            if (vectorLength(shortestPath) != 0.f)
            {
                direction = unitVector(shortestPath);
                mine->entity.ref.unitI = direction;
                mine->entity.ref.unitJ = rotateVector90(mine->entity.ref.unitI);
                entity_move(&mine->entity, E_FORWARD, deltaTime);
            }
            mine->entity.maxSpeed = MAGNETIC_MINE_SPEED;
        }
    }
    
    else
    {
        float speed = FLOATING_MINE_SPEED;
        switch (mine->sizeType)
        {
            case BIG :
                speed *= 1.f;
                break;
            case MEDIUM :
                speed *= 1.3f;
                break;
            case SMALL :
                speed *= 1.5f;
                break;

            default:
                break;
        }
        if (mine->entity.velocity.x != 0.0f && mine->entity.velocity.y != 0.0f)
            mine->entity.velocity = scaleVector(unitVector(mine->entity.velocity), speed);
    }
}
