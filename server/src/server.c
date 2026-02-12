#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>
#include "server.h"
#include <stdlib.h>
#include <uthash/uthash.h>

Server *Server_Init()
{
    if (SDLNet_Init() < 0) {
        SDL_Log("SDLNet_Init: %s\n", SDLNet_GetError());
        return NULL;
    }

    Server *server = malloc(sizeof(Server));


    server->socket = SDLNet_UDP_Open(SERVER_PORT);

    server->packet = SDLNet_AllocPacket(512);

    server->players = NULL;

    server->player_counter = 0;

    return server;

}


void Server_joinPlayer(Server *server, IPaddress address)
{
    ServerPlayer *player = malloc(sizeof(ServerPlayer));

    player->x = 200;
    player->y = 200;
    player->health = 100;
    player->speed = 85;
    player->address = address;
    player->id = ++server->player_counter;


    HASH_ADD_INT(
        server->players,
        id,
        player
    );

    ServerPacket reply;
    reply.type = MSG_ASSIGN_ID;
    reply.id = player->id;

    server->packet->address = address;

    memcpy(server->packet->data, &reply, sizeof(ServerPacket));
    server->packet->len = sizeof(ServerPacket);

    SDLNet_UDP_Send(server->socket, -1, server->packet);
    SDL_Log("player joined %d", player->id);

}

void Server_loop(Server *server)
{
    SDL_Log("Server listening on port %d...\n", SERVER_PORT);
    while(1)
    {
        if(SDLNet_UDP_Recv(server->socket, server->packet))
        {
            ClientPacket received;
            memcpy(&received, server->packet->data, sizeof(ClientPacket));
            SDL_Log("Received from client: type=%u id=%d\n", received.type, received.id);


            if(received.type == MSG_JOIN)
            {
                Server_joinPlayer(server, server->packet->address);
            }
        }
    }
}


