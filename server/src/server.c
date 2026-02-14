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

void Server_broadcast(Server *server)
{
    ServerPlayer *player, *tmp;

    HASH_ITER(hh, server->players, player, tmp)
    {
        
    }

}

void Server_sendToClient(Server *server, ServerPacket *s_packet)
{
    memcpy(server->packet->data, s_packet, sizeof(ServerPacket));
    server->packet->len = sizeof(ServerPacket);

    SDLNet_UDP_Send(server->socket, -1, server->packet);

}

void Server_joinPlayer(Server *server)
{
    ServerPlayer *player = malloc(sizeof(ServerPlayer));

    player->x = 100;
    player->y = 100;
    player->health = 100;
    player->speed = 2;
    player->address = server->packet->address;
    player->id = ++server->player_counter;


    HASH_ADD_INT(
        server->players,
        id,
        player
    );

    ServerPacket s_packet;
    s_packet.type = MSG_ASSIGN_ID;
    s_packet.id = player->id;

    // memcpy(server->packet->data, &s_packet, sizeof(ServerPacket));
    // server->packet->len = sizeof(ServerPacket);

    // SDLNet_UDP_Send(server->socket, -1, server->packet);
    Server_sendToClient(server, &s_packet);

    SDL_Log("player joined %d", server->player_counter - 1);

}

void Server_updatePlayer(Server *server, ClientPacket c_packet)
{
    ServerPlayer *player;

    HASH_FIND_INT(server->players, &c_packet.id, player);
    if(!player)
    {
        SDL_Log("not found");
        return;
    }
    else {
        //SDL_Log("find");

    }

    player->y += ((Network_checkKeyPressed(c_packet.keys, KEY_DOWN) - Network_checkKeyPressed(c_packet.keys, KEY_UP)) * player->speed);
    player->x += ((Network_checkKeyPressed(c_packet.keys, KEY_RIGHT) - Network_checkKeyPressed(c_packet.keys, KEY_LEFT)) * player->speed);

    /*SDL_Log("left: %d, right %d, up %d, down %d",
         Network_checkKeyPressed(c_packet.keys, KEY_LEFT),
         Network_checkKeyPressed(c_packet.keys, KEY_RIGHT),
         Network_checkKeyPressed(c_packet.keys, KEY_UP),
    Network_checkKeyPressed(c_packet.keys, KEY_DOWN));
*/
    ServerPacket s_packet;
    s_packet.id = player->id;
    s_packet.x = player->x;
    s_packet.y = player->y;
    s_packet.type = MSG_STATE;

    memcpy(server->packet->data, &s_packet, sizeof(ServerPacket));
    server->packet->len = sizeof(ServerPacket);

    SDLNet_UDP_Send(server->socket, -1, server->packet);

    Server_sendToClient(server, &s_packet);
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
            //SDL_Log("Received from client: type=%u id=%d\n", received.type, received.id);


            if(received.type == MSG_JOIN)
            {
                Server_joinPlayer(server);
            }
            if(received.type == MSG_INPUT)
            {
                Server_updatePlayer(server, received);
            }
        }
    }
}


