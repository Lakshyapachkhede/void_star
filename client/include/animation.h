#ifndef ANIMATION_H
#define ANIMATION_H


#include <SDL2/SDL.h>
#include <stdbool.h>

typedef struct Animation
{
    int frames_count;
    int current_frame;
    float frame_time;
    float timer;
    SDL_Rect *frames;
    bool loop;

}Animation;

Animation *Animation_create(int frames_count, int current_frame, float frame_time, SDL_Rect *frames, int loop);
void Animation_update(Animation *anim, float dt);
void Animation_delete(Animation *anim);
SDL_Rect Animation_getCurrentFrameRect(Animation *anim);

#endif