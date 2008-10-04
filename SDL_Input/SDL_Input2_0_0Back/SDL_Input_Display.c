/* Copyright (C) 2008 Diotte Olivier

This file is part of the SDL_Input library

SDL_Input is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public
License along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA */

/*This library uses the excellent SDL_TTF library:
 * http://www.libsdl.org/projects/SDL_ttf */
/* For sure, the SDL library is also needed to use this library:
 * www.libsdl.org */

/*SDL_Input_Display.c
 *-----------
 *Purpose: Text-blitting functions
 *Author: Olivier Diotte
 *Created on: March 17th 2008
 *Last update: Sept. 3rd, 2008
 */

#include <stdio.h>
#include <stdlib.h>

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

#include "SDL_Input.h"

/*Checks if text can be displayed. Returns 0 if successful or -1 otherwise.*/
static int SDL_Input_CheckErrors( SDL_Input *input, SDL_Surface *dest ) {
	int b_error = 0;

	if( !TTF_WasInit() || !SDL_WasInit( SDL_INIT_VIDEO ) || input->font == NULL
			|| input->string == NULL || strlen( (char*)input->string ) < 1 ) {
		b_error = -1;
	}
	if( input->position.w > dest-> w - input->position.x - 1 ||
			input->position.w < 1 ) {
		input->position.w = dest->w - input->position.x - 1;
	}

	return b_error;
}

/*Returns 0 if blit is successful or -1 otherwise*/
static int SDL_Input_BlitString( SDL_Surface *text, SDL_Surface *dest,
		SDL_Input *input ) {
	SDL_Rect	clip = { 0, 0, 0, 0 },
				pos = input->position;
	clip.w = input->position.w;
	clip.h = text->h;
	clip.x = input->offset;

	return SDL_BlitSurface( text, &clip, dest, &pos );
}

/*Returns o if successful or -1 otherwise*/
static int SDL_Input_BlitCursor( SDL_Surface *cursor, SDL_Surface *dest,
		SDL_Input *input ) {
	int b_error,
		cursorAbscissa;
	SDL_Rect pos = { 0, 0, 0, 0 };

	/*If no b_error is returned by SDL_Input_GetCursorPos*/
	if( !( b_error = -( 0 > ( cursorAbscissa =
			_SDL_Input_GetCursorPos( input ))))) {
		pos.y = input->position.y;
		pos.x = input->position.x + cursorAbscissa - input->offset;
		b_error = SDL_BlitSurface( cursor, NULL, dest, &pos );
	}
	return b_error;
}

/*Returns SDL_Surface pointer to text surface if successful or NULL otherwise*/
static SDL_Surface* SDL_Input_RenderText( SDL_Input *input ) {
	SDL_Surface	*text;

	/*Renders text*/
	switch( input->rendering ) {
		case SDL_INPUT_BLENDED:
			text = TTF_RenderText_Blended( input->font, (char*)input->string,
					input->fg );
			break;
		case SDL_INPUT_SHADED:
			text = TTF_RenderText_Shaded( input->font, (char*)input->string,
					input->fg, input->bg );
			break;
		default:
			text = TTF_RenderText_Solid( input->font, (char*)input->string,
					input->fg );
			break;
	}

	return text;
}

/*Returns 0 if successful or -1 otherwise*/
static int SDL_Input_InitCursor( SDL_Surface **p_cursor, SDL_Surface *dest,
		SDL_Surface *text ) {
	int b_error = 0;

	/*Allocate cursor*/
	if ( *p_cursor == NULL ) {
		if(( *p_cursor = SDL_CreateRGBSurface( SDL_HWSURFACE, 2, text->h, 8, 0,
				0, 0, 0 )) == NULL ) {
			b_error = -1;
		} else {
			SDL_FillRect( *p_cursor, NULL, SDL_MapRGB( dest->format, 0, 0, 0 ));
		}
	}

	return b_error;
}

/*Returns 0 if successful or -1 otherwise.
 * /!\ This function is for internal use. Don't use it unless you know exactly
 * what you're doing! /!\ */
int _SDL_Input_Display( SDL_Input *input, SDL_Surface *dest,
		SDL_Surface *backSpace, int b_lockingInput ) {

	SDL_Surface *text	= NULL,
				*cursor	= NULL;
	SDL_Rect	pos = { 0, 0, 0, 0 };
	int b_error = 0;

	if( !( b_error = SDL_Input_CheckErrors( input, dest ))) {
		if( !( b_error = ( NULL == ( text = SDL_Input_RenderText( input ))))) {
			if( !( b_error = SDL_Input_InitCursor( &cursor, dest, text ))) {
				if( backSpace != NULL ) {
					pos = input->position;
					SDL_BlitSurface( backSpace, NULL, dest, &pos );
				}

				SDL_Input_BlitString( text, dest, input );
				SDL_Input_BlitCursor( cursor, dest, input );

				if( b_lockingInput ) {
					SDL_Flip( dest );
				}

                /*Frees memory*/
 				SDL_FreeSurface( cursor ), cursor = NULL;
			}
			SDL_FreeSurface( text ), text = NULL;
		}
	}

	return -b_error;
}
