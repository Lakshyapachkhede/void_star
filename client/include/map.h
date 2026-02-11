#ifndef MAP_H
#define MAP_H

#include <stdint.h>
#include "settings.h"
#include "object.h"
#include "list.h"
#include <SDL2/SDL.h>
#include "texture_manager.h"

typedef struct MapTile
{
    int z;
    SDL_Rect screen_rect;
    SDL_Rect tex_src_rect;
}MapTile;


typedef struct GameMap
{
    // List *layers;
    List *tiles;
    List *game_objects;
    List *collision_objects;
    SDL_Texture *tileset_texture;
    SDL_Renderer *renderer;
    SDL_FRect *camera;
    
}GameMap;

GameMap *GameMap_create(SDL_Renderer *renderer, TextureManager *tm, const char *map_path, const char *tileset_texture_path, SDL_FRect *camera);
void GameMap_loadMap(GameMap *map, TextureManager *tm, const char *map_path);
void Map_renderMap(GameMap *map);
void Map_delete(GameMap *map);

#endif