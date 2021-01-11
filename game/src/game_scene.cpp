#include <stdlib.h>
#include <stdio.h>
#include <libgba-sprite-engine/sprites/affine_sprite.h>
#include <libgba-sprite-engine/sprites/sprite_builder.h>
#include <libgba-sprite-engine/gba/tonc_memmap.h>
#include <libgba-sprite-engine/gba_engine.h>
#include <libgba-sprite-engine/background/text_stream.h>
#include <libgba-sprite-engine/effects/fade_out_scene.h>
#include "game_scene.h"
#include "end_scene.h"

#include "game_sprites.h"
#include "game_back.h"
#include "game_music.h"

    // vector reference //

std::vector<Sprite *> GameScene::sprites() {
    return {
        player.get(), ball.get(), gball.get()//, uball.get()//, mball.get()
    };
}

std::vector<Background *> GameScene::backgrounds() {
    return {
        bg.get()
    };
}

    // load foregroundPalette, backgroundPalette //

void GameScene::load() {
    srand(engine->getTimer()->getTotalMsecs());

    foregroundPalette = std::unique_ptr<ForegroundPaletteManager>(new ForegroundPaletteManager(game_spritesPal, sizeof(game_spritesPal)));
    backgroundPalette = std::unique_ptr<BackgroundPaletteManager>(new BackgroundPaletteManager(game_backPal, sizeof(game_backPal)));

    SpriteBuilder<Sprite> builder;
    SpriteBuilder<AffineSprite> affineBuilder;

    // create sprites //

    player = builder
            .withData(walkTiles, sizeof(walkTiles))
            .withSize(SIZE_32_32)
            .withAnimated(4, 4)
            .withinBounds()
            .withLocation(103, 110)
            .buildPtr();

    // start value pokeballs //

    ballX = rand() % GBA_SCREEN_WIDTH;
    gballX = rand() % GBA_SCREEN_WIDTH;
    //uballX = rand() % GBA_SCREEN_WIDTH;
    //mballX = rand() % GBA_SCREEN_WIDTH;

    // create sprite //

    ball = affineBuilder
            .withData(ballTiles, sizeof(ballTiles))
            .withSize(SIZE_16_16)
            .withLocation(ballX, 0)
            .withVelocity(0, 1)
            .buildPtr();

    gball = affineBuilder
            .withData(gballTiles, sizeof(gballTiles))
            .withSize(SIZE_16_16)
            .withLocation(gballX, 0)
            .withVelocity(0, 1)
            .buildPtr();

    /*uball = affineBuilder
            .withData(uballTiles, sizeof(uballTiles))
            .withSize(SIZE_16_16)
            .withLocation(uballX, 0)
            .withVelocity(0, 1)
            .buildPtr();

    /*mball = affineBuilder
            .withData(mballTiles, sizeof(mballTiles))
            .withSize(SIZE_16_16)
            .withLocation(mballX, 0)
            .withVelocity(0, 1)
            .buildPtr();
*/
    // create background //

    bg = std::unique_ptr<Background>(new Background(1, game_backTiles, sizeof(game_backTiles), game_backMap, sizeof(game_backMap)));
    bg.get()->useMapScreenBlock(16);

    // create timer //

    engine->enqueueMusic(intro, intro_bytes);
    timeTemp = engine->getTimer()->getTotalMsecs();
}

void GameScene::tick(u16 keys) {
    // ball rotation //
    rotate += 300;
    rotate1 += 200;
    //rotate2 += 100;
    //rotate3 += 100;

    ball.get()->rotate(rotate);
    gball.get()->rotate(rotate1);
    //uball.get()->rotate(rotate2);
    //mball.get()->rotate(rotate3);

    // Player inputs //
    // ---controls--- //

    if(keys & KEY_LEFT) {
        player->flipHorizontally(false);
        player->animate();
        player->setVelocity(-2, 0);
    } else if(keys & KEY_RIGHT) {
        player->flipHorizontally(true);
        player->animate();
        player->setVelocity(+2, 0);
    } else {
        player->flipHorizontally(false);
        player->stopAnimating();
        player->animateToFrame(4);
        player->setVelocity(0, 0);
    }

    // In-game events //

    if(ball->getY() >= (GBA_SCREEN_HEIGHT + 50) && ball->getY() > 0) {
        ballX = rand() % GBA_SCREEN_WIDTH;
        ball->moveTo(ballX, -10);
    }
    if(gball->getY() >= (GBA_SCREEN_HEIGHT + 50) && gball->getY() > 0) {
        gballX = rand() % GBA_SCREEN_WIDTH;
        gball->moveTo(gballX, -10);
    }
    /*if(uball->getY() >= (GBA_SCREEN_HEIGHT + 50) && uball->getY() > 0) {
        uballX = rand() % GBA_SCREEN_WIDTH;
        uball->moveTo(uballX, -10);
    }
    /*if(mball->getY() >= (GBA_SCREEN_HEIGHT + 50) && mball->getY() > 0) {
        mballX = rand() % GBA_SCREEN_WIDTH;
        mball->moveTo(mballX, -10);
    }
     */
    // player loses //

    if(player->collidesWith(*ball)){
        ball->animateToFrame(1);
        if (!engine->isTransitioning()) {
            engine->enqueueSound(ball_open, ball_open_bytes);
            engine->transitionIntoScene(new EndScene(engine), new FadeOutScene(3));
        }
    }
    if(player->collidesWith(*gball)){
        gball->animateToFrame(1);
        if (!engine->isTransitioning()) {
            engine->enqueueSound(ball_open, ball_open_bytes);
            engine->transitionIntoScene(new EndScene(engine), new FadeOutScene(3));
        }
    }
    /*if(player->collidesWith(*uball)){
        uball->animateToFrame(1);
        if (!engine->isTransitioning()) {
            engine->enqueueSound(ball_open, ball_open_bytes);
            engine->transitionIntoScene(new EndScene(engine), new FadeOutScene(3));
        }
    }*/

    // Music intro //

    if(!playingMusic && (engine->getTimer()->getTotalMsecs() - timeTemp) >= 2300){
        engine->enqueueMusic(game_music, game_music_bytes);
        playingMusic = true;
    }
}
