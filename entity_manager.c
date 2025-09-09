#include "entity_manager.h"
#include "common.h"
#include "mask.h"

$public void 
em_initialize(EntityManager* em, uint32_t nbEntities)
{
    m_initialize(&em->allocator, nbEntities * sizeof(Entity) * 4);
    em->entities = m_alloc(&em->allocator, nbEntities * sizeof(Entity));
    em->capacity = nbEntities;
    em->length = 0;
}

$public void 
em_destroy(EntityManager* em)
{
    em->capacity = 0;
    em->length = 0;
    em->entities = 0;
    m_destroy(&em->allocator);
}

$public void 
em_for_all_entities(EntityManager* em, ProcessEntityCallback process)
{
    for (uint32_t i = 0; i < em->length; i++) {
        if (em->entities[i].alloced) {
            process(&em->entities[i]);
        }
    }
}

$public void
em_for_all_entities_with_tag(EntityManager* em, mask8_t tag,
    ProcessEntityCallback process)
{
    for (uint32_t i = 0; i < em->length; i++) {
        if (em->entities[i].alloced && (em->entities[i].tags & tag)) {
            process(&em->entities[i]);
        }
    }
}

$public void 
em_for_all_components(ComponentsContainer* cc, Entity* entity,
    ProcessComponentCallback process)
{
    for (int cmpType = 0; cmpType < CMP_MAX; cmpType++) {
        if (m64_has(entity->components, CMP_MASK(cmpType))) {
            process(cc, cmpType, entity);
        }
    }
}

$public void 
em_remove_entity(EntityManager* em, ComponentsContainer* cc, Entity* entity)
{
    if (!entity->alloced)
        return;

    em_for_all_components(cc, entity, &cc_remove_component);
}

$public uint32_t
em_add_entity_at_initialize(EntityManager* em, Entity* entity)
{
    entity->id = em->length;
    entity->alloced = true;
    em->entities[entity->id] = *entity;
    em->length++;

    return entity->id;
}

$public Entity*
em_get_entity_with_tag(EntityManager* em, mask8_t tag)
{
    for (uint32_t idx = 0; idx < em->length; idx++) {
        if (m8_has(em->entities[idx].tags, tag)) {
            return &em->entities[idx];
        }
    }
    return 0;
}

$public Entity*
em_get_entity(EntityManager* em, uint32_t id)
{
    return &em->entities[id];
}
