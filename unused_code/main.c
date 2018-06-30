#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <time.h>

//screen resolution
#define HEIGHT 720
#define WIDTH 1024

//player
#define PLAYER_HITBOX_X 32
#define PLAYER_HITBOX_Y 32

//world
#define MAX_BLOCKS 128
#define GRAVITY  0.35f

typedef struct
{
    int x, y;
    float velX, velY;
    short life;
    char *name;
    int JumpState;
} Player;

typedef struct
{
    int x, y;
} Tree;

typedef struct
{
    int x, y, w, h;
} Block;

typedef struct
{
    //Players
    Player player;
    
    //Blocks
    Block blocks[MAX_BLOCKS];
    
    //Trees
    Tree trees[100];
    
    //Images
    SDL_Texture *Tplayer;
    SDL_Texture *tree;
    SDL_Renderer *renderer;
} GameState;

void CheckCollide(GameState *game)
{
    float mw = PLAYER_HITBOX_X, mh = PLAYER_HITBOX_Y;
    float mx = game->player.x, my = game->player.y;
    float bx, by, bw, bh;
    
    for (int i = 0; i < MAX_BLOCKS; i++)
    {
        bx = game->blocks[i].x;
        by = game->blocks[i].y;
        bw = game->blocks[i].w;
        bh = game->blocks[i].h;
        
        if(mx+mw/2 > bx && mx+mw/2<bx+bw)
        {
            //are we bumping our head?
            if(my < by+bh && my > by && game->player.velY < 0)
            {
                //correct y
                game->player.y = by+bh;
                my = by+bh;
                
                //bumped our head, stop any jump velocity
                game->player.velY = 0;
                game->player.JumpState = 1;
            }
        }
        if(mx+mw > bx && mx<bx+bw)
        {
            //are we landing on the ledge
            if(my+mh > by && my < by && game->player.velY > 0)
            {
                //correct y
                game->player.y = by-mh;
                my = by-mh;
                
                //landed on this ledge, stop any jump velocity
                game->player.velY = 0;
                game->player.JumpState = 1;
            }
        }
        
        if(my+mh > by && my<by+bh)
        {
            //rubbing against right edge
            if(mx < bx+bw && mx+mw > bx+bw && game->player.velX < 0)
            {
                //correct x
                game->player.x = bx+bw;
                mx = bx+bw;
                
                game->player.velX = 0;
            }
            //rubbing against left edge
            else if(mx+mw > bx && mx < bx && game->player.velX > 0)
            {
                //correct x
                game->player.x = bx-mw;
                mx = bx-mw;
                
                game->player.velX = 0;
            }
        }
    }
}

void loadGame(GameState *game)
{
    SDL_Surface *treeSurface = NULL;
    SDL_Surface *playerSurface = NULL;
    
    //Load images and create rendering textures from them
    treeSurface = IMG_Load("data/img/tree.png");
    playerSurface = IMG_Load("data/img/chichilkurian.png");
    if(treeSurface == NULL)
    {
        printf("Cannot find tree.png!\n\n");
        SDL_Quit();
        exit(1);
    }
    if(playerSurface == NULL)
    {
        printf("Cannot find chichilkurian.png!\n\n");
        SDL_Quit();
        exit(1);
    }
    
    game->tree = SDL_CreateTextureFromSurface(game->renderer, treeSurface);
    game->Tplayer = SDL_CreateTextureFromSurface(game->renderer, playerSurface);
    SDL_FreeSurface(treeSurface);
    SDL_FreeSurface(playerSurface);
    
    game->player.x = 320-40;
    game->player.y = 240-40;
    game->player.JumpState = 0; //-1=falling 0=grounded 1=flying up
    
    //init trees
    for(int i = 0; i < 100; i++)
    {
        game->trees[i].x = random()%640;
        game->trees[i].y = random()%480;
    }
    
    //init blocks
    for(int i = 0; i < MAX_BLOCKS; i++)
    {
        game->blocks[i].w = 64;
        game->blocks[i].h = 64;
        game->blocks[i].x = 64*i;
        game->blocks[i].y = 400;
    }
    
    game->blocks[99].x = 256;
    game->blocks[99].y = 100;
}

int processEvents(SDL_Window *window, GameState *game)
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
                    case SDLK_UP:
                        if(game->player.JumpState)
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
    if(state[SDL_SCANCODE_LEFT])
    {
        game->player.velX -= 0.5;
        if(game->player.velX < -6)
        {
            game->player.velX = -6;
        }
    }
    else if(state[SDL_SCANCODE_RIGHT])
    {
        game->player.velX += 0.5;
        if(game->player.velX > 6)
        {
            game->player.velX = 6;
        }
    }
    else
    {
        game->player.velX *= 0.8f;
        if(fabsf(game->player.velX) < 0.1f)
        {
            game->player.velX = 0;
        }
    }
    
    return done;
}

void doRender(SDL_Renderer *renderer, GameState *game)
{
    //set the drawing color to light grey
    SDL_SetRenderDrawColor(renderer, 160, 160, 160, 255);
    
    //Clear the screen (to light grey)
    SDL_RenderClear(renderer);
    
    //set the drawing color to dark grey
    SDL_SetRenderDrawColor(renderer, 55, 55, 55, 255);
    
    
    for (int i = 0; i < MAX_BLOCKS; i++)
    {
        SDL_Rect blockRect = { game->blocks[i].x, game->blocks[i].y, game->blocks[i].w, game->blocks[i].h };
        SDL_RenderFillRect(renderer, &blockRect);
    }
    
    //draw a rectangle at player's position
    //SDL_Rect rect = { game->player.x, game->player.y, 80, 80 };
    SDL_Rect rect = { game->player.x, game->player.y, 80, 80 };
    
    SDL_RenderCopy(renderer, game->Tplayer, NULL, &rect);
    //SDL_RenderFillRect(renderer, &rect);
    
    //draw the tree image
    for(int i = 0; i < 100; i++)
    {
        SDL_Rect treeRect = { game->trees[i].x, game->trees[i].y, 128, 128 };
        SDL_RenderCopy(renderer, game->tree, NULL, &treeRect);
    }
    
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
    
    // The window is open: enter program loop (see SDL_PollEvent)
    int done = 0;
    
    //Event loop
    while(!done)
    {
        //Check for events
        done = processEvents(window, &gameState);
        
        CheckCollide(&gameState);
        
        //Render display
        doRender(renderer, &gameState);
        
        //don't burn up the CPU
        SDL_Delay(10);
    }
    
    
    //Shutdown game and unload all memory
    SDL_DestroyTexture(gameState.tree);
    
    // Close and destroy the window
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    
    // Clean up
    SDL_Quit();
    return 0;
}
