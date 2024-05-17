#include "base/base_inc.h"
#include "global.h"

void init_particle_prefabs(PrefabStore *prefabs)
{
  *prefabs = (PrefabStore) {
    .particle.smoke = (ParticleDesc) {
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
      .scale_delta = v2f(-8.0f, -8.0f),
      .speed = 60.0f,
      .speed_delta = -4000.0f,
      .rot_delta = 20.0f,
    },

    .particle.blood = (ParticleDesc) {
      .emmission_type = ParticleEmmissionType_Burst,
      .props = ParticleProp_ScaleOverTime |
               ParticleProp_RotateOverTime,
      .count = 6,
      .duration = 0.3f,
      .spread = 180.0f,
      .color_primary = v4f(0.47f, 0.13f, 0.13f, 1.0f),
      .scale = v2f(5, 5),
      .scale_delta = v2f(-6.0f, -6.0f),
      .speed = 60.0f,
      .rot_delta = 50.0f,
    },

    .particle.death = (ParticleDesc) {
      .emmission_type = ParticleEmmissionType_Burst,
      .props = ParticleProp_VariateColor |
              //  ParticleProp_AffectedByGravity |
               ParticleProp_CollidesWithGround,
      .count = 40,
      .duration = 5.0f,
      .spread = 180.0f,
      .color_primary = v4f(0.57f, 0.23f, 0.23f, 1.0f),
      .color_secondary = v4f(0.17, 0.0f, 0.0f, 1.0f),
      .scale = v2f(10, 10),
      .speed = 3000.0f,
      .vel = v2f(0.0f, 0.0f),
    },

    .particle.debug = (ParticleDesc) {
      .emmission_type = ParticleEmmissionType_Burst,
      .props = ParticleProp_ScaleOverTime |
               ParticleProp_SpeedOverTime |
               ParticleProp_VariateColor,
      .scale = v2f(20, 20),
      .count = 100,
      .duration = 2.0f,
      .speed = 80.0f,
      .spread = 180.0f,
      .color_primary = D_RED,
      .color_secondary = D_BLUE,
      .scale_delta = v2f(-0.5f, -0.5f),
      .speed_delta = 50.0f,
    },
  };
}
