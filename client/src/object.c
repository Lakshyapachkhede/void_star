#include "object.h"
#include "graphics.h"

GameObject *GameObject_create(SDL_Renderer *renderer, GameObjectType type, SDL_FRect rect, SDL_Texture *texture, SDL_Rect *tex_src_rect, Animation *anim)
{
    GameObject *obj = (GameObject *)malloc(sizeof(GameObject));
    obj->renderer = renderer;
    obj->type = type;
    obj->rect = rect;
    obj->texture = texture;
    obj->tex_src_rect = tex_src_rect;
    obj->anim = anim;


    return obj;
}

void GameObject_update(GameObject *obj, float dt)
{
    switch (obj->type)
    {
    case STATIC_OBJECT:
    case COLLISION_RECT_OBJECT:

        break;

    case ANIMATED_OBJECT:
        Animation_update(obj->anim, dt);

    default:
        break;
    }
}

void GameObject_render(GameObject *obj, SDL_FRect *camera)
{
    SDL_FRect dst_rect;

    dst_rect.x = obj->rect.x - camera->x;
    dst_rect.y = obj->rect.y - camera->y;
    dst_rect.w = obj->rect.w;
    dst_rect.h = obj->rect.h;

    switch (obj->type)
    {
    case STATIC_OBJECT:
        if (obj->tex_src_rect != NULL)
            Graphics_renderTextureFS(obj->renderer, obj->texture, obj->tex_src_rect, &dst_rect);
        else
            Graphics_renderTextureF(obj->renderer, obj->texture, &dst_rect);

        break;
    case COLLISION_RECT_OBJECT:

        break;

    case ANIMATED_OBJECT:
        SDL_Rect frame_rect = Animation_getCurrentFrameRect(obj->anim);
        Graphics_renderTextureFS(obj->renderer, obj->texture, &frame_rect, &dst_rect);
        break;
    default:
        break;
    }
}

void GameObject_delete(GameObject *obj)
{
    if (!obj->anim)
        return;
    Animation_delete(obj->anim);
    free(obj);
}
