#include "components.h"
#include <assert.h>

static uint32_t ComponentTypeSize[] = { 
    [CMP_RenderComponent]    = sizeof(RenderComponent),
    [CMP_TransformComponent] = sizeof(TransformComponent),
    [CMP_PhysicsComponent]   = sizeof(PhysicsComponent),
    [CMP_PlayerFSMComponent] = sizeof(PlayerFSMComponent)
};

uint32_t
get_component_size(uint32_t cmpTypeIdx)
{
    assert(cmpTypeIdx < sizeof(ComponentTypeSize) / sizeof(uint32_t)
        && "Error, must add CMP_#TypeIdx into ComponentTypeSize array.");
    return ComponentTypeSize[cmpTypeIdx];
}
