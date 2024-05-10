#include "base/base_inc.h"

#include "global.h"

void init_prefabs(PrefabStore *prefabs)
{
  *prefabs = (PrefabStore) 
  {
    .smoke_particles = (ParticleDesc) {
      .emmission_type = ParticleEmmissionType_Burst,
      .color = v4f(0.2f, 0.2f, 0.17f, 1.0f),
      .scale = v2f(5, 5),
      .count = 3,
      .duration = 1.0f,
      .speed = 0.7f,
      .spread = 180.0f,
    },
    .blood_particles = (ParticleDesc) {
      .emmission_type = ParticleEmmissionType_Burst,
      .color = D_RED,
      .scale = v2f(5, 5),
      .count = 6,
      .duration = 0.3f,
      .speed = 2.0f,
      .spread = 180.0f,
    }
  };
}
