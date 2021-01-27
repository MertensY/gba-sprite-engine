#ifndef GBA_SPRITE_ENGINE_PROJECT_BALL_H
#define GBA_SPRITE_ENGINE_PROJECT_BALL_H

#include <libgba-sprite-engine/sprites/sprite.h>

class Ball {
private:
    std::unique_ptr<Sprite> sprite;
public:
    // Constructor
    Ball(std::unique_ptr<Sprite> sprite) : sprite(std::move(sprite)) {}

    // Variables
    int fallSpeed = 1000; // Startvalue
    int ballX = 0;
    int ballY = 0;

    // Functions
    void tick();
    bool isOffScreen() { return sprite->isOffScreen(); }
    Sprite* getSprite() { return sprite.get(); }
};

#endif //GBA_SPRITE_ENGINE_PROJECT_BALL_H
