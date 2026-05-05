#pragma once

#include <stdlib.h>

typedef struct {
    size_t capacity;
    size_t count;
    size_t tail;
} stack_header_t;

#define STACK(ptr, cap) do { \
    stack_header_t* h = malloc(sizeof(stack_header_t) + sizeof(*ptr) * (cap)); \
    if (!h) break; \
    h->capacity = (cap); \
    h->count = 0; \
    h->tail = 0; \
    (ptr) = (void*) (h + 1); \
} while (0)

#define STACK_HEADER(ptr) ((ptr) ? (stack_header_t*)((ptr)) - 1 : NULL)

#define STACK_CAPACITY(ptr) ((ptr) ? STACK_HEADER((ptr))->capacity : 0 )

#define STACK_COUNT(ptr) ((ptr) ? STACK_HEADER((ptr))->count : 0)

#define STACK_FREE(ptr) do { if (ptr) free(STACK_HEADER((ptr))); } while (0)

#define STACK_APPEND(ptr, item) do { \
    if (!(ptr)) break; \
    stack_header_t* h = STACK_HEADER((ptr)); \
    (ptr)[h->tail] = item; \
    h->tail = (h->tail + 1) % h->capacity; \
    if (h->count < h->capacity) h->count++; \
} while (0)

#define STACK_PEEK(ptr) \
    ((ptr)[STACK_HEADER((ptr))->tail == 0 \
        ? STACK_HEADER((ptr))->capacity - 1 \
        : STACK_HEADER((ptr))->tail - 1])

#define STACK_DISCARD(ptr) do {                             \
    stack_header_t* h = STACK_HEADER((ptr));                \
    if (h->count == 0) break;                               \
    h->tail = (h->tail == 0) ? h->capacity - 1 : h->tail - 1; \
    h->count--;                                             \
} while (0)
