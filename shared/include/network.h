#ifndef NETWORK_H
#define NETWORK_H

#include <stdint.h>
#include <stdbool.h>
#include "vector2.h"
#include <uthash/uthash.h>
#include <SDL2/SDL_net.h>


#define SERVER_PORT 1234


typedef enum {KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT, MOUSE_LEFT, MOUSE_SCROLL, KEY_COUNT} KeyTypes;

typedef enum {
    MSG_JOIN,
    MSG_ASSIGN_ID,
    MSG_INPUT,
    MSG_STATE,
    MSG_LEAVE
} MessageType;


typedef struct ClientPacket
{   
    uint32_t id;
    uint32_t tick;
    uint8_t type;
    uint8_t keys;

}ClientPacket;

typedef struct ServerPlayer
{   
    uint32_t id;
    int32_t x, y;
    uint8_t health;
    uint8_t speed;
    IPaddress address;
    UT_hash_handle hh;

}ServerPlayer;

typedef struct ServerPacket
{
    uint32_t tick;
    uint32_t id;
    uint32_t x, y;
    uint8_t type;
    uint8_t health;
}ServerPacket;






bool Network_checkKeyPressed(uint8_t keys, KeyTypes key);


void Network_setKey(uint8_t *keys, KeyTypes key);


void Network_clearKey(uint8_t *keys, KeyTypes key);

bool IPaddress_equal(IPaddress a, IPaddress b);

void Network_setKeyValue(uint8_t *keys, KeyTypes key, bool value);


#endif