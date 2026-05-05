#pragma once

#include <stdlib.h>

typedef struct {
    size_t count;
    size_t capacity;
} array_header_t;

// Allocs memory for the array data and its header. Pointer is set to null if
// allocation fails. If given capacity is 0, a default value of 4 is used.
#define ARRAY(ptr, cap) do { \
    size_t capacity = (cap) ? (cap) : 4; \
    array_header_t* h = malloc(sizeof(*(ptr)) * capacity + sizeof(array_header_t)); \
    if (!h) { \
        (ptr) = NULL; \
        break; \
    } \
    h->capacity = capacity; \
    h->count = 0; \
    (ptr) = (void*) (h + 1); \
} while (0)

// Moves backwards from array pointer and retrieves array header. Not safe.
#define ARRAY_HEADER(ptr) ((array_header_t*) ((ptr)) - 1)

// Retrieves capacity for the array. Not safe.
#define ARRAY_CAPACITY(ptr) (ARRAY_HEADER((ptr))->capacity)

// Retrieves count for the array. Not safe.
#define ARRAY_COUNT(ptr) (ARRAY_HEADER((ptr))->count)

// Frees array memory. Safe on NULL pointers.
#define ARRAY_FREE(ptr) do { if ((ptr)) free(ARRAY_HEADER((ptr))); } while (0)

// Adds item to the back of the array. Safe on NULL pointers. 
// If reallocation fails, appending is skipped.
#define ARRAY_APPEND(ptr, item) do { \
    if (!(ptr)) break; \
    array_header_t* h = ARRAY_HEADER((ptr)); \
    if (h->count == h->capacity) { \
        size_t ncap = h->capacity * 2; \
        array_header_t* nh = realloc(h, sizeof(array_header_t) + sizeof(*(ptr)) * ncap); \
        if (!nh) break; \
        nh->capacity = ncap; \
        (ptr) = (void*) (nh + 1); \
        h = nh; \
    } \
    (ptr)[h->count++] = (item);\
} while (0)

// Retrieves last element of the array and reduces array count. Not safe.
#define ARRAY_POP(ptr) ((ptr)[--ARRAY_COUNT(ptr)])

// Reallocates array memory to fit current count. Safe on NULL pointers.
#define ARRAY_SHRINK_TO_FIT(ptr) do { \
    if (!(ptr)) break; \
    array_header_t* h = ARRAY_HEADER((ptr)); \
    size_t ncap = h->count; \
    array_header_t* nh = realloc(h, sizeof(array_header_t) + sizeof(*(ptr)) * ncap); \
    if (!nh) break; \
    nh->capacity = ncap; \
    nh->count = ncap; \
    (ptr) = (void*) (nh + 1); \
} while (0)
    
// Resets array count to zero. Safe on NULL pointers.
#define ARRAY_CLEAR(ptr) do { \
    if (!(ptr)) break; \
    ARRAY_HEADER((ptr))->count = 0; \
} while (0)
