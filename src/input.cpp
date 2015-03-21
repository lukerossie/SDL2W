#include "../include/input.hpp"
#include <map>

namespace
{
std::map<SDL_Keycode, bool> keyState;
SDL_mutex *mutex = SDL_CreateMutex();

std::map<char, bool> mouseState;
util::Vector2 mousePos;//internal use
/*
mouse() will either return null or a pointer to this mouseCopy,
copied respecting mutex.
*/
util::Vector2 mouseCopy;
util::Vector2 *mousePointer;
}

int poll( void *ptr )
{
	SDL_Event event;//I might need to expose this, but you'd have to play nice with the mutex
	bool locked = false;
	while( 1 )
	{
		locked = false;
		SDL_PollEvent( &event );
		if( event.type == SDL_QUIT ) exit( 0 );//if they try to close the window we just quit immediately
		if( event.key.keysym.sym < 1073742106 && event.key.keysym.sym > 0 )
		{//maximum and minimum value for SDL_Keycode (probably not necessary since a map is being used now)
			if( event.key.type == SDL_KEYDOWN )
			{
				SDL_LockMutex( mutex );
				locked = true;
				keyState[event.key.keysym.sym] = true;
			}
			else if( event.key.type == SDL_KEYUP )
			{
				SDL_LockMutex( mutex );
				locked = true;
				keyState[event.key.keysym.sym] = false;
			}
			
			if( locked ) SDL_UnlockMutex(mutex);
			
			if( event.button.type == SDL_MOUSEBUTTONDOWN )
			{
				SDL_LockMutex( mutex );
				locked = true;
				mouseState[event.button.button] = true;
				mousePos.x = event.button.x;
				mousePos.y = event.button.y;
			}
			else if( event.button.type == SDL_MOUSEBUTTONUP )
			{
				SDL_LockMutex( mutex );
				locked = true;
				mouseState[event.button.button] = false;
				mousePos.x = event.button.x;
				mousePos.y = event.button.y;
			}
			
			if( locked ) SDL_UnlockMutex(mutex);
			
			if( event.button.type == SDL_MOUSEMOTION )
			{
				SDL_LockMutex( mutex );
				locked = true;
				mousePos.x = event.motion.x;
				mousePos.y = event.motion.y;
			}
			
			if( locked ) SDL_UnlockMutex(mutex);
		}
	}
}
bool key( SDL_Keycode whichKey )
{
	if( whichKey > 1073742106 || whichKey < 0 ) return false;
	SDL_LockMutex( mutex );
	bool value = keyState[whichKey];
	SDL_UnlockMutex( mutex );
	return value;
}
util::Vector2 *mouse( MB flag )
{
	SDL_LockMutex( mutex );
	mouseCopy = mousePos;
	SDL_UnlockMutex( mutex );
	mousePointer = &mouseCopy;
	
	if(flag == MB::LEFT)
	{
		if(mouseState[SDL_BUTTON_LEFT])
		{
			return mousePointer;
		}
		else
		{
			return 0;
		}
	}
	else if(flag == MB::RIGHT)
	{
		if(mouseState[SDL_BUTTON_RIGHT])
		{
			return mousePointer;
		}
		else
		{
			return 0;
		}
	}
	else if(flag == MB::MIDDLE)
	{
		if(mouseState[SDL_BUTTON_MIDDLE])
		{
			return mousePointer;
		}
		else
		{
			return 0;
		}
	}
	else if(flag == MB::NONE)
	{
		return mousePointer;
	}
	return 0;
}

