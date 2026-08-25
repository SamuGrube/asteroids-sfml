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
add_tappa(tappa01 TAPPA-01/game.cpp TAPPA-01/main.cpp)
add_tappa(tappa02 TAPPA-02/game.cpp TAPPA-02/main.cpp)
add_tappa(tappa03 TAPPA-03/game.cpp TAPPA-03/main.cpp)
add_tappa(tappa04 TAPPA-04/game.cpp TAPPA-04/main.cpp TAPPA-04/asteroid.cpp)
add_tappa(tappa05 TAPPA-05/game.cpp TAPPA-05/main.cpp TAPPA-05/asteroid.cpp TAPPA-05/bullet.cpp)
add_tappa(tappa06 TAPPA-06/game.cpp TAPPA-06/main.cpp TAPPA-06/asteroid.cpp TAPPA-06/bullet.cpp)
add_tappa(tappa07 TAPPA-07/game.cpp TAPPA-07/main.cpp TAPPA-07/asteroid.cpp TAPPA-07/bullet.cpp)
add_tappa(tappa08 TAPPA-08/game.cpp TAPPA-08/main.cpp TAPPA-08/asteroid.cpp TAPPA-08/bullet.cpp)
add_tappa(tappa09 TAPPA-09/game.cpp TAPPA-09/main.cpp TAPPA-09/asteroid.cpp TAPPA-09/bullet.cpp)
add_tappa(tappa10 TAPPA-10/game.cpp TAPPA-10/main.cpp TAPPA-10/asteroid.cpp TAPPA-10/bullet.cpp)
add_tappa(tappa11 TAPPA-11/game.cpp TAPPA-11/main.cpp TAPPA-11/asteroid.cpp TAPPA-11/bullet.cpp TAPPA-11/particle.cpp)
add_tappa(tappa12 TAPPA-12/game.cpp TAPPA-12/main.cpp TAPPA-12/asteroid.cpp TAPPA-12/bullet.cpp TAPPA-12/particle.cpp)
add_tappa(tappa13 TAPPA-13/game.cpp TAPPA-13/main.cpp TAPPA-13/asteroid.cpp TAPPA-13/bullet.cpp TAPPA-13/particle.cpp)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT main)