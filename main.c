#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <time.h>

#include "init_graphics.h"
#include "game_physics.h"
#include "visual_effects.h"


void process(GameState *game)
{
    //add time
    game->time++;
    
    //player movement
    Player *player = &game->player;
    player->x += player->velX;
    player->y += player->velY;
    
    if(player->velX != 0 && player->JumpState && !player->slowingDown)
    {
        if(game->time % 8 == 0)
        {
            if(player->animFrame == 0)
            {
                player->animFrame = 1;
            }
            else
            {
                player->animFrame = 0;
            }
        }
    }
    
    player->velY += GRAVITY;
}

int processEvents(SDL_Window *window, GameState *game, ChillRender * pRend)
{
    SDL_Event event;
    int done = 0;
    
    while(SDL_PollEvent(&event))
    {
        switch(event.type)
        {
            case SDL_WINDOWEVENT_CLOSE:
            {
                if(window)
                {
                    SDL_DestroyWindow(window);
                    window = NULL;
                    done = 1;
                }
            }
            break;
            case SDL_KEYDOWN:
            {
                switch(event.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                        done = 1;
                    break;
                    case SDLK_SPACE:
                        if(game->player.JumpState == 1)
                        {
                            game->player.velY = -8;
                            game->player.JumpState = 0;
                        }
                    break;
                }
            }
            break;
            case SDL_QUIT:
                //quit out of the game
                done = 1;
            break;
        }
    }
    
    //More jumping
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    if(state[SDL_SCANCODE_UP])
    {
        game->player.velY -= 0.2f;
    }
    
    //Walking
    if(state[SDL_SCANCODE_A])
    {
        game->player.velX -= 0.5;
        if(game->player.velX < -6)
        {
            game->player.velX = -6;
        }
        game->player.facingLeft = 1;
        game->player.slowingDown = 0;
    }
    else if(state[SDL_SCANCODE_D])
    {
        game->player.velX += 0.5;
        if(game->player.velX > 6)
        {
            game->player.velX = 6;
        }
        game->player.facingLeft = 0;
        game->player.slowingDown = 0;
    }
    else if (state[SDL_SCANCODE_B])
    {
        for (int i = 0; i < 2; i++)
        {
            game->blocks[i].IsAlive = 0;
        }
        CreateBlood(game->player.x, game->player.y, pRend);
    }
    else
    {
        game->player.animFrame = 0;
        game->player.velX *= 0.8f;
        game->player.slowingDown = 1;
        if(fabsf(game->player.velX) < 0.1f)
        {
            game->player.velX = 0;
        }
    }
    
    return done;
}

void doRender(SDL_Renderer *renderer, GameState *game, ChillRender * pRend)
{
    //set the drawing color to blue
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    
    //Clear the screen (to blue)
    SDL_RenderClear(renderer);
    
    //set the drawing color to white
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    
    for(int i = 0; i < MAX_BLOCKS; i++)
    {
        if (game->blocks[i].IsAlive == 1)
        {
            SDL_Rect blockRect = { game->blocks[i].x, game->blocks[i].y, game->blocks[i].w, game->blocks[i].h };
            SDL_RenderCopy(renderer, game->block_img, NULL, &blockRect);
        }
    }
    
    //draw player
    SDL_Rect rect = { game->player.x, game->player.y, 48, 48 };
    SDL_RenderCopyEx(renderer, game->playerFrames[game->player.animFrame],
                     NULL, &rect, 0, NULL, (game->player.facingLeft == 0));
    
    //draw monster
    SDL_Rect MonsterRect = { game->aMonsters[0].x, game->aMonsters[0].y, MONSTER_WIDTH, MONSTER_HEIGHT };
    SDL_RenderCopyEx(renderer, game->monster_img[game->aMonsters[0].AnimState],
                     NULL, &MonsterRect, 0, NULL, 1);
    
    //draw blood
    for (int i = 0; i < MAX_BLOODS; i++)
    {
        if (pRend->aBlood[i].state) //found active blood animation
        {
            for (int k = 0; k < BLOOD_PARTICLES; k++) //render all particles
            {
                SDL_SetRenderDrawColor(renderer, 255, 30, 30, 255);
                
                SDL_Rect BloodParticel0 = { pRend->aBlood[i].aParticles[k].x, pRend->aBlood[i].aParticles[k].y , 4, 4 };
                SDL_RenderFillRect(renderer, &BloodParticel0);
                //printf("drew blood particel at x=%d y=%d\n", pRend->aBlood[i].aParticles[k].x, pRend->aBlood[i].aParticles[k].y);
            }
        }
    }
     
    
    
    //draw the Trees
    /*
    for(int i = 0; i < 100; i++)
    {
        SDL_Rect treRect = { game->trees[i].x, game->trees[i].y, 64, 64 };
     SDL_RenderCopy(renderer, game->tree, NULL, &treeRect);
    }
     */

    
    TTF_Font* Sans = TTF_OpenFont("/System/Library/Fonts/Keyboard.ttf", 24); //this opens a font style and sets a size
    
    if (Sans == NULL)
    {
        printf("TTF_OpenFont: %s\n", TTF_GetError());
        printf("error loading font\n");
    }
    
    SDL_Color White = {255, 255, 255};  // this is the color in rgb format, maxing out all would give you the color white, and it will be your text's color
    
    SDL_Surface* surfaceMessage = TTF_RenderText_Solid(Sans, "put your text here", White); // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first
    
    SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage); //now you can convert it into a texture
    
    SDL_Rect Message_rect; //create a rect
    Message_rect.x = 0;  //controls the rect's x coordinate
    Message_rect.y = 0; // controls the rect's y coordinte
    Message_rect.w = 100; // controls the width of the rect
    Message_rect.h = 100; // controls the height of the rect
    
    //Mind you that (0,0) is on the top left of the window/screen, think a rect as the text's box, that way it would be very simple to understance
    
    //Now since it's a texture, you have to put RenderCopy in your game loop area, the area where the whole code executes
    
    SDL_RenderCopy(renderer, Message, NULL, &Message_rect); //you put the renderer's name first, the Message, the crop size(you can ignore this if you don't want to dabble with cropping), and the rect which is the size and coordinate of your texture
    
    //Don't forget too free your surface and texture
    
    //We are done drawing, "present" or show to the screen what we've drawn
    SDL_RenderPresent(renderer);
}

int main(int argc, char *argv[])
{
    GameState gameState;
    SDL_Window *window = NULL;                    // Declare a window
    SDL_Renderer *renderer = NULL;                // Declare a renderer
    
    SDL_Init(SDL_INIT_VIDEO);              // Initialize SDL2
    
    srandom((int)time(NULL));
    
    //Create an application window with the following settings:
    window = SDL_CreateWindow("chichilku ze game",                     // window title
                              SDL_WINDOWPOS_UNDEFINED,           // initial x position
                              SDL_WINDOWPOS_UNDEFINED,           // initial y position
                              WIDTH,                               // width, in pixels
                              HEIGHT,                               // height, in pixels
                              0                                  // flags
                              );
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    gameState.renderer = renderer;
    
    loadGame(&gameState);
    ChillRender cRender;
    InitAnimations(&cRender);
    
    // The window is open: enter program loop (see SDL_PollEvent)
    int done = 0;
    
    //Event loop
    while(!done)
    {
        //Check for events
        done = processEvents(window, &gameState, &cRender);
        
        process(&gameState);
        collisionDetect(&gameState);
        
        //Render display
        doRender(renderer, &gameState, &cRender);
        
        gameState.aMonsters[0].AnimState = rand() % 3;
        
        RenderTick(&cRender, &gameState);
        
        
        //don't burn up the CPU
        SDL_Delay(10);
    }
    
    
    //Shutdown game and unload all memory
    SDL_DestroyTexture(gameState.tree);
    SDL_DestroyTexture(gameState.playerFrames[0]);
    SDL_DestroyTexture(gameState.playerFrames[1]);
    SDL_DestroyTexture(gameState.block_img);
    
    // Close and destroy the window
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    
    // Clean up
    SDL_Quit();
    return 0;
}
