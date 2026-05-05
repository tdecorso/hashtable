#pragma once
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "stack.h"

static uint32_t djb2(const char* key) {
    uint32_t hash = 5381;
    int c;
    while ((c = *key++))
        hash = ((hash << 5) + hash) + c;
    return hash;
}

typedef struct {
    size_t size;
    size_t count;
} ht_header_t;

#define HASHTABLE(ptr, buckets) do { \
    ht_header_t* _h = calloc(1, sizeof(ht_header_t) + (buckets) * sizeof(*(ptr)) \
                                                    + (buckets) * sizeof(size_t)); \
    if (!_h) break; \
    _h->size = (buckets); \
    (ptr) = (void*)(_h + 1); \
} while (0)

#define HASH_HEADER(ptr) ((ht_header_t*)(ptr) - 1)
#define HASH_SIZE(ptr)   ((ptr) ? HASH_HEADER(ptr)->size : 0)
#define HASH_COUNT(ptr)   ((ptr) ? HASH_HEADER(ptr)->count : 0)

#define HASH_INT(key)    ((size_t)     (key))
#define HASH_STRING(key) ((size_t) djb2(key))  

#define HASH_STORE(ptr) ((size_t*)((ptr) + HASH_SIZE(ptr)))

#define HASHTABLE_FREE(ptr) do { \
    if (!(ptr)) break; \
    ht_header_t* _h = HASH_HEADER((ptr)); \
    for (size_t _i = 0; _i < _h->size; ++_i) { \
        if ((ptr)[_i]) STACK_FREE((ptr)[_i]); \
    } \
    free(_h); \
} while (0)

#define _HASHTABLE_PUT_RAW(ptr, hash, value) do { \
    size_t _key = (hash) % HASH_SIZE((ptr)); \
    if (!(ptr)[_key]) { STACK((ptr)[_key], 8); } \
    HASH_STORE(ptr)[_key] = (hash); \
    STACK_APPEND((ptr)[_key], (value)); \
    HASH_HEADER((ptr))->count++; \
} while (0)

#define HASHTABLE_REHASH(ptr, new_buckets) do { \
    __typeof__(ptr) _new = NULL; \
    HASHTABLE(_new, (new_buckets)); \
    if (!_new) break; \
    for (size_t _i = 0; _i < HASH_SIZE(ptr); _i++) { \
        if (!(ptr)[_i]) continue; \
        size_t _raw = HASH_STORE(ptr)[_i]; \
        size_t _new_idx = _raw % (new_buckets); \
        stack_header_t* _sh = STACK_HEADER((ptr)[_i]); \
        size_t _bytes = sizeof(stack_header_t) + _sh->capacity * sizeof(**(ptr)); \
        stack_header_t* _copy = malloc(_bytes); \
        if (!_copy) { \
            HASHTABLE_FREE(_new); \
            break; \
        } \
        memcpy(_copy, _sh, _bytes); \
        (_new)[_new_idx] = (void*)(_copy + 1); \
        HASH_STORE(_new)[_new_idx] = _raw; \
        free(_sh); \
    } \
    HASH_HEADER(_new)->count = HASH_HEADER(ptr)->count; \
    free(HASH_HEADER(ptr)); \
    (ptr) = _new; \
} while (0)

#define HASHTABLE_PUT(ptr, hash, value) do { \
    if (!(ptr)) break; \
    _HASHTABLE_PUT_RAW(ptr, hash, value); \
    if (HASH_LOAD(ptr) > 0.75f) HASHTABLE_REHASH(ptr, HASH_SIZE(ptr) * 2); \
} while (0)

#define HASHTABLE_GET(ptr, hash) (STACK_PEEK((ptr)[(hash) % HASH_SIZE(ptr)]))
#define HASHTABLE_DISCARD(ptr, hash) do { \
    if (!(ptr)) break; \
    STACK_DISCARD((ptr)[(hash) % HASH_SIZE(ptr)]);\
} while (0)

#define HASH_LOAD(ptr) ((ptr) ? (float)HASH_COUNT(ptr) / HASH_SIZE(ptr) : 0)
