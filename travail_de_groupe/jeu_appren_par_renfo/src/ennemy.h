#ifndef ENNEMY_H
#define ENNEMY_H

#include "player.h"
#include "map.h"
#include "render.h"
#include "ball.h"
#include "qlearn.h"


extern int landingPointEnnemyX;
extern int landingPointEnnemyY;

extern int lastHitPointEnnemyX;
extern int lastHitPointEnnemyY;

extern int landingPointEnnemyIsFind;

void initEnnemy();
void manageEnnemy();

#endif