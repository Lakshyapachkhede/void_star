#include "camera.h"
#include "settings.h"

void Camera_update(SDL_FRect *camera, SDL_FRect *player)
{
    float player_cx = player->x + (player->w * 0.5f);
    float player_cy = player->y + (player->h * 0.5f);


    camera->x = player_cx - (camera->w * 0.5f);
    camera->y = player_cy - (camera->h * 0.5f);


    if(camera->x < 0) camera->x = 0;
    if(camera->x > ((MAP_SIZE * TILE_SIZE) - (camera->w))) camera->x = ((MAP_SIZE * TILE_SIZE) - (camera->w));
    if(camera->y < 0) camera->y = 0;
    if(camera->y > ((MAP_SIZE * TILE_SIZE) - (camera->h))) camera->y = ((MAP_SIZE * TILE_SIZE) - (camera->h));



}