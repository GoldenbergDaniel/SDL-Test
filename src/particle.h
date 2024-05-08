#include "base/base_common.h"
#include "base/base_math.h"

typedef enum ParticleProp
{
  ParticleProp_AffectedByGravity,
} ParticleProp;

typedef struct Particle Particle;
struct Particle
{
  Vec2F pos;
  f32 rot;
  b8 props;
};

typedef struct ParticleParams ParticleParams;
struct ParticleParams
{
  Vec4F color;
  i32 count;
  f32 speed;
  f32 duration;
};
