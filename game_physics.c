#include "game_physics.h"
#include "global.h"

int collide2d(float x1, float y1, float x2, float y2, float wt1, float ht1, float wt2, float ht2)
{
    return (!((x1 > (x2+wt2)) || (x2 > (x1+wt1)) || (y1 > (y2+ht2)) || (y2 > (y1+ht1))));
}

void CheckMonster(GameState *game)
{
    for (int i = 0; i < MAX_MONSTERS; i++)
    {
        //
    }
}

int IsColliding(int x, int y, int velX, int velY, int w, int h, GameState *game)
{
    float mw = w, mh = h;
    float mx = x, my = y;
    
    //Check for collision with any blocks
    for(int i = 0; i < MAX_BLOCKS; i++)
    {
        float bx = game->blocks[i].x, by = game->blocks[i].y, bw = game->blocks[i].w, bh = game->blocks[i].h;
        
        if(mx+mw/2 > bx && mx+mw/2<bx+bw)
        {
            //are we bumping our head?
            if(my < by+bh && my > by && velY < 0)
            {
                return 1;
            }
        }
        if(mx+mw > bx && mx<bx+bw)
        {
            //are we landing on the block
            if(my+mh > by && my < by && velY > 0)
            {
                return 1;
            }
        }
        if(my+mh > by && my<by+bh)
        {
            //rubbing against right edge
            if(mx < bx+bw && mx+mw > bx+bw && velX < 0)
            {
                return 1;
            }
            //rubbing against left edge
            else if(mx+mw > bx && mx < bx && velX > 0)
            {
                return 1;
            }
        }
    }
    return 0;
}

void collisionDetect(GameState *game)
{
    float mw = PLAYER_HITBOX_X, mh = PLAYER_HITBOX_Y;
    float mx = game->player.x, my = game->player.y;

    //Check for collision with any blocks
    for(int i = 0; i < MAX_BLOCKS; i++)
    {
        float bx = game->blocks[i].x, by = game->blocks[i].y, bw = game->blocks[i].w, bh = game->blocks[i].h;
        
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
                //game->player.JumpState = 1;
            }
        }
        if(mx+mw > bx && mx<bx+bw)
        {
            //are we landing on the block
            if(my+mh > by && my < by && game->player.velY > 0)
            {
                //correct y
                game->player.y = by-mh;
                my = by-mh;
                
                //landed on this block, stop any jump velocity
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

