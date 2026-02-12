#include <SDL2/SDL.h>
#include "server.h"


int main(int argc, char *argv[])
{

    (void)(argc);
    (void)(argv);

    Server *server = Server_Init();

    Server_loop(server);

    return 0;
}
