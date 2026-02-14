#include "player.h"
#include <stdlib.h>
#include "vector2.h"
#include "animation.h"
#include "settings.h"
#include "object.h"
#include "utils.h"
#include <stdbool.h>
#include <math.h>

Player *Player_create(SDL_Renderer *renderer, int x, int y, SDL_Texture *texture, PlayerColor color, float speed, float health, List *collision_rects)
{
    Player *p = malloc(sizeof(Player));
    p->direction = (Vector2){0, 0};

    SDL_FRect rect = {x, y, PLAYER_SIZE, PLAYER_SIZE};

    SDL_Rect *idle_tex_rect = Utils_createRect(color * PLAYER_SIZE, 0, PLAYER_SIZE, PLAYER_SIZE);

    p->object = GameObject_create(renderer, ANIMATED_OBJECT, rect, texture, idle_tex_rect, NULL);

    p->speed = speed;
    p->health = health;
    p->collision_rects = collision_rects;

    int loaded_frames = 0;
    for (int i = 0; i < PLAYER_ANIM_COUNT; i++)
    {
        int frame_count = 3;
        if (i == PLAYER_ANIM_DIE || i == PLAYER_ANIM_EXPLODE)
            frame_count = 4;

        SDL_Rect *frames = malloc(sizeof(SDL_Rect) * frame_count);

        for(int j = 0; j < frame_count; j++)
        {
            frames[j] = (SDL_Rect){color * PLAYER_SIZE, (loaded_frames + j) * PLAYER_SIZE, PLAYER_SIZE, PLAYER_SIZE};
        }
        loaded_frames += frame_count;

        p->animations[i] = Animation_create(frame_count, 0, ANIMATION_FRAME_TIME, frames, 1);
    }

    p->object->anim = p->animations[PLAYER_ANIM_DOWN];

    return p;
}


void Player_input(Player *player)
{
    const Uint8* key_state = SDL_GetKeyboardState(NULL);

    player->direction.x = (float)(key_state[SDL_SCANCODE_RIGHT] || key_state[SDL_SCANCODE_D]) - (float)(key_state[SDL_SCANCODE_LEFT] || key_state[SDL_SCANCODE_A]);
    player->direction.y = (float)(key_state[SDL_SCANCODE_DOWN] || key_state[SDL_SCANCODE_S]) - (float)(key_state[SDL_SCANCODE_UP] || key_state[SDL_SCANCODE_W]);

    if (player->direction.x != 0 && player->direction.y != 0) // for normalising digonal movement
        player->direction = Vector2_Normalise(player->direction);

}

void Player_move(Player *player, float dt)
{
    player->object->rect.x += player->speed * player->direction.x * dt;
    player->object->rect.y += player->speed * player->direction.y * dt;
}

void Player_animationUpdate(Player *player)
{
    
    if(player->direction.x == 0 && player->direction.y == 0)
        player->object->type = STATIC_OBJECT;
    else
        player->object->type = ANIMATED_OBJECT;
    

    if(player->direction.x > 0) player->object->anim = player->animations[PLAYER_ANIM_RIGHT];
    else if(player->direction.x < 0) player->object->anim = player->animations[PLAYER_ANIM_LEFT];

    if(player->direction.y > 0) player->object->anim = player->animations[PLAYER_ANIM_DOWN];
    else if(player->direction.y < 0) player->object->anim = player->animations[PLAYER_ANIM_UP];

    if(player->health < 0)
    {
        player->object->anim->loop = 0;
        player->object->anim = player->animations[PLAYER_ANIM_DIE];
    }
}

void Player_checkCollisions(Player *player)
{
    for (unsigned int i = 0; i < player->collision_rects->size; i++)
    {   
        GameObject *collision_obj = List_getAt(player->collision_rects, i);

        if(Collision_CheckCollisionF(&player->object->rect, &collision_obj->rect))
        {
            float overlapX = 0.0f;
            float overlapY = 0.0f;

            if(player->object->rect.x < collision_obj->rect.x)
                overlapX = (player->object->rect.x + player->object->rect.w) - collision_obj->rect.x;
            else
                overlapX = player->object->rect.x - (collision_obj->rect.x + collision_obj->rect.w);

            if(player->object->rect.y < collision_obj->rect.y)
                overlapY = (player->object->rect.y + player->object->rect.h) - collision_obj->rect.y;
            else
                overlapY = player->object->rect.y - (collision_obj->rect.y + collision_obj->rect.h);

      
            if(fabsf(overlapX) < fabsf(overlapY))
            {
                player->object->rect.x -= overlapX;
            }
            else
            {
                player->object->rect.y -= overlapY;
            }
        }
    }
}


void Player_render(Player *player, SDL_FRect *camera)
{
    GameObject_render(player->object, camera);
}

void Player_update(Player *player, float dt, SDL_FRect *camera)
{
    // Player_input(player);
    // Player_move(player, dt);
    // Player_animationUpdate(player);
    // Player_checkCollisions(player);
    GameObject_update(player->object, dt);
    Player_render(player, camera);
}


void Player_delete(Player *player)
{
    player->object->anim = NULL;
    GameObject_delete(player->object);
    for (int i = 0; i < PLAYER_ANIM_COUNT; i++)
    {
        Animation_delete(player->animations[i]);
    }

    free(player);
    
}