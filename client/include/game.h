#ifndef GAME_H
#define GAME_H
#include "map.h"
#include "list.h"
#include "player.h"
#include "network_manager.h"
#include <stdbool.h>

typedef struct Game
{   

    SDL_Window *window;
    SDL_Renderer *renderer;

    TextureManager *texture_manager;

    SDL_Rect camera;

    bool running;
    SDL_Event event;

    NetworkManager *network_manager;
    Player *player;
    Player *other_players;
}Game;

Game *Game_create()
{

}



#endif