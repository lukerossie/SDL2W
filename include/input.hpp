#pragma once
#include "SDL2/SDL.h"
#include "SDL2/SDL_thread.h"
#include "SDL2/SDL_mutex.h"
#include "util.hpp"

/*	
returns the state of the key (true for pressed false for released).
SDL_Keycode is a direct map to the char most of the time.
For example you can pass 'c' to see if SDLK_C is being pressed.
*/
bool key( SDL_Keycode );

enum class MB
{
	LEFT,
	RIGHT,
	MIDDLE,
	NONE
};
/*	
If you specify a button and it isn't pressed, returns null.
If it is pressed, returns mouse position.
If you specify NONE it will return *Point. (if you just want position)
*/
util::Vector2 *mouse( MB flag );
