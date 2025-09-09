#ifndef entity_H
#define entity_H

#include "alloc.h"
#include "common.h"

#define MAX_COMPONENTS 64

/// @brief Represents a single entity in the ECS system.
typedef struct Entity {
    uint32_t id;
    mask64_t components;
    mask8_t tags;
    bool alloced : 1;
} Entity;

/// @brief Enumeration of possible entity tags used for filtering and identification.
typedef enum {
    TAG_INVALID = 1 << 0,
    TAG_PLAYER  = 1 << 1,
    TAG_ENEMY   = 1 << 2
} EntityTag;

#endif // entity_H