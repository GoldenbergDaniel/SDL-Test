#include <stdio.h>
#include <math.h>

#include "base/base_common.h"
#include "base/base_math.h"
#include "base/base_random.h"

#include "physx/physx.h"
#include "input.h"
#include "event.h"
#include "game.h"
#include "entity.h"

#define DEBUG

extern Global *GLOBAL;

static void print_lists(Game *game);
static void init_timers(Entity *en);
static Timer *get_timer(Entity *en, u8 index);
static bool tick_timer(Timer *timer, f64 dt);

const b64 PLAYER_PROPS = EntityProp_Rendered | EntityProp_Controlled | EntityProp_Movable
  | EntityProp_Combatant | EntityProp_Collides;

const b64 ENEMY_PROPS = EntityProp_Rendered | EntityProp_Autonomous | EntityProp_Hostile | EntityProp_Movable | EntityProp_Combatant;

const b64 LASER_PROPS = EntityProp_Rendered | EntityProp_Movable | EntityProp_Autonomous;

// @InitEntity =================================================================================

void init_entity(Entity *en, EntityType type)
{
  en->type = type;
  en->xform = m3x3f(1.0f);
  en->active = TRUE;
  en->visible = TRUE;
  // en->gravity = 0.0f;

  init_timers(en);

  switch (type)
  {
    case EntityType_Player:
    {
      en->move_type = MoveType_Walking;
      en->combat_type = CombatType_Ranged;
      en->props = PLAYER_PROPS;
      en->pos = v2f(W_WIDTH / 2.0f, W_HEIGHT / 2.0f);
      en->speed = PLAYER_SPEED;
      en->color = COLOR_WHITE;
      en->col.vertex_count = 4;

      set_entity_scale(en, v2f(4.0f, 4.0f));

      Timer *timer = get_timer(en, TIMER_COMBAT);
      timer->max_duration = 0.25f;
      timer->curr_duration = 0.0f;
      timer->should_tick = FALSE;
    }
    break;
    case EntityType_EnemyShip:
    {
      en->move_type = MoveType_Flying;
      en->combat_type = CombatType_Ranged;
      en->props = ENEMY_PROPS;
      en->pos = v2f(random_u32(0, W_WIDTH), random_u32(0, W_HEIGHT));
      en->speed = 100.0f;
      en->color = COLOR_RED;
      en->view_dist = 350;

      set_entity_scale(en, v2f(1.5f, 1.5f));

      Timer *timer = get_timer(en, TIMER_COMBAT);
      timer->should_loop = TRUE;
    }
    break;
    case EntityType_Laser:
    {
      en->move_type = MoveType_Projectile;
      en->combat_type = CombatType_Melee;
      en->props = LASER_PROPS;
      set_entity_scale(en, v2f(3.0f, 0.15f));
    }
    break;
    case EntityType_Wall:
    {
      en->props = EntityProp_Rendered | EntityProp_Collides;
      en->pos = v2f(W_WIDTH/2.0f, W_HEIGHT/2.0f + 100.0f);
      en->color = COLOR_RED;
      en->col.vertex_count = 4;
      
      set_entity_scale(en, v2f(2.0f, 2.0f));
      update_entity_collider(en);
    }
    break;
    case EntityType_DebugLine:
    {
      en->props = EntityProp_Rendered;
      en->color = COLOR_YELLOW;
    }
    break;
    default:
    {
      fprintf(stderr, "ERROR: Failed to initialize en. Invalid type!");
      fprintf(stderr, "Entity ID: %llu\n", en->id);
      fprintf(stderr, "Entity Type: %i\n", en->type);
      fprintf(stderr, "Vertex Count: %i\n", en->col.vertex_count);
      assert(FALSE);
    }
  }
}

inline
void clear_entity(Entity *en)
{
  Entity *next = en->next;
  Entity *next_free = en->next_free;
  zero(*en);
  en->next = next;
  en->next_free = next_free;
}

// @UpdateEntity ===============================================================================

void update_entity_collider(Entity *en)
{
  Collider2D *col = &en->col;

  switch (col->type)
  {
    case ColliderType_Polygon:
    {
      switch (col->vertex_count)
      {
        case 3:
        {
          col->vertices[0].x = en->pos.x;
          col->vertices[0].y = en->pos.y + en->height/2.0f;
          col->vertices[1].x = en->pos.x + en->width/2.0f;
          col->vertices[1].y = en->pos.y + en->height/2.0f;
          col->vertices[2].x = en->pos.x + en->width/2.0f;
          col->vertices[2].y = en->pos.y - en->height/2.0f;

          col->edges[0][0] = 0;
          col->edges[0][1] = 1;
          col->edges[1][0] = 1;
          col->edges[1][1] = 2;
          col->edges[2][0] = 2;
          col->edges[2][1] = 0;

          col->normals[0] = normal_2f(col->vertices[col->edges[0][0]], 
                                      col->vertices[col->edges[0][1]]);
          col->normals[1] = normal_2f(col->vertices[col->edges[1][0]], 
                                      col->vertices[col->edges[1][1]]);
          col->normals[2] = normal_2f(col->vertices[col->edges[2][0]], 
                                      col->vertices[col->edges[2][1]]);
        }
        case 4:
        {
          col->vertices[0].x = en->pos.x - en->width/2.0f;
          col->vertices[0].y = en->pos.y + en->height/2.0f;
          col->vertices[1].x = en->pos.x + en->width/2.0f;
          col->vertices[1].y = en->pos.y + en->height/2.0f;
          col->vertices[2].x = en->pos.x + en->width/2.0f;
          col->vertices[2].y = en->pos.y - en->height/2.0f;
          col->vertices[3].x = en->pos.x - en->width/2.0f;
          col->vertices[3].y = en->pos.y - en->height/2.0f;

          col->edges[0][0] = 0;
          col->edges[0][1] = 1;
          col->edges[1][0] = 1;
          col->edges[1][1] = 2;
          col->edges[2][0] = 2;
          col->edges[2][1] = 3;
          col->edges[3][0] = 3;
          col->edges[3][1] = 4;

          col->normals[0] = normal_2f(col->vertices[col->edges[0][0]], 
                                      col->vertices[col->edges[0][1]]);
          col->normals[1] = normal_2f(col->vertices[col->edges[1][0]], 
                                      col->vertices[col->edges[1][1]]);
          col->normals[2] = normal_2f(col->vertices[col->edges[2][0]], 
                                      col->vertices[col->edges[2][1]]);
          col->normals[3] = normal_2f(col->vertices[col->edges[3][0]], 
                                      col->vertices[col->edges[3][1]]);
        }
        break;
        default:
        {
          fprintf(stderr, "ERROR: Invalid number of vertices in collider!\n");
          fprintf(stderr, "Entity ID: %llu\n", en->id);
          fprintf(stderr, "Entity Type: %i\n", en->type);
          fprintf(stderr, "Vertex Count: %i\n", en->col.vertex_count);
          assert(FALSE);
        }
      }
    }
    break;
    case ColliderType_Circle:
    {

    }
    break;
    default: 
    {
      printf("ERROR: Failed to update collider. Invalid type!");
      assert(FALSE);
    }
  }
}

void update_entity_xform(Game *game, Entity *en)
{
  Mat3x3F xform = m3x3f(1.0f);
  Entity *parent = entity_from_ref(en->parent);

  if (en->flip_x) xform = mul_3x3f(scale_3x3f(-1.0f, 1.0f), xform);
  if (en->flip_y) xform = mul_3x3f(scale_3x3f(1.0f, -1.0f), xform);

  // NOTE: You are not properly updating global position. Shoud you?
  if (is_entity_valid(parent))
  {
    xform = mul_3x3f(scale_3x3f(en->scale.x, en->scale.y), xform);
    xform = mul_3x3f(translate_3x3f(en->pos_offset.x, en->pos_offset.y), xform);
    xform = mul_3x3f(rotate_3x3f(en->rot * RADIANS), xform);
    xform = mul_3x3f(translate_3x3f(en->local_pos.x, en->local_pos.y), xform);
    
    xform = mul_3x3f(parent->model, xform);
  }
  else
  {
    xform = mul_3x3f(scale_3x3f(en->scale.x, en->scale.y), xform);
    xform = mul_3x3f(translate_3x3f(en->pos_offset.x, en->pos_offset.y), xform);
    xform = mul_3x3f(rotate_3x3f(en->rot * RADIANS), xform);
    xform = mul_3x3f(translate_3x3f(en->pos.x, en->pos.y), xform);
  }

  en->model = xform;

  xform = mul_3x3f(game->camera, xform);
  Mat3x3F ortho = orthographic_3x3f(0.0f, W_WIDTH, 0.0f, W_HEIGHT);
  xform = mul_3x3f(ortho, xform);

  en->xform = xform;
}

void update_controlled_entity_movement(Game *game, Entity *en)
{
  f64 dt = game->dt;

  switch (en->move_type)
  {
    case MoveType_Walking:
    {
      if (is_key_pressed(KEY_A) && !is_key_pressed(KEY_D))
      {
        en->dv.x = lerp_1f(en->dv.x, -en->speed, PLAYER_ACC * dt);
        en->input_dir.x = -1.0f;
        en->flip_x = TRUE;
      }

      if (is_key_pressed(KEY_D) && !is_key_pressed(KEY_A))
      {
        en->dv.x = lerp_1f(en->dv.x, en->speed, PLAYER_ACC * dt);
        en->input_dir.x = 1.0f;
        en->flip_x = FALSE;
      }

      if (is_key_pressed(KEY_A) && is_key_pressed(KEY_D))
      {
        en->dv.x = lerp_1f(en->dv.x, 0.0f, PLAYER_FRIC * 2.0f * dt);
        en->dv.x = to_zero(en->dv.x, 1.0f);
      }
      
      if (!is_key_pressed(KEY_A) && !is_key_pressed(KEY_D))
      {
        en->dv.x = lerp_1f(en->dv.x, 0.0f, PLAYER_FRIC * dt);
        en->dv.x = to_zero(en->dv.x, 1.0f);
      }

      // GRAVITY
      if (en->dv.y >= -1000.0f && !en->grounded)
      {
        en->dv.y -= GRAVITY;
      }

      // JUMPING
      if (is_key_pressed(KEY_W) && en->grounded)
      {
        en->dv.y += PLAYER_JUMP_HEIGH;
        en->grounded = FALSE;
      }

      en->vel = scale_2f(en->dv, dt);

      printf("VelX: %.02f\n", en->vel.x);
      printf("VelY: %.02f\n", en->vel.y);
    }
    break;
    case MoveType_Flying:
    {
      f32 omega = clamp(90.0f * magnitude_2f(en->vel), 100.0f, 180.0f);

      if (is_key_pressed(KEY_A)) en->rot += omega * dt;
      if (is_key_pressed(KEY_D)) en->rot -= omega * dt;

      if (is_key_pressed(KEY_W) && !is_key_pressed(KEY_S))
      {
        en->speed = lerp_1f(en->speed, PLAYER_SPEED, PLAYER_ACC * dt);
      }
      else if (!is_key_pressed(KEY_W) && is_key_pressed(KEY_S))
      {
        en->speed = lerp_1f(en->speed, 0.0f, PLAYER_FRIC * 2 * dt);
        en->speed = to_zero(en->speed, 1.0f);
      }
      else
      {
        en->speed = lerp_1f(en->speed, 0.0f, PLAYER_FRIC * dt);
        en->speed = to_zero(en->speed, 1.0f);
      }

      en->vel.x = cosf(en->rot * RADIANS) * en->speed * dt;
      en->vel.y = sinf(en->rot * RADIANS) * en->speed * dt;

      en->dir = scale_2f(en->vel, 1.0f / magnitude_2f(en->vel));
    }
    default: break;
  }

  en->pos = add_2f(en->pos, en->vel);
}

void update_autonomous_entity_movement(Game *game, Entity *en)
{
  f64 dt = game->dt;

  if (en->move_type == MoveType_Flying)
  {
    if (en->has_target)
    {
      en->input_dir.x = sinf(en->target_angle);
      en->input_dir.y = cosf(en->target_angle);

      en->rot = -(en->target_angle * DEGREES + 270.0f);
    }
    else
    {
      en->input_dir = V2F_ZERO;
    }

    if (en->input_dir.x != 0.0f || en->input_dir.y != 0.0f)
    {
      en->input_dir = normalize_2f(en->input_dir);
    }

    // X Acceleration
    if (en->input_dir.x != 0.0f)
    {
      en->vel.x += PLAYER_ACC * dir(en->input_dir.x) * dt;
      en->vel.x = clamp(
                            en->vel.x, 
                            -en->speed * abs(en->input_dir.x) * dt,
                            en->speed * abs(en->input_dir.x) * dt);
    }
    else
    {
      en->vel.x = lerp_1f(en->vel.x, 0.0f, PLAYER_FRIC * dt);
      en->vel.x = to_zero(en->vel.x, 0.1f);
    }

    // Y Acceleration
    if (en->input_dir.y != 0.0f)
    {
      en->vel.y += PLAYER_ACC * dir(en->input_dir.y) * dt;
      en->vel.y = clamp(
                            en->vel.y, 
                            -en->speed * abs(en->input_dir.y) * dt, 
                            en->speed * abs(en->input_dir.y) * dt);
    }
    else 
    {
      en->vel.y = lerp_1f(en->vel.y, 0.0f, PLAYER_FRIC * dt);
      en->vel.y = to_zero(en->vel.y, 0.1f);
    }

    en->dir = scale_2f(en->vel, 1.0f / magnitude_2f(en->vel));
  }
  else if (en->move_type == MoveType_Projectile)
  {
    Timer *timer = get_timer(en, TIMER_KILL);
    tick_timer(timer, dt);

    if (timer->timeout)
    {
      EventDesc desc = {.id = en->id};
      push_event(game, EventType_KillEntity, desc);
    }

    en->vel.x = cosf(en->rot * RADIANS) * en->speed * dt;
    en->vel.y = sinf(en->rot * RADIANS) * en->speed * dt;
  }

  en->pos = add_2f(en->pos, en->vel);
}

void update_controlled_entity_combat(Game *game, Entity *en)
{
  Timer *timer = get_timer(en, TIMER_COMBAT);

  if (timer->should_tick)
  {
    tick_timer(timer, game->dt);
  }

  bool can_shoot = is_key_pressed(KEY_SPACE) && (timer->timeout || !timer->should_tick);
  if (can_shoot)
  {
    EventDesc desc = 
    {
      .type = EntityType_Laser,
      .position = v2f(en->pos.x, en->pos.y + 10.0f),
      .rotation = en->flip_x ? (en->rot + 180.0f) : en->rot,
      .speed = 700.0f,
      .color = COLOR_BLUE,
    };
    
    push_event(game, EventType_SpawnEntity, desc);

    timer->should_tick = TRUE;
  }
}

void update_targetting_entity_combat(Game *game, Entity *en)
{
  if (en->has_target)
  {
    Timer *timer = get_timer(en, TIMER_COMBAT);
    tick_timer(timer, game->dt);

    bool can_shoot = timer->timeout;
    if (can_shoot)
    {
      EventDesc desc = 
      {
        .type = EntityType_Laser,
        .position = v2f(en->pos.x, en->pos.y),
        .rotation = en->rot,
        .speed = 1000.0f,
        .color = COLOR_GREEN,
      };

      push_event(game, EventType_SpawnEntity, desc);
    }
  }
}

// @SetEntity ==================================================================================

inline
void set_entity_size(Entity *en, f32 width, f32 height)
{
  en->width = width;
  en->height = height;
  en->scale = v2f(en->width/20.0f, en->height/20.0f);
}

inline
void set_entity_scale(Entity *en, Vec2F scale)
{
  en->scale = scale;
  en->width = 20.0f * en->scale.x;
  en->height = 20.0f * en->scale.y;
}

void set_entity_origin(Entity *en, Vec2I origin)
{
  if (origin.x == 0 && origin.y == 0)
  {
    en->pos_offset = V2F_ZERO;
  }
  else if (origin.x == -1 && origin.y == 0)
  {
    en->pos_offset = v2f(en->width/2.0f, 0.0f);
  }
  else if (origin.x == 1 && origin.y == 0)
  {
    en->pos_offset = v2f(-en->width/2.0f, 0.0f);
  }
  else if (origin.x == -1 && origin.y == 1)
  {
    en->pos_offset = v2f(en->width/2.0f, -en->height/2.0f);
  }
}

void set_entity_target(Entity *en, EntityRef target)
{
  Entity *target_entity = entity_from_ref(target);
  
  if (target_entity == NULL) return;

  Vec2F target_pos = target_entity->pos;

  if (distance_2f(en->pos, target_pos) <= en->view_dist)
  {
    f32 dist_x = target_pos.x - en->pos.x;
    f32 dist_y = target_pos.y - en->pos.y;
    en->target_angle = atan2(dist_x, dist_y);
    en->has_target = TRUE;
  }
  else
  {
    en->target_pos = V2F_ZERO;
    en->has_target = FALSE;
  }
}

// @OtherEntity ================================================================================

void sort_entities_by_z_index(Game *game)
{
  EntityList entities = game->entities;

  for (u64 i = 0; i < entities.count; i++)
  {
    
  }
}

inline
bool is_entity_valid(Entity *en)
{
  return (en != NULL && en->type != EntityType_Nil);
}

inline
void resolve_entity_collision(Entity *a, Entity *b)
{
  if (a->pos.y + a->vel.y - a->height/2.0f <= b->pos.y)
  {
    a->pos.y = b->pos.y + a->height/2.0f;
    a->vel.y = 0.0f;
    a->dv.y = 0.0f;
    a->grounded = TRUE;
  }
  else
  {
    a->grounded = FALSE;
  }
}

void wrap_entity_at_edges(Entity *en)
{
  if (en->pos.x + en->width <= 0.0f)
  {
    en->pos.x = W_WIDTH;
  }
  else if (en->pos.x >= W_WIDTH)
  {
    en->pos.x = -(en->width);
  }
  else if (en->pos.y + en->height <= 0.0f)
  {
    en->pos.y = W_HEIGHT;
  }
  else if (en->pos.y >= W_HEIGHT)
  {
    en->pos.y = -(en->height);
  }
}

void damage_entity(Entity *en, u8 damage)
{
  en->curr_health -= damage;

  if (en->curr_health <= 0)
  {
    en->curr_health = 0;
    en->active = FALSE;
    en->visible = FALSE;
    printf("Player ded\n");
  }
}

// @EntityRef ==================================================================================

inline
EntityRef ref_from_entity(Entity *en)
{
  return (EntityRef) {en, en->id};
}

inline
Entity *entity_from_ref(EntityRef ref)
{
  Entity *result = GLOBAL->nil_entity;

  if (ref.ptr != NULL && ref.ptr->id == ref.id)
  {
    result = ref.ptr;
  }

  return result;
}

// @EntityList =================================================================================

Entity *create_entity(Game *game)
{
  EntityList *list = &game->entities;
  Entity *new_entity = list->first_free;

  if (new_entity == NULL)
  {
    new_entity = arena_alloc(&game->entity_arena, sizeof (Entity));
    clear_entity(new_entity);

    if (list->head == NULL)
    {
      list->head = new_entity;
    }

    if (list->tail != NULL)
    {
      list->tail->next = new_entity;
    }

    new_entity->next = NULL;
    list->tail = new_entity;
    list->count++;
  }
  else
  {
    list->first_free = list->first_free->next_free;
  }

  new_entity->id = random_u64(1, UINT64_MAX);

  return new_entity;
}

void destroy_entity(Game *game, Entity *en)
{
  EntityList *list = &game->entities;
  clear_entity(en);
  en->next_free = list->first_free;
  list->first_free = en;
}

Entity *get_entity_by_id(Game *game, u64 id)
{
  Entity *result = GLOBAL->nil_entity;
  
  for (Entity *en = game->entities.head; en != NULL; en = en->next)
  {
    if (en->id == id)
    {
      result = en;
      break;
    }
  }

  return result;
}

Entity *get_nearest_entity_of_type(Game *game, Vec2F pos, EntityType type)
{
  Entity *result = GLOBAL->nil_entity;
  f32 min_dist = FLT_MAX;

  for (Entity *en = game->entities.head; en; en = en->next)
  {
    if (en->type == type)
    {
      f32 dist = distance_2f(en->pos, pos);

      if (dist < min_dist)
      {
        result = en;
        min_dist = dist;
      }
    }
  }

  return result;
}

// @EntityTree =================================================================================

void set_entity_parent(Entity *en, Entity *parent)
{
  en->parent = ref_from_entity(parent);

  if (parent->first_child == NULL)
  {
    parent->first_child = en;
  }
  else
  {
    parent->last_child->next_child = en;
  }

  parent->last_child = en;
}

void add_entity_child(Entity *en, Entity *child)
{
  en->last_child->next_child = child;
  en->last_child = child;
  child->parent = ref_from_entity(en);
}

// NOTE: NEED TO TEST!
void remove_entity_child(Entity *en, u64 id)
{
  Entity *prev = NULL;

  for (Entity *curr = en->first_child; curr; curr = curr->next_child)
  {
    if (curr->id == id)
    {
      zero(curr->parent);
      prev->next_child = curr->next_child;
      break;
    }

    prev = curr;
  }
}

Entity *get_entity_child_at_index(Entity *en, u8 index)
{
  Entity *result = GLOBAL->nil_entity;

  u8 i = 0;
  for (Entity *curr = en->first_child; curr; curr = curr->next_child)
  {
    if (i == index)
    {
      result = curr;
      break;
    }

    i++;
  }

  return result;
}

Entity *get_entity_child_of_id(Entity *en, u64 id)
{
  Entity *result = {0};

  for (Entity *curr = en->first_child; curr; curr = curr->next_child)
  {
    if (curr->id == id)
    {
      result = curr;
      break;
    }
  }

  return result;
}

Entity *get_entity_child_of_type(Entity *en, EntityType type)
{
  Entity *result = {0};

  for (Entity *curr = en->first_child; curr; curr = curr->next_child)
  {
    if (curr->type == type)
    {
      result = curr;
      break;
    }
  }

  return result;
}

// @Timer ======================================================================================

static
void init_timers(Entity *en)
{
  en->timers[TIMER_COMBAT] = (Timer)
  {
    .max_duration = 1.0f,
    .curr_duration = 1.0f,
    .should_tick = TRUE,
    .ticking = FALSE,
    .timeout = FALSE,
    .should_loop = FALSE
  };

  en->timers[TIMER_HEALTH] = (Timer)
  {
    .max_duration = 1.0f,
    .curr_duration = 1.0f,
    .should_tick = TRUE,
    .ticking = FALSE,
    .timeout = FALSE,
    .should_loop = FALSE
  };

  en->timers[TIMER_KILL] = (Timer)
  {
    .max_duration = 5.0f,
    .curr_duration = 5.0f,
    .should_tick = TRUE,
    .ticking = FALSE,
    .timeout = FALSE,
    .should_loop = FALSE
  };
}

static inline
Timer *get_timer(Entity *en, u8 index)
{
  return &en->timers[index];
}

static
bool tick_timer(Timer *timer, f64 dt)
{
  if (timer->ticking)
  {
    timer->curr_duration -= dt;

    if (timer->curr_duration <= 0.0f)
    {
      timer->timeout = TRUE;
      timer->ticking = FALSE;
      timer->should_tick = timer->should_loop;
    }
  }
  else
  {
    timer->curr_duration = timer->max_duration;
    timer->timeout = FALSE;
    timer->ticking = TRUE;
  }

  return timer->timeout;
}
