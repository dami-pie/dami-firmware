#if !defined(__hash_gen_h__)
#define __hash_gen_h__

#include <inttypes.h>

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

// uint32_t hash(void *obj, size_t size)
// {
//     uint32_t hash = 5381;
//     uint8_t *ptr = (uint8_t *)obj;
//     uint8_t *end = (uint8_t *)(obj + size);

//     while (c = *ptr++)
//     {
//         hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

//     }

//     return hash;
// }

#endif // __hash_gen_h__
