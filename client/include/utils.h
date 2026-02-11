#ifndef UTILS_H
#define UTILS_H
#include <SDL2/SDL.h>
#include "settings.h"
#include <time.h>
#include <stdlib.h>

SDL_Rect* Utils_createRect(int x, int y, int w, int h);
SDL_FRect* Utils_createFRect(float x, float y, float w, float h);
int Utils_generateRandomNumber(int min, int max);
char *Utils_getFileContents(const char *path);
char *Utils_str_concat(const char *a, const char *b);
#endif
