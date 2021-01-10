#include <libgba-sprite-engine/sprites/sprite_builder.h>
#include <libgba-sprite-engine/background/text_stream.h>
#include <libgba-sprite-engine/gba/tonc_memdef.h>
#include <libgba-sprite-engine/gba_engine.h>
#include <libgba-sprite-engine/effects/fade_out_scene.h>
#include "end_scene.h"
#include "game_scene.h"

#include "end_sprites.h"
#include "end_music.h"
#include "start_music.h"
#include "start_back.h"

    // vector reference //

std::vector<Background *> EndScene::backgrounds() {
    return {bg.get()};
}

std::vector<Sprite *> EndScene::sprites() {
    return {ball.get()};
}

    // load foregroundPalette, backgroundPalette //

void EndScene::load() {
    foregroundPalette = std::unique_ptr<ForegroundPaletteManager>(
            new ForegroundPaletteManager(ballPal, sizeof(ballPal)));
    backgroundPalette = std::unique_ptr<BackgroundPaletteManager>(
            new BackgroundPaletteManager(start_backPal, sizeof(start_backPal)));

    SpriteBuilder<Sprite> builder;

    // create sprites //

    ball = builder
            .withData(ballTiles, sizeof(ballTiles))
            .withSize(SIZE_32_32)
            .withAnimated(4, 10)
            .withLocation(103, 35)
            .buildPtr();
    ball->animateToFrame(3);

    // interface //

    TextStream::instance().setText("PRESS START TO RESTART", 16, 4);

    // create background //

    bg = std::unique_ptr<Background>(new Background(1, start_backTiles, sizeof(start_backTiles), start_backMap, sizeof(start_backMap)));
    bg.get()->useMapScreenBlock(16);

    timeTemp = engine->getTimer()->getTotalMsecs();
}

void EndScene::tick(u16 keys) {

    // Player inputs //
    // ---controls--- //

    if (keys & KEY_START) {
        if (!engine->isTransitioning()) {
            engine->enqueueSound(button, button_bytes);
            engine->transitionIntoScene(new GameScene(engine), new FadeOutScene(3));
        }

    }

    // Animation //

    if(wiggles < 3 && (engine->getTimer()->getTotalMsecs() - timeTemp) >= 1000){
        engine->enqueueSound(ball_wiggle, ball_wiggle_bytes);
        ball->animate();
        timeTemp = engine->getTimer()->getTotalMsecs();
        wiggles++;
    } else if(!playedSound && (engine->getTimer()->getTotalMsecs() - timeTemp) >= 1000){
        engine->enqueueSound(jingle, jingle_bytes);
        ball->stopAnimating();
        ball->animateToFrame(3);
        TextStream::instance().setText("Gotcha!", 1, 11);
        TextStream::instance().setText("Pikachu was caught!", 2, 5);
        playedSound = true;
    }

    if(playedSound && !playingMusic && (engine->getTimer()->getTotalMsecs() - timeTemp) >= 4396){
        engine->enqueueMusic(end_music, end_music_bytes);
        playingMusic = true;
    }
}
