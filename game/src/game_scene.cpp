#include <stdlib.h>
#include <stdio.h>
#include <algorithm>
#include <vector>
#include <libgba-sprite-engine/sprites/affine_sprite.h>
#include <libgba-sprite-engine/sprites/sprite_builder.h>
#include <libgba-sprite-engine/gba/tonc_memdef.h>
#include <libgba-sprite-engine/gba_engine.h>
#include <libgba-sprite-engine/background/text_stream.h>
#include <libgba-sprite-engine/effects/fade_out_scene.h>
#include "game_scene.h"
#include "end_scene.h"

#include "game_sprites.h"
#include "game_back.h"
#include "game_music.h"
#include "ball.h"

#define MAX_BALLS 3

std::vector<Sprite *> GameScene::sprites() {
    std::vector < Sprite * > sprites;
    sprites.push_back(player.get());
    sprites.push_back(ballSprite.get());
    sprites.push_back(gballSprite.get());
    sprites.push_back(uballSprite.get());
    sprites.push_back(mballSprite.get());
    for (auto &ball : balls) {
        sprites.push_back(ball->getSprite());
    }

    return sprites;
}

std::vector<Background *> GameScene::backgrounds() {
    return {
            bg.get()
    };
}

void GameScene::load() {
    // Generate random seed
    srand(engine->getTimer()->getTotalMsecs());

    // Set palettes
    foregroundPalette = std::unique_ptr<ForegroundPaletteManager>(
            new ForegroundPaletteManager(game_spritesPal, sizeof(game_spritesPal)));
    backgroundPalette = std::unique_ptr<BackgroundPaletteManager>(
            new BackgroundPaletteManager(game_backPal, sizeof(game_backPal)));

    // Create sprites
    player = spriteBuilder
            .withData(walkTiles, sizeof(walkTiles))
            .withSize(SIZE_32_32)
            .withAnimated(4, 4)
            .withinBounds()
            .withLocation(103, 110)
            .buildPtr();

    ballSprite = spriteBuilder
            .withData(ballTiles, sizeof(ballTiles))
            .withSize(SIZE_16_16)
            .withLocation(GBA_SCREEN_WIDTH + 20, GBA_SCREEN_HEIGHT + 20)
            .buildPtr();

    gballSprite = spriteBuilder
            .withData(gballTiles, sizeof(gballTiles))
            .withSize(SIZE_16_16)
            .withLocation(GBA_SCREEN_WIDTH + 20, GBA_SCREEN_HEIGHT + 20)
            .buildPtr();

    uballSprite = spriteBuilder
            .withData(uballTiles, sizeof(uballTiles))
            .withSize(SIZE_16_16)
            .withLocation(GBA_SCREEN_WIDTH + 20, GBA_SCREEN_HEIGHT + 20)
            .buildPtr();

    mballSprite = spriteBuilder
            .withData(mballTiles, sizeof(mballTiles))
            .withSize(SIZE_16_16)
            .withLocation(GBA_SCREEN_WIDTH + 20, GBA_SCREEN_HEIGHT + 20)
            .buildPtr();

    // Create background
    bg = std::unique_ptr<Background>(
            new Background(1, game_backTiles, sizeof(game_backTiles), game_backMap, sizeof(game_backMap)));
    bg.get()->useMapScreenBlock(16);

    // Start intro music
    engine->enqueueMusic(intro, intro_bytes);

    // Set temporary time to current
    timeTemp = engine->getTimer()->getTotalMsecs();
}

void GameScene::tick(u16 keys) {
    // Music intro
    if (!playingMusic && (engine->getTimer()->getTotalMsecs() - timeTemp) >= 2300) {
        engine->enqueueMusic(game_music, game_music_bytes);
        playingMusic = true;
    }

    int prevBallCount = balls.size();

    // Player inputs
    if (keys & KEY_LEFT && !engine->isTransitioning()) {
        player->flipHorizontally(false);
        player->animate();
        player->setVelocity(-2, 0);
    } else if (keys & KEY_RIGHT && !engine->isTransitioning()) {
        player->flipHorizontally(true);
        player->animate();
        player->setVelocity(+2, 0);
    } else {
        player->flipHorizontally(false);
        player->stopAnimating();
        player->animateToFrame(4);
        player->setVelocity(0, 0);
    }

    // Erase balls going off-screen
    eraseBalls();

    // Spawn balls
    if (balls.size() < MAX_BALLS) {
        if (initBalls >= MAX_BALLS) {
            balls.push_back(makeBall());
        } else if ((engine->getTimer()->getTotalMsecs() - timeTemp) >= (1000 * initBalls)) {
            balls.push_back(makeBall());
            initBalls++;
        }
        prevBallCount = 100;
    }

    // Update sprites
    if (prevBallCount != balls.size()) {
        engine.get()->updateSpritesInScene();
    }

    //Check all balls
    for (auto &ball : balls) {
        if (!engine->isTransitioning()) {
            // Game over when player collides with ball
            if (player->collidesWith(*ball->getSprite())) {
                ball->getSprite()->animateToFrame(1);
                player->animateToFrame(4);
                engine->enqueueSound(ball_open, ball_open_bytes);
                engine->transitionIntoScene(new EndScene(engine), new FadeOutScene(1));
            } else {
                // Move ball
                ball->tick();
            }
        }
    }
}

std::unique_ptr <Ball> GameScene::makeBall() {
    // Randomise x coordinate of ball
    ballX = (rand() % GBA_SCREEN_WIDTH + 16) - 16;

    // Randomise type of ball
    switch (rand() % 4) {
        case 1:
            return std::unique_ptr<Ball>(new Ball(
                    spriteBuilder
                            .withLocation(ballX, 0)
                            .buildWithDataOf(*gballSprite.get())));
            break;
        case 2:
            return std::unique_ptr<Ball>(new Ball(
                    spriteBuilder
                            .withLocation(ballX, 0)
                            .buildWithDataOf(*uballSprite.get())));
            break;
        case 3:
            return std::unique_ptr<Ball>(new Ball(
                    spriteBuilder
                            .withLocation(ballX, 0)
                            .buildWithDataOf(*mballSprite.get())));
            break;
        default:
            return std::unique_ptr<Ball>(new Ball(
                    spriteBuilder
                            .withLocation(ballX, 0)
                            .buildWithDataOf(*ballSprite.get())));
            break;
    }
}

void GameScene::eraseBalls() {
    balls.erase(
            std::remove_if(balls.begin(), balls.end(),
                           [](std::unique_ptr <Ball> &ball) { return ball->isOffScreen(); }), balls.end());
}