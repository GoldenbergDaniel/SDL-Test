#pragma once

#include "base/base.h"
#include "input.h"
#include "entity.h"

#ifdef _WIN32
#define TIME_STEP (1.0f / 120)
#define ANIM_TICK 1
#else
#define TIME_STEP (1.0f / 60)
#define ANIM_TICK 2
#endif

#define WIDTH 960.0f
#define HEIGHT 540.0f
#define SPRITE_SCALE 5

#define GRAVITY 3600.0f

// @Event ////////////////////////////////////////////////////////////////////////////////

typedef enum EventType
{
  EventType_Nil,
  EventType_EntityKilled,
} EventType;

typedef struct EventDesc EventDesc;
struct EventDesc
{
  Entity *en;
  u64 id;
  u64 type;
  b64 props;
};

typedef struct Event Event;
struct Event
{
  Event *next;
  EventType type;
  EventDesc desc;
};

typedef struct EventQueue EventQueue;
struct EventQueue
{
  Event *front;
  Event *back;
  u64 count;
};

void push_event(EventType type, EventDesc desc);
void pop_event(void);
Event *peek_event(void);

// @Globals //////////////////////////////////////////////////////////////////////////////

typedef struct Globals Globals;
struct Globals
{
  Arena perm_arena;

  Input input;
  Resources resources;
  R_Renderer renderer;
  Vec2F window;
  Vec4F viewport;
  bool debug;

  struct
  {
    f64 current_time;
    f64 elapsed_time;
    f64 accumulator;
  } frame;
};

// @Game /////////////////////////////////////////////////////////////////////////////////

#define MAX_PARTICLES 2048

typedef struct ParticleBuffer ParticleBuffer;
struct ParticleBuffer
{
  Particle data[MAX_PARTICLES];
  u32 pos;
};

Particle *get_next_free_particle(void);

#define TOTAL_WAVE_COUNT 5

typedef struct WaveDesc WaveDesc;
struct WaveDesc
{
  f32 time_btwn_spawns;
  u16 zombie_counts[ZombieKind_COUNT];
};

i32 zombies_to_spawn_this_wave(void);

typedef struct Game Game;
struct Game
{
  Arena frame_arena;
  Arena draw_arena;
  Arena entity_arena;

  EntityList entities;
  EventQueue event_queue;
  ParticleBuffer particle_buffer;

  f64 t;
  f64 dt;
  Mat3x3F camera;
  bool should_quit;
  bool is_so_over;
  bool won;
  
  bool is_grace_period;
  bool wave_just_began;
  Timer grace_period_timer;
  Timer spawn_timer;
  f64 time_alive;

  u16 coin_count;
  u16 soul_count;

  struct
  {
    u8 num;
    u16 zombies_spawned;
    u16 zombies_killed;
  } current_wave;
};

void init_game(void);
void update_game(void);
void render_game(void);
bool game_should_quit(void);

Vec2F screen_to_world(Vec2F pos);
