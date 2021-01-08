#ifndef GBA_SPRITE_ENGINE_END_SCENE_H
#define GBA_SPRITE_ENGINE_END_SCENE_H

#include <libgba-sprite-engine/scene.h>

class EndScene : public Scene {
private:
    std::unique_ptr<Sprite> ball;
    std::unique_ptr<Background> bg;

public:
    std::vector<Sprite *> sprites() override;
    std::vector<Background *> backgrounds() override;

    EndScene(std::shared_ptr<GBAEngine> engine) : Scene(engine) {}

    void load() override;
    void tick(u16 keys) override;

};


#endif //GBA_SPRITE_ENGINE_END_SCENE_H
