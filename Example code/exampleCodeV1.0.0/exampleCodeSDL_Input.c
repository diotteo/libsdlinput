#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

#include "SDL_Input/SDL_Input.h"
#include "SDL_Input/SDL_Input_TTF/SDL_Input_TTF.h"

#define MAX_CHARS 100

#if 1
#define FILL_SCREEN SDL_FillRect( screen, NULL, SDL_MapRGB( screen->format, \
							0x5F, 0x67, 0 ));
#else
#define FILL_SCREEN SDL_BlitSurface( img, NULL, screen, NULL );
#endif

/*Prints error to both stdout and stderr, then exit*/
void reportError( const char msg[], ... ) {
	va_list ap;

	va_start( ap, msg );
	vfprintf( stderr, msg, ap );
	va_end( ap );
	va_start( ap, msg );
	vprintf( msg, ap );
	va_end( ap );

	TTF_Quit();
	SDL_Quit();
	exit( EXIT_FAILURE );
}

int main( int argc, char *argv[] ) {

	SDL_Surface *screen		= NULL,
				*infoText	= NULL;
    SDL_Event event;
	SDL_Rect pos			= { 10, 10, 200, 0 },
			 posInfoText	= { 50, 50, 0, 0 };
	SDL_Color	fg		= { 0, 0, 0, 0 },
				bg		= { 0xFF, 0xFF, 0xFF, 0 };
	SDL_Input_TTF *ttf	= NULL;
	TTF_Font *font		= NULL;
	int b_quit = 0;

	if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
		reportError( "SDL init error:%s\n", SDL_GetError());
	}

	if( TTF_Init() < 0 ) {
		reportError( "TTF init error:%s\n", TTF_GetError());
	}

	if( NULL == ( screen = SDL_SetVideoMode( 500, 500, 32,
			SDL_HWSURFACE | SDL_DOUBLEBUF ))) {
		reportError( "Video mode set error:%s\n", SDL_GetError());
	}

	if( NULL == ( font = TTF_OpenFont( "verdana.ttf", 15 ))) {
		reportError( "Can't find font:%s\n", "verdana.ttf" );
	}

	if( NULL == ( infoText = TTF_RenderText_Shaded( font, "Appuyez sur la barre"
			" d'espacement pour tester la saisie", fg, bg ))) {
		reportError( "Can't render infoText:%s\n", TTF_GetError());
	}

	if( NULL == ( ttf = SDL_Input_TTF_Create( MAX_CHARS, font, fg, &bg, pos,
			SDL_INPUT_TTF_BLENDED ))) {
		TTF_CloseFont( font );
		reportError( "SDL_Input_TTF_Create error.\nError may be: %s\n",
				SDL_GetError());
	}

	SDL_EnableKeyRepeat( SDL_DEFAULT_REPEAT_DELAY,
			SDL_DEFAULT_REPEAT_INTERVAL );
	SDL_EnableUNICODE( 1 );

	SDL_Input_SetText( ttf->input, "Bonjour les Zer0s :) !" );
	SDL_Input_SetCursorIndex( ttf->input, 5 );
	do {
		FILL_SCREEN
		SDL_BlitSurface( infoText, NULL, screen, &posInfoText );
		SDL_Flip( screen );
		SDL_WaitEvent( &event );
		switch( event.type ) {
		case SDL_QUIT:
			b_quit = 1;
			break;
		case SDL_KEYDOWN:
			/*Only activate input if SDLK_SPACE is pressed*/
			if( event.key.keysym.sym == SDLK_SPACE ) {
				do {
					SDL_WaitEvent( &event );
					if( -1 == SDL_Input_TTF_Input( ttf, screen, &event )) {
						reportError( "Error happened in "
								"SDL_Input_Input. Exiting...\n" );
					}

					/* Cleans screen and reblit input (this is needed to
 					 * clean up when "erasing characters" */
					FILL_SCREEN
					SDL_Input_TTF_Display( ttf, screen, NULL );
					SDL_Flip( screen );

				/*Loops until SDLK_RETURN ("ENTER KEY") is pressed*/
				} while( ttf->input->lastEvent.key.keysym.sym != SDLK_RETURN );
			}
			break;
		default:
			;
        }
    } while( !b_quit );

	SDL_Input_TTF_Free( ttf );
	SDL_FreeSurface( infoText ), infoText = NULL;
	TTF_Quit();
	SDL_Quit();

	/*Prevents "unused parameters argc/argc" warnings*/
	(void) argv;
	(void) argc;

	return 0;
}
