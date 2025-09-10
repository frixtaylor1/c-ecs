#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef _WIN32
#include <windows.h>
static void
sleep_ms(unsigned ms)
{
    Sleep(ms);
}
#else
#include <unistd.h>
static void
sleep_ms(unsigned ms)
{
    usleep(ms * 1000);
}
#endif

#include "components.h"
#include "entity_manager.h"

$private EntityManager em;
$private ComponentsContainer cc;

$private const int WINDOW_WIDTH = 5;
$private const int WINDOW_HEIGHT = 10;
$private const float Y_VELOCITY = 3.0f;

$private bool game_over = false;
$private int score = 0;

$private char map[10][5]
    = { { '#', '#', '#', '#', '#' },
          { '#', ' ', ' ', ' ', '#' },
          { '#', ' ', ' ', ' ', '#' },
          { '#', ' ', ' ', ' ', '#' },
          { '#', ' ', ' ', ' ', '#' },
          { '#', ' ', ' ', ' ', '#' },
          { '#', ' ', ' ', ' ', '#' },
          { '#', ' ', ' ', ' ', '#' },
          { '#', ' ', ' ', ' ', '#' },
          { '#', ' ', ' ', ' ', '#' } };

typedef enum {
    PLAYER_STATE_HOLD,
    PLAYER_STATE_MOVE_LEFT,
    PLAYER_STATE_MOVE_RIGHT,
} PlayerState;

typedef enum {
    PLAYER_CONDITION_ENEMY_AHEAD,
    PLAYER_CONDITION_SHOULD_MOVE_LEFT,
    PLAYER_CONDITION_SHOULD_MOVE_RIGHT
} PlayerCondition;

/**
 * Spanw enemy function in random position, it is going to be the same 5
 * enemies.
 */
$private int
random_between(int x, int y)
{
    if (x > y) {
        int temp = x;
        x = y;
        y = temp;
    }
    return x + rand() % (y - x + 1);
}

$private void
update_enemy(Entity* e)
{
    TransformComponent* tc = cc_get_component(&cc, e, CMP_TransformComponent);
    PhysicsComponent* pc = cc_get_component(&cc, e, CMP_PhysicsComponent);

    tc->position.y += pc->velocity;

    if (tc->position.y >= WINDOW_HEIGHT - 1) {
        Entity* player = em_get_entity_with_tag(&em, TAG_PLAYER);
        TransformComponent* ptc = cc_get_component(&cc, player, CMP_TransformComponent);

        if (tc->position.x == ptc->position.x) {
            game_over = true;
        } else {
            score++;
        }

        pc->velocity = 1;
        tc->position.y = 1;
        tc->position.x = random_between(1, 3);
    }
}

$private void
initialize_ec(EntityManager* em, ComponentsContainer* cc, int nbEntities)
{
    em_initialize(em, (nbEntities + 1));
    cc_initialize(cc, (nbEntities + 1) * CMP_MAX);
}

$private void
reset_map()
{
    for (int i = 0; i < WINDOW_HEIGHT; i++) {
        for (int j = 0; j < WINDOW_WIDTH; j++) {
            if (j == 0 || j == WINDOW_WIDTH - 1 || i == 0) {
                map[i][j] = '#';
            } else {
                map[i][j] = ' ';
            }
        }
    }
}

$private void
clear_background()
{
    reset_map();
    printf("\x1b[2J\x1b[H");
}

$private void
draw_map()
{
    for (int i = 0; i < WINDOW_HEIGHT; i++) {
        for (int j = 0; j < WINDOW_WIDTH; j++) {
            printf("%c ", map[i][j]);
        }
        printf("\n");
    }
    printf("Score: %d\n", score);
    if (game_over) {
        printf("GAME OVER!\n");
    }
}

$private void
draw_entity(Entity* e)
{
    TransformComponent* tc = cc_get_component(&cc, e, CMP_TransformComponent);
    RenderComponent* rc = cc_get_component(&cc, e, CMP_RenderComponent);

    map[tc->position.y][tc->position.x] = rc->image;
}

$private void
initialize_enemy()
{
    uint32_t enemy_id = em_add_entity_at_initialize(
        &em, &(Entity) {
                 .tags = TAG_ENEMY,
                 .components = CMP_TransformComponent | CMP_PhysicsComponent | CMP_RenderComponent,
                 .alloced = true,
             });
    cc_add_component(
        &cc, em_get_entity(&em, enemy_id), CMP_TransformComponent,
        &(TransformComponent) { .position = { .x = random_between(1, 2), .y = 1 } });
    cc_add_component(
        &cc, em_get_entity(&em, enemy_id), CMP_RenderComponent,
        &(RenderComponent) { .image = '0' });
    cc_add_component(
        &cc, em_get_entity(&em, enemy_id), CMP_PhysicsComponent,
        &(PhysicsComponent) { .velocity = 1 });
}

$private void
on_player_hold(void* ptc)
{
}

$private void
on_player_move_left(void* arg)
{
    TransformComponent* ptc = arg;
    ptc->position.x -= 1;
}
$private void
on_player_move_right(void* arg)
{
    TransformComponent* ptc = arg;
    ptc->position.x += 1;
}

typedef struct {
    TransformComponent* ptc;
} PlayerContext;

$private void
fsm_run(PlayerFSMComponent* fsm, void* context)
{
    uint32_t nodeIndex = 0;
    for (;;) {
        if (nodeIndex >= (sizeof(fsm->tree.tree) / sizeof(fsm->tree.tree[0]))) {
            return;
        }
        FSMTreeNode node = fsm->tree.tree[nodeIndex];
        if (node.is_condition) {
            uint8_t condIndex = node.index;
            bool result = false;
            if (condIndex < MAX_CONDITIONS_PER_FSM && fsm->conditions[condIndex]) {
                result = fsm->conditions[condIndex](context);
            }
            nodeIndex = result ? node.yes_index : node.no_index;
        } else {
            uint8_t actionIndex = node.index;
            if (actionIndex < MAX_STATES_PER_FSM && fsm->actions[actionIndex]) {
                fsm->actions[actionIndex](context);
            }
            return;
        }
    }
}

$private bool
enemy_ahead(void* arg)
{
    TransformComponent* ptc = (TransformComponent*)arg;
    for (int i = 1; i <= 3; i++) {
        if (map[ptc->position.y - i][ptc->position.x] == '0')
            return true;
    }
    return false;
}

$private bool
player_should_move_left(void* arg)
{
    TransformComponent* ptc = (TransformComponent*)arg;
    return ptc->position.x > 1 && (map[ptc->position.y - 3][ptc->position.x - 1] == ' ' && map[ptc->position.y - 2][ptc->position.x - 1] == ' ');
}

$private bool
player_should_move_right(void* arg)
{
    TransformComponent* ptc = (TransformComponent*)arg;
    return (ptc->position.x < 4 && (map[ptc->position.y - 3][ptc->position.x + 1] == ' ' && map[ptc->position.y - 2][ptc->position.x + 1] == ' '))
        || (ptc->position.x == 1 && (map[ptc->position.y - 3][ptc->position.x] != ' ' && map[ptc->position.y - 3][ptc->position.x + 1] != ' '));
}

$private void
initialize_player()
{
    uint32_t player_id = em_add_entity_at_initialize(
        &em, &(Entity) {
                 .tags = TAG_PLAYER,
                 .components = CMP_TransformComponent,
                 .alloced = true,
             });

    int player_x = 2;
    int player_y = WINDOW_HEIGHT - 1;

    Entity* player = em_get_entity(&em, player_id);

    cc_add_component(
        &cc, player, CMP_TransformComponent,
        &(TransformComponent) {
            .position = (Int_Vector2_t) { .x = player_x, .y = player_y } });
    cc_add_component(
        &cc, player, CMP_RenderComponent,
        &(RenderComponent) {
            .image = '^' });
    cc_add_component(
        &cc, player, CMP_PlayerFSMComponent,
        &(PlayerFSMComponent) {
            .currentCondition = PLAYER_CONDITION_ENEMY_AHEAD,
            .currentAction = PLAYER_STATE_HOLD,
            .tree = {
                .tree = {
                    [0] = { .is_condition = true, .index = PLAYER_CONDITION_ENEMY_AHEAD, .yes_index = 1, .no_index = 2 },
                    [1] = { .is_condition = true, .index = PLAYER_CONDITION_SHOULD_MOVE_LEFT, .yes_index = 3, .no_index = 5 },
                    [2] = { .is_condition = false, .index = PLAYER_STATE_HOLD },
                    [3] = { .is_condition = false, .index = PLAYER_STATE_MOVE_LEFT },
                    [4] = { .is_condition = false, .index = PLAYER_STATE_MOVE_RIGHT },
                    [5] = { .is_condition = true, .index = PLAYER_CONDITION_SHOULD_MOVE_RIGHT, .yes_index = 4, .no_index = 3 } } },
            .conditions = {
                [PLAYER_CONDITION_ENEMY_AHEAD] = &enemy_ahead,
                [PLAYER_CONDITION_SHOULD_MOVE_LEFT] = &player_should_move_left,
                [PLAYER_CONDITION_SHOULD_MOVE_RIGHT] = &player_should_move_right,
            },
            .actions = {
                [PLAYER_STATE_HOLD] = &on_player_hold,
                [PLAYER_STATE_MOVE_LEFT] = &on_player_move_left,
                [PLAYER_STATE_MOVE_RIGHT] = &on_player_move_right,
            } });
}

$private void
initialize_enemies()
{
    for (int idx = 0; idx < 2; idx++) {
        initialize_enemy();
    }
}

$private void
update_player_state(Entity* player)
{
    TransformComponent* tc = (TransformComponent*)cc_get_component(&cc, player, CMP_TransformComponent);
    PlayerFSMComponent* fsmc = (PlayerFSMComponent*)cc_get_component(&cc, player, CMP_PlayerFSMComponent);
    fsm_run(fsmc, tc);
}

$private void
update_player(Entity* player)
{
    update_player_state(player);
}

int 
main(int argc, char** argv)
{
    srand(time(NULL));

    initialize_ec(&em, &cc, 7);
    defer(em_destroy, &em);
    defer(cc_destroy, &cc);

    initialize_player();
    initialize_enemies();

    int counter = 0;

    while (true) {
        { /** @brief drawing... */
            em_for_all_entities_with_tag(&em, TAG_PLAYER | TAG_ENEMY, &draw_entity);
            draw_map();
        }

        { /** @brief updating... */
            em_for_all_entities_with_tag(&em, TAG_ENEMY, &update_enemy);
            em_for_all_entities_with_tag(&em, TAG_PLAYER, &update_player);
        }

        { /** @brief handle fps... */
            sleep_ms(160);
        }

        { /** @brief clearing background... */
            clear_background();
        }

        if (counter++ > 250 || game_over)
            break;
    }

    return 0;
}