#include "network.h"
#include "settings.h"
#include "graphics.h"
#include "audio.h"
#include "player.h"
#include "map.h"
#include <stdio.h>
#include "texture_manager.h"
#include "utils.h"
#include "camera.h"
#include <SDL2/SDL_net.h>
#include <SDL2/SDL.h>


int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    if (SDLNet_Init() < 0)
    {
        SDL_Log("SDLNet_Init: %s\n", SDLNet_GetError());
        return 1;
    }

    UDPsocket socket = SDLNet_UDP_Open(0);

    UDPpacket *packet = SDLNet_AllocPacket(512);

    IPaddress serverIP;
    if (SDLNet_ResolveHost(&serverIP, "127.0.0.1", SERVER_PORT) < 0)
    {
        SDL_Log("ResolveHost: %s\n", SDLNet_GetError());
        return 1;
    }

    packet->address = serverIP;

    ClientPacket c_packet = {0};

    c_packet.type = MSG_JOIN;

    memcpy(packet->data, &c_packet, sizeof(ClientPacket));
    packet->len = sizeof(ClientPacket);

    SDLNet_UDP_Send(socket, -1, packet);
    SDL_Log("sent");

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

    Player *player = Player_create(renderer, 100, 100, TextureManager_get(tm, PLAYER_SPRITE_SHEET_PATH), PLAYER_RED, PLAYER_SPEED, PLAYER_HEALTH, map->collision_objects);

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

    SDLNet_Quit();
    Map_delete(map);
    Player_delete(player);
    TextureManager_delete(tm);
    Audio_CleanUpAudio();
    Graphics_cleanupGraphics(window, renderer);

    return 0;
}