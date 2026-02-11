#ifndef COLLISION_H
#define COLLISION_H

#include <stdbool.h>
#include <SDL2/SDL.h>


bool Collision_CheckCollisionF(SDL_FRect *rect1, SDL_FRect *rect2);
bool Collision_CheckCollision(SDL_Rect *rect1, SDL_Rect *rect2);
bool Collision_CheckCollisionF_Offset(SDL_FRect *r1, SDL_FRect *r2, float offset);

#endif