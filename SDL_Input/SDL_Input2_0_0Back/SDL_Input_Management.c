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

/*SDL_Input.h
 *------------
 *Author: Olivier Diotte
 *Created on: Sept. 3rd, 2008
 *Last update: Sept. 3rd, 2008
 */

#include <stdio.h>
#include <stdlib.h>

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

#include "SDL_Input.h"

/*This function allocates a SDL_Input struct and returns its address
 * if successful or NULL otherwise*/
SDL_Input* SDL_Input_Create( long maxChars, SDL_INPUT_RENDER_TYPE rendering,
		SDL_Rect position, SDL_Color fg, SDL_Color *p_bg, TTF_Font *font ) {

    SDL_Input *structure = NULL;
    if( NULL != ( structure = malloc( sizeof( SDL_Input )))) {
        if( NULL != ( structure->string = malloc( maxChars *
				sizeof( *structure->string ) + 1 ))) {
            structure->cursorIndex	= SDL_INPUT_NOTHING;
			structure->offset		= 0;
			structure->maxChars		= maxChars;
			structure->rendering	= rendering;
            structure->position		= position;
			structure->fg			= fg;
			structure->bg			= *p_bg;
			structure->font			= font;
			structure->f			= NULL;
			SDL_Input_EmptyString( structure );
        } else {
            SDL_Input_Free( structure ), structure = NULL;
        }
    }

    return structure;
}

/*This function frees memory used by the *structure pointer.*/
void SDL_Input_Free( SDL_Input *input ) {
	if( input != NULL ) {
    	free( input->string ), input->string = NULL;
    	free( input ), input = NULL;
	}
}

/*Returns -1 if unicode wasn't enabled or 1 or 0 depending on if key is handled
 * otherwise*/
int SDL_Input_IsHandled( SDL_Event event ) {
	int b_handled = ( SDL_EnableUNICODE( -1 ) ? 0 : -1 );

	if( b_handled != -1 ) {
		switch( event.type ) {
		case SDL_KEYDOWN:
			/* *MUST* use !iscntrl() rather than isprint() */
			if( !( b_handled = !iscntrl( event.key.keysym.unicode ))) {
				switch( event.key.keysym.sym ) {
				case SDLK_BACKSPACE:
				case SDLK_DELETE:
				case SDLK_RETURN:
				case SDLK_ESCAPE:
				case SDLK_LEFT:
				case SDLK_RIGHT:
				case SDLK_HOME:
				case SDLK_END:
				case SDLK_INSERT:
					b_handled = 1;
					break;
				default:
					;
				}
			}
		}
	}

	return b_handled;
}

void SDL_Input_EmptyString( SDL_Input *input ) {
	memset( input->string, '\0', input->maxChars );
    input->cursorIndex	= 0;
	input->offset		= 0;
}

SDL_INPUT_INSERT_MODE SDL_Input_InsertMode( SDL_INPUT_INSERT_MODE insMode ) {
	static SDL_INPUT_INSERT_MODE insertMode = SDL_INPUT_INS_MOD_INSERT;

	switch( insMode ) {
	case SDL_INPUT_INS_MOD_INSERT:
	case SDL_INPUT_INS_MOD_CRUSH:
		insertMode = insMode;
		break;
	case SDL_INPUT_INS_MOD_TOGGLE:
		insertMode ^= 1;
	default:
		;
	}

	return insertMode;
}

/*Returns 0 if successful or -1 otherwise*/
int _SDL_Input_ComputeTextOffset( SDL_Input *input ) {
	int cursorAbscissa,
		textWidth,
		b_error;

	if( !( b_error = -( TTF_SizeText( input->font,
			(char*)input->string, &textWidth, NULL ) || ( 0 > ( cursorAbscissa =
			_SDL_Input_GetCursorPos( input )))))) {
		if( !( b_error = -( 0 > ( cursorAbscissa =
				_SDL_Input_GetCursorPos( input ))))) {
			if( cursorAbscissa < input->offset ) {
				input->offset = cursorAbscissa;
			} else if( cursorAbscissa >= input->position.w + input->offset ||
					textWidth <= cursorAbscissa ||
					cursorAbscissa <= input->offset ) {
				input->offset = cursorAbscissa - input->position.w;
				if( input->offset < 0 ) {
					input->offset = 0;
				}
			}
		}
	}

	return b_error;
}

/*This function computes the cursor's position and returns the actual x coord
 * of cursor relative to input->position.x or -1 in case of error
 * Please note the return value of this is not the real position of the
 * cursor ( return value - input->offset is ).*/
int _SDL_Input_GetCursorPos( SDL_Input *input ) {
	unsigned char c;
	int width;

	c = input->string[ input->cursorIndex ];
	input->string[ input->cursorIndex ] = '\0';
	if( TTF_SizeText( input->font, (char*)input->string, &width, NULL )) {
		width = -1;
	}

	input->string[ input->cursorIndex ] = c;
	return width;
}

void SDL_Input_SetText( SDL_Input *input, const char* str ) {
	long length;

	strncpy( (char*)input->string, str, input->maxChars );
	input->string[ input->maxChars + 1 ] = '\0';

	if( input->cursorIndex > ( length = (long)strlen( (char*)input->string ))) {
		input->cursorIndex = length;
	}
	_SDL_Input_ComputeTextOffset( input );
}

/*Returns 0 if succesful or -1 if it fails.*/
int SDL_Input_SetCursorIndex( SDL_Input *input, long cursorIndex ) {
	int b_error = -1;

	if( cursorIndex > -1 && cursorIndex <= input->maxChars ) {
		input->cursorIndex = cursorIndex;
		b_error = 0;
	}

	return b_error;
}
