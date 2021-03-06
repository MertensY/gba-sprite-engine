#include <libgba-sprite-engine/scene.h>
#include <libgba-sprite-engine/gba_engine.h>
#include <libgba-sprite-engine/background/text.h>
#include <libgba-sprite-engine/background/text_stream.h>
#include <libgba-sprite-engine/palette/palette_manager.h>
#include <libgba-sprite-engine/allocator.h>

#include "start_scene.h"

int main() {
    std::shared_ptr<GBAEngine> engine(new GBAEngine());

    // start engine //

    StartScene* startScene = new StartScene(engine);
    engine->setScene(startScene);

    // update engine //

    while (true) {
        engine->update();
        engine->delay(1000);
    }

    //engine not running//

    return 0;
}