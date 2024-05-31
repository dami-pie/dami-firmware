#if !defined(__hash_gen_h__)
#define __hash_gen_h__

#include <Arduino.h>

template <typename T>
uint32_t hash(T *obj)
{
    uint32_t hash = 5381;
    uint8_t *ptr = (uint8_t *)obj;
    int c;

    while (c = *ptr++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

#endif // __hash_gen_h__
