#include <stdio.h>

#include "base/base_inc.h"

#include "draw/draw.h"
#include "input/input.h"
#include "phys/physics.h"
#include "entity.h"
#include "global.h"
#include "game.h"

#define EN_IN_ENTITIES Entity *en = game->entities.head; en; en = en->next

extern Global *GLOBAL;

// @Events //////////////////////////////////////////////////////////////////////////

void push_event(Game *game, EventType type, EventDesc desc)
{
  EventQueue *queue = &game->event_queue;
  Event *new_event = queue->first_free;

  if (new_event == NULL)
  {
    new_event = arena_alloc(&game->frame_arena, sizeof (Event));
    zero(*new_event, Event);
  }
  else
  {
    queue->first_free = queue->first_free->next_free;
  }

  if (queue->front == NULL)
  {
    queue->front = new_event;
  }

  if (queue->back != NULL)
  {
    queue->back->next = new_event;
  }

  new_event->type = type;
  new_event->desc = desc;
  new_event->next = NULL;

  queue->back = new_event;
  queue->count++;
}

void pop_event(Game *game)
{
  assert(game->event_queue.count > 0);

  EventQueue *queue = &game->event_queue;
  Event *next = queue->front->next;
  zero(*queue->front, Event);
  
  if (queue->count == 2)
  {
    queue->back = next;
  }

  queue->front = next;
  queue->count--;
}

Event *peek_event(Game *game)
{
  return game->event_queue.front;
}

// @Main ////////////////////////////////////////////////////////////////////////////

void init_game(Game *game)
{
  game->camera = m3x3f(1.0f);

  // Starting entities ----------------
  {
    Entity *ground = create_entity(game, EntityType_Wall);
    ground->pos = v2f(0.0f, 0.0f);
    ground->scale = v2f(512.0f, 24.0f);
    ground->tint = v4f(0.7f, 0.6f, 0.4f, 1.0f);

    Entity *player = create_entity(game, EntityType_Player);
    player->id = 1;
    player->pos = v2f(WIDTH/2.0f, HEIGHT/2.0f + 50.0f);

    Entity *gun = create_entity(game, EntityType_Equipped);
    attach_entity_child(player, gun);
    gun->pos = v2f(48.0f, 0.0f);

    Entity *shot_point = create_entity(game, EntityType_Debug);
    attach_entity_child(gun, shot_point);
    shot_point->pos = v2f(24.0f, 0.0f);
    shot_point->scale = v2f(.1, .1);

    Entity *zombie = create_entity(game, EntityType_ZombieWalker);
    zombie->pos = v2f(WIDTH - 300.0f, HEIGHT/2.0f + 50.0f);
  }
}

void update_game(Game *game)
{
  game->is_sim_started = TRUE;

  Entity *player = get_entity_of_id(game, 1);
  Vec2F mouse_pos = screen_to_world(get_mouse_pos());

  f64 t = game->t;
  f64 dt = game->dt;

  // Update entity spawning and dying ----------------
  for (EN_IN_ENTITIES)
  {
    if (en->marked_for_spawn)
    {
      en->marked_for_spawn = FALSE;
      en->is_active = TRUE;
      entity_add_prop(en, EntityProp_Renders);
    }

    if (en->marked_for_death)
    {
      en->marked_for_death = FALSE;
      free_entity(game, en);
    }
  }

  // Update entity position ----------------
  for (EN_IN_ENTITIES)
  {
    if (!en->is_active) continue;
    
    // Apply gravity ----------------
    if (entity_has_prop(en, EntityProp_AffectedByGravity))
    {
      en->new_vel.y -= GRAVITY * dt;
    }

    // Update entitiy movement ----------------
    if (entity_has_prop(en, EntityProp_Moves))
    {
      if (entity_has_prop(en, EntityProp_Controlled))
      {
        entity_look_at(en, mouse_pos);

        if (is_key_pressed(KEY_A) && !is_key_pressed(KEY_D))
        {
          en->new_vel.x = lerp_1f(en->new_vel.x, -en->speed * dt, PLAYER_ACC * dt);
        }

        if (is_key_pressed(KEY_D) && !is_key_pressed(KEY_A))
        {
          en->new_vel.x = lerp_1f(en->new_vel.x, en->speed * dt, PLAYER_ACC * dt);
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

        if (is_key_pressed(KEY_W) && en->grounded)
        {
          en->new_vel.y += PLAYER_JUMP_VEL;
          en->grounded = FALSE;
        }
      }
      else
      {
        Vec2F player_pos = pos_from_entity(player);
        entity_look_at(en, player_pos);

        switch (en->move_type)
        {
          case MoveType_Grounded:
          {
            // Enemy grounded movement goes here
          }
          break;
          case MoveType_Flying:
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
              en->new_vel.x += PLAYER_ACC * dir(en->input_dir.x) * dt;
              f32 bound = en->speed * abs(en->input_dir.x) * dt;
              en->new_vel.x = clamp(en->vel.x, -bound, bound);
            }
            else
            {
              en->new_vel.x = lerp_1f(en->vel.x, 0.0f, PLAYER_FRIC * dt);
              en->new_vel.x = to_zero(en->vel.x, 0.1f);
            }

            // Y Acceleration
            if (en->input_dir.y != 0.0f)
            {
              en->new_vel.y += PLAYER_ACC * dir(en->input_dir.y) * dt;
              f32 bound = en->speed * abs(en->input_dir.y) * dt;
              en->new_vel.y = clamp(en->vel.y, -bound, bound);
            }
            else
            {
              en->new_vel.y = lerp_1f(en->vel.y, 0.0f, PLAYER_FRIC * dt);
              en->new_vel.y = to_zero(en->vel.y, 0.1f);
            }
          }
          case MoveType_Projectile:
          {
            if (!en->kill_timer.is_ticking)
            {
              en->kill_timer.end_time = t + BULLET_KILL_TIME;
              en->kill_timer.is_ticking = TRUE;
            }

            if (is_timer_over(en->kill_timer, t))
            {
              kill_entity(game, .entity=en);
              en->kill_timer.is_ticking = FALSE;
            }

            en->new_vel.x = cos_1f(en->rot * RADIANS) * en->speed * dt;
            en->new_vel.y = sin_1f(en->rot * RADIANS) * en->speed * dt;
          }
          break;
          default: break;
        }
      }

      en->vel = en->new_vel;
      en->pos = add_2f(en->pos, en->vel);

      // Handle entity wrapping ----------------
      if (entity_has_prop(en, EntityProp_WrapsAtEdges))
      {
        Vec2F dim = dim_from_entity(en);
        if (en->pos.x + dim.width <= 0.0f)
        {
          en->pos.x = WIDTH;
        }
        else if (en->pos.x >= WIDTH)
        {
          en->pos.x = -(dim.width);
        }
      }
    }

    // Update entity xform ----------------
    {
      Mat3x3F xform = m3x3f(1.0f);
      Entity *parent = entity_from_ref(en->parent);

      // Scale
      xform = mul_3x3f(scale_3x3f(en->scale.x, en->scale.y), xform);
      if (en->flip_x) xform = mul_3x3f(scale_3x3f(-1.0f, 1.0f), xform);
      if (en->flip_y) xform = mul_3x3f(scale_3x3f(1.0f, -1.0f), xform);

      // Rotate
      xform = mul_3x3f(rotate_3x3f(en->rot * RADIANS), xform);

      // Translate
      {
        Vec2F scale = v2f(1.0f, 1.0f);
        for (Entity *p = parent; is_entity_valid(p); p = entity_from_ref(p->parent))
        {
          scale = mul_2f(scale, p->scale);
        }

        xform = mul_3x3f(translate_3x3f(en->pos.x/scale.x, en->pos.y/scale.y), xform);
      }

      en->model_mat = xform;

      // Move to world space
      {
        Mat3x3F model = m3x3f(1.0f);
        for (Entity *p = parent; is_entity_valid(p); p = entity_from_ref(p->parent))
        {
          model = mul_3x3f(p->model_mat, model);
        }
        
        xform = mul_3x3f(model, xform);
      }

      xform = mul_3x3f(game->camera, xform);
      xform = mul_3x3f(GLOBAL->renderer.projection, xform);

      en->xform = xform;
    }
  }
  
  // Update entity collision ----------------
  for (EN_IN_ENTITIES)
  {
    if (!en->is_active) continue;

    if (entity_has_prop(en, EntityProp_Collides))
    {
      Vec2F dim = dim_from_entity(en);

      // Update entity colliders ----------------
      {
        Vec2F pos = pos_from_entity(en);

        if (en->body_col.type == P_ColliderType_Rect)
        {
          en->body_col.pos = add_2f(pos_bl_from_entity(en), en->body_col.offset);
        }
        else
        {
          en->body_col.pos = add_2f(pos, en->body_col.offset);
        }
      }

      if (en->type == EntityType_Player || en->type == EntityType_ZombieWalker)
      {
        f32 ground_y = 0.0f;
        for (Entity *other = game->entities.head; other; other = other->next)
        {
          if (other->type == EntityType_Wall)
          {
            ground_y = pos_from_entity(other).y + dim_from_entity(other).height;
            break;
          }
        }

        P_CollisionParams params = {.collider=en->body_col, .vel=en->vel};
        if (p_rect_y_range_intersect(params, v2f(-3000.0f, 3000.0f), ground_y))
        {
          en->pos.y = ground_y + dim.height / 2.0f - en->body_col.offset.y;
          en->vel.y = 0.0f;
          en->new_vel.y = 0.0f;
          en->grounded = TRUE;
        }
        else
        {
          en->grounded = FALSE;
        }
      }

      if (en->type == EntityType_Bullet)
      {
        for (Entity *other = game->entities.head; other; other = other->next)
        {
          if (other->type == EntityType_ZombieWalker)
          {
            P_CollisionParams rect_params = {.collider=other->body_col, .vel=other->vel};
            P_CollisionParams circle_params = {.collider=en->body_col, .vel=en->vel};
            if (p_rect_circle_intersect(rect_params, circle_params))
            {
              kill_entity(game, .entity=en);
            }
          }
        }
      }
    }
  }

  // Update entity combat ----------------
  for (EN_IN_ENTITIES)
  {
    if (!en->is_active) continue;

    if (entity_has_prop(en, EntityProp_Fights))
    {
      if (entity_has_prop(en, EntityProp_Controlled))
      {
        if (!en->attack_timer.is_ticking)
        {
          en->attack_timer.end_time = t + en->attack_timer.duration;
          en->attack_timer.is_ticking = TRUE;
        }

        // Shoot weapon if can shoot
        if (is_key_pressed(KEY_MOUSE_1) && is_timer_over(en->attack_timer, t))
        {
          Entity *gun = get_entity_child_at(en, 0);
          Entity *shot_point = get_entity_child_at(gun, 0);
          Vec2F spawn_pos = pos_from_entity(shot_point);
          f32 spawn_rot = en->flip_x ? -gun->rot + 180 : gun->rot;

          Entity *bullet = spawn_entity(game, EntityType_Bullet, .pos=spawn_pos);
          bullet->rot = spawn_rot;
          bullet->speed = PROJ_SPEED;
          entity_rem_prop(bullet, EntityProp_Renders);

          ParticleDesc desc = {
            .emmission_type = ParticleEmmissionType_Burst,
            .color = v4f(0.2f, 0.2f, 0.17f, 1.0f),
            .scale = v2f(5, 5),
            .count = 3,
            .duration = 1.0f,
            .speed = 0.7f,
            .spread = 180.0f,
          };
          spawn_entity(game, EntityType_ParticleGroup, .pos=spawn_pos, .particle_desc=desc);

          en->attack_timer.is_ticking = FALSE;
        }
      }
      else
      {
        if (is_entity_valid(player) && player->is_active)
        {
          set_entity_target(en, ref_from_entity(player));
        }
        else
        {
          en->has_target = FALSE;
        }

        if (en->has_target)
        {
          switch (en->combat_type)
          {
            case CombatType_Ranged:
            {
              if (!en->attack_timer.is_ticking)
              {
                en->attack_timer.end_time = t + en->attack_timer.duration;
                en->attack_timer.is_ticking = TRUE;
              }

              // Shoot weapon if can shoot
              if (is_timer_over(en->attack_timer, t))
              {
                Vec2F spawn_pos = v2f(en->pos.x, en->pos.y);
                Entity *laser = spawn_entity(game, EntityType_Bullet, .pos=spawn_pos);
                laser->tint = D_GREEN;
                laser->rot = en->rot;
                laser->speed = 700.0f;
              }
            }
            break;
            default: break;
          }
        }
      }
    }
  }

  // Update equipped entities ----------------
  for (EN_IN_ENTITIES)
  {
    if (!en->is_active) continue;

    if (en->props & EntityProp_Equipped)
    {
      Vec2F entity_pos = pos_from_entity(en);
      f32 angle = atan_2f(sub_2f(mouse_pos, entity_pos)) * DEGREES;

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
  }

  // Update particle groups ----------------
  for (EN_IN_ENTITIES)
  {
    if (en->type == EntityType_ParticleGroup)
    {
      ParticleDesc desc = en->particle_desc;
      switch (desc.emmission_type)
      {
        case ParticleEmmissionType_Burst:
        {
          for (i32 i = 0; i < desc.count; i++)
          {
            Particle *particle = &en->particles[i];
            Vec2F vel = {
              sin_1f(particle->rot) * desc.speed,
              cos_1f(particle->rot) * desc.speed,
            };
            particle->pos = add_2f(particle->pos, vel);
          }

          if (!en->particle_timer.is_ticking)
          {
            en->particle_timer.end_time = t + desc.duration;
            en->particle_timer.is_ticking = TRUE;
          }

          if (t >= en->particle_timer.end_time)
          {
            en->particle_timer.is_ticking = FALSE;
            kill_entity(game, .entity=en);
          }
        }
        case ParticleEmmissionType_Linear:
        {
          // ...
        }
        break;
      }
    }
  }

  // Developer tools ----------------
  {
    // Toggle debug
    if (is_key_just_pressed(KEY_TAB))
    {
      GLOBAL->debug = !GLOBAL->debug; 
    }

    for (EN_IN_ENTITIES)
    {
      if (!en->is_active) continue;
      
      if (en->type == EntityType_Debug)
      {
        if (GLOBAL->debug)
        {
          entity_add_prop(en, EntityProp_Renders);
        }
        else
        {
          entity_rem_prop(en, EntityProp_Renders);
        }
      }
    }

    // Kill player
    if (is_key_just_pressed(KEY_BACKSPACE))
    { 
      kill_entity(game, .entity=player);
    }

    // Spawn particles
    if (is_key_just_pressed(KEY_0))
    {
      ParticleDesc desc = {
        .emmission_type = ParticleEmmissionType_Burst,
        .color = D_RED,
        .scale = v2f(10, 10),
        .count = 12,
        .duration = 1.0f,
        .speed = 5.0f,
        .spread = 180.0f,
      };

      spawn_entity(game, EntityType_ParticleGroup, .pos=player->pos, .particle_desc=desc);
    }
  }
}

void handle_game_events(Game *game)
{
  for (Event *e = peek_event(game); e != NULL; pop_event(game))
  {
    switch (e->type)
    {
      case EventType_EntityKilled:
      {
        switch (e->desc.type)
        {
          case EntityType_Player:
          {
            // game_over = TRUE
          }
          break;
        }
      }
      default: 
      {
        fprintf(stderr, "ERROR: Failed to process event. Invalid type!");
        assert(FALSE);
      }
    }

    pop_event(game);
  }
}

void draw_game(Game *game, Game *prev)
{
  if (!game->is_sim_started)
  {
    d_clear_frame(V4F_ZERO);
    return;
  }
  
  d_clear_frame(v4f(0.34f, 0.44f, 0.47f, 1.0f));

  // Batch sprites ----------------
  for (EN_IN_ENTITIES)
  {
    if (en->draw_type == DrawType_Sprite && entity_has_prop(en, EntityProp_Renders))
    {
      d_draw_sprite_x(en->xform, en->tint, en->texture);
    }
  }
  
  // Batch primitives ----------------
  for (EN_IN_ENTITIES)
  {
    if (entity_has_prop(en, EntityProp_Renders))
    {
      if (en->draw_type == DrawType_Primitive)
      {
        d_draw_rectangle_x(en->xform, en->tint);
      }

      if (en->type == EntityType_ParticleGroup)
      {
        for (i32 i = 0; i < en->particle_desc.count; i++)
        {
          Vec2F pos = en->particles[i].pos;
          Vec2F dim = en->particle_desc.scale;
          f32 rot = en->particles[i].rot;
          d_draw_rectangle(pos, dim, rot, en->particle_desc.color);
        }
      }

      // Render colliders ----------------
      if (GLOBAL->debug)
      {
        if (entity_has_prop(en, EntityProp_Collides))
        {
          Vec2F pos = add_2f(en->body_col.pos, en->body_col.offset);
          Vec2F dim = en->body_col.dim;
          d_draw_rectangle(pos, dim, 0, v4f(0, 1, 0, 0.35f));
        }
      }
    }
  }

  r_flush(&GLOBAL->renderer);
}

inline
bool game_should_quit(Game *game)
{
  return game->should_quit || is_key_pressed(KEY_ESCAPE);
}

inline
void copy_game_state(Game *game, Game *prev)
{
  // Note(dg): is this a bug?
  EntityList *old_list = &game->entities;
  EntityList *new_list = &prev->entities;

  new_list->head = NULL;
  new_list->count = old_list->count;

  Entity *old_en = old_list->head;
  for (i32 i = 0; i < old_list->count; i++)
  {
    Entity *new_en = alloc_entity(prev);
    *new_en = *old_en;
    old_en = old_en->next;
  }
}
