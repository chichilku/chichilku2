#ifndef VISUAL_EFFECTS_H
#define VISUAL_EFFECTS_H
#include "global.h"

typedef struct //the sub particel
{
    int x, y;
    int velX, velY;
    int IsGrounded;
} Blood;

typedef struct //the whole animation
{
    Blood aParticles[BLOOD_PARTICLES];
    int x, y;
    int state;
} BloodSpread;

typedef struct
{
    BloodSpread aBlood[16];
} ChillRender;


void BloodTick(ChillRender * pRend, GameState * pGame);

void InitAnimations(ChillRender * rend);

void RenderTick(ChillRender * rend, GameState * pGame);

void CreateBlood(int x, int y, ChillRender * pRend);


#endif
