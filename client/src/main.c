#include "game.h"

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    Game *game = Game_create();

    Game_loop(game);

    Game_delete(game);


    return 0;
}