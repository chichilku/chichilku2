#ifndef GAME_PHYSICS_H
#define GAME_PHYSICS_H

#include "global.h"

int IsColliding(int x, int y, int velX, int velY, int w, int h, GameState *game);
void collisionDetect(GameState *game);
void CheckMonster(GameState *game);

//useful utility function to see if two rectangles are colliding at all
int collide2d(float x1, float y1, float x2, float y2, float wt1, float ht1, float wt2, float ht2);


#endif
