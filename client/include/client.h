#ifndef CLIENT_H
#define CLIENT_H
#include <SDL2/SDL_net.h>
#include <stdlib.h>
#include "player.h"
#include "network.h"
#include "settings.h"

#define MAX_NETWORK_PACKET_SIZE 512
#define NETWORK_FREQUENCY 20
#define JOIN_WAIT_SECONDS 100

typedef struct
{

    UDPsocket socket;
    UDPpacket *packet;
    uint32_t tick;
    int id;
    Uint64 last_send_time;

} Client;

Client *Client_create();

void Client_sendPlayerInputState(Client *client);

void Client_sendToServer(Client *client, ClientPacket *c_packet);

int Client_receiveFromServer(Client *client, ServerPacket *out);


int Client_join(Client *client); // returns assigned player id


#endif