#include "enemy.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <cstdlib>
#include <ctime>

Enemy::Enemy() {
    position = glm::vec3(0.0f);
    velocity_x = glm::vec3(0.0f);
    knockback_velocity = glm::vec3(0.0f);
    velocity_y = 0.0f;
    health = 3;
    is_grounded = true;
    active = false;
    shape = nullptr;
    size = 0.13f;
}

void Enemy::Spawn(float spawn_x, float spawn_y) {
    position = glm::vec3(spawn_x, spawn_y, 0.0f);
    knockback_velocity = glm::vec3(0.0f);
    active = true;
    health = 3;
}

void Enemy::SetShape(BasicShape* enemy_shape) {
    shape = enemy_shape;
}

void Enemy::Update(glm::vec3 target_position, float delta_time) {
    if (!active) return;
    
    if (is_grounded) {
        glm::vec3 direction = glm::normalize(target_position - position);
        direction.y = 0.0f;
        if (glm::length(direction) > 0.001f) {
            direction = glm::normalize(direction);
        }
        float speed = 0.3f;
        velocity_x = direction * speed;
        position += velocity_x * delta_time;
    } else {
        position += knockback_velocity * delta_time;
        knockback_velocity *= 0.8f;
    }

    velocity_y -= 3.5f * delta_time;
    position.y += velocity_y * delta_time;
    if (position.y <= GROUND_LEVEL) {
        position.y = GROUND_LEVEL;
        velocity_y = 0.0f;
        is_grounded = true;
    }
}

void Enemy::TakeDamage(glm::vec3 bullet_position, float knockback_force) {
    health--;
    if (health <= 0) {
        Deactivate();
    } else {
        glm::vec3 direction = glm::normalize(position - bullet_position);
        direction.y = 0.0f;
        knockback_velocity.x = direction.x * knockback_force;
        knockback_velocity.y = 0.0f;
        
        velocity_y = 0.3f;
        is_grounded = false;
    }
}

void Enemy::Draw(unsigned int shader_program, glm::mat4 view_matrix) {
    if (!active || !shape) return;
    
    glm::mat4 model = view_matrix;
    model = glm::translate(model, position);
    model = glm::scale(model, glm::vec3(size, size, 1.0f));
    
    glUniformMatrix4fv(glGetUniformLocation(shader_program, "model"), 1, GL_FALSE, &model[0][0]);
    shape->Draw();
}