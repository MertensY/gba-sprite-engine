#include <libgba-sprite-engine/sprites/sprite_builder.h>
#include <libgba-sprite-engine/background/text_stream.h>
#include <libgba-sprite-engine/gba/tonc_memdef.h>
#include <libgba-sprite-engine/gba_engine.h>
#include <libgba-sprite-engine/effects/fade_out_scene.h>
#include "end_scene.h"
#include "start_scene.h"

#include "end_sprites.h"
#include "start_back.h"

std::vector<Background *> EndScene::backgrounds() {
    return {bg.get()};
}

std::vector<Sprite *> EndScene::sprites() {
    return {ball.get()};
}

void EndScene::load() {
    foregroundPalette = std::unique_ptr<ForegroundPaletteManager>(
            new ForegroundPaletteManager(ballPal, sizeof(ballPal)));
    backgroundPalette = std::unique_ptr<BackgroundPaletteManager>(
            new BackgroundPaletteManager(start_backPal, sizeof(start_backPal)));

    SpriteBuilder<Sprite> builder;

    ball = builder
            .withData(ballTiles, sizeof(ballTiles))
            .withSize(SIZE_32_32)
            .withAnimated(4, 5)
            .withLocation(103, 35)
            .buildPtr();

    TextStream::instance().setText("PRESS START", 16, 10);

    bg = std::unique_ptr<Background>(new Background(1, start_backTiles, sizeof(start_backTiles), start_backMap, sizeof(start_backMap)));
    bg.get()->useMapScreenBlock(16);
}

void EndScene::tick(u16 keys) {
    if (keys & KEY_START) {
        if (!engine->isTransitioning()) {
            engine->transitionIntoScene(new StartScene(engine), new FadeOutScene(2));
        }
    }
}
