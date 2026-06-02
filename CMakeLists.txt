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

add_executable(main src/main.cpp src/map.cpp src/input.cpp src/context.cpp src/utils.cpp src/npc.cpp src/NPCs/squad.cpp src/tracedPath.cpp src/npcMaster.cpp
    src/NPCs/wanderer.cpp
    src/Animations/attackAnimation.cpp
    src/Locations/location.cpp
    src/Locations/outpost.cpp
    src/Entity.cpp
    src/Components/HealthComponent.cpp
    src/Components/StillAttackComponent.cpp
    src/Components/AreaScanComponent.cpp
    src/Components/RenderComponent.cpp
    src/Components/PathFollowerComponent.cpp
    src/Components/MouseHitboxComponent.cpp
    src/EntityFactory.cpp
    src/Components/CircleRenderComponent.cpp
    src/Components/AttackComponent.cpp
    src/Components/ChaseAttackComponent.cpp
    src/StrategyDriver.cpp
    src/Strategies/PathFollowerStrategy.cpp
    src/Strategies/ChaseStrategy.cpp)
target_include_directories(main PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_compile_features(main PRIVATE cxx_std_20)
target_link_libraries(main PRIVATE SFML::Graphics)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT main map input)
