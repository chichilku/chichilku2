#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <time.h>

#include "init_graphics.h"

void loadGame(GameState *game)
{
    SDL_Surface *surface = NULL;

    // Load images and create rendering textures from them
    // TREE
    surface = IMG_Load("data/img/tree.png");
    if(surface == NULL)
    {
        printf("Cannot find tree.png!\n\n");
        SDL_Quit();
        exit(1);
    }
    game->tree = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);

    // MONSTER 0
    surface = IMG_Load("data/img/crykurcia_action0.png");
    if(surface == NULL)
    {
        printf("Cannot find chichilkurian_steppin.png!\n\n");
        SDL_Quit();
        exit(1);
    }
    game->monster_img[0] = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);

    // MONSTER 1
    surface = IMG_Load("data/img/crykurcia_action1.png");
    if(surface == NULL)
    {
        printf("Cannot find crykurcia_action0.png!\n\n");
        SDL_Quit();
        exit(1);
    }
    game->monster_img[1] = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);

    // MONSTER 2
    surface = IMG_Load("data/img/crykurcia_action2.png");
    if(surface == NULL)
    {
        printf("Cannot find crykurcia_action1.png!\n\n");
        SDL_Quit();
        exit(1);
    }
    game->monster_img[2] = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);

   // CHICHILKURIAN steppin
    surface = IMG_Load("data/img/chichilkurian_steppin.png");
    if(surface == NULL)
    {
        printf("Cannot find chichilkurian_steppin.png!\n\n");
        SDL_Quit();
        exit(1);
    }
    game->playerFrames[0] = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);

    // CHICHILKURIAN
    surface = IMG_Load("data/img/chichilkurian.png");
    if(surface == NULL)
    {
        printf("Cannot find chichilkurian.png!\n\n");
        SDL_Quit();
        exit(1);
    }
    game->playerFrames[1] = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);

    //BLOCK
    surface = IMG_Load("data/img/block.png");
    if(surface == NULL)
    {
        printf("Cannot find block.png!\n\n");
        SDL_Quit();
        exit(1);
    }
    game->block_img = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);

    game->player.x = 320-40;
    game->player.y = 240-40;
    game->player.velX = 0;
    game->player.velY = 0;
    game->player.JumpState = 0;
    game->player.animFrame = 0;
    game->player.facingLeft = 1;
    game->player.slowingDown = 0;

    game->time = 0;

    //init trees
    for(int i = 0; i < 100; i++)
    {
        game->trees[i].x = random()%640;
        game->trees[i].y = random()%480;
    }

    //init monsters
    for(int i = 0; i < MAX_BLOCKS; i++)
    {
        game->aMonsters[i].x = 800;
        game->aMonsters[i].y = 274;
        game->aMonsters[i].AnimState = 0;
    }

    //init blocks
    for(int i = 0; i < MAX_BLOCKS; i++)
    {
        game->blocks[i].w = 256;
        game->blocks[i].h = 64;
        game->blocks[i].x = i*256;
        game->blocks[i].y = 400;
        game->blocks[i].IsAlive = 1;
    }

    game->blocks[99].x = 350;
    game->blocks[99].y = 150;

    game->blocks[98].x = 350;
    game->blocks[98].y = 350;
}
