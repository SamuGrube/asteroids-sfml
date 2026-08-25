cmake_minimum_required(VERSION 3.28)
project(CMakeSFMLProject LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.1.0
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

# Macro o funzione helper per evitare ripetizioni
function(add_tappa TARGET_NAME)
    add_executable(${TARGET_NAME} ${ARGN})
    target_compile_features(${TARGET_NAME} PRIVATE cxx_std_23)
    target_link_libraries(${TARGET_NAME} PRIVATE SFML::Graphics SFML::Window SFML::System SFML::Audio)
    if(MSVC)
        target_compile_options(${TARGET_NAME} PRIVATE /W4)
    else()
        target_compile_options(${TARGET_NAME} PRIVATE -Wall -Wextra)
    endif()
endfunction()

# Definizione dei singoli eseguibili
add_tappa(tappa01 TAPPA-01/src/game.cpp TAPPA-01/src/main.cpp)
add_tappa(tappa02 TAPPA-02/src/game.cpp TAPPA-02/src/main.cpp)
add_tappa(tappa03 TAPPA-03/src/game.cpp TAPPA-03/src/main.cpp)
add_tappa(tappa04 TAPPA-04/src/game.cpp TAPPA-04/src/main.cpp TAPPA-04/src/asteroid.cpp)
add_tappa(tappa05 TAPPA-05/src/game.cpp TAPPA-05/src/main.cpp TAPPA-05/src/asteroid.cpp TAPPA-05/src/bullet.cpp)
add_tappa(tappa06 TAPPA-06/src/game.cpp TAPPA-06/src/main.cpp TAPPA-06/src/asteroid.cpp TAPPA-06/src/bullet.cpp)
add_tappa(tappa07 TAPPA-07/src/game.cpp TAPPA-07/src/main.cpp TAPPA-07/src/asteroid.cpp TAPPA-07/src/bullet.cpp)
add_tappa(tappa08 TAPPA-08/src/game.cpp TAPPA-08/src/main.cpp TAPPA-08/src/asteroid.cpp TAPPA-08/src/bullet.cpp)
add_tappa(tappa09 TAPPA-09/src/game.cpp TAPPA-09/src/main.cpp TAPPA-09/src/asteroid.cpp TAPPA-09/src/bullet.cpp)
add_tappa(tappa10 TAPPA-10/src/game.cpp TAPPA-10/src/main.cpp TAPPA-10/src/asteroid.cpp TAPPA-10/src/bullet.cpp)
add_tappa(tappa11 TAPPA-11/src/game.cpp TAPPA-11/src/main.cpp TAPPA-11/src/asteroid.cpp TAPPA-11/src/bullet.cpp TAPPA-11/src/particle.cpp)
add_tappa(tappa12 TAPPA-12/src/game.cpp TAPPA-12/src/main.cpp TAPPA-12/src/asteroid.cpp TAPPA-12/src/bullet.cpp TAPPA-12/src/particle.cpp)
add_tappa(tappa13 TAPPA-13/src/game.cpp TAPPA-13/src/main.cpp TAPPA-13/src/asteroid.cpp TAPPA-13/src/bullet.cpp TAPPA-13/src/particle.cpp)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT main)