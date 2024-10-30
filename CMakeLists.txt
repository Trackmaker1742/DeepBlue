cmake_minimum_required(VERSION 3.30)
project(DeepBlue)

# Cpp 20
set(CMAKE_CXX_STANDARD 20)

include_directories(
    libraries/include
    include
)

# Libraries
link_directories(
    libraries/lib
)

# Data
set(DATA ${PROJECT_SOURCE_DIR}/data)
file(COPY ${DATA} DESTINATION ${CMAKE_BINARY_DIR})

# Resources
set(RES ${PROJECT_SOURCE_DIR}/res)
file(COPY ${RES} DESTINATION ${CMAKE_BINARY_DIR})
add_executable(DeepBlue
    src/audio.cpp
    src/block.cpp
    src/camera.cpp
    src/collision.cpp
    src/config.cpp
    src/editor.cpp
    src/enemy.cpp
    src/file_handler.cpp
    src/input.cpp
    src/object2d.cpp
    src/player.cpp
    src/projectile.cpp
    src/renderer.cpp
    src/scene.cpp
    src/stage.cpp
    main.cpp
)

target_link_libraries(DeepBlue
    mingw32
    SDL2main
    SDL2
    SDL2_image
    SDL2_mixer 
)

add_custom_command(TARGET DeepBlue POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy 
    $<TARGET_FILE:DeepBlue>
    ${PROJECT_SOURCE_DIR}/DeepBlue.exe
)