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

    prefab.texture.chicken_idle = v2i(0, 2);
    prefab.texture.chicken_lay_0 = v2i(1, 2);
    prefab.texture.chicken_lay_1 = v2i(2, 2);
    
    prefab.texture.revolver = v2i(0, 3);
    prefab.texture.rifle = v2i(1, 3);
    prefab.texture.shotgun = v2i(2, 3);
    prefab.texture.smg = v2i(3, 3);
    prefab.texture.pistol = v2i(4, 3);

    prefab.texture.muzzle_flash = v2i(7, 3);
    prefab.texture.bullet = v2i(8, 3);
    prefab.texture.coin = v2i(7, 4);
    prefab.texture.soul = v2i(8, 4);

    prefab.texture.egg_0 = v2i(0, 4);
    prefab.texture.egg_1 = v2i(1, 4);
    prefab.texture.egg_2 = v2i(2, 4);
  }

  // Animation ----------------
  {
    prefab.animation.player_idle = (AnimationDesc) {
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

    prefab.animation.chicken_idle = (AnimationDesc) {
      .frame_count = 1,
      .frames[0] = prefab.texture.chicken_idle,
    };

    prefab.animation.chicken_lay = (AnimationDesc) {
      .ticks_per_frame = 20,
      .frame_count = 3,
      .frames[0] = prefab.texture.chicken_idle,
      .frames[1] = prefab.texture.chicken_lay_0,
      .frames[2] = prefab.texture.chicken_lay_1,
    };
  }

  // Particle ----------------
  {
    prefab.particle[ParticleKind_Smoke] = (ParticleDesc) {
      .emmission_type = ParticleEmmissionType_Burst,
      .props = ParticleProp_ScaleOverTime |
               ParticleProp_SpeedOverTime |
               ParticleProp_VariateColor |
               ParticleProp_RotateOverTime |
               ParticleProp_KillAfterTime,
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

    prefab.particle[ParticleKind_Blood] = (ParticleDesc) {
      .emmission_type = ParticleEmmissionType_Burst,
      .props = ParticleProp_ScaleOverTime |
               ParticleProp_RotateOverTime |
               ParticleProp_KillAfterTime,
      .count = 6,
      .duration = 0.3f,
      .spread = 180.0f,
      .color_primary = v4f(0.47f, 0.13f, 0.13f, 1.0f),
      .scale = v2f(5, 5),
      .scale_delta = v2f(-6.0f, -6.0f),
      .speed = 60.0f,
      .rot_delta = 50.0f,
    };

    prefab.particle[ParticleKind_Death] = (ParticleDesc) {
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

    prefab.particle[ParticleKind_PickupCoin] = (ParticleDesc) {
      .emmission_type = ParticleEmmissionType_Burst,
      .props = ParticleProp_ScaleOverTime |
               ParticleProp_RotateOverTime |
               ParticleProp_KillAfterTime,
      .count = 6,
      .duration = 0.3f,
      .spread = 500.0f,
      .color_primary = v4f(0.89f, 0.78f, 0.11f, 1.0f),
      .scale = v2f(5, 5),
      .scale_delta = v2f(-6.0f, -6.0f),
      .speed = 60.0f,
      .rot_delta = 50.0f,
    };

    prefab.particle[ParticleKind_Debug] = (ParticleDesc) {
      .emmission_type = ParticleEmmissionType_Burst,
      .props = ParticleProp_ScaleOverTime |
               ParticleProp_SpeedOverTime |
               ParticleProp_VariateColor |
               ParticleProp_KillAfterTime,
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

  // Zombie ----------------
  {
    prefab.zombie[ZombieKind_Walker] = (ZombieDesc) {
      .props = 0,
      .speed = 50,
      .health = 20,
      .damage = 1,
    };

    prefab.zombie[ZombieKind_Chicken] = (ZombieDesc) {
      .props = 0,
      .speed = 150,
      .health = 14,
      .damage = 1,
    };
  }

  // Weapon ----------------
  {
    prefab.weapon[WeaponKind_Revolver] = (WeaponDesc) {
      .texture = prefab.texture.revolver,
      .ancor = v2f(35, 0),
      .shot_point = v2f(20, 2),
      .shot_cooldown = 0.75f,
      .damage = 6,
      .bullet_speed = 1000.0f,
      .ammo = 6,
      .reload_duration = 3,
    };

    prefab.weapon[WeaponKind_Rifle] = (WeaponDesc) {
      .texture = prefab.texture.rifle,
      .ancor = v2f(35, 5),
      .shot_point = v2f(40, 2),
      .shot_cooldown = 1.15f,
      .damage = 14,
      .bullet_speed = 1500.0f,
      .ammo = 5,
      .reload_duration = 5,
    };

    prefab.weapon[WeaponKind_Shotgun] = (WeaponDesc) {
      .texture = prefab.texture.shotgun,
      .ancor = v2f(35, 5),
      .shot_point = v2f(40, 2),
      .shot_cooldown = 0.95f,
      .damage = 4,
      .bullet_speed = 1000.0f,
      .ammo = 7,
      .reload_duration = 3,
    };

    prefab.weapon[WeaponKind_SMG] = (WeaponDesc) {
      .texture = prefab.texture.smg,
      .ancor = v2f(25, 0),
      .shot_point = v2f(35, 2),
      .shot_cooldown = 0.085f,
      .damage = 1,
      .bullet_speed = 1500.0f,
      .ammo = 40,
      .reload_duration = 5
    };

    prefab.weapon[WeaponKind_Pistol] = (WeaponDesc) {
      .texture = prefab.texture.pistol,
      .ancor = v2f(35, 5),
      .shot_point = v2f(20, 2),
      .shot_cooldown = 0.75f,
      .damage = 6,
      .bullet_speed = 1000.0f,
      .ammo = 12,
      .reload_duration = 5,
    };
  }

  // Collectable ----------------
  {
    prefab.collectable[CollectableKind_Coin] = (CollectableDesc) {
      .texture = prefab.texture.coin,
      .draw_chance = 30,
    };
    
    prefab.collectable[CollectableKind_Soul] = (CollectableDesc) {
      .texture = prefab.texture.soul,
      .draw_chance = 5,
    };
  }

  // Wave ----------------
  {
    prefab.wave[0] = (WaveDesc) {
      .time_btwn_spawns = 3,
      .zombie_counts = {
        [ZombieKind_Walker] = 5,
      }
    };

    prefab.wave[1] = (WaveDesc) {
      .time_btwn_spawns = 3,
      .zombie_counts = {
        [ZombieKind_Walker] = 7,
      }
    };

    prefab.wave[2] = (WaveDesc) {
      .time_btwn_spawns = 2,
      .zombie_counts = {
        [ZombieKind_Walker] = 10,
        [ZombieKind_Chicken] = 1,
      }
    };
    
    prefab.wave[3] = (WaveDesc) {
      .time_btwn_spawns = 2,
      .zombie_counts = {
        [ZombieKind_Walker] = 10,
        [ZombieKind_Chicken] = 3,
      }
    };

    prefab.wave[4] = (WaveDesc) {
      .time_btwn_spawns = 2,
      .zombie_counts = {
        [ZombieKind_Walker] = 15,
        [ZombieKind_Chicken] = 5,
      }
    };
  }

  return prefab;
}
