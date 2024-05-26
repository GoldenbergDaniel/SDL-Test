#include <stdio.h>
#include <stdlib.h>

#include "base/base_inc.h"

#include "phys/phys.h"
#include "draw.h"
#include "input.h"
#include "entity.h"
#include "globals.h"
#include "prefabs.h"
#include "game.h"

#define EN_IN_ENTITIES Entity *en = game->entities.head; en; en = en->next

extern Globals *GLOBAL;
extern Prefabs *PREFAB;

#define GROUND_Y 30 * 5

// @Main /////////////////////////////////////////////////////////////////////////////////

void init_game(Game *game)
{
  game->camera = m3x3f(1.0f);

  // Starting entities ----------------
  {
    Entity *player = create_entity(game, EntityType_Player);
    player->sp = SP_Player;
    player->pos = v2f(get_width()/2.0f, get_height()/2.0f);

    Entity *gun = create_entity(game, EntityType_Equipped);
    attach_entity_child(player, gun);
    gun->sp = SP_Gun;
    gun->pos = v2f(35.0f, 5.0f);

    Entity *shot_point = create_entity(game, EntityType_Debug);
    attach_entity_child(gun, shot_point);
    shot_point->pos = v2f(24.0f, 2.0f);
  }
}

void update_game(Game *game)
{
  Entity *player = get_entity_of_sp(game, SP_Player);
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
    if (entity_has_prop(en, EntityProp_AffectedByGravity) &&
       !entity_has_prop(en, EntityProp_Grounded))
    {
      en->new_vel.y -= GRAVITY * dt * dt;
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

        if (is_key_pressed(KEY_W) && entity_has_prop(en, EntityProp_Grounded))
        {
          en->new_vel.y = PLAYER_JUMP_VEL * dt;
          entity_rem_prop(en, EntityProp_Grounded);
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
            static f32 zombie_speed = 50.0f;
            f32 dist_from_player = distance_2f(pos_from_entity(en), player_pos);

            if (entity_has_prop(en, EntityProp_Grounded) && dist_from_player >= 50.0f)
            {
              en->new_vel.x = en->flip_x ? -zombie_speed * dt : zombie_speed * dt;
            }
            else
            {
              en->new_vel.x = 0;
            }
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

            if (timeout(en->kill_timer, t))
            {
              kill_entity(en);
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
        f32 left = screen_to_world(v2f(GLOBAL->viewport.x, 0)).x;
        f32 right = screen_to_world(v2f(GLOBAL->viewport.z, 0)).x;

        if (en->pos.x + dim.width <= left)
        {
          en->pos.x = right;
        }
        else if (en->pos.x >= right)
        {
          en->pos.x = left;
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

      // Ground collision
      if (entity_has_prop(en, EntityProp_CollidesWithGround))
      {
        if (p_rect_y_range_intersect(
              collision_params_from_entity(en->colliders[Collider_Body], en->vel), 
              v2f(-3000.0f, 3000.0f), GROUND_Y))
        {
          en->pos.y = GROUND_Y + dim.height / 2.0f;
          en->vel.y = 0.0f;
          en->new_vel.y = 0.0f;
          entity_add_prop(en, EntityProp_Grounded);
        }
        else
        {
          entity_rem_prop(en, EntityProp_Grounded);
        }
      }

      // Bullet vs Zombie collision
      if (en->type == EntityType_Bullet)
      {
        for (Entity *other = game->entities.head; other; other = other->next)
        {
          if (other->type == EntityType_ZombieWalker)
          {
            if (p_rect_circle_intersect(
                  collision_params_from_entity(other->colliders[Collider_Body], other->vel),
                  collision_params_from_entity(en->colliders[Collider_Hit], en->vel)))
            {
              Vec2F spawn_pos = pos_from_entity(en);
              spawn_entity(game, EntityType_ParticleGroup, 
                            .pos=spawn_pos, 
                            .particle_desc=PREFAB->particle.blood);

              damage_entity(game, en, other);
              kill_entity(en);
            }
          }
        }
      }

      // Zombie vs Player collision
      if (en->combat_type == CombatType_Melee)
      {
        if (p_rect_rect_intersect(
              collision_params_from_entity(en->colliders[Collider_Hit], en->vel),
              collision_params_from_entity(player->colliders[Collider_Body], player->vel)))
        {
          if (!en->attack_timer.is_ticking)
          {
            en->attack_timer.end_time = en->attack_timer.duration + t;
            en->attack_timer.is_ticking = TRUE;
          }

          if (timeout(en->attack_timer, t))
          {
            printf("Player was hit\n");
            damage_entity(game, en, player);
            en->attack_timer.is_ticking = FALSE;
          }
        }
        else
        {
          en->attack_timer.is_ticking = FALSE;
        }
      }
    }
  }

  // Update entity combat ----------------
  for (EN_IN_ENTITIES)
  {
    if (!en->is_active) continue;

    if (entity_has_prop(en, EntityProp_Controlled))
    {
      if (!en->attack_timer.is_ticking)
      {
        en->attack_timer.end_time = t + en->attack_timer.duration;
        en->attack_timer.is_ticking = TRUE;
      }

      // Shoot weapon if can shoot
      if (is_key_pressed(KEY_MOUSE_1) && timeout(en->attack_timer, t))
      {
        Entity *gun = get_entity_child_of_sp(en, SP_Gun);
        Entity *shot_point = get_entity_child_at(gun, 0);
        Vec2F spawn_pos = pos_from_entity(shot_point);
        f32 spawn_rot = en->flip_x ? -gun->rot + 180 : gun->rot;

        Entity *bullet = spawn_entity(game, EntityType_Bullet, .pos=spawn_pos);
        bullet->rot = spawn_rot;
        bullet->speed = PROJ_SPEED;
        entity_rem_prop(bullet, EntityProp_Renders);

        spawn_entity(game, EntityType_ParticleGroup, 
                      .pos=spawn_pos, 
                      .particle_desc=PREFAB->particle.smoke);

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
            if (timeout(en->attack_timer, t))
            {
              Vec2F spawn_pos = v2f(en->pos.x, en->pos.y);
              Entity *laser = spawn_entity(game, EntityType_Bullet, .pos=spawn_pos);
              laser->tint = DEBUG_GREEN;
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

            if (desc.props & ParticleProp_VariateColor)
            {
              particle->color = lerp_4f(particle->color, desc.color_secondary, dt);
            }

            if (desc.props & ParticleProp_ScaleOverTime)
            {
              particle->scale = add_2f(particle->scale, scale_2f(desc.scale_delta, dt));
              particle->scale.x = clamp_bot(particle->scale.x, 0.0f);
              particle->scale.y = clamp_bot(particle->scale.y, 0.0f);
            }

            if (desc.props & ParticleProp_SpeedOverTime)
            {
              particle->speed += desc.speed_delta * dt * dt;
              particle->speed = clamp_bot(particle->speed, 0.0f);
            }

            if (desc.props & ParticleProp_RotateOverTime)
            {
              particle->rot += desc.rot_delta * dt * dt;
            }

            if (desc.props & ParticleProp_AffectedByGravity)
            {
              if (!particle->is_grounded)
              {
                particle->vel.x = sin_1f(particle->dir) * particle->speed * dt;
                particle->vel.y -= GRAVITY * dt * dt;
              }
            }
            else
            {
              particle->vel = v2f(
                sin_1f(particle->dir) * particle->speed * dt,
                cos_1f(particle->dir) * particle->speed * dt
              );
            }

            if (desc.props & ParticleProp_CollidesWithGround)
            {
              Vec2F p_col_pos = {
                particle->pos.x, 
                particle->pos.y + particle->scale.y
              };

              P_CollisionParams params = {.pos=p_col_pos, .vel=particle->vel};
              if (p_point_y_range_intersect(params, v2f(-3000.0f, 3000.0f), GROUND_Y))
              {
                particle->pos.y = GROUND_Y - particle->scale.y;
                particle->vel = V2F_ZERO;
                particle->is_grounded = TRUE;
              }
            }
            
            particle->pos = add_2f(particle->pos, particle->vel);
          }

          if (!en->particle_timer.is_ticking)
          {
            en->particle_timer.end_time = t + desc.duration;
            en->particle_timer.is_ticking = TRUE;
          }

          if (t >= en->particle_timer.end_time)
          {
            en->particle_timer.is_ticking = FALSE;
            kill_entity(en);
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

  // Spawn entities ----------------
  {
    if (!game->spawn_timer.is_ticking)
    {
      game->spawn_timer.end_time = 5.0f + t;
      game->spawn_timer.is_ticking = TRUE;
    }

    if (timeout(game->spawn_timer, t))
    {
      f32 spawn_x[2] = {-50, get_width() + 50};
      u32 roll = (u32) random_u64(0, 1);

      spawn_entity(game, EntityType_ZombieWalker, .pos=v2f(spawn_x[roll], get_height() / 2));
      game->spawn_timer.is_ticking = FALSE;
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
      kill_entity(player);
    }

    // Spawn particles
    if (is_key_just_pressed(KEY_9))
    {
      spawn_entity(game, EntityType_ParticleGroup, 
                    .pos=player->pos, 
                    .particle_desc=PREFAB->particle.debug);
    }
    
    if (is_key_pressed(KEY_0))
    {
      spawn_entity(game, EntityType_ParticleGroup, 
                    .pos=player->pos, 
                    .particle_desc=PREFAB->particle.debug);
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

void render_game(Game *game)
{
  clear_frame(V4F_ZERO);

  draw_scene(v2f(0, 0), mul_2f(v2f(192, 108), SPRITE_SCALE), v4f(1, 1, 1, 1));

  // Batch sprites ----------------
  for (EN_IN_ENTITIES)
  {
    if (!en->is_active) continue;

    if (en->draw_type == DrawType_Sprite && entity_has_prop(en, EntityProp_Renders))
    {
      draw_sprite_x(en->xform, en->tint, en->texture, entity_has_prop(en, EntityProp_FlashWhite));

      // NOTE(dg): TEMPORTARY SOLUTION BEFORE ANIMATION SYSTEM
      if (entity_has_prop(en, EntityProp_FlashWhite))
      {
        entity_rem_prop(en, EntityProp_FlashWhite);
      }
    }
  }
  
  // Batch primitives ----------------
  for (EN_IN_ENTITIES)
  {
    if (!en->is_active) continue;

    if (entity_has_prop(en, EntityProp_Renders))
    {
      if (en->draw_type == DrawType_Primitive)
      {
        if (en->type == EntityType_Collider && GLOBAL->debug)
        {
          Vec4F color = en->tint;
          switch (en->col_id)
          {
            case Collider_Body: 
            case Collider_Head: color = v4f(0, 1, 0, 0.35f);
            break;
            case Collider_Hit: color = v4f(1, 0, 0, 0.35f);
            break;
            default: color = v4f(1, 1, 1, 0.35f);
            break;
          }

          draw_rectangle_x(en->xform, color);
        }
        else if (en->type != EntityType_Collider)
        {
          draw_rectangle_x(en->xform, en->tint);
        }
      }

      if (en->type == EntityType_ParticleGroup)
      {
        draw_particles(en);
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

// TODO(dg): Please make this better! Also viewport.y needs to factor in here somewhere.
inline
Vec2F screen_to_world(Vec2F pos)
{
  return v2f(pos.x * (WIDTH / GLOBAL->window.width) + GLOBAL->viewport.x, 
            (get_height() - pos.y) * (HEIGHT / GLOBAL->window.height));
}

// @Events ///////////////////////////////////////////////////////////////////////////////

void push_event(Game *game, EventType type, EventDesc desc)
{
  EventQueue *queue = &game->event_queue;
  Event *new_event = arena_push(&game->frame_arena, sizeof (Event));

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
  queue->count += 1;
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
  queue->count -= 1;
}

Event *peek_event(Game *game)
{
  return game->event_queue.front;
}
