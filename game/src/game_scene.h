#ifndef GBA_SPRITE_ENGINE_GAME_SCENE_SCENE_H
#define GBA_SPRITE_ENGINE_GAME_SCENE_SCENE_H

#include <libgba-sprite-engine/sprites/sprite.h>
#include <libgba-sprite-engine/sprites/affine_sprite.h>
#include <libgba-sprite-engine/scene.h>
#include <libgba-sprite-engine/background/background.h>

class GameScene : public Scene {
private:

    // pointers //

    std::unique_ptr<Sprite> player;
    std::unique_ptr<AffineSprite> ball;
    std::unique_ptr<Background> bg;

    // variables //

    int ballX;
    int playerSpeed = 32;
    int rotate;
    int timeTemp;
    bool playingMusic = false;

public:

    // pointers //

    GameScene(std::shared_ptr<GBAEngine> engine) : Scene(engine), ballX(0), playerSpeed(32), rotate(0), timeTemp(0) {}

    std::vector<Sprite *> sprites() override;
    std::vector<Background *> backgrounds() override;

    // functions //

    void load() override;
    void tick(u16 keys) override;
};


#endif //GBA_SPRITE_ENGINE_GAME_SCENE_SCENE_H
