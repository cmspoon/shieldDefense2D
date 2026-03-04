#ifndef GAME_HPP
#define GAME_HPP

#include <vector>
#include <glm/glm.hpp>
#include "player.hpp"
#include "enemy.hpp"

class Game {
    private:
        void ProcessBullets(float delta_time, Player& player);
        void ProcessEnemies(float delta_time);
        void BulletEnemyCollisions(Player& player);
        void PlayerEnemyCollisions(Player& player);

        float enemy_spawn_timer = 0.0f;
        const float ENEMY_SPAWN_INTERVAL = 2.0f;


    public:
        Game();

        bool game_over = false;
        int shield_health = 4;
        float shield_scale = 1.0f;
        glm::vec3 power_core_position = glm::vec3(0.0f, -0.2f, 0.0f);

        void Update(float delta_time, Player& player, bool is_firing);

        std::vector<Enemy*> enemies;
    };

#endif