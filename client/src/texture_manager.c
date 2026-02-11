#include "texture_manager.h"
#include <SDL2/SDL_image.h>
#include <stdlib.h>
#include <string.h>
#include "graphics.h"

TextureManager *TextureManager_create(SDL_Renderer *renderer)
{
    TextureManager *tm = malloc(sizeof *tm);
    tm->renderer = renderer;
    tm->textures = NULL;
    return tm;
}

SDL_Texture *TextureManager_get(TextureManager *tm, const char *path)
{
    Texture *entry = NULL;

    HASH_FIND_STR(tm->textures, path, entry);
    if (entry) {
        return entry->texture;
    }

    SDL_Texture *tex = Graphics_getTextureFromPath(tm->renderer, path);
  
    entry = malloc(sizeof *entry);
    entry->name = strdup(path);
    entry->texture = tex;

    HASH_ADD_KEYPTR(
        hh,
        tm->textures,
        entry->name,
        strlen(entry->name),
        entry
    );

    return tex;
}

void TextureManager_remove(TextureManager *tm, const char *path)
{
    Texture *entry = NULL;

    HASH_FIND_STR(tm->textures, path, entry);
    if (!entry) return;

    SDL_DestroyTexture(entry->texture);
    HASH_DEL(tm->textures, entry);
    free(entry->name);
    free(entry);
}

void TextureManager_delete(TextureManager *tm)
{
    Texture *cur, *tmp;

    HASH_ITER(hh, tm->textures, cur, tmp) {
        SDL_DestroyTexture(cur->texture);
        HASH_DEL(tm->textures, cur);
        free(cur->name);
        free(cur);
    }


    free(tm);
}
