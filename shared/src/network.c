#include "network.h"

bool Network_checkKeyPressed(uint8_t keys, KeyTypes key)
{
    return ((keys & (1u << key)) != 0);
}

void Network_setKey(uint8_t *keys, KeyTypes key)
{
    if (key < KEY_COUNT)
        *keys |= (1u << key);
}

void Network_clearKey(uint8_t *keys, KeyTypes key)
{
    *keys &= ~(1u << key);
}

bool IPaddress_equal(IPaddress a, IPaddress b)
{
    return (a.host == b.host) && (a.port == b.port);
}

void Network_setKeyValue(uint8_t *keys, KeyTypes key, bool value)
{
    if (value)
        Network_setKey(keys, key);
    else
        Network_clearKey(keys, key);
}



