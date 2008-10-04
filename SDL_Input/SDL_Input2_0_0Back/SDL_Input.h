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
 *Created on: March 18th, 2008
 *Last update: Sept. 3rd, 2008
 */

#ifndef _SDL_INPUT_H_GUARD
#define _SDL_INPUT_H_GUARD

/*version-related macros (thanks to SDL_ttf.h for guidelines)*/
#define SDL_INPUT_MAJOR_VERSION 3
#define SDL_INPUT_MINOR_VERSION 0
#define SDL_INPUT_PATCHLEVEL	0

#define SDL_INPUT_VERSION( X )				\
{											\
	(X)->major = SDL_INPUT_MAJOR_VERSION;	\
	(X)->minor = SDL_INPUT_MINOR_VERSION;	\
	(X)->patch = SDL_INPUT_PATCHLEVEL;		\
}

#define SDL_INPUT_NOTHING -1
#define SDL_INPUT_CAPS ( SDL_GetModState() & ( KMOD_SHIFT | KMOD_CAPS ))
#define SDL_INPUT_IS_PRINT( x ) \
		( !iscntrl( x.key.keysym.unicode ))

typedef enum {
	SDL_INPUT_SOLID,
	SDL_INPUT_SHADED,
	SDL_INPUT_BLENDED
} SDL_INPUT_RENDER_TYPE;

typedef enum {
	SDL_INPUT_INS_MOD_QUERY		= 0,
	SDL_INPUT_INS_MOD_INSERT	= 1,
	SDL_INPUT_INS_MOD_CRUSH		= 2,
	SDL_INPUT_INS_MOD_TOGGLE	= 4
} SDL_INPUT_INSERT_MODE;

typedef struct {
    unsigned char	*string;		/*Read-only*/
	int	offset,						/*Read-only*/
		(*f)( SDL_Event, void* );
	long	cursorIndex,			/*Read-only*/
			maxChars;				/*Read-only*/
	SDL_INPUT_RENDER_TYPE	rendering;
	SDL_Rect	position;
	SDL_Event	lastEvent;
	SDL_Color	fg,
				bg;
	TTF_Font	*font;				/*Read-mostly*/
	void* params;
} SDL_Input;

SDL_Input* SDL_Input_Create( long, SDL_INPUT_RENDER_TYPE, SDL_Rect, SDL_Color,
		SDL_Color*, TTF_Font* );

void SDL_Input_Free( SDL_Input* );

int SDL_Input_InputString( SDL_Input*, SDL_Surface*, SDL_Event* );

int SDL_Input_IsHandled( SDL_Event );

void SDL_Input_EmptyString( SDL_Input* );

SDL_INPUT_INSERT_MODE SDL_Input_InsertMode( SDL_INPUT_INSERT_MODE );

void SDL_Input_SetText( SDL_Input*, const char* );

int SDL_Input_SetCursorIndex( SDL_Input*, long );

/*Functions beginning with '_' should *NOT* be called directly by the user!*/
int _SDL_Input_ComputeTextOffset( SDL_Input *input );

#define SDL_Input_GetVirtualCursorPos( a ) \
	_SDL_Input_GetCursorPos( a )

#define SDL_Input_GetRelativeCursorPos( a ) \
	( _SDL_Input_GetCursorPos( a ) - a->offset )
int _SDL_Input_GetCursorPos( SDL_Input* );

#define SDL_Input_Display( a, b ) \
	_SDL_Input_Display( a, b, NULL, 0 )

int _SDL_Input_Display( SDL_Input*, SDL_Surface*, SDL_Surface*, int );

#endif
