#ifndef OBJECT_H
#define OBJECT_H

#include <SDL2/SDL.h>
#include "animation.h"
#include "vector2.h"
#include <stdbool.h>

typedef enum {STATIC_OBJECT, ANIMATED_OBJECT, COLLISION_RECT_OBJECT} GameObjectType;



typedef struct GameObject
{

    SDL_Renderer *renderer;

    // base 
    GameObjectType type;
    SDL_FRect rect;

    // static object
    SDL_Texture *texture;
    SDL_Rect *tex_src_rect;

    // animated object
    Animation *anim;




}GameObject;


GameObject *GameObject_create(SDL_Renderer *renderer, GameObjectType type, SDL_FRect rect, SDL_Texture *texture, SDL_Rect *tex_src_rect, Animation *anim);
void GameObject_update(GameObject *obj, float dt);
void GameObject_render(GameObject *obj, SDL_FRect *camera);
void GameObject_delete(GameObject *obj);

#endif