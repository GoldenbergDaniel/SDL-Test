/*
  Simple DirectMedia Layer
  Copyright (C) 1997-2024 Sam Lantinga <slouken@libsdl.org>

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/

/**
 *  \file SDL.h
 *
 *  Main include header for the SDL library
 */


#ifndef SDL_h_
#define SDL_h_

#include <sdl3/SDL_stdinc.h>
#include <sdl3/SDL_assert.h>
#include <sdl3/SDL_atomic.h>
#include <sdl3/SDL_audio.h>
#include <sdl3/SDL_bits.h>
#include <sdl3/SDL_blendmode.h>
#include <sdl3/SDL_camera.h>
#include <sdl3/SDL_clipboard.h>
#include <sdl3/SDL_cpuinfo.h>
#include <sdl3/SDL_dialog.h>
#include <sdl3/SDL_endian.h>
#include <sdl3/SDL_error.h>
#include <sdl3/SDL_events.h>
#include <sdl3/SDL_filesystem.h>
#include <sdl3/SDL_gamepad.h>
#include <sdl3/SDL_guid.h>
#include <sdl3/SDL_haptic.h>
#include <sdl3/SDL_hidapi.h>
#include <sdl3/SDL_hints.h>
#include <sdl3/SDL_init.h>
#include <sdl3/SDL_iostream.h>
#include <sdl3/SDL_joystick.h>
#include <sdl3/SDL_keyboard.h>
#include <sdl3/SDL_keycode.h>
#include <sdl3/SDL_loadso.h>
#include <sdl3/SDL_locale.h>
#include <sdl3/SDL_log.h>
#include <sdl3/SDL_messagebox.h>
#include <sdl3/SDL_metal.h>
#include <sdl3/SDL_misc.h>
#include <sdl3/SDL_mouse.h>
#include <sdl3/SDL_mutex.h>
#include <sdl3/SDL_pen.h>
#include <sdl3/SDL_pixels.h>
#include <sdl3/SDL_platform.h>
#include <sdl3/SDL_power.h>
#include <sdl3/SDL_properties.h>
#include <sdl3/SDL_quit.h>
#include <sdl3/SDL_rect.h>
#include <sdl3/SDL_render.h>
#include <sdl3/SDL_scancode.h>
#include <sdl3/SDL_sensor.h>
#include <sdl3/SDL_storage.h>
#include <sdl3/SDL_surface.h>
#include <sdl3/SDL_system.h>
#include <sdl3/SDL_thread.h>
#include <sdl3/SDL_time.h>
#include <sdl3/SDL_timer.h>
#include <sdl3/SDL_touch.h>
#include <sdl3/SDL_version.h>
#include <sdl3/SDL_video.h>
#include <sdl3/SDL_oldnames.h>

#endif /* SDL_h_ */
