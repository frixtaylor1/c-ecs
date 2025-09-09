#ifndef entity_manager_H
#define entity_manager_H

#include "alloc.h"
#include "common.h"
#include "components.h"
#include "components_container.h"
#include "entity.h"

/// @brief Manages a collection of entities and provides efficient allocation.
typedef struct EntityManager {
    Entity* entities;         ///< Array of entities managed by the manager
    PoolAllocator allocator;  ///< Memory allocator used for entities
    uint32_t length;          ///< Current number of active entities
    uint32_t capacity;        ///< Maximum capacity of entities
    uint32_t freelist[1000];  ///< Free indices for recycling entity slots
} EntityManager;

/// @brief Function pointer type to process an entity.
/// @param e Pointer to the entity to process.
typedef void (*ProcessEntityCallback)(Entity*);

/// @brief Initialize the EntityManager with a given number of entities.
/// @param em Pointer to the EntityManager to initialize.
/// @param nbEntities Number of entities to preallocate.
void em_initialize(EntityManager* em, uint32_t nbEntities);

/// @brief Destroy the EntityManager and free all associated memory.
/// @param em Pointer to the EntityManager to destroy.
void em_destroy(EntityManager* em);

/// @brief Iterate over all entities and apply a callback function.
/// @param em Pointer to the EntityManager.
/// @param process Callback function to execute on each entity.
void em_for_all_entities(EntityManager* em, ProcessEntityCallback process);

/// @brief Iterate over all entities that have a specific tag and apply a callback.
/// @param em Pointer to the EntityManager.
/// @param tag Tag mask used to filter entities.
/// @param process Callback function to execute on each matching entity.
void em_for_all_entities_with_tag(EntityManager* em, mask8_t tag,
    ProcessEntityCallback process);

/// @brief Iterate over all components of a given entity and apply a callback.
/// @param cc Pointer to the ComponentsContainer.
/// @param entity Pointer to the entity whose components will be processed.
/// @param process Callback function to execute on each component.
void em_for_all_components(ComponentsContainer* cc, Entity* entity,
    ProcessComponentCallback process);

/// @brief Remove an entity from the manager and its components from the container.
/// @param em Pointer to the EntityManager.
/// @param cc Pointer to the ComponentsContainer.
/// @param entity Pointer to the entity to remove.
void em_remove_entity(EntityManager* em, ComponentsContainer* cc,
    Entity* entity);

/// @brief Add an entity during initialization phase.
/// @param em Pointer to the EntityManager.
/// @param entity Pointer to the entity to add.
/// @return The ID of the added entity.
uint32_t em_add_entity_at_initialize(EntityManager* em, Entity* entity);

/// @brief Get the first entity that has a specific tag.
/// @param em Pointer to the EntityManager.
/// @param tag Tag mask to search for.
/// @return Pointer to the first entity with the given tag, or NULL if none found.
Entity* em_get_entity_with_tag(EntityManager* em, mask8_t tag);

/// @brief Get an entity by its ID.
/// @param em Pointer to the EntityManager.
/// @param id ID of the entity to retrieve.
/// @return Pointer to the entity with the given ID, or NULL if invalid.
Entity* em_get_entity(EntityManager* em, uint32_t id);

#endif // entity_manager_H