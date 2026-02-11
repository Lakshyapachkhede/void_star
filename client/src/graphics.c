#include "graphics.h"
#include <SDL2/SDL_image.h>


TTF_Font *mainFont16  = NULL;
TTF_Font *mainFont50  = NULL;
TTF_Font *mainFont100 = NULL;

int Graphics_initializeGraphics(SDL_Window **window, SDL_Renderer **renderer, const char *title, int width, int height)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return -1;
    }

    if (IMG_Init(IMG_INIT_PNG) == 0)
    {
        SDL_Log("Unable to initialize SDL Image: %s", IMG_GetError());
        return -1;
    }

    if (TTF_Init() == -1)
    {
        SDL_Log("TTF_Init Error: %s", TTF_GetError());
        SDL_Quit();
        return 1;
    }

    *window = SDL_CreateWindow(title,
                               SDL_WINDOWPOS_CENTERED,
                               SDL_WINDOWPOS_CENTERED,
                               width, height,
                               SDL_WINDOW_SHOWN);

    if (!*window)
    {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        return -1;
    }

    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);

    if (!*renderer)
    {
        SDL_Log("Failed to create renderer: %s", SDL_GetError());
        SDL_DestroyWindow(*window);
        return -1;
    }

    mainFont16 = TTF_OpenFont("assets/fonts/main.TTF", 16);
    if (mainFont16 == NULL)
    {
        SDL_Log("Unable to load font: %s", TTF_GetError());
    }

    mainFont50 = TTF_OpenFont("assets/fonts/main.TTF", 50);
    if (mainFont50 == NULL)
    {
        SDL_Log("Unable to load font: %s", TTF_GetError());
    }
    mainFont100 = TTF_OpenFont("assets/fonts/main.TTF", 100);
    if (mainFont100 == NULL)
    {
        SDL_Log("Unable to load font: %s", TTF_GetError());
    }

    return 0;
}

void Graphics_clearScreen(SDL_Renderer *renderer)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
}

void Graphics_renderTextureF(SDL_Renderer *renderer, SDL_Texture *texture, SDL_FRect *rect)
{
    SDL_RenderCopyF(renderer, texture, NULL, rect);
}
void Graphics_renderTextureFS(SDL_Renderer *renderer, SDL_Texture *texture, SDL_Rect *s_rect, SDL_FRect *d_rect)
{
    SDL_RenderCopyF(renderer, texture, s_rect, d_rect);
}

void Graphics_renderTextureS(SDL_Renderer *renderer, SDL_Texture *texture, SDL_Rect *s_rect, SDL_Rect *d_rect)
{
    SDL_RenderCopy(renderer, texture, s_rect, d_rect);
}


void Graphics_renderTexture(SDL_Renderer *renderer, SDL_Texture *texture, SDL_Rect *rect)
{
    SDL_RenderCopy(renderer, texture, NULL, rect);
}

void Graphics_renderTextureFAngle(SDL_Renderer *renderer, SDL_Texture *texture, SDL_FRect *rect, double angle)
{
    SDL_RenderCopyExF(renderer, texture, NULL, rect, angle, NULL, SDL_FLIP_NONE);
}

void Graphics_renderTextureAngle(SDL_Renderer *renderer, SDL_Texture *texture, SDL_Rect *rect, double angle)
{
    SDL_RenderCopyEx(renderer, texture, NULL, rect, angle, NULL, SDL_FLIP_NONE);
}

void Graphics_presentScreen(SDL_Renderer *renderer)
{
    SDL_RenderPresent(renderer);
}

void Graphics_cleanupGraphics(SDL_Window *window, SDL_Renderer *renderer)
{
    if (renderer)
        SDL_DestroyRenderer(renderer);
    if (window)
        SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
}

SDL_Texture *Graphics_getTextureFromPath(SDL_Renderer *renderer, const char *path)
{
    SDL_Texture *texture = IMG_LoadTexture(renderer, path);
    if (!texture)
    {
        SDL_Log("Unable to load texture: %s", IMG_GetError());
    }

    return texture;
}

void Graphics_ShowText(SDL_Renderer *renderer, TTF_Font *font, const char *text, int x, int y, SDL_Color color)
{
    if (!font || !text)
        return;

    SDL_Surface *surface = TTF_RenderText_Solid(font, text, color);
    if (!surface)
    {
        SDL_Log("Text surface error: %s", TTF_GetError());
        return;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture)
    {
        SDL_Log("Text texture error: %s", SDL_GetError());
        SDL_FreeSurface(surface);
        return;
    }

    SDL_Rect rect = {x - surface->w / 2, y - surface->h / 2, surface->w, surface->h};

    SDL_FreeSurface(surface);

    Graphics_renderTexture(renderer, texture, &rect);

    SDL_DestroyTexture(texture);
}
