#include <stdlib.h>
#include <stdio.h>
#include <libgba-sprite-engine/sprites/affine_sprite.h>
#include <libgba-sprite-engine/sprites/sprite_builder.h>
#include <libgba-sprite-engine/gba/tonc_memmap.h>
#include <libgba-sprite-engine/gba_engine.h>
#include <libgba-sprite-engine/background/text_stream.h>
#include <libgba-sprite-engine/effects/fade_out_scene.h>
#include "game_scene.h"
#include "start_scene.h"

#include "game_sprites.h"
#include "game_back.h"
#include "game_music.h"

std::vector<Sprite *> GameScene::sprites() {
    return {
        player.get(), ball.get()
    };
}

std::vector<Background *> GameScene::backgrounds() {
    return {
        bg.get()
    };
}

void GameScene::load() {
    srand(engine->getTimer()->getTotalMsecs());

    foregroundPalette = std::unique_ptr<ForegroundPaletteManager>(new ForegroundPaletteManager(game_spritesPal, sizeof(game_spritesPal)));
    backgroundPalette = std::unique_ptr<BackgroundPaletteManager>(new BackgroundPaletteManager(game_backPal, sizeof(game_backPal)));

    SpriteBuilder<Sprite> builder;
    SpriteBuilder<AffineSprite> affineBuilder;

    player = builder
            .withData(walkTiles, sizeof(walkTiles))
            .withSize(SIZE_32_32)
            .withAnimated(4, 4)
            .withinBounds()
            .withLocation(103, 110)
            .buildPtr();

    ballX = rand() % GBA_SCREEN_WIDTH;

    ball = affineBuilder
            .withData(ballTiles, sizeof(ballTiles))
            .withSize(SIZE_16_16)
            .withLocation(ballX, 0)
            .withVelocity(0, 1)
            .buildPtr();

    bg = std::unique_ptr<Background>(new Background(1, game_backTiles, sizeof(game_backTiles), game_backMap, sizeof(game_backMap)));
    bg.get()->useMapScreenBlock(16);

    engine->enqueueMusic(game_music, game_music_bytes);
}

void GameScene::tick(u16 keys) {
    rotate += 300;
    ball.get()->rotate(rotate);

    // Player inputs
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

    // In-game events
    if(ball->getY() >= (GBA_SCREEN_HEIGHT + 50) && ball->getY() > 0) {
        ballX = rand() % GBA_SCREEN_WIDTH;
        ball->moveTo(ballX, -10);
    }

    if(player->collidesWith(*ball)){
        if (!engine->isTransitioning()) {
            engine->transitionIntoScene(new StartScene(engine), new FadeOutScene(2));
        }
    }
}
