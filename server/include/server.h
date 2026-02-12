#ifndef SERVER_H
#define SERVER_H

#include "network.h"
#include <SDL2/SDL_net.h>


typedef struct Server
{
    uint32_t tick;
    UDPsocket socket;
    UDPpacket *packet;
    ServerPlayer *players;
    uint32_t player_counter;
}Server;

Server *Server_Init();
void Server_joinPlayer(Server *server, IPaddress address);
void Server_loop(Server *server);

#endif