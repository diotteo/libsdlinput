This is the readme file for the SDL_Input_TTF library. Currently, the only
	tested compiler is GCC (GNU Compiler Collection) which can be used on every
	platform (needs MinGW under Windows though).

Should any mistake, error or bug be found in any of SDL_Input_TTF files,
	you may email me at vhann3000 at gmail dot com

Note: A more "Windows adapted" howto may be found to the following location(s):
	http://www.newbiesparadise.com/ (Not yet available) 

Which is a translation of the original, french version:
	http://www.siteduzero.com/ (Not yet available)

Updated as of: 15/01/2009 (DD/MM/YYYY)

Index:
1. Compiling (GCC)

======
1. GCC
======
*** Compiling as a static library ***
Note: This step is unnecessary if you use the libSDL_Input_TTF.a file provided
	in the archive file (zip or tar depending on your system)

1- Unpack the SDL_Input_TTF archive and place all resulting files in an empty
	folder.

2- Copy the SDL_Input.h file shipped in the SDL_Input archive you downloaded to
	the folder where you placed all SDL_Input_TTF files.

2- Enter the forementionned empty folder (which is now no more empty) and
	compile SDL_Input_TTF source files as object files like this:
	gcc -Wall -Wextra -ansi -pedantic -c *.c

3- Now, use "ar" to produce the static library:
	ar rcs libSDL_Input_TTF.a *.o

4- You may now link the resulting file called libSDL_Input_TTF.a to your
	projects.

*** Compiling your programs with the static library ***
1- Place yourself in your program's sources folder

2- Copy/move the libSDL_Input_TTF.a and the libSDL_Input.a (shipped in the
	SDL_Input archive) files in subfolder SDL_Input (create it if it doesn't
	exist).

2- Compile all source files like this:
	gcc -Wall -Wextra -ansi -pedantic -lSDL -lSDL_ttf *.c \
		-L[libSDL_Input.a folder] -lSDL_Input -L[libSDL_Input_TTF.a folder] \
		-lSDL_Input_TTF
Note: replace "[libSDL_Input_TTF.a folder]" and "[libSDL_Input.a folder]" by the
	relative paths to where you respectively put the libSDL_Input_TTF.a and
	libSDL_Input.a files. Theorically, this should be "SDL_Input" for both.

3- Shall everything work fine, the compilation should have produced a file named
	a.out (under Linux) which is your executable.
Note: you may add a "-o [executableName]" to the above compilation line to
	choose the name of the generated executable (so it won't be named a.out).
