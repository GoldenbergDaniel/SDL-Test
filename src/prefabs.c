#include "base/base_inc.h"
#include "prefabs.h"

void init_prefabs(Prefabs *prefabs)
{
  *prefabs = (Prefabs) {
    // Texture ----------------
    .texture.player_idle = v2i(0, 0),
    .texture.player_walk_0 = v2i(1, 0),
    .texture.player_walk_1 = v2i(2, 0),
    .texture.player_walk_2 = v2i(3, 0),
    .texture.player_walk_3 = v2i(4, 0),
    .texture.player_walk_4 = v2i(5, 0),
    .texture.player_jump = v2i(6, 0),

    .texture.walker_idle = v2i(0, 1),
    .texture.walker_walk_0 = v2i(1, 1),
    .texture.walker_walk_1 = v2i(2, 1),
    .texture.walker_walk_2 = v2i(3, 1),
    .texture.walker_walk_3 = v2i(4, 1),
    .texture.walker_walk_4 = v2i(5, 1),
    
    .texture.pistol = v2i(0, 2),
    .texture.bullet = v2i(1, 2),


    // Animation ----------------
    .animation.player_idle = (AnimationDesc) {
      
    },

    // Particle ----------------
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
      .props = ParticleProp_CollidesWithGround,
      .count = 40,
      .duration = 10.0f,
      .spread = 180.0f,
      .color_primary = v4f(0.37f, 0.0f, 0.0f, 1.0f),
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
      .color_primary = DEBUG_RED,
      .color_secondary = DEBUG_BLUE,
      .scale_delta = v2f(-0.5f, -0.5f),
      .speed_delta = 50.0f,
    },
  };
}