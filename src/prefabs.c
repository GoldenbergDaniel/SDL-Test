#include "vecmath/vecmath.h"
#include "prefabs.h"

Prefabs create_prefabs(void)
{
  Prefabs prefab = {0};

  // Texture ----------------
  {
    prefab.texture.player_idle = v2i(0, 0);
    prefab.texture.player_walk_0 = v2i(1, 0);
    prefab.texture.player_walk_1 = v2i(2, 0);
    prefab.texture.player_walk_2 = v2i(3, 0);
    prefab.texture.player_walk_3 = v2i(4, 0);
    prefab.texture.player_walk_4 = v2i(5, 0);
    prefab.texture.player_jump = v2i(6, 0);

    prefab.texture.walker_idle = v2i(0, 1);
    prefab.texture.walker_walk_0 = v2i(1, 1);
    prefab.texture.walker_walk_1 = v2i(2, 1);
    prefab.texture.walker_walk_2 = v2i(3, 1);
    prefab.texture.walker_walk_3 = v2i(4, 1);
    prefab.texture.walker_walk_4 = v2i(5, 1);
    
    prefab.texture.pistol = v2i(0, 3);
    prefab.texture.rifle = v2i(1, 3);
    prefab.texture.shotgun = v2i(2, 3);
    prefab.texture.smg = v2i(3, 3);

    prefab.texture.bullet = v2i(5, 3);
  }

  // Animation ----------------
  {
    prefab.animation.player_idle = (AnimationDesc) {
      .ticks_per_frame = 0,
      .frame_count = 1,
      .frames[0] = prefab.texture.player_idle,
    };

    prefab.animation.player_walk = (AnimationDesc) {
      .ticks_per_frame = 10,
      .frame_count = 5,
      .frames[0] = prefab.texture.player_walk_0,
      .frames[1] = prefab.texture.player_walk_1,
      .frames[2] = prefab.texture.player_walk_2,
      .frames[3] = prefab.texture.player_walk_3,
      .frames[4] = prefab.texture.player_walk_4,
    };

    prefab.animation.player_jump = (AnimationDesc) {
      .ticks_per_frame = 0,
      .frame_count = 1,
      .frames[0] = prefab.texture.player_jump,
    };

    prefab.animation.walker_idle = (AnimationDesc) {
      .ticks_per_frame = 0,
      .frame_count = 1,
      .frames[0] = prefab.texture.walker_idle,
    };

    prefab.animation.walker_walk = (AnimationDesc) {
      .ticks_per_frame = 20,
      .frame_count = 5,
      .frames[0] = prefab.texture.walker_walk_0,
      .frames[1] = prefab.texture.walker_walk_1,
      .frames[2] = prefab.texture.walker_walk_2,
      .frames[3] = prefab.texture.walker_walk_3,
      .frames[4] = prefab.texture.walker_walk_4,
    };
  }

  // Particle ----------------
  {
    prefab.particle.smoke = (ParticleDesc) {
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
    };

    prefab.particle.blood = (ParticleDesc) {
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
    };

    prefab.particle.death = (ParticleDesc) {
      .emmission_type = ParticleEmmissionType_Burst,
      .props = ParticleProp_CollidesWithGround,
      .count = 40,
      .duration = 10.0f,
      .spread = 180.0f,
      .color_primary = v4f(0.37f, 0.0f, 0.0f, 1.0f),
      .scale = v2f(10, 10),
      .speed = 3000.0f,
      .vel = v2f(0.0f, 0.0f),
    };

    prefab.particle.debug = (ParticleDesc) {
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
    };
  }

  // Weapon ----------------
  {
    prefab.weapon.pistol = (WeaponDesc) {
      .type = WeaponType_Pistol,
      .texture = prefab.texture.pistol,
      .ancor = v2f(35, 5),
      .shot_point = v2f(20, 2),
      .shot_cooldown = 0.45f,
      .damage = 4,
      .bullet_speed = 1000.0f,
    };

    prefab.weapon.rifle = (WeaponDesc) {
      .type = WeaponType_Rifle,
      .texture = prefab.texture.rifle,
      .ancor = v2f(35, 5),
      .shot_point = v2f(35, 2),
      .shot_cooldown = 0.75f,
      .damage = 7,
      .bullet_speed = 1500.0f,
    };

    prefab.weapon.shotgun = (WeaponDesc) {
      .type = WeaponType_Shotgun,
      .texture = prefab.texture.shotgun,
      .ancor = v2f(35, 5),
      .shot_point = v2f(35, 2),
      .shot_cooldown = 0.75f,
      .damage = 2,
      .bullet_speed = 1000.0f,
    };

    prefab.weapon.smg = (WeaponDesc) {
      .type = WeaponType_SMG,
      .texture = prefab.texture.smg,
      .ancor = v2f(25, 0),
      .shot_point = v2f(35, 2),
      .shot_cooldown = 0.075f,
      .damage = 1,
      .bullet_speed = 1500.0f,
    };
  }

  return prefab;
}
