#include <stdio.h>

#include "base/base_common.h"
#include "base/base_math.h"
#include "base/base_random.h"

#include "input.h"
#include "event.h"
#include "game.h"
#include "entity.h"
#include "physx/physx.h"

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
  en->size = v2f(10, 10);
  en->active = TRUE;
  en->visible = TRUE;
  en->draw_type = DrawType_Rectangle;
  en->color = D_WHITE;

  init_timers(en);

  switch (type)
  {
    case EntityType_General:
    {
      en->draw_type = DrawType_Sprite;
    }
    break;
    case EntityType_Player:
    {
      en->scale = v2f(8.0f, 8.0f);
      en->draw_type = DrawType_Sprite;
      en->texture = D_TEXTURE_PLAYER;
      en->move_type = MoveType_Walking;
      en->combat_type = CombatType_Ranged;
      en->props = PLAYER_PROPS;
      en->pos = v2f(WIDTH / 2.0f, HEIGHT / 2.0f);
      en->speed = PLAYER_SPEED;
      en->col.vertex_count = 4;

      Timer *timer = get_timer(en, TIMER_COMBAT);
      timer->max_duration = 0.25f;
      timer->curr_duration = 0.0f;
      timer->should_tick = FALSE;
    }
    break;
    case EntityType_EnemyShip:
    {
      en->scale = v2f(1.5f, 1.5f);
      en->move_type = MoveType_Flying;
      en->combat_type = CombatType_Ranged;
      en->props = ENEMY_PROPS;
      en->pos = v2f(random_u32(0, WIDTH), random_u32(0, HEIGHT));
      en->speed = 100.0f;
      en->color = D_RED;
      en->view_dist = 350;

      Timer *timer = get_timer(en, TIMER_COMBAT);
      timer->should_loop = TRUE;
    }
    break;
    case EntityType_Equipped:
    {
      en->draw_type = DrawType_Sprite;
      en->texture = D_TEXTURE_GUN;
      en->props = EntityProp_Rendered | EntityProp_Equipped;
    }
    break;
    case EntityType_Laser:
    {
      en->scale = v2f(6.0f, 0.3f);
      en->move_type = MoveType_Projectile;
      en->combat_type = CombatType_Melee;
      en->props = LASER_PROPS;
    }
    break;
    case EntityType_Wall:
    {
      en->scale = v2f(2.0f, 2.0f);
      en->props = EntityProp_Rendered | EntityProp_Collides;
      en->pos = v2f(WIDTH/2.0f, HEIGHT/2.0f + 100.0f);
      en->color = D_GRAY;
      en->col.vertex_count = 4;
      
      update_entity_collider(en);
    }
    break;
    case EntityType_DebugLine:
    {
      en->props = EntityProp_Rendered;
      en->color = D_YELLOW;
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

// NOTE: probably a mess
void update_entity_collider(Entity *en)
{
  Collider2D *col = &en->col;
  Vec2F size = size_from_entity(en);

  switch (col->type)
  {
    case ColliderType_Polygon:
    {
      switch (col->vertex_count)
      {
        case 3:
        {
          col->vertices[0].x = en->pos.x;
          col->vertices[0].y = en->pos.y + size.height/2.0f;
          col->vertices[1].x = en->pos.x + size.width/2.0f;
          col->vertices[1].y = en->pos.y + size.height/2.0f;
          col->vertices[2].x = en->pos.x + size.width/2.0f;
          col->vertices[2].y = en->pos.y - size.height/2.0f;

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
          col->vertices[0].x = en->pos.x - size.width/2.0f;
          col->vertices[0].y = en->pos.y + size.height/2.0f;
          col->vertices[1].x = en->pos.x + size.width/2.0f;
          col->vertices[1].y = en->pos.y + size.height/2.0f;
          col->vertices[2].x = en->pos.x + size.width/2.0f;
          col->vertices[2].y = en->pos.y - size.height/2.0f;
          col->vertices[3].x = en->pos.x - size.width/2.0f;
          col->vertices[3].y = en->pos.y - size.height/2.0f;

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

// TODO: Figure out how to rotate children relative to parent. Maybe use the parent as the pivot point?
void update_entity_xform(Game *game, Entity *en)
{
  Mat3x3F xform = m3x3f(1.0f);
  Entity *parent = entity_from_ref(en->parent);

  xform = mul_3x3f(scale_3x3f(en->scale.x, en->scale.y), xform);

  // Adjust position offset based on origin
  {
    Vec2F size = mul_2f(en->size, en->scale);
    Vec2F offset = v2f(size.width/2.0f * -en->origin.x, size.height/2.0f * -en->origin.y);
    xform = mul_3x3f(translate_3x3f(offset.x, offset.y), xform);
  }

  if (en->flip_x) { xform = mul_3x3f(scale_3x3f(-1.0f, 1.0f), xform); }
  if (en->flip_y) { xform = mul_3x3f(scale_3x3f(1.0f, -1.0f), xform); }

  xform = mul_3x3f(rotate_3x3f(en->rot * RADIANS), xform);

  // Adjust relative translation based on cumulative parent scale
  {
    Vec2F scale = v2f(1.0f, 1.0f);
    for (Entity *p = parent; is_entity_valid(p); p = entity_from_ref(p->parent))
    {
      scale = mul_2f(scale, p->scale);
    }

    xform = mul_3x3f(translate_3x3f(en->pos.x/scale.x, en->pos.y/scale.y), xform);
  }

  if (is_entity_valid(parent))
  {
    xform = mul_3x3f(parent->model_mat, xform);
  }

  en->model_mat = xform;

  xform = mul_3x3f(game->camera, xform);
  xform = mul_3x3f(game->projection, xform);

  en->xform = xform;
}

void update_controlled_entity_movement(Game *game, Entity *en)
{
  f32 dt = game->dt;

  switch (en->move_type)
  {
    case MoveType_Walking:
    {
      if (is_key_pressed(KEY_A) && !is_key_pressed(KEY_D))
      {
        en->new_vel.x = lerp_1f(en->new_vel.x, -en->speed * dt, PLAYER_ACC * dt);
        en->input_dir.x = -1.0f;
        en->flip_x = TRUE;
      }

      if (is_key_pressed(KEY_D) && !is_key_pressed(KEY_A))
      {
        en->new_vel.x = lerp_1f(en->new_vel.x, en->speed * dt, PLAYER_ACC * dt);
        en->input_dir.x = 1.0f;
        en->flip_x = FALSE;
      }

      if (is_key_pressed(KEY_A) && is_key_pressed(KEY_D))
      {
        en->new_vel.x = lerp_1f(en->new_vel.x, 0.0f, PLAYER_FRIC * 2.0f * dt);
        en->new_vel.x = to_zero(en->new_vel.x, 1.0f);
      }
      
      if (!is_key_pressed(KEY_A) && !is_key_pressed(KEY_D))
      {
        en->new_vel.x = lerp_1f(en->new_vel.x, 0.0f, PLAYER_FRIC * dt);
        en->new_vel.x = to_zero(en->new_vel.x, 1.0f);
      }

      // GRAVITY
      if (!en->grounded)
      {
        en->new_vel.y -= GRAVITY * dt;
      }

      // JUMPING
      if (is_key_pressed(KEY_W) && en->grounded)
      {
        en->new_vel.y += PLAYER_JUMP_VEL;
        en->grounded = FALSE;
      }

      en->vel = en->new_vel;
    }
    break;
    case MoveType_Rocket:
    {
      
    }
    default: break;
  }

  en->pos = add_2f(en->pos, en->vel);

  // printf("VelX: %.02f p/f\n", en->vel.x);
  // printf("VelY: %.02f p/f\n", en->vel.y);
}

void update_autonomous_entity_movement(Game *game, Entity *en)
{
  f64 dt = game->dt;

  if (en->move_type == MoveType_Flying)
  {
    if (en->has_target)
    {
      en->input_dir.x = cos_1f(en->target_angle);
      en->input_dir.y = sin_1f(en->target_angle);

      en->rot = en->target_angle * DEGREES;
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
      kill_entity(game, .entity = en);
    }

    en->vel.x = cos_1f(en->rot * RADIANS) * en->speed * dt;
    en->vel.y = sin_1f(en->rot * RADIANS) * en->speed * dt;
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
    Vec2F spawn_pos = v2f(en->pos.x, en->pos.y + 10.0f);
    f32 spawn_rot = 0.0f;
    Entity *gun = get_entity_child_of_type(en, EntityType_Equipped);
    if (is_entity_valid(gun))
    {
      spawn_rot = gun->rot ? !gun->flip_x : gun->rot + 180;
    }

    Entity *laser = spawn_entity(game, EntityType_Laser, .pos=spawn_pos, .color=D_BLUE);
    laser->rot = spawn_rot;
    laser->speed = 700.0f;

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
      Vec2F spawn_pos = v2f(en->pos.x, en->pos.y);
      Entity *laser = spawn_entity(game, EntityType_Laser, .pos=spawn_pos, .color=D_GREEN);
      laser->rot = en->rot;
      laser->speed = 700.0f;
    }
  }
}

void update_equipped_entity(Game *game, Entity *en)
{
  Vec2F entity_pos = pos_from_entity(en);
  Vec2F mouse_pos = screen_to_world(get_mouse_pos());
  Vec2F diff = v2f(mouse_pos.x - entity_pos.x, mouse_pos.y - entity_pos.y);
  f32 angle = atan_2f(diff) * DEGREES;

  if (!en->parent.ptr->flip_x)
  {
    angle = clamp(angle, -90, 90);
  }
  else
  {
    if (angle < 0)
    {
      angle = angle + 360;
    }

    angle = -clamp(angle, 90, 270) + 180;
  }

  en->rot = angle;
}

// @OtherEntity ================================================================================

Vec2F pos_from_entity(Entity *en)
{
  Vec2F result = en->pos;

  Entity *parent = entity_from_ref(en->parent);
  while (is_entity_valid(parent))
  {
    result.x += parent->pos.x;
    result.y += parent->pos.y;
    parent = entity_from_ref(parent->parent);
  }

  return result;
}

f32 rot_from_entity(Entity *en)
{
  f32 result = en->rot;

  Entity *parent = entity_from_ref(en->parent);
  while (is_entity_valid(parent))
  {
    result += parent->rot;
    parent = entity_from_ref(parent->parent);
  }

  return result;
}

Vec2F scale_from_entity(Entity *en)
{
  Vec2F result = en->scale;

  Entity *parent = entity_from_ref(en->parent);
  while (is_entity_valid(parent))
  {
    result.x *= parent->scale.x;
    result.y *= parent->scale.y;
    parent = entity_from_ref(parent->parent);
  }

  return result;
}

Vec2F size_from_entity(Entity *en)
{
  Vec2F result = en->size;
  result.width *= en->scale.x;
  result.height *= en->scale.y;

  Entity *parent = entity_from_ref(en->parent);
  while (is_entity_valid(parent))
  {
    result.width *= parent->scale.x;
    result.height *= parent->scale.y;
    parent = entity_from_ref(parent->parent);
  }

  return result;
}

void set_entity_target(Entity *en, EntityRef target)
{
  Entity *target_entity = entity_from_ref(target);
  
  if (!is_entity_valid(target_entity)) return;

  Vec2F target_pos = pos_from_entity(target_entity);

  if (distance_2f(en->pos, target_pos) <= en->view_dist)
  {
    Vec2F diff = v2f(target_pos.x - en->pos.x, target_pos.y - en->pos.y);
    en->target_angle = atan_2f(diff);
    en->has_target = TRUE;
  }
  else
  {
    en->target_pos = V2F_ZERO;
    en->has_target = FALSE;
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
  Vec2F size = size_from_entity(a);

  if (a->pos.y + a->vel.y - size.height/2.0f <= b->pos.y)
  {
    a->pos.y = b->pos.y + size.height/2.0f;
    a->vel.y = 0.0f;
    a->new_vel.y = 0.0f;
    a->grounded = TRUE;
  }
  else
  {
    a->grounded = FALSE;
  }
}

void wrap_entity_at_edges(Entity *en)
{
  Vec2F size = size_from_entity(en);
  
  if (en->pos.x + size.width <= 0.0f)
  {
    en->pos.x = WIDTH;
  }
  else if (en->pos.x >= WIDTH)
  {
    en->pos.x = -(size.width);
  }
  else if (en->pos.y + size.height <= 0.0f)
  {
    en->pos.y = HEIGHT;
  }
  else if (en->pos.y >= HEIGHT)
  {
    en->pos.y = -(size.height);
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

Entity *get_entity_of_id(Game *game, u64 id)
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
