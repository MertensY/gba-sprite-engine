#include <libgba-sprite-engine/sprites/sprite_builder.h>
#include <libgba-sprite-engine/background/text_stream.h>
#include <libgba-sprite-engine/gba/tonc_memdef.h>
#include <libgba-sprite-engine/gba_engine.h>
#include <libgba-sprite-engine/effects/fade_out_scene.h>
#include "start_scene.h"
#include "flying_stuff_scene.h"

#include "wave.h"
#include "back.h"
#include "sample_sound.h"

std::vector<Background *> StartScene::backgrounds() {
    return {bg.get()};
}

std::vector<Sprite *> StartScene::sprites() {
    return {animation.get()};
}

void StartScene::load() {
    foregroundPalette = std::unique_ptr<ForegroundPaletteManager>(
            new ForegroundPaletteManager(wavePal, sizeof(wavePal)));
    backgroundPalette = std::unique_ptr<BackgroundPaletteManager>(
            new BackgroundPaletteManager(backPal, sizeof(backPal)));

    SpriteBuilder<Sprite> builder;

    animation = builder
            .withData(waveTiles, sizeof(waveTiles))
            .withSize(SIZE_32_32)
            .withAnimated(2, 5)
            .withLocation(103, 35)
            .buildPtr();

    TextStream::instance().setText("PRESS START", 3, 426);

    bg = std::unique_ptr<Background>(new Background(1, backTiles, sizeof(backTiles), backMap, sizeof(backMap)));
    bg.get()->useMapScreenBlock(16);

    engine->enqueueMusic(zelda_music_16K_mono, zelda_music_16K_mono_bytes);
}

void StartScene::tick(u16 keys) {

    if (pressingAorB && !((keys & KEY_A) || (keys & KEY_B))) {
        pressingAorB = false;
    }

    if (keys & KEY_START) {
        if (!engine->isTransitioning()) {
            engine->enqueueSound(zelda_secret_16K_mono, zelda_secret_16K_mono_bytes);

            TextStream::instance().setText("entered: starting next scene", 18, 1);

            engine->transitionIntoScene(new FlyingStuffScene(engine), new FadeOutScene(2));
        }
    } else if ((keys & KEY_A) || (keys & KEY_B)) {
        pressingAorB = true;
    }
}
