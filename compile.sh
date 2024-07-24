#!/bin/bash

set -euo pipefail

# chichilku2 the game
# Idea by ChillerDragon (2016)
# Started development 2018

CFLAGS=''

if [[ -x "$(command -v sdl2-config)" ]]
then
	CFLAGS="$(sdl2-config --cflags)"
elif [[ -x "$(command -v sdl-config)" ]]
then
	CFLAGS="$(sdl-config --cflags)"
fi

LINK_STUFF=''

if [ -f /Library/Frameworks/SDL2.framework/Headers ]
then
	LINK_STUFF=-F/Library/Frameworks/SDL2.framework/Headers
fi

LDFLAGS="$(pkgconf --cflags SDL2_image)"
LIBS="-lSDL2_ttf -lSDL2_image -lSDL2"

# shellcheck disable=SC2086
gcc ./*.c -o chichilku2 $LIBS $LDFLAGS $CFLAGS $LINK_STUFF -g

