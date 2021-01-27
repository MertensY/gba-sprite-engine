#include "ball.h"
#include <libgba-sprite-engine/gba_engine.h>

void Ball::tick(){
    // Only generate random numbers when fallSpeed = starting value
    if(fallSpeed == 1000){
        ballX = sprite->getX();
        fallSpeed = rand() % 3 + 1;
    }

    // Move and rotate ball every engine tick
    sprite->moveTo(ballX, ballY);
    ballY += fallSpeed;
}