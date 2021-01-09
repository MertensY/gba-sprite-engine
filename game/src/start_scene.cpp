#include <libgba-sprite-engine/sprites/sprite_builder.h>
#include <libgba-sprite-engine/background/text_stream.h>
#include <libgba-sprite-engine/gba/tonc_memdef.h>
#include <libgba-sprite-engine/gba_engine.h>
#include <libgba-sprite-engine/effects/fade_out_scene.h>
#include "start_scene.h"
#include "game_scene.h"

#include "start_sprites.h"
#include "start_back.h"
#include "start_music.h"

    // vector reference //

std::vector<Background *> StartScene::backgrounds() {
    return {bg.get()};
}

std::vector<Sprite *> StartScene::sprites() {
    return {wave.get()};
}
    // load foregroundPalette, backgroundPalette //

void StartScene::load() {

    foregroundPalette = std::unique_ptr<ForegroundPaletteManager>(new ForegroundPaletteManager(wavePal, sizeof(wavePal)));
    backgroundPalette = std::unique_ptr<BackgroundPaletteManager>(new BackgroundPaletteManager(start_backPal, sizeof(start_backPal)));

    SpriteBuilder<Sprite> builder;

    // create sprites //

    wave = builder
            .withData(waveTiles, sizeof(waveTiles))
            .withSize(SIZE_32_32)
            .withAnimated(2, 5)
            .withLocation(103, 35)
            .buildPtr();

    TextStream::instance().setText("PRESS START", 16, 10);

    // create background //

    bg = std::unique_ptr<Background>(new Background(1, start_backTiles, sizeof(start_backTiles), start_backMap, sizeof(start_backMap)));
    bg.get()->useMapScreenBlock(16);

    // create timer //

    engine->getTimer()->start(); // For random seed in next scene
    engine->enqueueMusic(start_music, start_music_bytes);
}

void StartScene::tick(u16 keys) {

    // Player inputs //
    // ---controls--- //

    if (keys & KEY_START) {
        if (!engine->isTransitioning()) {
            engine->enqueueSound(button, button_bytes);
            //overgang nieuwe sene
            engine->transitionIntoScene(new GameScene(engine), new FadeOutScene(3));
        }
    }
}
