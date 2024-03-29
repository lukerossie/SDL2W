#include "../include/graphics.hpp"
#include "../include/util.hpp"
#include <iostream>
/*
TODO: Add rotation for images
TODO: Add support for multiple windows? Would have to change init 
	and add some kind of resource management for windows etc.
*/

//input polling function defined in input.cpp
int poll( void * );

namespace
{
SDL_Renderer *renderer = 0;
SDL_Window *window = 0;
SDL_DisplayMode mode;
}

util::Vector2 init( char const *title, int width, int height, int flags )
{	
	if( SDL_Init( SDL_INIT_EVERYTHING ) < 0 )
	{//if SDL_Init fails you can't render a msgBox
		std::cout << "Critical Error\n" << "SDL_Init failed: " << SDL_GetError() << ". Application will now exit.\n";
		exit(-1);	
	}
	if( SDL_GetCurrentDisplayMode(0, &mode) < 0 )
	{
		msgBox( "Warning", "SDL_GetCurrentDisplayMode failed." );
	}
	//Linear texture filtering
	if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
	{
		msgBox( "Warning", "SDL_SetHint failed." );
	}
	if( !( window = SDL_CreateWindow( title, 
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
		width, height, SDL_WINDOW_SHOWN | flags ) ) )//show the window by default
	{
		msgBox( "Critical Error", "SDL_CreateWindow failed. Application will now exit." );
		exit(-1);
	}
	if( !( renderer = SDL_CreateRenderer( window, -1, 
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC) ) )
	{
		msgBox( "Error", "SDL_CreateRenderer failed." );
	}
	//Black renderer
	if( SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF ) < 0 )
	{
		msgBox( "Warning", "SDL_SetRenderDrawColor failed." );
	}
	if( !IMG_Init( IMG_INIT_PNG ) )
	{
		msgBox( "Error", "IMG_Init failed." );
	}	
	SDL_CreateThread( poll, "poll", (void *)0 );//a separate thread for polling input
	return util::Vector2(mode.w, mode.h);
}
/*
Sets width and height from screen size (displaymode).
Returns the screen width and height.
*/
util::Vector2 init( char const *title, int flags )
{	
	if( SDL_Init( SDL_INIT_EVERYTHING ) < 0 )
	{//if SDL_Init fails you can't render a msgBox
		std::cout << "Critical Error\n" << "SDL_Init failed: " << SDL_GetError() << ". Application will now exit.\n";
		exit(-1);	
	}
	if( SDL_GetCurrentDisplayMode(0, &mode) < 0 )
	{
		msgBox( "Warning", "SDL_GetCurrentDisplayMode failed." );
	}
	//Linear texture filtering
	if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
	{
		msgBox( "Warning", "SDL_SetHint failed." );
	}
	if( !( window = SDL_CreateWindow( title, 
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
		mode.w, mode.h, SDL_WINDOW_SHOWN | flags ) ) )//show the window by default
	{
		msgBox( "Critical Error", "SDL_CreateWindow failed. Application will now exit." );
		exit(-1);
	}
	if( !( renderer = SDL_CreateRenderer( window, -1, 
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC) ) )
	{
		msgBox( "Error", "SDL_CreateRenderer failed." );
	}
	//Black renderer
	if( SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF ) < 0 )
	{
		msgBox( "Warning", "SDL_SetRenderDrawColor failed." );
	}
	if( !IMG_Init( IMG_INIT_PNG ) )
	{
		msgBox( "Error", "IMG_Init failed." );
	}	
	SDL_CreateThread( poll, "poll", (void *)0 );//a separate thread for polling input
	return util::Vector2(mode.w, mode.h);
}
void clear()
{
	SDL_RenderClear( renderer );
}
void flip()
{
	SDL_RenderPresent( renderer );
}
void msgBox( char const *title, char const *msg )
{
	SDL_ShowSimpleMessageBox( 0, title, msg, 0 );
}

Image::Texture::Texture()
{
	
}
Image::Texture::Texture( char const *path )
{
	SDL_Surface* surf = IMG_Load( path );
	rawTexture = SDL_CreateTextureFromSurface( renderer, surf );
	SDL_FreeSurface( surf );
}
Image::Texture::~Texture()
{
	if(rawTexture)
	{
		SDL_DestroyTexture( rawTexture );
	}
}

Image::Image( char const *path )
{
	texture = std::shared_ptr<Texture>( new Texture( path ) );
	SDL_Surface* surf = IMG_Load( path );
    src.x = 0;
    src.y = 0;
    src.w = surf->w;
    src.h = surf->h;
    dest.x = 0;
    dest.y = 0;
    dest.w = src.w;
    dest.h = src.h;
	SDL_FreeSurface( surf );
}
Image::Image( char const *path, int srcX, int srcY, int srcW, int srcH )
{
	texture = std::shared_ptr<Texture>( new Texture( path ) );
    src.x = srcX;
    src.y = srcY;
    src.w = srcW;
    src.h = srcH;
    dest.x = 0;
    dest.y = 0;
    dest.w = src.w;
    dest.h = src.h;
}
Image::~Image()
{

}
void Image::draw()
{
	SDL_RenderCopy( renderer, texture.get()->rawTexture, &src, &dest );
}

