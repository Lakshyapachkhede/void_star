#include "animation.h"
#include <stdlib.h>

Animation *Animation_create(int frames_count, int current_frame, float frame_time, SDL_Rect *frames, int loop)
{
    Animation *anim = (Animation *)malloc(sizeof(Animation));

    anim->frames_count = frames_count;
    anim->current_frame = current_frame;
    anim->frame_time = frame_time;
    anim->timer = 0;
    anim->frames = frames;
    anim->loop = loop;

    return anim;
}

SDL_Rect Animation_getCurrentFrameRect(Animation *anim)
{
    return anim->frames[anim->current_frame];
}

void Animation_update(Animation *anim, float dt)
{

    if (!anim)
        return;

    anim->timer += dt;

    while (anim->timer >= anim->frame_time)
    {
        anim->timer -= anim->frame_time;
        
        anim->current_frame++;
        
        if (anim->current_frame >= anim->frames_count)
        {
            anim->current_frame = (anim->loop) ? 0 : anim->frames_count - 1;
        }
    }
}

void Animation_delete(Animation *anim)
{
    if(!anim->frames) return;
    free(anim->frames);
    free(anim);
}