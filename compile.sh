#chichilku the game
#Idea by ChillerDragon (2016)
#Started development 2018

#c++ version
#g++ *.cpp -o chichilku -lSDL2_Image -lSDL2 `sdl-config --cflags` -F/Library/Frameworks/SDL2.framework/Headers

#c version
gcc *.c -o chichilku -lSDL2_ttf -lSDL2_Image -lSDL2 `sdl-config --cflags` -F/Library/Frameworks/SDL2.framework/Headers

