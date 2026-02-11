#include "settings.h"
#include "graphics.h"
#include "audio.h"
#include "player.h"
#include "map.h"
#include <stdio.h>
#include "texture_manager.h"
#include "utils.h"
#include "camera.h"

/*

TODO:
1.Camera
2.Collisions
3.Weapons
4.Z based drawing

*/

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    TextureManager *tm = NULL;

    Graphics_initializeGraphics(&window, &renderer, GAME_NAME, WINDOW_WIDTH, WINDOW_HEIGHT);
    Audio_InitialiseAudio();
    tm = TextureManager_create(renderer);

    SDL_FRect camera = {0.0f, 0.0f, WINDOW_WIDTH, WINDOW_HEIGHT};

    GameMap *map = GameMap_create(renderer, tm, MAP_PATH, MAP_IMG_TILESET_PATH, &camera);

    int running = 1;
    SDL_Event event;

    Player *player = Player_create(renderer, 100, 100, TextureManager_get(tm, PLAYER_SPRITE_SHEET_PATH), PLAYER_RED, PLAYER_SPEED, PLAYER_HEALTH);


    Uint64 last_time = SDL_GetTicks64();


    while (running)
    {
        Uint64 current_time = SDL_GetTicks64();
        Uint64 diff = current_time - last_time;
        last_time = current_time;

        float dt = diff / 1000.0f;

        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                running = 0;
                break;

            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE)
                    running = 0;
                break;

            default:
                break;
            }
        }

        Graphics_clearScreen(renderer);

        Map_renderMap(map);
        
        Player_update(player, dt, &camera);

        Camera_update(&camera, &player->object->rect);
       
        Graphics_presentScreen(renderer);
    }

    Map_delete(map);
    Player_delete(player);
    TextureManager_delete(tm);
    Audio_CleanUpAudio();
    Graphics_cleanupGraphics(window, renderer);

    return 0;
}