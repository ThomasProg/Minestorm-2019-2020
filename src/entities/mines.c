#include "mines.h"
#include "mines_subtype/minelayer.h"
#include "worldField.h"
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

void floatingMine_init(t_mine* mine, E_SIZE sizeType)
{
    mine_collisionBox_init(mine->entity.collision, 3, 
        25 * mine->size * MINE_SIZE_RATIO, 100 * mine->size * MINE_SIZE_RATIO);
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
}

void magneticMine_init(t_mine* mine, E_SIZE sizeType)
{
    mine_collisionBox_init(mine->entity.collision, 4, 
        25 * mine->size * MINE_SIZE_RATIO, 100 * mine->size * MINE_SIZE_RATIO);
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
}

void fireballMine_init(t_mine* mine, E_SIZE sizeType)
{
    mine_collisionBox_init(mine->entity.collision, 4, 
        50 * mine->size * MINE_SIZE_RATIO, 100 * mine->size * MINE_SIZE_RATIO);
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
}

void magneticFireballMine_init(t_mine* mine, E_SIZE sizeType)
{
    mine_collisionBox_init(mine->entity.collision, 4, 
            10 * mine->size * MINE_SIZE_RATIO, 100 * mine->size * MINE_SIZE_RATIO);
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
}

void mine_initForDifferentTypes(t_mine* mine, unsigned int type, E_SIZE sizeType)
{
    //floating mine
    switch (type)
    {
        case 0 :
            floatingMine_init(mine, sizeType);
            break;

        case 1 :
            fireballMine_init(mine, sizeType);
            break;

        case 2 :
            magneticMine_init(mine, sizeType);
            break;

        case 3 :
            magneticFireballMine_init(mine, sizeType);
            break;
        default :
            //assert(false); //no mine of this id exists
            break;
        case 4 :
            minelayer_init(mine, sizeType);
            break;
    }
}

void mine_init(t_mine* mine, unsigned int type, vector2D location, E_SIZE sizeType)
{
    entity_init(&mine->entity);

    mine->isMineLayer = false;
    mine->sizeType = sizeType;
    mine->mineLayerDelay = 5.f;

    mine->entity.collision = malloc(sizeof(polygon));

    if (sizeType == SMALL)
        mine->size = SMALL_SIZE;
    if (sizeType == MEDIUM)
        mine->size = MEDIUM_SIZE;
    if (sizeType == BIG)
        mine->size = BIG_SIZE;

    mine_initForDifferentTypes(mine, type, sizeType);
    //set floating mines speed
    if (!mine->followPlayer && FLOATING_MINE_SPEED != 0)
    {
        vector2D velocity = {rand() % 100 - 50, rand() % 100 - 50};
        if (vectorIsNearlyEqual(velocity, nullVector(), 1))
            velocity.x = 1.f;
        velocity = unitVector(velocity);
            mine->entity.velocity = scaleVector(velocity, 3 / mine->size * FLOATING_MINE_SPEED * 3);
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

void mine_newTarget(t_dynamicArray* players, t_mine* mine)
{
    unsigned int firstPlayerID = 0; 
    if (!dynamicArray_GetValidItemIndex(players, &firstPlayerID))
        return;
    t_player* player = dynamicArray_GetItem(players, firstPlayerID); 
    mine->target = player;
    for (unsigned int j = 1; dynamicArray_GetValidItemIndex(players, &j); j++)
    {
        player = dynamicArray_GetItem(players, j);
        if (mine->target != NULL)
        {
            float distance = vectorSquareLength(substractVectors(
                            player->entity.ref.origin, mine->entity.ref.origin));
            float currentDistance = vectorSquareLength(substractVectors(
                                    mine->target->entity.ref.origin, mine->entity.ref.origin));
            if (currentDistance > distance)
                mine->target = player;
        }
        else
            mine->target = player;
    }
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

void magneticMine_searchForPlayer(t_mine* mine, float deltaTime)
{
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

void mine_tick(t_mine* mine, float deltaTime)
{
    if (mine->followPlayer)
        entity_tick(&mine->entity, deltaTime);
    else
    {
        vector2D newVelocity = scaleVector(mine->entity.velocity, deltaTime);
        mine->entity.ref.origin = addVectors(mine->entity.ref.origin, newVelocity);
        if (mine->entity.isTeleportingAtBorder)
            border_teleportation(&mine->entity.ref.origin);
    }

    //Magnetic mines
    if (mine->followPlayer)
        magneticMine_searchForPlayer(mine, deltaTime);
    
    else
    {
        float speed = FLOATING_MINE_SPEED;
        switch (mine->sizeType)
        {
            case BIG :
                speed *= BIG_SPEED;
                break;
            case MEDIUM :
                speed *= MEDIUM_SPEED;
                break;
            case SMALL :
                speed *= SMALL_SPEED;
                break;

            default:
                break;
        }
        if (!vectorIsNearlyEqual(mine->entity.velocity, nullVector(), 0.1))
            mine->entity.velocity = scaleVector(unitVector(mine->entity.velocity), speed);
    }
}

void throwFireball(t_mine* mine, t_dynamicArray* bullets)
{
	vector2D bulletLocation = mine->entity.ref.origin;
	t_bullet* bullet 	    = dynamicArray_AddItem(bullets);

	vector2D shootDirection = substractVectors(mine->target->entity.ref.origin, mine->entity.ref.origin);
	
	if (!floatIsNearlyEqual(vectorLength(shootDirection), 0, 0.001f))
		bullet_init(bullet, bulletLocation, (vector2D) {0.f, 0.f}, unitVector(shootDirection), false);
}