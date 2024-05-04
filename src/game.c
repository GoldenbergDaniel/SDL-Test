#include <stdio.h>

#include "base/base_inc.h"

#include "draw/draw.h"
#include "input/input.h"
#include "phys/physics.h"
#include "entity.h"
#include "global.h"
#include "game.h"

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

void clear_event_queue(Game *game)
{
  EventQueue *queue = &game->event_queue;
  Event *curr = queue->front;

  while (curr != NULL)
  {
    Event *next = curr->next;
    zero(*curr, Event);
    curr = next;
  }

  zero(*queue, EventQueue);
}

// @Init ////////////////////////////////////////////////////////////////////////////

void init_game(Game *game)
{
  game->camera = translate_3x3f(0.0f, 0.0f);

  SCOPE("Starting entities")
  {
    Entity *ground = create_entity(game, EntityType_Wall);
    ground->pos = v2f(0.0f, 0.0f);
    ground->scale = v2f(512.0f, 24.0f);
    ground->color = v4f(0.7f, 0.6f, 0.4f, 1.0f);
    ground->visible = TRUE;

    Entity *player = create_entity(game, EntityType_Player);
    player->id = 1;
    player->pos = v2f(WIDTH/2.0f, HEIGHT/2.0f + 50.0f);

    Entity *gun = create_entity(game, EntityType_Equipped);
    attach_entity_child(player, gun);
    gun->pos = v2f(48.0f, 0.0f);

    Entity *shot_point = create_entity(game, EntityType_Debug);
    attach_entity_child(gun, shot_point);
    shot_point->pos = v2f(32.0f, 0.0f);
    shot_point->scale = v2f(.1, .1);
    shot_point->visible = FALSE;

    Entity *zombie = create_entity(game, EntityType_ZombieWalker);
    zombie->pos = v2f(WIDTH - 300.0f, HEIGHT/2.0f + 50.0f);
  }
}

// @Update //////////////////////////////////////////////////////////////////////////

void update_game(Game *game)
{
  game->is_sim_started = TRUE;

  Entity *player = get_entity_of_id(game, 1);
  Vec2F mouse_pos = screen_to_world(get_mouse_pos());
  f64 dt = game->dt;

  // printf("%f, %f\n", mouse_pos.x, mouse_pos.y);
  
  for (Entity *en = game->entities.head; en; en = en->next)
  {
    if (!en->active) continue;

    // Handle entitiy movement ----------------
    if (en->props & EntityProp_Moves)
    {
      if (en->props & EntityProp_Controlled)
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

        // Apply gravity
        if (!en->grounded)
        {
          en->new_vel.y -= GRAVITY * dt;
        }

        if (is_key_pressed(KEY_W) && en->grounded)
        {
          en->new_vel.y += PLAYER_JUMP_VEL;
          en->grounded = FALSE;
        }

        en->vel = en->new_vel;
        en->pos = add_2f(en->pos, en->vel);
      }
      else
      {
        Vec2F player_pos = pos_from_entity(player);
        entity_look_at(en, player_pos);

        switch (en->move_type)
        {
          case MoveType_Walking:
          {
            // Apply gravity
            if (!en->grounded)
            {
              en->vel.y -= GRAVITY * dt;
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
              en->vel.x += PLAYER_ACC * dir(en->input_dir.x) * dt;
              f32 bound = en->speed * abs(en->input_dir.x) * dt;
              en->vel.x = clamp(en->vel.x, -bound, bound);
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
              f32 bound = en->speed * abs(en->input_dir.y) * dt;
              en->vel.y = clamp(en->vel.y, -bound, bound);
            }
            else
            {
              en->vel.y = lerp_1f(en->vel.y, 0.0f, PLAYER_FRIC * dt);
              en->vel.y = to_zero(en->vel.y, 0.1f);
            }
          }
          case MoveType_Projectile:
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
          break;
          default: break;
        }

        en->pos = add_2f(en->pos, en->vel);
      }


      // Handle entity wrapping ----------------
      if (en->props & EntityProp_WrapAtEdge)
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
    if (en->props & EntityProp_Rendered)
    {
      Mat3x3F xform = m3x3f(1.0f);
      Entity *parent = entity_from_ref(en->parent);

      Vec2F offset;

      // Offset position based on origin
      {
        // (@dg): please rewrite this
        Vec2F mult = div_2f(dim_from_entity(en), en->scale);
        // Vec2F mult = en->dim;
        offset = v2f(mult.x * en->origin.x, mult.y * en->origin.y);
        xform = mul_3x3f(translate_3x3f(-offset.x, -offset.y), xform);
      }

      // Scale
      xform = mul_3x3f(scale_3x3f(en->scale.x, en->scale.y), xform);
      if (en->flip_x) xform = mul_3x3f(scale_3x3f(-1.0f, 1.0f), xform);
      if (en->flip_y) xform = mul_3x3f(scale_3x3f(1.0f, -1.0f), xform);

      // Rotate
      xform = mul_3x3f(rotate_3x3f(en->rot * RADIANS), xform);

      // Move offset position back
      {
        Vec2F mult = mul_2f(offset, en->scale);
        xform = mul_3x3f(translate_3x3f(mult.x, mult.y), xform);
      }

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

    // Handle entity collision ----------------
    if (en->props & EntityProp_Collides)
    {
      Vec2F dim = dim_from_entity(en);

      // Update entity colliders ----------------
      {
        Vec2F pos = pos_from_entity(en);
        en->body_col.pos = v2f(pos.x - dim.width * 0.5f, pos.y + dim.height * 0.5f);
      }

      if (en->type == EntityType_Player || en->type == EntityType_ZombieWalker)
      {
        f32 ground_y = 0.0f;
        for (Entity *other = game->entities.head; other; other = other->next)
        {
          if (other->type == EntityType_Wall)
          {
            ground_y = pos_from_entity(other).y + dim_from_entity(other).height;
          }
        }

        P_CollisionParams params = {.collider=en->body_col, .vel=en->vel};
        if (p_rect_y_range_intersect(params, v2f(-3000.0f, 3000.0f), ground_y))
        {
          en->pos.y = ground_y + dim.height * 0.5f;
          en->vel.y = 0.0f;
          en->new_vel.y = 0.0f;
          en->grounded = TRUE;
        }
        else
        {
          en->grounded = FALSE;
        }
      }
    }

    // Handle entity combat ----------------
    if (en->props & EntityProp_Combatant)
    {
      Timer *timer = get_timer(en, TIMER_COMBAT);

      if (en->props & EntityProp_Controlled)
      {
        if (timer->should_tick)
        {
          tick_timer(timer, game->dt);
        }

        // Shoot weapon if can shoot
        if (is_key_pressed(KEY_MOUSE_1) && (timer->timeout || !timer->should_tick))
        {
          Entity *gun = get_entity_child_at(en, 0);
          Entity *shot_point = get_entity_child_at(gun, 0);
          Vec2F spawn_pos = pos_from_entity(shot_point);
          f32 spawn_rot = en->flip_x ? -gun->rot + 180 : gun->rot;

          Entity *laser = spawn_entity(game, EntityType_Laser, .pos=spawn_pos);
          laser->rot = spawn_rot;
          laser->speed = 100.0f;

          timer->should_tick = TRUE;
        }
      }
      else
      {
        if (is_entity_valid(player) && player->active)
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
              if (timer->should_tick)
              {
                tick_timer(timer, game->dt);
              }

              // Shoot weapon if can shoot
              if (timer->timeout)
              {
                Vec2F spawn_pos = v2f(en->pos.x, en->pos.y);

                Entity *laser = spawn_entity(game, EntityType_Laser, .pos=spawn_pos);
                laser->color = D_GREEN;
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

    // Handle equipped entity ----------------
    if (en->props & EntityProp_Equipped)
    {
      Vec2F entity_pos = pos_from_entity(en);
      Vec2F diff = sub_2f(mouse_pos, entity_pos);
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

    cleanup:
    {

    }
  }

  SCOPE("Developer")
  {
    // Kill player on backspace
    if (is_key_just_pressed(KEY_BACKSPACE))
    { 
      kill_entity(game, .entity=player);
    }
  }
}

// @HandleEvents ////////////////////////////////////////////////////////////////////

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

// @Draw ////////////////////////////////////////////////////////////////////////////

void draw_game(Game *game, Game *prev)
{
  if (!game->is_sim_started)
  {
    d_clear_frame(V4F_ZERO);
    return;
  }
  
  d_clear_frame(v4f(0.34f, 0.44f, 0.47f, 1.0f));

  // Batch sprites ----------------
  for (Entity *en = game->entities.head; en; en = en->next)
  {
    if (en->draw_type == DrawType_Sprite && en->visible)
    {
      d_draw_sprite_x(en->xform, en->color, en->texture);
    }
  }
  
  // Batch primitives ----------------
  for (Entity *en = game->entities.head; en; en = en->next)
  {
    if (en->draw_type == DrawType_Rectangle && en->visible)
    {
      d_draw_rectangle_x(en->xform, en->color);
    }
  }

  r_flush(&GLOBAL->renderer);
}

inline
bool game_should_quit(Game *game)
{
  bool result = FALSE;
  
  if (game->should_quit || is_key_pressed(KEY_ESCAPE))
  {
    result = TRUE;
  }

  return result;
}

void copy_game_state(Game *game, Game *prev)
{
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
