This file instructs users on how to compile and/or use SDL_Input in their own
	projects. Currently, the only tested compiler is GCC (GNU Compiler
	Collection) which can be used on every platform (needs MinGW under
	Windows though). GCC can be obtained from: http://gcc.gnu.org/

Should any mistake, error or bug be found in any file shipped with SDL_Input,
	you may email me at vhann3000 at gmail dot com

Note: A more "Windows adapted" howto may be found to the following location(s):
	http://www.newbiesparadise.com/ (Not yet available)

Which is a translation of the original, french version:
	http://www.siteduzero.com/ (Not yet available)

Updated as of: 15/01/2009 (DD/MM/YYYY)

Index:
1. Compiling (GCC)
2. TODO list

======
1. GCC
======
*** Compiling as a static library ***
Note: This step is unnecessary if you use the libSDL_Input.a file provided in
	the archive file (zip or tar depending on your system)

1- Unpack the SDL_Input archive and place all resulting files in an empty folder

2- Enter the forementionned empty folder (which is now no more empty) and
	compile SDL_Input source files as object files like this:
	gcc -Wall -Wextra -ansi -pedantic -c *.c

3- Now, use "ar" to produce the static library:
	ar rcs libSDL_Input.a *.o

4- You may now link the resulting file called libSDL_Input.a to your projects.

*** Compiling your programs with the static library ***
1- Place yourself in your program's sources folder

2- Copy/move the libSDL_Input.a file in subfolder SDL_Input (create it if it
	doesn't exist).

3- Compile all source files like this:
	gcc -Wall -Wextra -ansi -pedantic -lSDL -lSDL_ttf *.c \
		-L[libSDL_Input.a folder] -lSDL_Input
Note: replace "[libSDL_Input.a folder]" by the relative path to where you put
	the libSDL_Input.a file. Theorically, this should be "SDL_Input".

4- If there are no error in your program, the compilation should have produced
	a file named a.out (under Linux) which is your executable.

Note: you may add a "-o [executableName]" to the above compilation line to
	choose the name of the generated executable (so it won't be named a.out).

============
2. TODO list
============

-Learn how to use GNU Make and provide makefile in archives
-Add support for multibytes characters (therefore enabling full unicode support)
-Automate version change ( via GNU Make or write a short program for doing it )
-Verify function return codes ( some functions possibly return 1 or -1 on errors while they are documented as always returning -1 on errors )
-Add failure error codes and, possibly, an equivalent to SDL_GetError() function.
