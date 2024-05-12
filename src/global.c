#include "base/base_inc.h"

#include "global.h"

void init_particle_prefabs(PrefabStore *prefabs)
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
      .color_primary = v4f(0.55f, 0.55f, 0.55f, 1.0f),
      .color_secondary = v4f(0.1f, 0.1f, 0.1f, 1.0f),
      .scale = v2f(9, 9),
      .scale_delta = v2f(-0.08f, -0.08f),
      .speed = 0.7f,
      .speed_delta = -0.007f,
      .rot_delta = 2.0f,
    },

    .blood_particles = (ParticleDesc) {
      .emmission_type = ParticleEmmissionType_Burst,
      .props = ParticleProp_ScaleOverTime |
               ParticleProp_RotateOverTime,
      .count = 6,
      .duration = 0.3f,
      .spread = 180.0f,
      .color_primary = v4f(0.47f, 0.13f, 0.13f, 1.0f),
      .scale = v2f(5, 5),
      .scale_delta = v2f(-0.1f, -0.1f),
      .speed = 2.0f,
      .rot_delta = 5.0f,
    },

    .death_particles = (ParticleDesc) {
      .emmission_type = ParticleEmmissionType_Burst,
      .props = ParticleProp_VariateColor |
               ParticleProp_AffectedByGravity |
               ParticleProp_CollidesWithGround,
      .count = 20,
      .duration = 5.0f,
      .spread = 180.0f,
      .color_primary = v4f(0.57f, 0.23f, 0.23f, 1.0f),
      .color_secondary = v4f(0.37, 0.0f, 0.0f, 1.0f),
      .scale = v2f(8, 8),
      .speed = 2.0f,
      .velocity_initial = v2f(0, 5.0f),
    },

    .debug_particles = (ParticleDesc) {
      .emmission_type = ParticleEmmissionType_Burst,
      .props = ParticleProp_ScaleOverTime |
               ParticleProp_SpeedOverTime |
               ParticleProp_VariateColor,
      .scale = v2f(20, 20),
      .count = 100,
      .duration = 2.0f,
      .speed = 1.0f,
      .spread = 180.0f,
      .color_primary = v4f(0.5, 0.5, 0.5, 1),
      .color_secondary = D_BLACK,
      .scale_delta = v2f(-0.05f, -0.05f),
      .speed_delta = 0.05f,
    },
  };
}
