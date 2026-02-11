#include "collision.h"

bool Collision_CheckCollisionF(SDL_FRect *rect1, SDL_FRect *rect2)
{
    return !(rect1->x + rect1->w < rect2->x || // rect1 is to the left of rect2
             rect1->x > rect2->x + rect2->w || // rect1 is to the right of rect2
             rect1->y + rect1->h < rect2->y || // rect1 is above rect2
             rect1->y > rect2->y + rect2->h);  // rect1 is below rect2
}

bool Collision_CheckCollision(SDL_Rect *rect1, SDL_Rect *rect2)
{
    return !(rect1->x + rect1->w < rect2->x || // rect1 is to the left of rect2
             rect1->x > rect2->x + rect2->w || // rect1 is to the right of rect2
             rect1->y + rect1->h < rect2->y || // rect1 is above rect2
             rect1->y > rect2->y + rect2->h);  // rect1 is below rect2
}

bool Collision_CheckCollisionF_Offset(SDL_FRect *r1, SDL_FRect *r2, float offset)
{
    return !((r1->x + r1->w - offset) < (r2->x + offset) ||   // left
             (r1->x + offset) > (r2->x + r2->w - offset) ||   // right
             (r1->y + r1->h - offset) < (r2->y + offset) ||   // top
             (r1->y + offset) > (r2->y + r2->h - offset));    // bottom
}
