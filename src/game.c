#include "sokol/sokol_time.h"
#include "stb/stb_sprintf.h"

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
#define GROUND_Y (30 * SPRITE_SCALE)

extern Globals global;
extern Prefabs prefab;
extern Game game;

void init_game(void)
{
  game.camera = m3x3f(1.0f);
  game.is_grace_period = TRUE;
  game.grace_period_timer.duration = 5.0f;
  game.current_wave.num = -1;

  ui_init_widgetstore(128, &global.perm_arena);

  // - Starting entities ---
  {
    Entity *merchant = create_entity(EntityType_Merchant);
    merchant->sp = SPID_Merchant;
    merchant->pos = v2f(WIDTH/2, GROUND_Y + 80);

    Entity *player = create_entity(EntityType_Player);
    player->sp = SPID_Player;
    player->pos = v2f(WIDTH/2.0f, HEIGHT/2.0f);

    Entity *gun = create_entity(EntityType_Equipped);
    gun->sp = SPID_Gun;
    gun->pos = v2f(35.0f, 5.0f);
    attach_entity_child(player, gun);

    Entity *shot_point = create_entity(EntityType_Debug);
    attach_entity_child(gun, shot_point);

    Entity *muzzle_flash = create_entity(EntityType_Decoration);
    muzzle_flash->sprite = prefab.sprite.muzzle_flash;
    entity_add_prop(muzzle_flash, EntityProp_HideAfterTime);
    entity_rem_prop(muzzle_flash, EntityProp_Renders);
    attach_entity_child(gun, muzzle_flash);

    // spawn_zombie(ZombieKind_BabyChicken, v2f(WIDTH - 100, GROUND_Y + 100));
  }

  for (i32 i = 0; i < 0; i++)
  {
    Entity *en = create_entity(EntityType_Any);
    en->pos = v2f(200, 200);
  }
}

void update_game(void)
{
  f64 t = game.t;
  f64 dt = game.dt;
  Vec2F mouse_pos = screen_to_world(get_mouse_pos());

  Entity *player = get_entity_by_sp(SPID_Player);
  if (!entity_is_valid(player)) player = NIL_ENTITY;

  ui_clear_widgetstore();

  // - Zombie waves ---
  if (!game.is_so_over)
  // if (0)
  {
    i32 total_zombies_this_wave = 0;
    if (game.current_wave.num >= 0)
    {
      for (i32 i = 0; i < ZombieKind_COUNT; i++)
      {
        total_zombies_this_wave += prefab.wave[game.current_wave.num].zombie_counts[i];
      }
    }

    if (game.is_grace_period)
    {
      String text = str("Next wave in %0.f");
      if (game.current_wave.num < 0)
      {
        text = str("Game begins in %0.f");
      }

      ui_text(text, 
              v2f(WIDTH/2 - 150, HEIGHT - 200), 30, 999,
              game.grace_period_timer.end_time - t);
      
      if (!game.grace_period_timer.ticking)
      {
        timer_start(&game.grace_period_timer, game.grace_period_timer.duration);
      }

      if (timer_timeout(&game.grace_period_timer))
      {
        game.grace_period_timer.ticking = FALSE;

        game.current_wave.num += 1;
        game.current_wave.zombies_spawned = 0;
        game.current_wave.zombies_killed = 0;
        game.current_wave.desc = prefab.wave[game.current_wave.num];
        game.wave_just_began = TRUE;
        game.is_grace_period = FALSE;
      }

      if (game.current_wave.num == TOTAL_WAVE_COUNT)
      {
        game.is_so_over = TRUE;
        game.won = TRUE;
      }
    }
    else
    {
      game.is_grace_period = game.current_wave.zombies_killed == total_zombies_this_wave;

      if (game.current_wave.zombies_spawned < total_zombies_this_wave)
      {
        game.wave_just_began = FALSE;
        WaveDesc *desc = &game.current_wave.desc;

        if (!game.spawn_timer.ticking)
        {
          timer_start(&game.spawn_timer, desc->time_btwn_spawns);
        }

        if (timer_timeout(&game.spawn_timer))
        {
          game.spawn_timer.ticking = FALSE;

          i32 spawn_roll = 0;
          while (is_zombie_remaining_to_spawn(desc))
          {
            spawn_roll = random_i32(1, ZombieKind_COUNT-1);
            if (desc->zombie_counts[spawn_roll] > 0)
            {
              desc->zombie_counts[spawn_roll] -= 1;
              break;
            }
          }

          f32 x_pos_options[2] = {-50.0f, WIDTH + 50.0f};
          i32 x_roll = random_i32(0, 1);
          spawn_zombie(spawn_roll, v2f(x_pos_options[x_roll], HEIGHT/2));
          game.current_wave.zombies_spawned += 1;
        }
      }
    }
  }

  // - Merchant ---
  {
    Entity *merchant = get_entity_by_sp(SPID_Merchant);

    if (game.is_grace_period)
    {
      merchant->scale = lerp_2f(merchant->scale, v2f(SPRITE_SCALE*4, SPRITE_SCALE*2), dt*3);
    }
    else
    {
      merchant->scale = lerp_2f(merchant->scale, V2F_ZERO, dt*3);
    }
  }

  // - Switch weapon ---
  {
    if (game.is_grace_period)
    {
      switch (game.current_wave.num)
      {
        case 1: game.progression.rifle   = TRUE; break;
        case 2: game.progression.shotgun = TRUE; break;
        case 3: game.progression.smg     = TRUE; break;
      }
    }

    if (entity_is_valid(player))
    {
      if (is_key_just_pressed(Key_0))
      {
        equip_weapon(player, WeaponKind_Nil);
      }
      else if (is_key_just_pressed(Key_1))
      {
        equip_weapon(player, WeaponKind_Revolver);
      }
      else if (is_key_just_pressed(Key_2) && game.progression.rifle)
      {
        equip_weapon(player, WeaponKind_Rifle);
      }
      else if (is_key_just_pressed(Key_3) && game.progression.shotgun)
      {
        equip_weapon(player, WeaponKind_Shotgun);
      }
      else if (is_key_just_pressed(Key_4) && game.progression.smg)
      {
        equip_weapon(player, WeaponKind_SMG);
      }
      else if (is_key_just_pressed(Key_5) && game.progression.smg)
      {
        equip_weapon(player, WeaponKind_LaserPistol);
      }
    }
  }

  // - Weapon reloading ---
  if (entity_is_valid(player))
  {
    Entity *gun = get_entity_child_of_sp(player, SPID_Gun);
    WeaponDesc desc = prefab.weapon[gun->weapon_kind];

    if (!game.weapon.is_reloading && is_key_just_pressed(Key_R) && player->is_weapon_equipped)
    {      
      game.weapon.is_reloading = TRUE;      
      gun->rot = -45;

      timer_start(&game.weapon.reload_timer, desc.reload_duration);
    }
    else if (timer_timeout(&game.weapon.reload_timer))
    {
      game.weapon.reload_timer.ticking = FALSE;
      game.weapon.ammo_remaining = desc.ammo;
      game.weapon.is_reloading = FALSE;
    }
  }
  
  // - Update entity spawning and dying ---
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

  if (!game.is_so_over)
  {
    game.time_alive = t;
  }

  // - Update entity position ---
  for (EN_IN_ENTITIES)
  {
    if (!en->is_active) continue;
    
    if (entity_has_prop(en, EntityProp_AffectedByGravity) &&
       !entity_has_prop(en, EntityProp_Grounded))
    {
      en->new_vel.y -= GRAVITY * dt * dt;
    }

    // - Update entitiy movement ---
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
        if (entity_is_valid(player) && !game.is_so_over)
        {
          entity_look_at(en, player_pos);
        }

        switch (en->move_type)
        {
          case MoveType_Grounded:
          {
            f32 dist_from_player = distance_2f(pos_from_entity(en), player_pos);

            if (entity_has_prop(en, EntityProp_Grounded) && en->anim_state != Animation_LayEgg)
            {
              if (dist_from_player >= 40.0f)
              {
                en->anim_state = Animation_Walk;
              }
              else
              {
                en->anim_state = Animation_Idle;
              }
            }

            switch (en->anim_state)
            {
            case Animation_Idle:
            case Animation_LayEgg:
              en->new_vel.x = 0;
            break;
            case Animation_Walk:
              en->new_vel.x = en->flip_x ? -en->speed * dt : en->speed * dt;
            break;
            default: break;
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
            if (!en->kill_timer.ticking)
            {
              timer_start(&en->kill_timer, en->kill_timer.duration);
            }

            if (timer_timeout(&en->kill_timer))
            {
              en->kill_timer.ticking = FALSE;

              kill_entity(en, TRUE);
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

      // - Handle entity wrapping ---
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

    // - Wagon merhchant face player ---
    if (en->type == EntityType_Merchant)
    {
      Vec2F en_pos = pos_from_entity(en);
      Vec2F player_pos = pos_from_entity(player);
      if (en_pos.x < player_pos.x)
      {
        en->sprite = prefab.sprite.wagon_left;
      }
      else
      {
        en->sprite = prefab.sprite.wagon_right;
      }
    }

    // - Update entity xform ---
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

      en->xform = xform;
    }
  }

  // - Update equipped entities ---
  for (EN_IN_ENTITIES)
  {
    if (!en->is_active) continue;

    if (en->props & EntityProp_Equipped)
    {
      bool parent_flipped = en->parent.ptr->flip_x;

      if (!game.weapon.is_reloading)
      {
        Vec2F entity_pos = pos_from_entity(en);
        f32 angle = atan_2f(sub_2f(mouse_pos, entity_pos)) * DEGREES;

        if (!parent_flipped)
        {
          angle = clamp(angle, -90, 90);
        }
        else
        {
          if (angle < 0)
          {
            angle += 360;
          }

          angle = -clamp(angle, 90, 270) + 180;
        }

        en->rot = angle;
      }
    }
  }

  // - Update zombie behaviors ---
  for (EN_IN_ENTITIES)
  {
    if (!en->is_active) continue;

    if (entity_has_prop(en, EntityProp_LaysEggs))
    {
      if (!en->egg_timer.ticking)
      {
        if (en->anim_state != Animation_LayEgg)
        {
          timer_start(&en->egg_timer, 2.0f);
        }
        else
        {
          timer_start(&en->egg_timer, 3.0f);
        }
      }

      if (timer_timeout(&en->egg_timer))
      {
        en->egg_timer.ticking = FALSE;

        if (en->anim_state != Animation_LayEgg)
        {
          en->anim_state = Animation_LayEgg;
        }
        else
        {
          spawn_entity(EntityType_Egg, en->pos);
          en->anim_state = Animation_Walk;
        }
      }
    }

    // - Update egg ---
    if (en->type == EntityType_Egg)
    {
      if (!en->egg_timer.ticking)
      {
        timer_start(&en->egg_timer, 3.0f);
      }

      f64 remaining_time = timer_remaining(&en->egg_timer);
      if (remaining_time <= 1.0f)
      {
        en->sprite = prefab.sprite.egg_2;
      }
      else if (remaining_time <= 2.0f)
      {
        en->sprite = prefab.sprite.egg_1;
      }

      // - Hatched ---
      if (timer_timeout(&en->egg_timer))
      {
        kill_entity(en, FALSE);
        spawn_zombie(ZombieKind_BabyChicken, pos_from_entity(en));
        spawn_particles(ParticleKind_EggHatch, sub_2f(pos_from_entity(en), v2f(0, 25)));
      }
    }

    // - Morphing ---
    if (entity_has_prop(en, EntityProp_Morphs))
    {
      if (!en->morphing.timer.ticking)
      {
        timer_start(&en->morphing.timer, BABY_CHICKEN_GROWTH_DURATION);
      }

      if (timer_timeout(&en->morphing.timer))
      {
        kill_entity(en, FALSE);
        spawn_zombie(ZombieKind_Chicken, pos_from_entity(en));
      }
    }
  }

  // - Update entity collision ---
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
          en->pos.y = GROUND_Y + 
                      dim_from_entity(en->cols[Collider_Body]).height/2 -
                      en->cols[Collider_Body]->pos.y;

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
      if (en->type == EntityType_Ammo)
      {
        for (Entity *other = game.entities.head; other; other = other->next)
        {
          if (other->type == EntityType_Zombie)
          {
            if (p_rect_circle_intersect(
                  collision_params_from_entity(other->cols[Collider_Body], other->vel),
                  collision_params_from_entity(en->cols[Collider_Hit], en->vel)))
            {
              spawn_particles(ParticleKind_Blood, pos_from_entity(en));
              damage_entity(other, en->damage);
              kill_entity(en, TRUE);
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
          if (!en->colliding_with_player)
          {
            // Collision enter
            en->colliding_with_player = TRUE;
            damage_entity(player, en->damage);
            timer_start(&en->invincibility_timer, en->invincibility_timer.duration);
          }
          else
          {
            if (!en->attack_timer.ticking)
            {
              timer_start(&en->attack_timer, en->attack_timer.duration);
            }
          }
          
          if (timer_timeout(&en->attack_timer) && timer_timeout(&player->invincibility_timer))
          {
            en->attack_timer.ticking = FALSE;
            player->invincibility_timer.ticking = FALSE;

            damage_entity(player, en->damage);
          }
        }
        else
        {
          en->colliding_with_player = FALSE;
          en->attack_timer.ticking = FALSE;
        }
      }

      // Item vs Player collision
      if (en->type == EntityType_Collectable && entity_is_valid(player))
      {
        if (p_rect_circle_intersect(
              collision_params_from_entity(player->cols[Collider_Body], player->vel),
              collision_params_from_entity(en->cols[Collider_Hit], en->vel)))
        {
          if (en->item_kind == CollectableKind_Coin)
          {
            spawn_particles(ParticleKind_PickupCoin, pos_from_entity(en));
            game.coin_count++;
          }
          else if (en->item_kind == CollectableKind_Soul)
          {
            spawn_particles(ParticleKind_PickupSoul, pos_from_entity(en));
            game.soul_count++;
          }
          
          kill_entity(en, TRUE);
        }
      }
    }
  }

  // - Update entity combat ---
  for (EN_IN_ENTITIES)
  {
    if (!en->is_active) continue;

    // Update player invinsibility timer
    if (en->sp == SPID_Player)
    {
      if (!en->invincibility_timer.ticking)
      {
        timer_start(&en->invincibility_timer, en->invincibility_timer.duration);
      }
    }

    if (entity_has_prop(en, EntityProp_Controlled))
    {
      if (en->is_weapon_equipped)
      {
        if (!en->attack_timer.ticking)
        {
          timer_start(&en->attack_timer, en->attack_timer.duration);
        }

        bool can_shoot = is_key_pressed(Key_Mouse1) && 
                         timer_timeout(&en->attack_timer) &&
                         game.weapon.ammo_remaining > 0 &&
                         !game.weapon.is_reloading;

        if (can_shoot)
        {
          en->attack_timer.ticking = FALSE;

          Entity *gun = get_entity_child_of_sp(en, SPID_Gun);
          Entity *shot_point = get_entity_child_at(gun, 0);
          Vec2F spawn_pos = pos_from_entity(shot_point);
          f32 spawn_rot = en->flip_x ? -gun->rot + 180 : gun->rot;
          Entity *ammo = spawn_ammo(prefab.weapon[gun->weapon_kind].ammo_kind, spawn_pos);
          ammo->rot = spawn_rot;
          ammo->speed = gun->speed;
          ammo->damage = gun->damage;

          Entity *muzzle_flash = get_entity_child_at(gun, 1);
          muzzle_flash->pos = shot_point->pos;
          if (gun->weapon_kind == WeaponKind_LaserPistol)
          {
            muzzle_flash->sprite = prefab.sprite.laser_flash;
          }
          else
          {
            muzzle_flash->sprite = prefab.sprite.muzzle_flash;
          }

          if (!muzzle_flash->muzzle_flash_timer.ticking)
          {
            timer_start(&muzzle_flash->muzzle_flash_timer, 0.08f);
            entity_add_prop(muzzle_flash, EntityProp_Renders);

            entity_add_prop(gun, EntityProp_DistortScaleX);
            gun->distort_x.state = 0;
            gun->distort_x.rate = 4.0f;
            gun->distort_x.scale = 0.75f;
            gun->distort_x.saved = 1.0f;
          }

          if (gun->weapon_kind != WeaponKind_LaserPistol)
          {
            spawn_particles(ParticleKind_Smoke, spawn_pos);
          }
            
          game.weapon.ammo_remaining -= 1;
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
            if (!en->attack_timer.ticking)
            {
              timer_start(&en->attack_timer, en->attack_timer.duration);
            }
            else if (timer_timeout(&en->attack_timer))
            {
              en->attack_timer.ticking = FALSE;

              Vec2F spawn_pos = v2f(en->pos.x, en->pos.y);
              Entity *ammo = spawn_ammo(AmmoKind_Laser, spawn_pos);
              ammo->tint = DEBUG_GREEN;
              ammo->rot = en->rot;
              ammo->speed = 700.0f;
            }
          }
          break;
          default: break;
        }
      }
    }
  }

  // - Animate entities ---
  for (EN_IN_ENTITIES)
  {
    if (entity_has_prop(en, EntityProp_BobsOverTime))
    {
      if (en->pos.y > en->bobbing.range.e[1] - 0.1f)
      {
        en->bobbing.state = -1;
      }

      if (en->pos.y < en->bobbing.range.e[0] + 0.1f)
      {
        en->bobbing.state = 1;
      }
      
      en->pos.y += 10 * dt * en->bobbing.state;
    }

    if (entity_has_prop(en, EntityProp_DistortScaleX))
    {
      if (en->distort_x.state == 0)
      {
        en->scale.x -= dt * en->distort_x.rate;

        if (en->scale.x <= en->distort_x.saved * en->distort_x.scale)
        {
          en->scale.x = en->distort_x.saved * en->distort_x.scale;
          en->distort_x.state = 1;
        }
      }

      if (en->distort_x.state == 1)
      {
        en->scale.x += dt * en->distort_x.rate;

        if (en->scale.x >= en->distort_x.saved)
        {
          en->scale.x = en->distort_x.saved;
          en->distort_x.state = 0;
          entity_rem_prop(en, EntityProp_DistortScaleX);
        }
      }
    }

    if (entity_has_prop(en, EntityProp_DistortScaleY))
    {
      if (en->distort_y.state == 0)
      {
        en->scale.y -= dt * en->distort_y.rate;

        if (en->scale.y <= en->distort_y.saved * en->distort_y.scale)
        {
          en->scale.y = en->distort_y.saved * en->distort_y.scale;
          en->distort_y.state = 1;
        }
      }

      if (en->distort_y.state == 1)
      {
        en->scale.y += dt * en->distort_y.rate;

        if (en->scale.y >= en->distort_y.saved)
        {
          en->scale.y = en->distort_y.saved;
          en->distort_y.state = 0;
          entity_rem_prop(en, EntityProp_DistortScaleY);
        }
      }
    }

    if (entity_has_prop(en, EntityProp_FlashWhite))
    {
      en->flash_timer.duration = FLASH_TIME;

      if (!en->flash_timer.ticking)
      {
        timer_start(&en->flash_timer, en->flash_timer.duration);
      }

      if (timer_timeout(&en->flash_timer))
      {
        en->flash_timer.ticking = FALSE;

        entity_rem_prop(en, EntityProp_FlashWhite);
      }
    }

    if (entity_has_prop(en, EntityProp_HideAfterTime))
    {
      if (timer_timeout(&en->muzzle_flash_timer))
      {
        en->muzzle_flash_timer.ticking = FALSE;
        
        entity_rem_prop(en, EntityProp_Renders);
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
    en->sprite = anim_desc.frames[en->anim.frame_idx];

    // Tick animation
    if (en->anims[en->anim_state].frame_count > 1)
    {
      en->anim.tick_counter += ANIM_TICK;

      if (en->anim.tick_counter % anim_desc.ticks_per_frame == 0)
      {
        en->anim.frame_idx += 1;

        if (en->anim.frame_idx == anim_desc.frame_count)
        {
          en->anim.frame_idx = 0;
        }
      }
    }
  }

  // - Update particles ---
  for (i32 i = 0; i < MAX_PARTICLES; i++)
  {
    Particle *particle = &game.particle_buffer.data[i];

    if (!particle->is_active) continue;

    Entity *owner = entity_from_ref(particle->owner);
    ParticleDesc desc = owner->particle_desc;

    switch (desc.emmission_type)
    {
    case ParticleEmmissionType_Burst:
      if (has_prop(desc.props, ParticleProp_VariateColor))
      {
        particle->color = lerp_4f(particle->color, desc.color_secondary, dt);
      }

      if (has_prop(desc.props, ParticleProp_ScaleOverTime))
      {
        particle->scale = add_2f(particle->scale, scale_2f(desc.scale_delta, dt));
        particle->scale.x = clamp_bot(particle->scale.x, 0.0f);
        particle->scale.y = clamp_bot(particle->scale.y, 0.0f);
      }

      if (has_prop(desc.props, ParticleProp_SpeedOverTime))
      {
        particle->speed += desc.speed_delta * dt * dt;
        particle->speed = clamp_bot(particle->speed, 0.0f);
      }

      if (has_prop(desc.props, ParticleProp_RotateOverTime))
      {
        particle->rot += desc.rot_delta * dt * dt;
      }

      if (has_prop(desc.props, ParticleProp_AffectedByGravity))
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

      if (has_prop(desc.props, ParticleProp_CollidesWithGround))
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

      if (has_prop(desc.props, ParticleProp_KillAfterTime))
      {
        if (!owner->particle_timer.ticking)
        {
          timer_start(&owner->particle_timer, desc.duration);
        }

        if (timer_timeout(&owner->particle_timer))
        {
          particle->is_active = FALSE;
          owner->particles_killed += 1;

          if (owner->particles_killed == desc.count)
          {
            kill_entity(owner, TRUE);
          }
        }
      }
      else
      {
        if (game.wave_just_began)
        {
          particle->is_active = FALSE;
          owner->particles_killed += 1;
          
          if (owner->particles_killed == desc.count)
          {
            kill_entity(owner, TRUE);
          }
        }
      }
    case ParticleEmmissionType_Linear:
      // ...
      break;
    }
  }

  if (game.is_so_over)
  {
    // FIXME(dg): center these alignments
    if (game.won)
    {
      ui_text(str("YOU WIN!"), v2f(WIDTH/2 - 150, HEIGHT/2), 50, 999);
    }
    else
    {
      ui_text(str("GAME OVER!"), v2f(WIDTH/2 - 150, HEIGHT/2), 50, 999);
    }
  }

  // - Event queue ---
  for (Event *ev = peek_event(); game.event_queue.count != 0; pop_event())
  {
    assert(ev);
    switch (ev->type)
    {
    case EventType_Nil: break;
    case EventType_EntityKilled:
      {}
      Entity *en = ev->desc.en;
      if (!entity_is_valid(en)) continue;

      if (ev->desc.type == EntityType_Player)
      {
        game.is_so_over = TRUE;
        logger_debug(str("Player has been killed.\n"));
      }

      if (en->type == EntityType_Zombie && ev->desc.slain)
      {
        game.current_wave.zombies_killed += 1;

        CollectableKind kind = CollectableKind_Nil;
        i32 roll = random_i32(1, 100);
        if (roll <= prefab.collectable[CollectableKind_Soul].draw_chance)
        {
          kind = CollectableKind_Soul;
        }
        else if (roll <= prefab.collectable[CollectableKind_Coin].draw_chance)
        {
          kind = CollectableKind_Coin;
        }

        // Spawn drop
        if (kind != CollectableKind_Nil)
        {
          spawn_collectable(kind, v2f(en->pos.x, GROUND_Y + (4 * SPRITE_SCALE)));
        }
      }
    break;
    }
  }

  // - HUD ---
  {
    // - Hearts ---
    {
      for (i32 heart_idx = 1; heart_idx <= PLAYER_HEALTH; heart_idx++)
      {
        Sprite sprite;
        if (heart_idx <= player->health)
        {
          sprite = prefab.sprite.ui_heart_full;
        }
        else
        {
          sprite = prefab.sprite.ui_heart_empty;
        }

        ui_rect(v2f((45 * heart_idx) - 45, HEIGHT - 75), 
                v2f(14 * SPRITE_SCALE, 14 * SPRITE_SCALE), 
                *(UI_Sprite *) &sprite);
      }
    }

    // - Ammo ---
    {
      ui_rect(v2f(0, HEIGHT-130), 
              v2f(14*SPRITE_SCALE, 14*SPRITE_SCALE), 
              *(UI_Sprite *) &prefab.sprite.ui_ammo);
      ui_text(str("%i"), v2f(65, HEIGHT-110), 30, 999, game.weapon.ammo_remaining);
    }

    // - Collectables ---
    {
      ui_rect(v2f(0, 65), 
              v2f(14*SPRITE_SCALE, 14*SPRITE_SCALE), 
              *(UI_Sprite *) &prefab.sprite.coin);
      ui_text(str("%i"), v2f(65, 85), 30, 999, game.coin_count);

      ui_rect(v2f(0, 15), 
              v2f(14*SPRITE_SCALE, 14*SPRITE_SCALE), 
              *(UI_Sprite *) &prefab.sprite.soul);
      ui_text(str("%i"), v2f(65, 35), 30, 999, game.soul_count);
    }

    ui_text(str("%.0f"), v2f(WIDTH/2 - 20, HEIGHT-50), 25, 999, game.time_alive);
    ui_text(str("Wave %i"), v2f(WIDTH-150, HEIGHT-50), 25, 999, game.current_wave.num+1);
  }

  if (global.debug)
  {
    String duration_str;

    duration_str = format_duration(game.update_time, &game.frame_arena);
    duration_str = str_concat(str("update: "), duration_str, &game.frame_arena);
    duration_str = str_concat(duration_str, str("\n"), &game.frame_arena);
    ui_text(duration_str, v2f(WIDTH - 150, HEIGHT - 75), 15, 999);

    duration_str = format_duration(game.render_time, &game.frame_arena);
    duration_str = str_concat(str("render: "), duration_str, &game.frame_arena);
    duration_str = str_concat(duration_str, str("\n"), &game.frame_arena);
    ui_text(duration_str, v2f(WIDTH - 150, HEIGHT - 100), 15, 999);
  }

  // - Developer tools ----------------
  {
    // - Toggle debug ---
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

    // - Kill player ---
    if (is_key_just_pressed(Key_Backspace) && entity_is_valid(player))
    { 
      kill_entity(player, TRUE);
    }

    // - Spawn particles --- 
    // if (is_key_pressed(Key_P) && entity_is_valid(player))
    // {
    //   spawn_particles(ParticleKind_Debug, player->pos);
    // }

    // - Unlock all progression ---
    if (is_key_just_pressed(Key_P) && entity_is_valid(player))
    {
      game.progression.rifle = TRUE;
      game.progression.shotgun = TRUE;  
      game.progression.smg = TRUE;
    }
  }

  zero(*NIL_ENTITY, Entity);
  arena_clear(&game.frame_arena);
}

void render_game(void)
{
  clear_frame(V4F_ZERO);

  // - Draw scene ----------------
  draw_scene(v2f(0, 0), scale_2f(v2f(192, 108), SPRITE_SCALE), v4f(1, 1, 1, 1));

  // - Draw sprite batch ----------------
  for (EN_IN_ENTITIES)
  {
    if (!en->is_active || en->draw_type != DrawType_Sprite) continue;

    if (entity_has_prop(en, EntityProp_Renders))
    {
      bool flash = entity_has_prop(en, EntityProp_FlashWhite);
      draw_sprite_x(en->xform, en->tint, en->sprite, flash);
    }
  }
  
  // - Draw primitive batch ---
  {
    // Draw entities
    for (EN_IN_ENTITIES)
    {
      if (!en->is_active || en->draw_type != DrawType_Primitive) continue;

      if (entity_has_prop(en, EntityProp_Renders))
      {
        if (en->type == EntityType_Collider && global.debug)
        {
          Vec4F color = en->tint;
          switch (en->col_id)
          {
          case Collider_Body: 
          case Collider_Head: 
            color = v4f(0, 1, 0, 0.35f); 
            break;
          case Collider_Hit:  
            color = v4f(1, 0, 0, 0.35f); 
            break;
          default:            
            color = v4f(1, 1, 1, 0.35f); 
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

    // Draw particles
    for (i32 i = 0; i < MAX_PARTICLES; i++)
    {
      Particle *particle = &game.particle_buffer.data[i];
      
      if (!particle->is_active) continue;

      f32 rot = particle->rot * RADIANS;
      draw_rectangle(particle->pos, particle->scale, rot, particle->color);
    }
  }

  // - Draw UI batch ---
  UI_WidgetStore *widgets = ui_get_widgetstore();
  {
    for (u64 wdgt_idx = 0; wdgt_idx < widgets->count; wdgt_idx++)
    {
      const f32 scale = 1.0f/8;

      UI_Widget widget = widgets->data[wdgt_idx];
      switch (widget.type)
      {
      case UI_WidgetType_Nil:
        break;
      case UI_WidgetType_Rect:
        draw_sprite(widget.pos, widget.dim, 0, v4f(1, 1, 1, 1), *(Sprite *) &widget.sprite, FALSE);
        break;
      case UI_WidgetType_Text:
        {}
        Vec2F offset = V2F_ZERO;
        u32 word_start_pos = 0;

        u32 len = widget.text.len;
        for (u32 chr_idx = 0; chr_idx < len; chr_idx++)
        {
          char chr = widget.text.data[chr_idx];
          if (((chr < 65 || chr > 90) && (chr < 97 || chr > 122)) || chr_idx == len-1)
          {
            UI_Glyph glyph = get_glyph(chr);

            // Calculate word length
            f32 word_len = 0;
            for (u32 i = word_start_pos; i <= chr_idx; i++)
            {
              chr = widget.text.data[i];
              word_len += (widget.text_size * (glyph.dim.width * scale)) + 
                          (widget.text_spacing.x * (widget.text_size * scale));
            }

            // Move to next line
            if (offset.x + word_len > widget.dim.width && chr_idx < len)
            {
              offset.x = 0;
              offset.y -= (widget.text_spacing.y * (widget.text_size * scale)) + 
                          widget.text_size;

              word_len += (glyph.dim.width + glyph.off.x + widget.space_width) * 
                          widget.text_size;
            }

            // Draw the word
            for (u32 i = word_start_pos; i <= chr_idx; i++)
            {
              chr = widget.text.data[i];
              if (chr != ' ')
              {
                UI_Glyph glyph = get_glyph(chr);

                Vec2F draw_pos = add_2f(add_2f(widget.pos, 
                                        scale_2f(glyph.off, widget.text_size*scale)), 
                                        offset);
                draw_glyph(draw_pos, widget.text_size, R_WHITE, glyph.coords);

                offset.x += (widget.text_size * (glyph.dim.width * scale)) + 
                            (widget.text_spacing.x * (widget.text_size * scale));
              }
              else
              {
                offset.x += widget.space_width * widget.text_size * scale;
              }
            }
            
            word_start_pos = chr_idx + 1;
          }
        }
        break;
      }
    }
  }

  r_flush(&global.renderer);
  arena_clear(&game.draw_arena);
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

bool is_zombie_remaining_to_spawn(WaveDesc *desc)
{
  bool result = FALSE;

  for (i32 i = 0; i < ZombieKind_COUNT; i++)
  {
    if (desc->zombie_counts[i] != 0)
    {
      result = TRUE;
      break;
    }
  }

  return result;
}

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
  queue->front = NULL;

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

String format_duration(u64 ns, Arena *arena)
{
  String result;
  cstr unit = "ns";
  f64 duration = ns;

  if (ns >= 1000000000)
  {
    duration /= 1000000000;
    unit = "s";
  }
  else if (ns >= 1000000)
  {
    duration /= 1000000;
    unit = "ms";
  }
  else if (ns >= 1000)
  {
    duration /= 1000;
    unit = "us";
  }

  char *buf = arena_push(arena, char, 64);
  i32 len = stbsp_snprintf(buf, 64, "%0.0f %s", duration, unit);
  result = (String) {buf, len};

  return result;
}
