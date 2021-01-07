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
        animation.get()
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

    animation = builder
            .withData(ballTiles, sizeof(ballTiles))
            .withSize(SIZE_16_16)
            //.withAnimated(4, 5)
            .withLocation(103, 110)
            .buildPtr();

    bg = std::unique_ptr<Background>(new Background(1, game_backTiles, sizeof(game_backTiles), game_backMap, sizeof(game_backMap)));
    bg.get()->useMapScreenBlock(16);

    engine->enqueueMusic(game_music, game_music_bytes);
}

void GameScene::tick(u16 keys) {

}
