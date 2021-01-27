project(game)

add_executable(${PROJECT_NAME}.elf
        src/main.cpp
        src/start_scene.cpp
        src/start_scene.h
        src/start_music.h
        src/start_sprites.h
        src/start_back.h
        src/game_scene.cpp
        src/game_scene.h
        src/game_music.h
        src/game_sprites.h
        src/game_back.h
        src/ball.cpp
        src/ball.h
        src/end_scene.cpp
        src/end_scene.h
        src/end_music.h
        src/end_sprites.h)

target_link_libraries(${PROJECT_NAME}.elf gba-sprite-engine)

add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -v -O binary ${PROJECT_NAME}.elf ${PROJECT_NAME}.gba
        )
