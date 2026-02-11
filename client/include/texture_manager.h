#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include <SDL2/SDL.h>
#include <uthash/uthash.h>

typedef struct Texture
{
    char *name;
    SDL_Texture *texture;
    UT_hash_handle hh;
} Texture;

typedef struct TextureManager
{
    SDL_Renderer *renderer;
    Texture *textures;
} TextureManager;

TextureManager *TextureManager_create(SDL_Renderer *renderer);
void TextureManager_delete(TextureManager *tm);

SDL_Texture *TextureManager_get(TextureManager *tm, const char *path);
void TextureManager_remove(TextureManager *tm, const char *path);

#endif
