#include <libgba-sprite-engine/sprites/affine_sprite.h>
#include <libgba-sprite-engine/sprites/sprite_builder.h>
#include <libgba-sprite-engine/gba/tonc_memmap.h>
#include <libgba-sprite-engine/gba_engine.h>
#include <libgba-sprite-engine/background/text_stream.h>
#include "game_scene.h"

#include "wave.h"
#include "back.h"
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
    foregroundPalette = std::unique_ptr<ForegroundPaletteManager>(new ForegroundPaletteManager(wavePal, sizeof(wavePal)));
    backgroundPalette = std::unique_ptr<BackgroundPaletteManager>(new BackgroundPaletteManager(backPal, sizeof(backPal)));

    SpriteBuilder<Sprite> builder;

    animation = builder
            .withData(waveTiles, sizeof(waveTiles))
            .withSize(SIZE_32_32)
            .withAnimated(2, 5)
            .withLocation(103, 35)
            .buildPtr();

    bg = std::unique_ptr<Background>(new Background(1, backTiles, sizeof(backTiles), backMap, sizeof(backMap)));
    bg.get()->useMapScreenBlock(16);

    engine->enqueueMusic(game_music, game_music_bytes);
}

void GameScene::tick(u16 keys) {

}
