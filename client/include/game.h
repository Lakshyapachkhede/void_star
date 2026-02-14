#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>
#include "graphics.h"
#include "texture_manager.h"
#include "map.h"
#include "camera.h"
#include "client.h"
#include "player.h"
#include "audio.h"

typedef struct Game
{

    SDL_Window *window;
    SDL_Renderer *renderer;

    TextureManager *texture_manager;
    GameMap *map;

    SDL_FRect camera;

    bool running;
    SDL_Event event;
    Uint64 last_frame_time;

    Client *client;

    Player *player;
    Player *other_players;
} Game;

Game *Game_create();

void Game_loop(Game *game);

void Game_delete(Game *game);

#endif