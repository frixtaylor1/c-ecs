#ifndef components_H
#define components_H

#include "common.h"

#define CMP_MASK(t) (1ULL << (t))

#include "entity.h"

typedef enum {
    CMP_RenderComponent,
    CMP_TransformComponent,
    CMP_PhysicsComponent,
    CMP_PlayerFSMComponent,
    CMP_MAX
} ComponentType;

typedef struct RenderComponent {
    uint8_t image;
} RenderComponent;

typedef struct Float_Vector2_t {
    float x, y;
} Float_Vector2_t;

typedef struct Float_Vector3_t {
    float x, y, z;
} Float_Vector3_t;

typedef struct Int_Vector2_t {
    int x, y;
} Int_Vector2_t;

typedef struct Int_Vector3_t {
    int x, y, z;
} Int_Vector3_t;

typedef struct TransformComponent {
    Int_Vector2_t position;
} TransformComponent;

typedef struct PhysicsComponent {
    int velocity;
} PhysicsComponent;

/// @brief Maximum number of states allowed per FSM.
#define MAX_STATES_PER_FSM 10

/// @brief Maximum number of conditions allowed per FSM.
#define MAX_CONDITIONS_PER_FSM 10

/// @brief Function pointer type for FSM condition evaluation.
/// @param ctx Pointer to the context passed to the condition function.
/// @return true if the condition is met, false otherwise.
typedef bool (*FSMConditionCallback)(void *ctx);

/// @brief Function pointer type for FSM action execution.
/// @param ctx Pointer to the context passed to the action function.
typedef void (*FSMActionCallback)(void *ctx);

/// @brief Represents a node in the FSM decision tree.
typedef struct {
    bool is_condition;
    uint8_t index;
    uint8_t yes_index;
    uint8_t no_index;
} FSMTreeNode;

/// @brief Finite State Machine decision tree containing nodes.
typedef struct FSMTree {
    FSMTreeNode tree[16];
} FSMTree;

/// @brief Component representing the player's finite state machine.
typedef struct PlayerFSMComponent {
    uint32_t currentCondition;
    uint32_t currentAction;
    FSMTree tree;
    FSMConditionCallback conditions[MAX_CONDITIONS_PER_FSM];
    FSMActionCallback actions[MAX_STATES_PER_FSM];
} PlayerFSMComponent;

/// @brief Returns the size in bytes of a component type given its index.
/// @param cmpTypeIdx Index of the component type
/// @return Size of the component type in bytes
uint32_t get_component_size(uint32_t cmpTypeIdx);

#endif // components_H