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

add_executable(main src/main.cpp src/input.cpp src/context.cpp src/utils.cpp src/tracedPath.cpp
    src/Animations/attackAnimation.cpp
    src/Entity.cpp
    src/Components/HealthComponent.cpp
    src/Components/StillAttackComponent.cpp
    src/Components/AreaScanComponent.cpp
    src/Components/RenderComponent.cpp
    src/Components/PathFollowerComponent.cpp
    src/Components/MouseHitboxComponent.cpp
    src/Components/CircleRenderComponent.cpp
    src/Components/RectRenderComponent.cpp
    src/Components/AttackComponent.cpp
    src/Strategies/PathFollowerStrategy.cpp
    src/Strategies/AreaPatrolStrategy.cpp
    src/Strategies/ChaseStrategy.cpp
    src/Strategies/MoveToEntityStrategy.cpp
    src/StrategyDrivers/StrategyDriver.cpp
    src/StrategyDrivers/LurkerStrategyDriver.cpp
    src/StrategyDrivers/TerritorialStrategyDriver.cpp
    src/StrategyDrivers/WandererStrategyDriver.cpp
    src/Components/TimedAttackComponent.cpp
    src/Components/HPColorShadingComponent.cpp
    src/Components/HealComponent.cpp
    src/Components/FogofWarComponent.cpp
    src/Components/VisibilityComponent.cpp
    src/Components/ProtectionSystemComponent.cpp
    src/Strategies/HuntStrategy.cpp
    src/StrategyDrivers/HunterStrategyDriver.cpp
    src/Components/DeathSystemComponent.cpp
    src/Components/RespawnComponent.cpp
    src/Components/RingIndicatorComponent.cpp
    src/Components/NumberComponent.cpp
    src/Components/ButtonComponent.cpp
    src/Components/SliderComponent.cpp
    src/Components/TriggerRadiusComponent.cpp
    src/Components/ResourceManager.cpp
    src/Components/ResourceComponent.cpp)
target_compile_definitions(main PRIVATE RESOURCE_DIR="${CMAKE_SOURCE_DIR}/resources")
target_include_directories(main PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_compile_features(main PRIVATE cxx_std_20)
target_link_libraries(main PRIVATE SFML::Graphics)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT main map input)
