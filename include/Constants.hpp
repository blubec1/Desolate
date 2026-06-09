#pragma once
#include <SFML/Graphics/Color.hpp>

const unsigned int MAP_HEIGHT = 800;
const unsigned int MAP_WIDTH = 800;
const int BRUSH_STARTING_RADIUS = 10;
const int STARTING_NUMBER_OF_SQUADS = 2;
const int SQUAD_CIRCLE_SIZE = 30;
const float SQUAD_SPEED = 100;

const float PLAYER_FACTION = 1;
const float MONSTER_FACTION = 2;

const float TRACED_PATH_NODE_DIST = 50.f;

const float STANDARD_VISIBILITY_RANGE = 500.f;

const sf::Color MAP_DRAW_COLOUR = sf::Color::Red;
const sf::Color MAP_ERASE_COLOUR = sf::Color::Black;
const sf::Color WANDERER_COLOUR = sf::Color::Yellow;
const sf::Color TERRITORIAL_COLOUR = sf::Color::Magenta;
const sf::Color OUTPOST_COLOUR = sf::Color::Cyan;
const sf::Color SQUAD_COLOUR = sf::Color::Red;
const sf::Color RESOURCE_COLOUR = sf::Color::Green;
const sf::Color LURKER_COLOUR = sf::Color(128, 64, 0);

const float WANDERER_RADIUS = 50.f;
const float WANDERER_MOVE_SPEED = 100.f;
const float WANDERER_CHASE_SPEED = 200.f;
const float WANDERER_DAMAGE = 50.f;
const float WANDERER_SHOOT_RANGE = 50.f;
const float WANDERER_ATTACK_COOLDOWN = 1.f;
const float WANDERER_MAX_HEALTH = 500.f;
const float WANDERER_AGGRO_RANGE = 300.f;
const float WANDERER_DE_AGGRO_RANGE = 300.f;
const float WANDERER_DE_AGGRO_COOLDOWN = 3.f;
const float WANDERER_TIME_TO_APPEAR = 1.f;

const float TERRITORIAL_RADIUS = 50.f;
const float TERRITORIAL_PATROL_SPEED = 100.f;
const float TERRITORIAL_PATROL_RADIUS = 200.f;
const float TERRITORIAL_CHASE_SPEED = 200.f;
const float TERRITORIAL_DAMAGE = 20.f;
const float TERRITORIAL_SHOOT_RANGE = 60.f;
const float TERRITORIAL_ATTACK_COOLDOWN = 0.5f;
const float TERRITORIAL_MAX_HEALTH = 10.f;
const float TERRITORIAL_AGGRO_RANGE = 100.f;
const float TERRITORIAL_DE_AGGRO_RANGE = 300.f;
const float TERRITORIAL_DE_AGGRO_COOLDOWN = 3.f;
const float TERRITORIAL_TIME_TO_APPEAR = 1.f;

const float OUTPOST_RADIUS = 100.f;
const float OUTPOST_HEAL_RANGE = 250.f;
const float OUTPOST_HEAL_VALUE = 20.f;
const float OUTPOST_TRIGGER_RADIUS = 250.f;

const float OUTPOST_SUPPLY_REPLENISH_RANGE = 250.f;
const float OUTPOST_SUPPLY_REPLENISH_RATE = 10.f;

const float AIRDROP_RADIUS = 20.f;
const float AIRDROP_TRIGGER_RADIUS = 50.f;
const float AIRDROP_VIEW_RANGE = 500.f;
const float AIRDROP_TIME_TO_APPEAR = 1.f;
const sf::Color AIRDROP_COLOUR = sf::Color(255, 215, 0);

const float SQUAD_SUPPLY_MAX = 100.f;
const float SQUAD_SUPPLY_DRAIN_RATE = 5.f;
const float SQUAD_SUPPLY_HP_DRAIN_RATE = 10.f;

const float SQUAD_DAMAGE = 100.f;
const float SQUAD_SHOOT_RANGE = 500.f;
const float SQUAD_ATTACK_COOLDOWN = 0.5f;
const float SQUAD_MAX_HEALTH = 100.f;
const float SQUAD_TIME_TO_APPEAR = 1.f;
const float SQUAD_VISIBILITY_RANGE = 500.f;

const float RESOURCE_RADIUS = 30.f;
const float RESOURCE_VIEW_RANGE = 2000.f;
const float RESOURCE_TIME_TO_APPEAR = 1.f;
const float RESOURCE_TICK_COOLDOWN = 5.f;
const float RESOURCE_FOOD_CONSUMPTION_RATE = 1.f;
const float RESOURCE_INCREASED_CONSUMPTION_RATE = 1.5f;
const float RESOURCE_METAL_PRODUCTION_RATE = 1.f;

const float LURKER_RADIUS = 40.f;
const float LURKER_PATROL_SPEED = 80.f;
const float LURKER_PATROL_RADIUS = 150.f;
const float LURKER_CHASE_SPEED = 200.f;
const float LURKER_DAMAGE = 30.f;
const float LURKER_SHOOT_RANGE = 60.f;
const float LURKER_ATTACK_COOLDOWN = 1.f;
const float LURKER_MAX_HEALTH = 400.f;
const float LURKER_AGGRO_RANGE = 250.f;
const float LURKER_DE_AGGRO_RANGE = 350.f;
const float LURKER_DE_AGGRO_COOLDOWN = 3.f;
const float LURKER_ARRIVAL_DISTANCE = 100.f;
const float LURKER_VISIBILITY_RANGE = 500.f;
const float LURKER_TIME_TO_APPEAR = 1.f;

const sf::Color HUNTER_COLOUR = sf::Color(30, 30, 30);
const float HUNTER_RADIUS = 20.f;
const float HUNTER_BASE_SPEED = 20.f;
const float HUNTER_MAX_SPEED = 150.f;
const float HUNTER_RAMP_UP_TIME = 5.f;
const float HUNTER_KILL_RANGE = 15.f;
const float HUNTER_VIEW_RANGE = 80.f;
const float HUNTER_TIME_TO_APPEAR = 0.5f;
const float HUNTER_MIN_RESPAWN_TIME = 2.f;
const float HUNTER_MAX_RESPAWN_TIME = 5.f;

const sf::Color HUNTER_LAIR_COLOUR = sf::Color(50, 50, 50);
const float HUNTER_LAIR_RADIUS = 40.f;
const float HUNTER_LAIR_VIEW_RANGE = 500.f;
const float HUNTER_LAIR_TIME_TO_APPEAR = 1.f;

const sf::Color PLAYER_HP_COLOUR = sf::Color::Blue;
const sf::Color MONSTER_HP_COLOUR = sf::Color::Red;