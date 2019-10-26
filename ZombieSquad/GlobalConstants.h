#pragma once
#include "mathUtil.h"

/**
* Holds all used constants of the game
*/

constexpr int GAME_SCALE = 1;
// Sizes
constexpr float PLAYER_SIZE = 6.0f;
constexpr float ZOMBIE_SIZE = 6.0f;
constexpr float BULLET_SIZE = 4.0f;
// Speeds
constexpr float BULLET_SPEED = 100.0f;
constexpr float ZOMBIE_SPEED = 55.0f;
constexpr float PLAYER_SPEED = 150.0f;
constexpr float PLAYER_TURN_SPEED = 2.5f;
constexpr float ZOMBIE_TURN_SPEED = 0.5f;
constexpr float ZOMBIE_ATTACK_SPEED = 0.75f;
constexpr float PLAYER_FIRING_RATE = 0.5f;
// Ranges
constexpr float ATTACK_RANGE = 16.0f;
constexpr int   MINIMUM_DISTANCE = 3;
constexpr float ZOMBIE_SIGHT_FOV_RAD = 32.5f * PI / 180.0f;
constexpr float PLAYER_SIGHT_FOV_RAD = 22.5f * PI / 180.0f;
constexpr float ZOMBIE_SIGHT_RANGE = 200.0f; // variable to control enemy sight range
constexpr int	ZOMBIES_MAX_NUM_PER_TILE = 4;
// Timings
constexpr float TIME_UNTIL_CHANGE_DIR = 5.0f;
constexpr float MIN_TIME_UNTIL_CHANGE_DIR = 1.0f;
constexpr float DYING_TIME = 0.5f;