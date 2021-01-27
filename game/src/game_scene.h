#ifndef GBA_SPRITE_ENGINE_GAME_SCENE_SCENE_H
#define GBA_SPRITE_ENGINE_GAME_SCENE_SCENE_H

#include <libgba-sprite-engine/sprites/sprite.h>
#include <libgba-sprite-engine/sprites/affine_sprite.h>
#include <libgba-sprite-engine/scene.h>
#include <libgba-sprite-engine/background/background.h>
#include "ball.h"

class GameScene : public Scene {
private:

    // Pointers
    std::unique_ptr<Sprite> player;
    std::unique_ptr<Sprite> ballSprite;
    std::unique_ptr<Sprite> gballSprite;
    std::unique_ptr<Sprite> uballSprite;
    std::unique_ptr<Sprite> mballSprite;
    std::unique_ptr<Background> bg;
    std::vector<std::unique_ptr<Ball>> balls;
    SpriteBuilder<Sprite> spriteBuilder;

    // Functions
    std::unique_ptr<Ball> makeBall();
    void eraseBalls();

    // Variables
    int playerSpeed = 32;
    int ballX = 0;
    int initBalls = 0;
    int timeTemp;
    bool playingMusic = false;

public:

    // Constructor
    GameScene(std::shared_ptr<GBAEngine> engine) : Scene(engine), playerSpeed(32), ballX(0), initBalls(0), timeTemp(0) {}

    std::vector<Sprite *> sprites() override;
    std::vector<Background *> backgrounds() override;

    // Functions
    void load() override;
    void tick(u16 keys) override;

};


#endif //GBA_SPRITE_ENGINE_GAME_SCENE_SCENE_H
