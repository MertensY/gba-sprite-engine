#include <libgba-sprite-engine/sprites/sprite_builder.h>
#include <libgba-sprite-engine/background/text_stream.h>
#include <libgba-sprite-engine/gba/tonc_memdef.h>
#include <libgba-sprite-engine/gba_engine.h>
#include <libgba-sprite-engine/effects/fade_out_scene.h>
#include "start_scene.h"
#include "flying_stuff_scene.h"

#include "wave.h"
//#include "back.h"
#include "sample_sound.h"

std::vector<Background *> StartScene::backgrounds() {
    return {/*bg.get()*/};
}

std::vector<Sprite *> StartScene::sprites() {
    return {animation.get()};
}

void StartScene::load() {
    foregroundPalette = std::unique_ptr<ForegroundPaletteManager>(new ForegroundPaletteManager(wavePal, sizeof(wavePal)));
    backgroundPalette = std::unique_ptr<BackgroundPaletteManager>(new BackgroundPaletteManager(/*backPal, sizeof(backPal)*/));

    SpriteBuilder<Sprite> builder;

    animation = builder
            .withData(waveTiles, sizeof(waveTiles))
            .withSize(SIZE_32_32)
            .withAnimated(2, 5)
            .withLocation(105, 80)
            .buildPtr();

    TextStream::instance().setText("PRESS START", 3, 8);

    /*bg = std::unique_ptr<Background>(new Background(1, backTiles, sizeof(backTiles), backMap, sizeof(backMap)));
    bg.get()->useMapScreenBlock(16);*/

    engine->enqueueMusic(zelda_music_16K_mono, zelda_music_16K_mono_bytes);
}

void StartScene::tick(u16 keys) {
    TextStream::instance().setText(/*engine->getTimer()->to_string()*/"", 18, 1);

    if (pressingAorB && !((keys & KEY_A) || (keys & KEY_B))) {
        //engine->getTimer()->toggle();
        pressingAorB = false;
    }

    if (keys & KEY_START) {
        if (!engine->isTransitioning()) {
            engine->enqueueSound(zelda_secret_16K_mono, zelda_secret_16K_mono_bytes);

            TextStream::instance() << "entered: starting next scene";

            engine->transitionIntoScene(new FlyingStuffScene(engine), new FadeOutScene(2));
        }
    } else if (keys & KEY_LEFT) {
        animation->flipHorizontally(true);
    } else if (keys & KEY_RIGHT) {
        animation->flipHorizontally(false);
    } else if (keys & KEY_UP) {
        animation->flipVertically(true);
    } else if (keys & KEY_DOWN) {
        animation->flipVertically(false);
    } else if ((keys & KEY_A) || (keys & KEY_B)) {
        pressingAorB = true;
    }
}
