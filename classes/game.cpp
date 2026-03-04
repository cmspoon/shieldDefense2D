#include "game.hpp"
#include <glm/gtc/matrix_transform.hpp>

Game::Game() {
    for (int i = 0; i < 100; i++) {
        enemies.push_back(new Enemy());
    }
}

void Game::Update(float delta_time, Player& player, bool is_firing) {
    if (game_over) return;

    player.Update(delta_time, is_firing);
    ProcessBullets(delta_time, player);
    ProcessEnemies(delta_time);
    BulletEnemyCollisions(player);
    PlayerEnemyCollisions(player);
    if (!player.IsAlive()) {
        game_over = true;
    }

    enemy_spawn_timer += delta_time;
    if (enemy_spawn_timer >= ENEMY_SPAWN_INTERVAL) {
        enemy_spawn_timer = 0.0f;
        Enemy::Spawn(enemies);
    }
}

void Game::ProcessBullets(float delta_time, Player& player) {
    auto& bullets = player.bullets;
    for (int i = (int)bullets.size() - 1; i >= 0; i--) {
        if (bullets[i].active) {
            bullets[i].position += bullets[i].velocity * delta_time;
            auto& pos = bullets[i].position;
            if (pos.x > 4.0f || pos.x < -4.0f || pos.y > 4.0f || pos.y < -0.68f) {
                bullets[i].active = false;
            }
        }
        if (!bullets[i].active) {
            bullets.erase(bullets.begin() + i);
        }
    }
}

void Game::ProcessEnemies(float delta_time) {
    for (auto& enemy : enemies) {
        if (!enemy->IsActive()) continue;
        enemy->Update(power_core_position, delta_time);
        float dist = glm::length(enemy->GetPosition().x - power_core_position.x);
        if (dist < 0.05f) {
            enemy->Deactivate();
            if (shield_health > 0) {
                shield_health--;
                if (shield_health <= 0) {
                    shield_scale = 0.0f;
                } else {
                    shield_scale = 0.6f + (shield_health * 0.1f);
                }
            } else {
                game_over = true;
            }
        }
    }
}

void Game::BulletEnemyCollisions(Player& player) {
    auto& bullets = player.bullets;
    for (int i = (int)bullets.size() - 1; i >= 0; i--) {
        if (!bullets[i].active) continue;
        for (auto& enemy : enemies) {
            if (!enemy->IsActive()) continue;
            float dist = glm::distance(enemy->GetPosition(), bullets[i].position);
            if (dist < 0.035f +  enemy->GetSize() * 0.5f) {
                bullets[i].active = false;
                enemy->TakeDamage(bullets[i].position, 0.5f);
                break;
            }
        }
    }
}

void Game::PlayerEnemyCollisions(Player& player) {
    for (auto& enemy : enemies) {
        if (!enemy->IsActive()) continue;
        float dist = glm::distance(enemy->GetPosition(), player.GetPosition());
        if (dist < enemy->GetSize() * 0.5f + 0.08f) {
            player.TakeDamage(enemy->GetPosition(), 3.0f);
            break;
        }
    }
}