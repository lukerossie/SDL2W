#pragma once
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "util.hpp"
#include <memory>

//for convenience. not all flags are here just the most basic ones
enum reducedFlags 
{
	FULLSCREEN = SDL_WINDOW_FULLSCREEN, 
	OPENGL = SDL_WINDOW_OPENGL, 
	FULLSCREEN_DESKTOP = SDL_WINDOW_FULLSCREEN_DESKTOP,
	RESIZABLE = SDL_WINDOW_RESIZABLE,
	BORDERLESS = SDL_WINDOW_BORDERLESS
};

//Open SDL and create window - last parameter is a bitflag that goes straight into SDL
//Returns the screen width and height.
util::Vector2 init( char const *title, int height, int width, int flags );
//width and height are screens default
//Returns the screen width and height.
util::Vector2 init( char const *title, int flags );
//Clear screen
void clear();
//Copy your renderer to the window
void flip();
void msgBox( char const *title, char const *msg );

/*
Copyable/assignable thing that is easy to draw.
*/
class Image
{
	struct Texture;
	
	public:
	SDL_Rect dest;
	std::shared_ptr<Texture> texture;
	
	Image( char const *path );//loads the whole image, dest is at 0,0
	Image( char const *path, int srcX, int srcY, int srcW, int srcH );
	~Image();
	//Copy the image to the renderer
	void draw();
	
	private:
	SDL_Rect src;
	/*
	SDL_Texture wrapper
	shared_ptr needs a defined struct (why? who knows).
	SDL_Texture's implementation isn't known by the compiler.
	This wrapper solves that problem.
	*/
	struct Texture
	{
		SDL_Texture *rawTexture;
	
		Texture();
		Texture( char const *path );
		~Texture();
	};
};
