#include "visual_effects.h"
#include "game_physics.h"
#include "global.h"

void CreateBlood(int x, int y, ChillRender * pRend)
{
    for (int i = 0; i < MAX_BLOODS; i++) //search free blood item in array
    {
        if (pRend->aBlood[i].state == 0)
        {
            pRend->aBlood[i].state = 1;
            pRend->aBlood[i].x = x;
            pRend->aBlood[i].y = y;
            
            //init particles
            for (int k = 0; k < BLOOD_PARTICLES; k++)
            {
                pRend->aBlood[i].aParticles[k].x = x;
                pRend->aBlood[i].aParticles[k].y = y;
                pRend->aBlood[i].aParticles[k].velX = rand() % 30 - 10;
                if (pRend->aBlood[i].aParticles[k].velX > -5 && pRend->aBlood[i].aParticles[k].velX < 5)
                {
                    pRend->aBlood[i].aParticles[k].velY = rand() % 16 - 32;
                }
                else
                {
                    pRend->aBlood[i].aParticles[k].velY = rand() % 16 - 20;
                }
                //printf("setting x=%d y=%d in varX=%d varY=%d\n", x, y, pRend->aBlood[i].aParticles[k].x, pRend->aBlood[i].aParticles[k].y);
            }
            
            //printf("CreatedBlood animation at x=%d y=%d\n", x, y);
            
            for (int k = 0; k < BLOOD_PARTICLES; k++)
            {
                //printf("pat x=%d y=%d\n", pRend->aBlood[i].aParticles[k].x, pRend->aBlood[i].aParticles[k].y);
            }
            
            break;
        }
    }
}

void BloodTick(ChillRender * pRend, GameState * pGame)
{
    for (int i = 0; i < MAX_BLOODS; i++)
    {
        if (!pRend->aBlood[i].state)
            continue;
        
        //move particles
        for ( int p = 0; p < BLOOD_PARTICLES; p++)
        {
            /*
            if (p % 2 == 0) //split particles in qual and unequal to decide which direction they fly
            {
                pRend->aBlood[i].aParticles[p].x = pRend->aBlood[i].x + (pRend->aBlood[i].state + p + rand() % 5);
            }
            else
            {
                pRend->aBlood[i].aParticles[p].x = pRend->aBlood[i].x - (pRend->aBlood[i].state + p + rand() % 5);
            }
            
            if (pRend->aBlood[i].state < 30) //fly upwards the first 30 ticks
            {
                pRend->aBlood[i].aParticles[p].y = pRend->aBlood[i].y - (pRend->aBlood[i].state + p + rand() % 5);
            }
            else //fall downwards after 30 ticks
            {
                pRend->aBlood[i].aParticles[p].y = pRend->aBlood[i].y + (pRend->aBlood[i].state + p + rand() % 5);
            }
             */
            
            if (!pRend->aBlood[i].aParticles[p].IsGrounded)
            {
                pRend->aBlood[i].aParticles[p].x += pRend->aBlood[i].aParticles[p].velX;
                pRend->aBlood[i].aParticles[p].y += pRend->aBlood[i].aParticles[p].velY;
                
                pRend->aBlood[i].aParticles[p].velY += 1.1f;
            }
            
            if (IsColliding(pRend->aBlood[i].aParticles[p].x, pRend->aBlood[i].aParticles[p].y, pRend->aBlood[i].aParticles[p].velX, pRend->aBlood[i].aParticles[p].velY, 4, 4, pGame))
            {
                pRend->aBlood[i].aParticles[p].velY = 0.0f;
                pRend->aBlood[i].aParticles[p].IsGrounded = 1;
            }
        }

        
        
        pRend->aBlood[i].state++;
        
        if (pRend->aBlood[i].state > 200)
        {
            pRend->aBlood[i].state = 0;
        }
    }
}

void RenderTick(ChillRender * rend, GameState * pGame)
{
    BloodTick(rend, pGame);
}

void InitAnimations(ChillRender * rend)
{
    for (int i = 0; i < MAX_BLOODS; i++)
    {
        rend->aBlood[i].state = 0;
    }
    
    //CreateBlood(420, 420, rend);
}
