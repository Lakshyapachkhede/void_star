#include "utils.h"
#include <stdlib.h>
#include <stdio.h>

SDL_Rect* Utils_createRect(int x, int y, int w, int h)
{
    SDL_Rect* rect = malloc(sizeof(SDL_Rect));
    rect->x = x;
    rect->y = y;
    rect->w = w;
    rect->h = h;

    return rect;
}

SDL_FRect* Utils_createFRect(float x, float y, float w, float h)
{
    SDL_FRect* rect = malloc(sizeof(SDL_FRect));
    rect->x = x;
    rect->y = y;
    rect->w = w;
    rect->h = h;

    return rect;
}


int Utils_generateRandomNumber(int min, int max)
{
    return (rand() % (max - min + 1)) + min;
}

char *Utils_getFileContents(const char *path)
{
    FILE *f = fopen(path, "rb");
    if(!f) return NULL;

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    rewind(f);


    char *data = malloc(size + 1);
    fread(data, 1, size, f);
    data[size] = '\0';

    fclose(f);
    return data;

}   


char *Utils_str_concat(const char *a, const char *b)
{
    if (!a || !b) return NULL;

    size_t len_a = strlen(a);
    size_t len_b = strlen(b);

    char *result = malloc(len_a + len_b + 1);
    if (!result) return NULL;

    memcpy(result, a, len_a);
    memcpy(result + len_a, b, len_b + 1); 

    return result;
}

