#include <libgba-sprite-engine/sprites/affine_sprite.h>
#include <libgba-sprite-engine/sprites/sprite_builder.h>
#include <libgba-sprite-engine/gba/tonc_memmap.h>
#include <libgba-sprite-engine/gba_engine.h>
#include <libgba-sprite-engine/background/text_stream.h>
#include "game_scene.h"

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

    ball = affineBuilder
            .withData(ballTiles, sizeof(ballTiles))
            .withSize(SIZE_16_16)
            .withLocation(100, 50)
            .withVelocity(0, 1)
            .buildPtr();

    bg = std::unique_ptr<Background>(new Background(1, game_backTiles, sizeof(game_backTiles), game_backMap, sizeof(game_backMap)));
    bg.get()->useMapScreenBlock(16);

    engine->enqueueMusic(game_music, game_music_bytes);
}

void GameScene::tick(u16 keys) {
    rotate += 300;
    ball.get()->rotate(rotate);

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
}
