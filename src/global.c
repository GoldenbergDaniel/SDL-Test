#include "base/base_inc.h"

#include "global.h"

void init_prefabs(PrefabStore *prefabs)
{
  *prefabs = (PrefabStore) {
    .smoke_particles = (ParticleDesc) {
      .emmission_type = ParticleEmmissionType_Burst,
      .props = ParticleProp_ScaleOverTime |
               ParticleProp_SpeedOverTime |
               ParticleProp_VariateColor |
               ParticleProp_RotateOverTime,
      .count = 3,
      .duration = 1.5f,
      .spread = 180.0f,
      .color_primary = v4f(1.0f, 0.65f, 0.0f, 1.0f),
      .color_secondary = v4f(0.1f, 0.1f, 0.1f, 1.0f),
      .scale = v2f(9, 9),
      .scale_delta = v2f(-0.05f, -0.05f),
      .speed = 0.7f,
      .speed_delta = -0.007f,
      .rot_delta = 3.0f,
    },
    .blood_particles = (ParticleDesc) {
      .emmission_type = ParticleEmmissionType_Burst,
      .color_primary = v4f(0.47f, 0.13f, 0.13f, 1.0f),
      .scale = v2f(5, 5),
      .count = 6,
      .duration = 0.3f,
      .speed = 2.0f,
      .spread = 180.0f,
    }
  };
}
