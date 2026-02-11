#ifndef PLAYER_H
#define PLAYER_H
#include "vector2.h"
#include <SDL2/SDL.h>
#include "animation.h"
#include "object.h"

#define PLAYER_SIZE 24

typedef enum
{
    PLAYER_ANIM_DOWN,
    PLAYER_ANIM_RIGHT,
    PLAYER_ANIM_LEFT,
    PLAYER_ANIM_UP,
    PLAYER_ANIM_DIE,
    PLAYER_ANIM_EXPLODE,
    PLAYER_ANIM_COUNT
} PlayerAnimationTypes;

typedef enum
{
    PLAYER_WHITE,
    PLAYER_BLACK,
    PLAYER_RED,
    PLAYER_YELLOW,
    PLAYER_GREEN,
    PLAYER_LBLUE,
    PLAYER_BLUE,
    PLAYER_PINK,
    PLAYER_COLOR_COUNT
} PlayerColor;



typedef struct Player
{
    GameObject *object;
    Vector2 direction;
    Animation *animations[PLAYER_ANIM_COUNT];
    float speed;
    float health;

} Player;

Player *Player_create(SDL_Renderer *renderer, int x, int y, SDL_Texture *texture, PlayerColor color, float speed, float health);
void Player_input(Player *player);
void Player_move(Player *player, float dt);
void Player_animationUpdate(Player *player);
void Player_delete(Player *player);
void Player_render(Player *player, SDL_FRect *camera);
void Player_update(Player *player, float dt, SDL_FRect *camera);

#endif