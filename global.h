#ifndef GLOBAL_H
#define GLOBAL_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <time.h>


//screen resolution
#define HEIGHT 720
#define WIDTH 1024

//player
#define PLAYER_HITBOX_X 48
#define PLAYER_HITBOX_Y 48

//mobs
#define MAX_MONSTERS 16
#define MONSTER_HEIGHT 128
#define MONSTER_WIDTH 128

//world
#define MAX_BLOCKS 128
#define GRAVITY  0.35f


//particles
#define MAX_BLOODS 16
#define BLOOD_PARTICLES 32

typedef struct
{
    float x, y;
    int AnimState;
} Monster;

typedef struct
{
    float x, y;
    float velX, velY;
    short life;
    char *name;
    int JumpState; //1=grounded 0=inAir
    
    int animFrame, facingLeft, slowingDown;
} Player;

typedef struct
{
    int x, y;
} Tree;

typedef struct
{
    int x, y, w, h;
    int IsAlive;
} Block;

typedef struct
{
    //Players
    Player player;
    
    //Monsters
    Monster aMonsters[10];
    
    //Blocks
    Block blocks[MAX_BLOCKS];
    
    //Trees
    Tree trees[100];
    
    //Images
    SDL_Texture *tree;
    SDL_Texture *playerFrames[2];
    SDL_Texture *block_img;
    SDL_Texture *monster_img[3];
    
    int time;
    
    //Renderer
    SDL_Renderer *renderer;
} GameState;


#endif
