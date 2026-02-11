#include "map.h"
#include <cJSON/cJSON.h>
#include "utils.h"
#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "object.h"
#include <stdio.h>
#include "graphics.h"
#include "settings.h"

GameMap *GameMap_create(SDL_Renderer *renderer, TextureManager *tm, const char *map_path, const char *tileset_texture_path, SDL_FRect *camera)
{
    GameMap *map = malloc(sizeof(GameMap));

    map->tiles = List_create(sizeof(MapTile), MAP_SIZE * MAP_SIZE);
    map->game_objects = List_create(sizeof(GameObject), DEFAULT_MAP_OBJECTS_COUNT);
    map->collision_objects = List_create(sizeof(GameObject), DEFAULT_MAP_OBJECTS_COUNT);
    map->renderer = renderer;

    map->tileset_texture = TextureManager_get(tm, tileset_texture_path);

    map->camera = camera;

    GameMap_loadMap(map, tm, map_path);

    return map;
}

void GameMap_loadMap(GameMap *map, TextureManager *tm, const char *map_path)
{
    char *json_text = Utils_getFileContents(map_path);

    cJSON *root = cJSON_Parse(json_text);

    free(json_text);

    if (!root)
    {
        printf("Map parse failed\n");
        return;
    }

    cJSON *json_layers = cJSON_GetObjectItem(root, "layers");


    for (int i = 0; i < cJSON_GetArraySize(json_layers); i++)
    {
        cJSON *json_layer = cJSON_GetArrayItem(json_layers, i);

        int count = cJSON_GetArraySize(json_layer);

        

        for (int j = 0; j < count; j++)
        {
            int cell_value = cJSON_GetArrayItem(json_layer, j)->valueint;
            if(cell_value == -1)
            {
                continue;
            }
            int y = j / MAP_SIZE;
            int x = j % MAP_SIZE;

            MapTile tile;
            tile.screen_rect = (SDL_Rect){x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE};

            int tile_x = cell_value % TILESET_COLS;
            int tile_y = cell_value / TILESET_COLS;
            int px = tile_x * TILE_SIZE;
            int py = tile_y * TILE_SIZE;

            tile.tex_src_rect = (SDL_Rect){px, py, TILE_SIZE, TILE_SIZE};

            tile.z = i;
           


            List_insert(map->tiles, &tile);
        }
  
    }

    cJSON *json_objects = cJSON_GetObjectItem(root, "objects");

    for (int i = 0; i < cJSON_GetArraySize(json_objects); i++)
    {
        cJSON *obj = cJSON_GetArrayItem(json_objects, i);

        float x = (float)cJSON_GetObjectItem(obj, "x")->valuedouble;
        float y = (float)cJSON_GetObjectItem(obj, "y")->valuedouble;
        float w = (float)cJSON_GetObjectItem(obj, "w")->valuedouble;
        float h = (float)cJSON_GetObjectItem(obj, "h")->valuedouble;

        const char *img =
            cJSON_GetObjectItem(obj, "img")->valuestring;

        SDL_FRect rect = (SDL_FRect){
            .x = x,
            .y = y - h,
            .w = w,
            .h = h};


        SDL_Texture *tex = TextureManager_get(tm, Utils_str_concat(MAP_IMG_PATH, img));
        GameObject *object = GameObject_create(map->renderer, STATIC_OBJECT, rect, tex, NULL, NULL);

        List_insert(map->game_objects, object);
    }

    cJSON *json_collisions = cJSON_GetObjectItem(root, "collisions");

    for (int i = 0; i < cJSON_GetArraySize(json_collisions); i++)
    {
        cJSON *obj = cJSON_GetArrayItem(json_collisions, i);

        float x = (float)cJSON_GetObjectItem(obj, "x")->valuedouble;
        float y = (float)cJSON_GetObjectItem(obj, "y")->valuedouble;
        float w = (float)cJSON_GetObjectItem(obj, "w")->valuedouble;
        float h = (float)cJSON_GetObjectItem(obj, "h")->valuedouble;

        SDL_FRect rect = (SDL_FRect){
            .x = x,
            .y = y,
            .w = w,
            .h = h};

        GameObject *object = GameObject_create(map->renderer, COLLISION_RECT_OBJECT, rect, NULL, NULL, NULL);

        List_insert(map->collision_objects, object);
    }

    cJSON_Delete(root);
}

void Map_renderMap(GameMap *map)
{
    for (unsigned int i = 0; i < map->tiles->size; i++)
    {
        MapTile *tile = (MapTile *)List_getAt(map->tiles, i);
        SDL_FRect dst_rect = {tile->screen_rect.x - map->camera->x, tile->screen_rect.y - map->camera->y, tile->screen_rect.w, tile->screen_rect.h};
        Graphics_renderTextureFS(map->renderer, map->tileset_texture, &tile->tex_src_rect, &dst_rect);
    }

    for (unsigned int i = 0; i < map->game_objects->size; i++)
    {
        GameObject *obj = (GameObject *)List_getAt(map->game_objects, i);

        GameObject_render(obj, map->camera);
    }
}

void Map_delete(GameMap *map)
{
    List_free(map->tiles);
    List_free(map->collision_objects);

    for (unsigned int i = 0; i < map->game_objects->size; i++)
    {
        GameObject *obj = (GameObject *)List_getAt(map->game_objects, i);
        GameObject_delete(obj);
    }

    List_free(map->game_objects);
}