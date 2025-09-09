#include "components_container.h"
#include <string.h>

#define MAX_COMPONENT_PER_STORAGE 100
#define SIZE_AVERAGE_COMPONENT 100

static const size_t cmp_sizes[CMP_MAX] = {
    [CMP_RenderComponent]    = sizeof(RenderComponent),
    [CMP_TransformComponent] = sizeof(TransformComponent),
    [CMP_PhysicsComponent]   = sizeof(PhysicsComponent),
    [CMP_PlayerFSMComponent] = sizeof(PlayerFSMComponent)
};

$public void
cc_initialize(ComponentsContainer* cc, uint32_t capacity)
{
    m_initialize(&cc->allocator, CMP_MAX * capacity * SIZE_AVERAGE_COMPONENT);

    for (int cmp = 0; cmp < CMP_MAX; cmp++) {
        if (cmp_sizes[cmp] > 0) {
            cc->storages[cmp] = m_alloc(&cc->allocator, capacity * cmp_sizes[cmp]);
        } else {
            cc->storages[cmp] = NULL;
        }
    }
}

$public void
cc_destroy(ComponentsContainer* cc)
{
    m_destroy(&cc->allocator);
}

$public void*
cc_get_component(ComponentsContainer* cc, const Entity* e, ComponentType cmp)
{
    return ((char*)cc->storages[cmp] + e->id * get_component_size(cmp));
}

$public void
cc_remove_component(ComponentsContainer* cc, int cmpTypeIdIdx, Entity* e)
{
    int sizeOfComponent = get_component_size(cmpTypeIdIdx);
    void* ptr = cc_get_component(cc, e, cmpTypeIdIdx);
    memset(ptr, 0, get_component_size(cmpTypeIdIdx));
}

$public void
cc_add_component(ComponentsContainer* cc, Entity* e, ComponentType cmp,
    void* component)
{
    switch (cmp) {
    case CMP_RenderComponent:
        CC_ADD(cc, e, RenderComponent, (RenderComponent*)component);
        break;
    case CMP_PhysicsComponent:
        CC_ADD(cc, e, PhysicsComponent, (PhysicsComponent*)component);
        break;
    case CMP_TransformComponent:
        CC_ADD(cc, e, TransformComponent, (TransformComponent*)component);
        break;
    case CMP_PlayerFSMComponent:
        CC_ADD(cc, e, PlayerFSMComponent, (PlayerFSMComponent*)component);
        break;
    }
    e->components |= CMP_MASK(cmp);
}