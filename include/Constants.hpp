#pragma once
#include <SFML/Graphics/Color.hpp>

enum class ResourceType
{
    Food,
    Metal,
    People
};

enum class EntityType
{
    None,
    Squad,
    Map,
    Wanderer,
    Outpost,
    FogofWar,
    Territorial,
    Lurker,
    Hunter,
    HunterLair,
    ProtectionSystem,
    DeathSystem,
    ResourceManager,
    Airdrop,
    UI,
    QuestSystem,
    AudioSystem
};

enum class SoundEvent
{
    Attack,
    AttackSound,
    Moving,
    Heard,
    Spotted,
    Stalked,
    Damaged,
    Death
};

const unsigned int MAP_HEIGHT = 1200;
const unsigned int MAP_WIDTH = 2133;
const int BRUSH_STARTING_RADIUS = 8;
const int STARTING_NUMBER_OF_SQUADS = 2;
const int SQUAD_CIRCLE_SIZE = 20;
const float SQUAD_SPEED = 80;

const float PLAYER_FACTION = 1;
const float MONSTER_FACTION = 2;

const float TRACED_PATH_NODE_DIST = 8.f;

const float STANDARD_VISIBILITY_RANGE = 200.f;

const float FOG_OVERLAY_ALPHA = 150.f;
const float FOG_GRADIENT_SIZE = 256.f;

const sf::Color MAP_DRAW_COLOUR = sf::Color::Red;
const sf::Color MAP_ERASE_COLOUR = sf::Color::Transparent;
const float GRID_CELL_SIZE = 40.f;
const sf::Color GRID_COLOUR = sf::Color(60, 60, 60);
const float SHOCKWAVE_COOLDOWN = 5.f;
const int SHOCKWAVE_DEFAULT_MAX_CHARGES = 3;
const float SHOCKWAVE_RECHARGE_RATE = 1.f;
const float SHOCKWAVE_RECHARGE_RANGE = 200.f;
const float SHOCKWAVE_CHARGE_THRESHOLD = 5.f;
const sf::Color WANDERER_COLOUR = sf::Color::Yellow;
const sf::Color TERRITORIAL_COLOUR = sf::Color::Magenta;
const sf::Color OUTPOST_COLOUR = sf::Color::Cyan;
const sf::Color SQUAD_1_COLOUR = sf::Color::Red;
const sf::Color SQUAD_2_COLOUR = sf::Color::Cyan;
const sf::Color RESOURCE_COLOUR = sf::Color::Green;
const sf::Color LURKER_COLOUR = sf::Color(128, 64, 0);

const float WANDERER_RADIUS = 15.f;
const float WANDERER_MOVE_SPEED = 60.f;
const float WANDERER_CHASE_SPEED = 120.f;
const float WANDERER_DAMAGE = 50.f;
const float WANDERER_SHOOT_RANGE = 40.f;
const float WANDERER_ATTACK_COOLDOWN = 1.f;
const float WANDERER_MAX_HEALTH = 500.f;
const float WANDERER_AGGRO_RANGE = 250.f;
const float WANDERER_DE_AGGRO_RANGE = 250.f;
const float WANDERER_DE_AGGRO_COOLDOWN = 3.f;
const float WANDERER_TIME_TO_APPEAR = 1.f;

const float TERRITORIAL_RADIUS = 15.f;
const float TERRITORIAL_PATROL_SPEED = 60.f;
const float TERRITORIAL_PATROL_RADIUS = 150.f;
const float TERRITORIAL_CHASE_SPEED = 120.f;
const float TERRITORIAL_DAMAGE = 20.f;
const float TERRITORIAL_SHOOT_RANGE = 50.f;
const float TERRITORIAL_ATTACK_COOLDOWN = 0.5f;
const float TERRITORIAL_MAX_HEALTH = 10.f;
const float TERRITORIAL_AGGRO_RANGE = 80.f;
const float TERRITORIAL_DE_AGGRO_RANGE = 250.f;
const float TERRITORIAL_DE_AGGRO_COOLDOWN = 3.f;
const float TERRITORIAL_TIME_TO_APPEAR = 1.f;

const float OUTPOST_RADIUS = 20.f;
const float OUTPOST_HEAL_RANGE = 200.f;
const float OUTPOST_HEAL_PERCENTAGE = 20.f;
const float OUTPOST_TRIGGER_RADIUS = 200.f;

const float OUTPOST_SUPPLY_REPLENISH_RANGE = 200.f;
const float OUTPOST_SUPPLY_REPLENISH_PERCENTAGE = 10.f;
const float OUTPOST_PROTECT_RANGE = 400.f;
const float OUTPOST_VIEW_RANGE = 200.f;

const float AIRDROP_RADIUS = 6.f;
const float AIRDROP_TRIGGER_RADIUS = 40.f;
const float AIRDROP_VIEW_RANGE = 400.f;
const float AIRDROP_TIME_TO_APPEAR = 1.f;
const sf::Color AIRDROP_COLOUR = sf::Color(255, 215, 0);

const float SHOCKWAVE_RADIUS = 80.f;

const float SQUAD_SUPPLY_MAX = 100.f;
const float SQUAD_SUPPLY_DRAIN_RATE = 5.f;
const float SQUAD_SUPPLY_HP_DRAIN_PERCENTAGE = 10.f;

const float SQUAD_DAMAGE = 10.f;
const float SQUAD_SHOOT_RANGE = 400.f;
const float SQUAD_ATTACK_COOLDOWN = 0.2f;
const float SQUAD_MAX_HEALTH = 100.f;
const float SQUAD_TIME_TO_APPEAR = 1.f;
const float SQUAD_VISIBILITY_RANGE = 400.f;

const float RESOURCE_RADIUS = 8.f;
const float RESOURCE_VIEW_RANGE = 1500.f;
const float RESOURCE_TIME_TO_APPEAR = 1.f;
const float RESOURCE_TICK_COOLDOWN = 5.f;
const float RESOURCE_FOOD_CONSUMPTION_RATE = 1.f;
const float RESOURCE_INCREASED_CONSUMPTION_RATE = 1.5f;
const float RESOURCE_METAL_PRODUCTION_RATE = 1.f;

const float LURKER_RADIUS = 12.f;
const float LURKER_PATROL_SPEED = 50.f;
const float LURKER_PATROL_RADIUS = 120.f;
const float LURKER_CHASE_SPEED = 120.f;
const float LURKER_DAMAGE = 30.f;
const float LURKER_SHOOT_RANGE = 50.f;
const float LURKER_ATTACK_COOLDOWN = 1.f;
const float LURKER_MAX_HEALTH = 400.f;
const float LURKER_AGGRO_RANGE = 200.f;
const float LURKER_DE_AGGRO_RANGE = 300.f;
const float LURKER_DE_AGGRO_COOLDOWN = 3.f;
const float LURKER_ARRIVAL_DISTANCE = 80.f;
const float LURKER_VISIBILITY_RANGE = 200.f;
const float LURKER_TIME_TO_APPEAR = 1.f;

const sf::Color HUNTER_COLOUR = sf::Color(30, 30, 30);
const float HUNTER_RADIUS = 6.f;
const float HUNTER_BASE_SPEED = 15.f;
const float HUNTER_MAX_SPEED = 100.f;
const float HUNTER_RAMP_UP_TIME = 5.f;
const float HUNTER_KILL_RANGE = 12.f;
const float HUNTER_MAX_HEALTH = 1000.f;
const float HUNTER_VIEW_RANGE = 60.f;
const float HUNTER_TIME_TO_APPEAR = 0.5f;
const float HUNTER_MIN_RESPAWN_TIME = 2.f;
const float HUNTER_MAX_RESPAWN_TIME = 5.f;

const sf::Color HUNTER_LAIR_COLOUR = sf::Color(50, 50, 50);
const float HUNTER_LAIR_RADIUS = 12.f;
const float HUNTER_LAIR_VIEW_RANGE = 250.f;
const float HUNTER_LAIR_TIME_TO_APPEAR = 1.f;

const float STANDARD_AUDIO_COOLDOWN = 5.f;
const float STANDARD_AUDIO_QUEUE_DELAY = 0.8f;
const float STANDARD_AUDIO_COMBAT_WINDOW = 3.f;
const int STANDARD_AUDIO_COMBAT_PRIORITY = 4;
const int STANDARD_AUDIO_PREEMPT_THRESHOLD = 3;

const float STANDARD_GUNSHOT_VOLUME = 50.f;
const float STANDARD_ATTACK_VOICE_VOLUME = 75.f;

const sf::Color PLAYER_HP_COLOUR = sf::Color::Blue;
const sf::Color MONSTER_HP_COLOUR = sf::Color::Red;

inline const char* entityTypeToString(EntityType t)
{
    switch (t)
    {
        case EntityType::Squad:       return "Squad";
        case EntityType::Wanderer:    return "Wanderer";
        case EntityType::Territorial: return "Territorial";
        case EntityType::Lurker:      return "Lurker";
        case EntityType::Hunter:      return "Hunter";
        default:                      return "";
    }
}

inline const char* soundEventToString(SoundEvent e)
{
    switch (e)
    {
        case SoundEvent::Attack:  return "Attack";
        case SoundEvent::AttackSound:  return "AttackSound";
        case SoundEvent::Moving:  return "Moving";
        case SoundEvent::Damaged: return "Damaged";
        case SoundEvent::Death:   return "Death";
        case SoundEvent::Spotted: return "Spotted";
        case SoundEvent::Stalked: return "Stalked";
        case SoundEvent::Heard:   return "Heard";
        default:                  return "";
    }
}

inline EntityType stringToEntityType(const std::string& s)
{
    if (s == "Squad")       return EntityType::Squad;
    if (s == "Wanderer")    return EntityType::Wanderer;
    if (s == "Territorial") return EntityType::Territorial;
    if (s == "Lurker")      return EntityType::Lurker;
    if (s == "Hunter")      return EntityType::Hunter;
    return EntityType::None;
}

inline SoundEvent stringToSoundEvent(const std::string& s)
{
    if (s == "Attack")  return SoundEvent::Attack;
    if (s == "AttackSound")  return SoundEvent::AttackSound;
    if (s == "Moving")  return SoundEvent::Moving;
    if (s == "Damaged") return SoundEvent::Damaged;
    if (s == "Death")   return SoundEvent::Death;
    if (s == "Spotted") return SoundEvent::Spotted;
    if (s == "Stalked") return SoundEvent::Stalked;
    if (s == "Heard")   return SoundEvent::Heard;
    return static_cast<SoundEvent>(-1);
}