#include "base/base.h"
#include "vecmath/vecmath.h"
#include "ui/ui.h"
#include "physics/physics.h"
#include "draw.h"
#include "input.h"
#include "entity.h"
#include "prefabs.h"
#include "game.h"

#define EN_IN_ENTITIES Entity *en = game.entities.head; en; en = en->next
#define E_IN_EVENT_QUEUE Event *e = peek_event(); game.event_queue.count != 0; pop_event()
#define GROUND_Y (30 * SPRITE_SCALE)

extern Globals global;
extern Prefabs prefab;
extern Game game;

// @Main /////////////////////////////////////////////////////////////////////////////////

void init_game(void)
{
  game.camera = m3x3f(1.0f);
  game.spawn_timer.duration = 5.0f;
  game.grace_period_timer.duration = 10.0f;

  ui_init_widgetstore(64, &global.perm_arena);

  // Starting entities ----------------
  {
    Entity *player = create_entity(EntityType_Player);
    player->sp = SP_Player;
    player->pos = v2f(WIDTH/2.0f, HEIGHT/2.0f);

    Entity *gun = create_entity(EntityType_Equipped);
    attach_entity_child(player, gun);
    gun->sp = SP_Gun;
    gun->pos = v2f(35.0f, 5.0f);

    Entity *shot_point = create_entity(EntityType_Debug);
    attach_entity_child(gun, shot_point);
    shot_point->pos = v2f(20.0f, 2.0f);

    // Entity *zombie = create_entity(EntityType_Zombie);
    // zombie->pos = v2f(WIDTH, HEIGHT/2.0f);
  }
}

void update_game(void)
{
  f64 t = game.t;
  f64 dt = game.dt;
  Vec2F mouse_pos = screen_to_world(get_mouse_pos());

  Entity *player = get_entity_of_sp(SP_Player);
  if (!entity_is_valid(player)) player = NIL_ENTITY;

  ui_clear_widgetstore();

  // Zombie waves ----------------
  if (!game.is_over)
  {
    if (game.current_wave.zombies_killed == zombies_to_spawn_this_wave())
    {
      u16 next_wave = game.current_wave.num + 1;

      ui_text(str("Next wave in: %0.f"), 
              v2f(WIDTH/2 - 150, HEIGHT - 200), 30, 999,
              game.grace_period_timer.end_time - t);

      if (!game.grace_period_timer.is_ticking)
      {
        timer_start(&game.grace_period_timer, game.grace_period_timer.duration);
      }

      if (timer_timeout(&game.grace_period_timer))
      {
        game.current_wave.zombies_spawned = 0;
        game.current_wave.zombies_killed = 0;
        game.current_wave.num = next_wave;
      }

      if (next_wave == TOTAL_WAVE_COUNT)
      {
        game.is_over = TRUE;
        game.won = TRUE;

        logger_debug(str("Victory!\n"));
      }
    }
    else
    {
      WaveDesc wave_desc = prefab.wave[game.current_wave.num];
      
      if (!game.spawn_timer.is_ticking)
      {
        timer_start(&game.spawn_timer, wave_desc.time_btwn_spawns);
      }

      if (timer_timeout(&game.spawn_timer))
      {
        ZombieKind spawn_roll = random_i32(1, ZombieKind_COUNT-1);
        // logger_debug(str("Roll: %u\n"), spawn_roll);

        f32 x_pos[2] = {-50.0f, WIDTH + 50.0f};
        u32 x_roll = (u32) random_i32(0, 1);

        Entity *zombie = spawn_zombie(spawn_roll);
        zombie->pos = v2f(x_pos[x_roll], HEIGHT/2);

        game.current_wave.zombies_spawned += 1;
      }
    }
  }

  // Switch weapon ----------------
  if (entity_is_valid(player))
  {
    if (is_key_just_pressed(Key_0))
    {
      equip_weapon(player, WeaponKind_Nil);
    }
    else if (is_key_just_pressed(Key_1))
    {
      equip_weapon(player, WeaponKind_Pistol);
    }
    else if (is_key_just_pressed(Key_2))
    {
      equip_weapon(player, WeaponKind_Rifle);
    }
    else if (is_key_just_pressed(Key_3))
    {
      equip_weapon(player, WeaponKind_Shotgun);
    }
    else if (is_key_just_pressed(Key_4))
    {
      equip_weapon(player, WeaponKind_SMG);
    }
  }
  
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
      free_entity(en);
    }
  }

  if (!game.is_over)
  {
    game.time_alive = t;
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
      // NOTE(dg): State changes are temporary. Need proper state machine
      if (entity_has_prop(en, EntityProp_Controlled))
      {
        entity_look_at(en, mouse_pos);

        if (is_key_pressed(Key_A) && !is_key_pressed(Key_D))
        {
          en->new_vel.x = lerp_1f(en->new_vel.x, -en->speed * dt, PLAYER_ACC * dt);
          en->anim_state = Animation_Walk;
        }

        if (is_key_pressed(Key_D) && !is_key_pressed(Key_A))
        {
          en->new_vel.x = lerp_1f(en->new_vel.x, en->speed * dt, PLAYER_ACC * dt);
          en->anim_state = Animation_Walk;
        }

        if (is_key_pressed(Key_A) && is_key_pressed(Key_D))
        {
          en->new_vel.x = lerp_1f(en->new_vel.x, 0.0f, PLAYER_FRIC * 2.0f * dt);
          en->new_vel.x = to_zero(en->new_vel.x, 1.0f);
          en->anim_state = Animation_Idle;
        }
        
        if (!is_key_pressed(Key_A) && !is_key_pressed(Key_D))
        {
          en->new_vel.x = lerp_1f(en->new_vel.x, 0.0f, PLAYER_FRIC * dt);
          en->new_vel.x = to_zero(en->new_vel.x, 1.0f);
          en->anim_state = Animation_Idle;
        }

        bool jump_key_pressed = is_key_pressed(Key_W) || is_key_pressed(Key_Space);
        if (jump_key_pressed && entity_has_prop(en, EntityProp_Grounded))
        {
          en->new_vel.y = PLAYER_JUMP_VEL * dt;
          entity_rem_prop(en, EntityProp_Grounded);
        }

        if (!entity_has_prop(en, EntityProp_Grounded))
        {
          en->anim_state = Animation_Jump;
        }
      }
      else
      {
        Vec2F player_pos = pos_from_entity(player);
        if (entity_is_valid(player) && !game.is_over)
        {
          entity_look_at(en, player_pos);
        }

        switch (en->move_type)
        {
          case MoveType_Grounded:
          {
            f32 dist_from_player = distance_2f(pos_from_entity(en), player_pos);

            if (entity_has_prop(en, EntityProp_Grounded) && dist_from_player >= 40.0f)
            {
              en->new_vel.x = en->flip_x ? -WALKER_SPEED * dt : WALKER_SPEED * dt;
              en->anim_state = Animation_Walk;
            }
            else
            {
              en->new_vel.x = 0;
              en->anim_state = Animation_Idle;
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
              f32 bound = en->speed * absv(en->input_dir.x) * dt;
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
              f32 bound = en->speed * absv(en->input_dir.y) * dt;
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
              timer_start(&en->kill_timer, en->kill_timer.duration);
            }

            if (timer_timeout(&en->kill_timer))
            {
              kill_entity(en);
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
        f32 left = screen_to_world(v2f(global.viewport.x, 0)).x;
        f32 right = screen_to_world(v2f(global.viewport.z, 0)).x;

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
        for (Entity *p = parent; entity_is_valid(p); p = entity_from_ref(p->parent))
        {
          scale = mul_2f(scale, p->scale);
        }

        xform = mul_3x3f(translate_3x3f(en->pos.x/scale.x, en->pos.y/scale.y), xform);
      }

      en->model_mat = xform;

      // Move to world space
      {
        Mat3x3F model = m3x3f(1.0f);
        for (Entity *p = parent; entity_is_valid(p); p = entity_from_ref(p->parent))
        {
          model = mul_3x3f(p->model_mat, model);
        }
        
        xform = mul_3x3f(model, xform);
      }

      xform = mul_3x3f(game.camera, xform);
      // xform = mul_3x3f(global.renderer.projection, xform);

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
              collision_params_from_entity(en->cols[Collider_Body], en->vel), 
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
        for (Entity *other = game.entities.head; other; other = other->next)
        {
          if (other->type == EntityType_Zombie)
          {
            if (p_rect_circle_intersect(
                  collision_params_from_entity(other->cols[Collider_Body], other->vel),
                  collision_params_from_entity(en->cols[Collider_Hit], en->vel)))
            {
              Vec2F spawn_pos = pos_from_entity(en);
              spawn_entity(EntityType_ParticleGroup, 
                            .pos=spawn_pos, 
                            .particle_desc=prefab.particle.blood);

              damage_entity(other, en->damage);
              kill_entity(en);
            }
          }
        }
      }

      // Zombie vs Player collision
      if (en->combat_type == CombatType_Melee && entity_is_valid(player))
      {
        if (p_rect_rect_intersect(
              collision_params_from_entity(en->cols[Collider_Hit], en->vel),
              collision_params_from_entity(player->cols[Collider_Body], player->vel)))
        {
          bool timeout = TRUE;
          if (!en->colliding_with_player)
          {
            en->colliding_with_player = TRUE;
          }
          else
          {
            if (!en->attack_timer.is_ticking)
            {
              timer_start(&en->attack_timer, en->attack_timer.duration);
            }
            
            timeout = timer_timeout(&en->attack_timer);
          }
          
          if (timeout && timer_timeout(&player->invincibility_timer))
          {
            damage_entity(player, en->damage);
            timer_reset(&player->invincibility_timer);
          }
        }
        else
        {
          timer_reset(&en->attack_timer);
          en->colliding_with_player = FALSE;
        }
      }

      // Item vs Player collision
      if (en->type == EntityType_Collectable && entity_is_valid(player))
      {
        if (p_rect_circle_intersect(
              collision_params_from_entity(player->cols[Collider_Body], player->vel),
              collision_params_from_entity(en->cols[Collider_Hit], en->vel)))
        {
          kill_entity(en);
          spawn_entity(EntityType_ParticleGroup,
                        .pos=pos_from_entity(en),
                        .particle_desc=prefab.particle.pickup_coin);
          
          if (en->item_kind == CollectableKind_Coin) game.coin_count++; 
          if (en->item_kind == CollectableKind_Soul) game.soul_count++; 
        }
      }  
    }
  }

  // Update entity combat ----------------
  for (EN_IN_ENTITIES)
  {
    if (!en->is_active) continue;

    // Update player invinsibility timer
    if (en->sp == SP_Player)
    {
      if (!en->invincibility_timer.is_ticking)
      {
        timer_start(&en->invincibility_timer, en->invincibility_timer.duration);
      }
    }

    if (entity_has_prop(en, EntityProp_Controlled))
    {
      if (en->weapon_equipped)
      {
        if (!en->attack_timer.is_ticking)
        {
          timer_start(&en->attack_timer, en->attack_timer.duration);
        }

        // Shoot weapon if can shoot
        if (is_key_pressed(Key_Mouse1) && timer_timeout(&en->attack_timer))
        {
          Entity *gun = get_entity_child_of_sp(en, SP_Gun);
          Entity *shot_point = get_entity_child_at(gun, 0);
          Vec2F spawn_pos = pos_from_entity(shot_point);
          f32 spawn_rot = en->flip_x ? -gun->rot + 180 : gun->rot;

          Entity *bullet = spawn_entity(EntityType_Bullet, .pos=spawn_pos);
          bullet->rot = spawn_rot;
          bullet->speed = gun->speed;
          bullet->damage = gun->damage;

          spawn_entity(EntityType_ParticleGroup, 
                        .pos=spawn_pos, 
                        .particle_desc=prefab.particle.smoke);
        }
      }
    }
    else
    {
      if (entity_is_valid(player) && player->is_active)
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
              timer_start(&en->attack_timer, en->attack_timer.duration);
            }

            // Shoot weapon if can shoot
            if (timer_timeout(&en->attack_timer))
            {
              Vec2F spawn_pos = v2f(en->pos.x, en->pos.y);
              Entity *laser = spawn_entity(EntityType_Bullet, .pos=spawn_pos);
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

  // Update entity bobbing ----------------
  for (EN_IN_ENTITIES)
  {
    if (entity_has_prop(en, EntityProp_BobsOverTime))
    {
      if (en->pos.y > en->bobbing_range.e[1] - 0.1f)
      {
        en->bobbing_state = -1;
      }

      if (en->pos.y < en->bobbing_range.e[0] + 0.1f)
      {
        en->bobbing_state = 1;
      }
      
      en->pos.y += 10 * dt * en->bobbing_state;
    }
  }

  // Update particles ----------------
  for (u32 i = 0; i < MAX_PARTICLES; i++)
  {
    Particle *particle = &game.particle_buffer.data[i];

    if (!particle->is_active) continue;

    Entity *owner = entity_from_ref(particle->owner);
    ParticleDesc desc = owner->particle_desc;

    switch (desc.emmission_type)
    {
      case ParticleEmmissionType_Burst:
      {
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

        if (!owner->particle_timer.is_ticking)
        {
          timer_start(&owner->particle_timer, desc.duration);
        }

        if (timer_timeout(&owner->particle_timer))
        {
          particle->is_active = FALSE;
          kill_entity(owner);
          // logger_debug(str("Timed out!\n"));
        }
      }
      case ParticleEmmissionType_Linear:
      {
        // ...
      }
      break;
    }
  }

  // Animate entities ----------------
  for (EN_IN_ENTITIES)
  {
    if (entity_has_prop(en, EntityProp_FlashWhite))
    {
      en->flash_timer.duration = FLASH_TIME;

      if (!en->flash_timer.is_ticking)
      {
        timer_start(&en->flash_timer, en->flash_timer.duration);
      }

      if (timer_timeout(&en->flash_timer))
      {
        entity_rem_prop(en, EntityProp_FlashWhite);
      }
    }

    if (en->anim_state == Animation_None) continue;
    
    // Clear prev animation if the state changed
    if (en->anim_state_prev != en->anim_state)
    {
      zero(en->anim, Animation);
      en->anim_state_prev = en->anim_state;
    }
    
    AnimationDesc anim_desc = en->anims[en->anim_state];
    en->texture = anim_desc.frames[en->anim.frame_idx];

    // Tick animation
    if (en->anims[en->anim_state].frame_count > 1)
    {
      en->anim.tick_counter += ANIM_TICK;

      if (en->anim.tick_counter % anim_desc.ticks_per_frame == 0)
      {
        if (en->anim.frame_idx == anim_desc.frame_count - 1)
        {
          en->anim.frame_idx = 0;
        }
        else
        {
          en->anim.frame_idx += 1;
        }
      }
    }
  }

  if (game.is_over)
  {
    String msg;
    if (game.won)
    {
      msg = str("YOU WIN");
    }
    else
    {
      msg = str("GAME OVER");  
    }
    ui_text(msg, v2f(WIDTH/2 - 150, HEIGHT/2), 50, 999);
  }

  for (E_IN_EVENT_QUEUE)
  {
    switch (e->type)
    {
      case EventType_Nil: break;
      case EventType_EntityKilled:
      {
        Entity *en = e->desc.en;
        if (!entity_is_valid(en)) continue;

        if (e->desc.type == EntityType_Player)
        {
          game.is_over = TRUE;
          logger_debug(str("Player has been killed."));
        }

        if (en->type == EntityType_Zombie)
        {
          game.current_wave.zombies_killed += 1;

          // Spawn drop
          i32 roll = random_i32(1, 100);

          CollectableDesc desc = {0};
          CollectableKind kind = 0;
          if (roll <= prefab.collectable[CollectableKind_Soul].draw_chance)
          {
            desc = prefab.collectable[CollectableKind_Soul];
            kind = CollectableKind_Soul;
          }
          else if (roll <= prefab.collectable[CollectableKind_Coin].draw_chance)
          {
            desc = prefab.collectable[CollectableKind_Coin];
            kind = CollectableKind_Coin;
          }

          if (kind != CollectableKind_Nil)
          {
            Entity *collectable = spawn_entity(EntityType_Collectable);
            collectable->item_kind = kind;
            collectable->pos = v2f(en->pos.x, en->pos.y - 10);
            collectable->texture = desc.texture;
            collectable->bobbing_range = v2f(collectable->pos.y - 5, collectable->pos.y + 5);
            collectable->bobbing_state = -1;
          }
        }
      }
    }
  }

  ui_text(str("%.1f"), v2f(WIDTH/2 - 20, HEIGHT - 50), 25, 999, game.time_alive);
  ui_text(str("Hearts: %i"), v2f(10, HEIGHT - 50), 25, 999, player->health);
  ui_text(str("Coins: %i"), v2f(10, HEIGHT - 75), 25, 999, game.coin_count);
  ui_text(str("Souls: %i"), v2f(10, HEIGHT - 100), 25, 999, game.soul_count);
  ui_text(str("Wave: %i"), v2f(WIDTH - 150, HEIGHT - 50), 25, 999, game.current_wave.num+1);

  // ui_text(str("xy: %.0f %.0f"), v2f(10, HEIGHT - 60), 20, 999, mouse_pos.x, mouse_pos.y);

  zero(*NIL_ENTITY, Entity);

  // Developer tools ----------------
  {
    // Toggle debug
    if (is_key_just_pressed(Key_Tab))
    {
      global.debug = !global.debug; 
    }
    
    for (EN_IN_ENTITIES)
    {
      if (!en->is_active) continue;
      
      if (en->type == EntityType_Debug)
      {
        if (global.debug)
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
    if (is_key_just_pressed(Key_Backspace) && entity_is_valid(player))
    { 
      kill_entity(player);
    }

    // Spawn particles 
    if (is_key_pressed(Key_P) && entity_is_valid(player))
    {
      spawn_entity(EntityType_ParticleGroup, 
                    .pos=player->pos, 
                    .particle_desc=prefab.particle.debug);
    }
  }
}

void render_game(void)
{
  clear_frame(V4F_ZERO);

  // Draw scene ----------------
  draw_scene(v2f(0, 0), scale_2f(v2f(192, 108), SPRITE_SCALE), v4f(1, 1, 1, 1));

  // Draw sprite batch ----------------
  for (EN_IN_ENTITIES)
  {
    if (!en->is_active) continue;

    if (en->draw_type == DrawType_Sprite && entity_has_prop(en, EntityProp_Renders))
    {
      bool flash = entity_has_prop(en, EntityProp_FlashWhite);
      draw_sprite_x(en->xform, en->tint, en->texture, flash);
    }
  }
  
  // Draw primitive batch ----------------
  {
    // Draw entities
    for (EN_IN_ENTITIES)
    {
      if (!en->is_active) continue;

      if (entity_has_prop(en, EntityProp_Renders))
      {
        if (en->draw_type == DrawType_Primitive)
        {
          if (en->type == EntityType_Collider && global.debug)
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
      }
    }

    // Draw particles
    for (i32 i = 0; i < MAX_PARTICLES; i++)
    {
      Particle *particle = &game.particle_buffer.data[i];
      
      if (!particle->is_active) continue;

      f32 rot = particle->rot * RADIANS;
      draw_rectangle(particle->pos, particle->scale, rot, particle->color);
    }
  }

  // Draw UI batch
  UI_WidgetStore *widgets = ui_get_widgetstore();
  for (u64 wdgt_idx = 0; wdgt_idx < widgets->count; wdgt_idx++)
  {
    UI_Widget *widget = &widgets->data[wdgt_idx];
    switch (widget->type)
    {
      case UI_WidgetType_Nil:
      break;
      case UI_WidgetType_Text:
      {
        const f32 scale = 1.0f/8;

        Vec2F offset = V2F_ZERO;
        u32 word_start_pos = 0;

        u32 len = widget->text.len;
        for (u32 chr_idx = 0; chr_idx < len; chr_idx++)
        {
          char chr = widget->text.data[chr_idx];
          if (((chr < 65 || chr > 90) && (chr < 97 || chr > 122)) || chr_idx == len-1)
          {
            UI_Glyph glyph = get_glyph(chr);

            // Calculate word length
            f32 word_len = 0;
            for (u32 i = word_start_pos; i <= chr_idx; i++)
            {
              chr = widget->text.data[i];
              word_len += (widget->text_size * (glyph.dim.width * scale)) + (widget->text_spacing.x * (widget->text_size * scale));
            }

            // Move to next line
            if (offset.x + word_len > widget->dim.width && chr_idx < len)
            {
              offset.x = 0;
              offset.y -= widget->text_size + (widget->text_spacing.y * (widget->text_size * scale));

              word_len += (glyph.dim.width + glyph.offset.x + widget->space_width) * widget->text_size;
            }

            // Draw the word
            for (u32 i = word_start_pos; i <= chr_idx; i++)
            {
              chr = widget->text.data[i];
              if (chr != ' ')
              {
                UI_Glyph glyph = get_glyph(chr);

                Vec2F draw_pos = add_2f(add_2f(widget->pos, scale_2f(glyph.offset, widget->text_size*scale)), offset);
                draw_glyph(draw_pos, widget->text_size, R_WHITE, glyph.coords);

                offset.x += (widget->text_size * (glyph.dim.width * scale)) + (widget->text_spacing.x * (widget->text_size * scale));
              }
              else
              {
                offset.x += widget->space_width * widget->text_size * scale;
              }
            }
            
            word_start_pos = chr_idx + 1;
          }
        }
      }
      break;
    }
  }

  arena_clear(&widgets->arena);
  
  r_flush(&global.renderer);
}

Particle *get_next_free_particle(void)
{
  ParticleBuffer *buffer = &game.particle_buffer;
  
  Particle *result = &buffer->data[buffer->pos];
  buffer->pos += 1;

  if (buffer->pos == MAX_PARTICLES)
  {
    buffer->pos = 0;
  }

  return result;
}

i16 zombies_to_spawn_this_wave(void)
{
  i16 result = 0;

  for (i32 i = 0; i < ZombieKind_COUNT; i++)
  {
    result += prefab.wave[game.current_wave.num].zombie_counts[i];
  }

  return result;
}

// @Events ///////////////////////////////////////////////////////////////////////////////

void push_event(EventType type, EventDesc desc)
{
  EventQueue *queue = &game.event_queue;

  Event *new_event = arena_push(&game.frame_arena, Event, 1);
  new_event->type = type;
  new_event->desc = desc;

  if (queue->front == NULL)
  {
    queue->front = new_event;
  }

  if (queue->back != NULL)
  {
    queue->back->next = new_event;
  }

  queue->back = new_event;
  queue->count += 1;
}

void pop_event(void)
{
  EventQueue *queue = &game.event_queue;
  
  Event *next = queue->front->next;
  zero(*queue->front, Event);

  if (next == NULL)
  {
    queue->back = NULL;
  }

  queue->front = next;
  queue->count -= 1;
}

Event *peek_event(void)
{
  return game.event_queue.front;
}

// @Misc /////////////////////////////////////////////////////////////////////////////////

inline
bool game_should_quit(void)
{
  return game.should_quit || is_key_pressed(Key_Escape);
}

inline
Vec2F screen_to_world(Vec2F pos)
{
  return (Vec2F) {
    (pos.x - global.viewport.x) * (WIDTH / global.viewport.z),
    (pos.y - global.viewport.y) * (HEIGHT / global.viewport.w),
  };
}
