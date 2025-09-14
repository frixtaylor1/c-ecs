#ifndef components_container_H
#define components_container_H

#include "alloc.h"
#include "common.h"
#include "components.h"
#include "spinlock.h"
#include "entity.h"

typedef struct ComponentsContainer {
    PoolAllocator allocator;
    void* storages[CMP_MAX];
    Spinlock locks[CMP_MAX];
} ComponentsContainer;

/// Callback used for processing components of entities
typedef void (*ProcessComponentCallback)(ComponentsContainer* cc, int cmpType,
    Entity* e);

/// @brief Adds a component of type `cmpType` to the entity `e`.
/// @param cc Components container
/// @param e Entity pointer
/// @param cmpType Component type
/// @param cmpValue Value to assign to the component
#define CC_ADD(cc, e, cmpType, cmpValue) \
    (((cmpType*)cc->storages[CMP_##cmpType])[(e)->id] = *(cmpValue))

/// @brief Retrieves a pointer to a component of type `cmpType` from entity `e`.
/// @param cc Components container
/// @param e Entity pointer
/// @param cmpType Component type
#define CC_GET(cc, e, cmpType) \
    (&((cmpType*)((cc)->storages[CMP_##cmpType]))[(e)->id])

/// @brief Gets the raw storage buffer for a given component type.
/// @param cc Components container
/// @param cmpTypeIdx Index of the component type
#define CC_GET_STORAGE(cc, cmpTypeIdx) (cc->storages[cmpTypeIdx])

/// @brief Initializes a ComponentsContainer with the given capacity.
/// @param cc Components container
/// @param capacity Maximum number of entities/components supported
$public void cc_initialize(ComponentsContainer* cc, uint32_t capacity);

/// @brief Releases all memory used by the ComponentsContainer.
/// @param cc Components container
$public void cc_destroy(ComponentsContainer* cc);

/// @brief Retrieves the specified component of an entity.
/// @param cc Components container
/// @param e Pointer to the entity
/// @param cmp Component type
/// @return Pointer to the component data, or NULL if not found
$public void* cc_get_component(ComponentsContainer* cc, const Entity* e,
    ComponentType cmp);

/// @brief Removes a component of a given type from an entity.
/// @param cc Components container
/// @param cmpTypeIdIdx Index of the component type
/// @param e Entity pointer
$public void cc_remove_component(ComponentsContainer* cc, int cmpTypeIdIdx,
    Entity* e);

/// @brief Adds a component to an entity.
/// @param cc Components container
/// @param e Entity pointer
/// @param cmp Component type
/// @param component Pointer to the component data to add
$public void cc_add_component(ComponentsContainer* cc, Entity* e, ComponentType cmp,
    void* component);

#endif // components_container_H
