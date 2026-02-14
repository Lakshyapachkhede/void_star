#include "client.h"

Client *Client_create()
{
    Client *client = malloc(sizeof(Client));
    client->socket = SDLNet_UDP_Open(0);
    client->packet = SDLNet_AllocPacket(MAX_NETWORK_PACKET_SIZE);
    client->tick = 0;

    IPaddress serverIP;
    if (SDLNet_ResolveHost(&serverIP, SERVER_IP, SERVER_PORT) < 0)
    {
        SDL_Log("ResolveHost: %s\n", SDLNet_GetError());
        return NULL;
    }

    client->last_send_time = 0;

    client->packet->address = serverIP;
    client->packet->len = sizeof(ClientPacket);

    return client;
}

void Client_sendPlayerInputState(Client *client)
{
    ClientPacket c_packet;
    c_packet.id = client->id;
    c_packet.type = MSG_INPUT;

    int m_x, m_y;

    Uint8 *keys = SDL_GetKeyboardState(NULL);
    Uint32 mouse = SDL_GetMouseState(&m_x, &m_y);

    Network_setKeyValue(&c_packet.keys, KEY_UP, keys[SDLK_UP] || keys[SDLK_w]);
    Network_setKeyValue(&c_packet.keys, KEY_DOWN, keys[SDLK_DOWN] || keys[SDLK_s]);
    Network_setKeyValue(&c_packet.keys, KEY_LEFT, keys[SDLK_LEFT] || keys[SDLK_a]);
    Network_setKeyValue(&c_packet.keys, KEY_RIGHT, keys[SDLK_RIGHT] || keys[SDLK_d]);

    Client_sendToServer(client, &c_packet);
}

void Client_sendToServer(Client *client, ClientPacket *c_packet)
{
    memcpy(client->packet->data, c_packet, sizeof(ClientPacket));
    client->packet->len = sizeof(ClientPacket);
    SDLNet_UDP_Send(client->socket, -1, client->packet);
}

int Client_receiveFromServer(Client *client, ServerPacket *out)
{
    if (SDLNet_UDP_Recv(client->socket, client->packet) <= 0)
        return 0;

    if (client->packet->len != sizeof(ServerPacket))
        return 0;

    memcpy(out, client->packet->data, sizeof(ServerPacket));
    return 1;
}

int Client_join(Client *client) // returns assigned player id
{
    ClientPacket c_packet = {0};
    c_packet.type = MSG_JOIN;

    Client_sendToServer(client, &c_packet);

    int timer = 0;

    while (timer < (1000 * JOIN_WAIT_SECONDS))
    {

        ServerPacket received;
        Client_receiveFromServer(client, &received);

        if (received.type == MSG_ASSIGN_ID)
        {   
            client->id = received.id;
            return received.id;
        }
        timer += 1;

        SDL_Delay(1);
    }

    return -1;
}