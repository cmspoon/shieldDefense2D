#ifndef ENEMY_HPP
#define ENEMY_HPP

#include <glm/glm.hpp>
#include "../support/basic_shape.hpp"

class Enemy {
private:
    glm::vec3 position;
    glm::vec3 velocity_x;
    glm::vec3 knockback_velocity;
    float velocity_y;
    int health;
    bool is_grounded;
    bool active;
    BasicShape* shape;
    float size;
    const float GROUND_LEVEL = -0.57f;
    

    
public:
    Enemy();
    void Spawn(float spawn_x, float spawn_y);
    void SetShape(BasicShape* enemy_shape);
    void Update(glm::vec3 target_position, float delta_time);
    void TakeDamage(glm::vec3 bullet_position, float knockback_force);
    bool IsActive() const { return active; }
    void Deactivate() { active = false; }
    glm::vec3 GetPosition() const { return position; }
    float GetSize() const { return size; }
    void Draw(unsigned int shader_program, glm::mat4 view_matrix);
};

#endif