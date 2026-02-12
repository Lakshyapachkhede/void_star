#ifndef NETWORK_MANAGER_H
#define NETWORK_MANAGER_H
#include <SDL_net.h>
#include <stdlib.h>
#include "player.h"
#include "network.h"

#define MAX_NETWORK_PACKET_SIZE 512
#define NETWORK_FREQUENCY 20

typedef struct
{
    UDPsocket socket;
    UDPpacket *packet;
    uint32_t tick;

} NetworkManager;

NetworkManager *NetworkManager_create()
{
    NetworkManager *nm = malloc(sizeof(NetworkManager));
    nm->socket = SDLNet_UDP_Open(0);
    nm->packet = SDLNet_AllocPacket(MAX_NETWORK_PACKET_SIZE);
    nm->tick = 0;

    IPaddress serverIP;
    if (SDLNet_ResolveHost(&serverIP, SERVER_IP, SERVER_PORT) < 0)
    {
        SDL_Log("ResolveHost: %s\n", SDLNet_GetError());
        return NULL;
    }

    nm->packet->address = serverIP;


    return nm;
}

void NetworkManager_sendPlayerInputState(NetworkManager *nm, Player *player)
{
    ClientPacket input_packet;
    
}

#endif