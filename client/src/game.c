#include "game.h"

Game *Game_create()
{
    Game *game = malloc(sizeof(Game));
    Graphics_initializeGraphics(&game->window, &game->renderer, GAME_NAME, WINDOW_WIDTH, WINDOW_HEIGHT);
    Audio_InitialiseAudio();
    game->texture_manager = TextureManager_create(game->renderer);
    game->map = GameMap_create(game->renderer, game->texture_manager, MAP_PATH, MAP_IMG_TILESET_PATH, &game->camera);
    game->camera = (SDL_FRect){0.0f, 0.0f, WINDOW_WIDTH, WINDOW_HEIGHT};



    game->running = false;
    game->last_frame_time = 0;
    game->client = Client_create();

    Client_join(game->client);

    game->player = Player_create(game->renderer, 100, 100, TextureManager_get(game->texture_manager, PLAYER_SPRITE_SHEET_PATH), PLAYER_PINK, PLAYER_SPEED, PLAYER_HEALTH, game->map->collision_objects);

    game->other_players = NULL;

    return game;
}

void Game_loop(Game *game)
{

    game->last_frame_time = SDL_GetTicks64();


    while (game->running)
    {
        Uint64 current_time = SDL_GetTicks64();
        Uint64 diff = current_time - game->last_frame_time;
        game->last_frame_time = current_time;

        float dt = diff / 1000.0f;

        while (SDL_PollEvent(&game->event))
        {
            

            switch (game->event.type)
            {
            case SDL_QUIT:
                game->running = false;
                break;

            case SDL_KEYDOWN:
                if (game->event.key.keysym.sym == SDLK_ESCAPE)
                    game->running = false;


                break;

            default:
                break;
            }
        }
        ServerPacket s_packet;
        if (Client_receiveFromServer(game->client, &s_packet))
        {
      
            //SDL_Log("Received from server: type=%u id=%d x=%d, y=%d\n", received.type, received.id, received.x, received.y);
            if(s_packet.id == game->client->id)
            game->player->object->rect.x = s_packet.x;
            game->player->object->rect.y = s_packet.y;
        }

        Graphics_clearScreen(game->renderer);

        Map_renderMap(game->map);

        Player_update(game->player, dt, &game->camera);

        Camera_update(&game->camera, &game->player->object->rect);

        Graphics_presentScreen(game->renderer);
    }
}


void Game_delete(Game *game)
{
    SDLNet_Quit();
    Map_delete(game->map);
    Player_delete(game->player);
    TextureManager_delete(game->texture_manager);
    Audio_CleanUpAudio();
    Graphics_cleanupGraphics(game->window, game->renderer);
}