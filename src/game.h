#pragma once

#include "base/base.h"
#include "input.h"
#include "entity.h"

#if defined(PLATFORM_LINUX) || defined(PLATFORM_WINDOWS)
  #define TIME_STEP (1.0f / 120)
  #define ANIM_TICK 1
#else
  #define TIME_STEP (1.0f / 60)
  #define ANIM_TICK 2
#endif

#define WIDTH 960.0f
#define HEIGHT 540.0f
#define SPRITE_SCALE 5
#define GROUND_Y (30 * SPRITE_SCALE)
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
  bool slain;
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

#define MAX_PARTICLES 1024

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

bool is_zombie_remaining_to_spawn(WaveDesc *desc);

typedef enum GameState
{
  GameState_GracePeriod,
  GameState_ZombieWave,
  GameState_SoOver,
} GameState;

typedef struct Game Game;
struct Game
{
  EntityList entities;
  EventQueue event_queue;
  ParticleBuffer particle_buffer;

  u64 update_time;
  u64 render_time;
  f64 t;
  f64 dt;
  Mat3x3F camera;
  bool should_quit;
  bool won;
  
  bool is_ui_hovered;
  bool just_entered_wave;
  bool just_entered_grace;
  Timer grace_period_timer;
  Timer spawn_timer;
  f64 time_alive;
  u16 coin_count;
  u16 soul_count;
  EntityGender player_gender;
  GameState state;

  struct
  {
    i16 num;
    u16 zombies_spawned;
    u16 zombies_killed;
    WaveDesc desc;
  } current_wave;

  struct
  {
    i8 shot_count;
    i16 ammo_remaining;
    i16 ammo_reserved;
    Timer reload_timer;
    bool is_reloading;
  } weapon;

  struct
  {
    bool weapon_unlocked[WeaponKind_COUNT]; 
  } progression;

  Arena frame_arena;
  Arena draw_arena;
  Arena entity_arena;
};

void init_game(void);
void update_game(void);
void render_game(void);
bool game_should_quit(void);

Vec2F screen_to_world(Vec2F pos);
String format_duration(u64 ns, Arena *arena);
