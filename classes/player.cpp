#include "player.hpp"
#include <glm/gtc/matrix_transform.hpp>

Player::Player(glm::vec3 start_pos) {
    position = start_pos;
    arm_angle = 0.0f;
    velocity_y = 0.0f;
    is_grounded = true;
    health = MAX_HEALTH;
    body_shape = nullptr;
    arm_shape = nullptr;

    kb_velocity = glm::vec3(0.0f);
}

void Player::SetShapes(BasicShape* body, BasicShape* arm) {
    body_shape = body;
    arm_shape = arm;
}

void Player::Move(float dx) {
    if (glm::length(kb_velocity) > 0.1f) return;
    position.x += dx;
    if (position.x > 2.0f) position.x = 2.0f;
    if (position.x < -2.0f) position.x = -2.0f;
}

void Player::Jump() {
    if (is_grounded) {
        velocity_y = 0.8f;
        is_grounded = false;
    }
}

void Player::TakeDamage(glm::vec3 enemy_position, float kb_force) {
    if (iframe_timer > 0.0f) return;
    else {
        health--;
        regen_timer = 0.0f;
        if (health < 0) health = 0;
        iframe_timer = IFRAME_DURATION;
        glm::vec3 direction = glm::normalize(position - enemy_position);
        direction.y = 0.0f;
        kb_velocity = direction * kb_force;
        velocity_y = 0.25f;
        is_grounded = false;
    }
}

void Player::Update(float delta_time, bool is_firing) {
    velocity_y -= 1.5f * delta_time;
    position.y += velocity_y * delta_time;
    if (position.y <= GROUND_LEVEL) {
        position.y = GROUND_LEVEL;
        velocity_y = 0.0f;
        is_grounded = true;
    }

    position += kb_velocity * delta_time;
    kb_velocity *= pow(0.01f, delta_time);
    position.x = glm::clamp(position.x, -2.0f, 2.0f);   

    if (iframe_timer > 0.0f) {
        iframe_timer -= delta_time;
    }

    regen_timer += delta_time;
    if (regen_timer >= REGEN_DELAY && health < MAX_HEALTH) {
        health = glm::min(health + REGEN_RATE * delta_time, MAX_HEALTH);
    }

    UpdateBullets(delta_time, is_firing);
}

void Player::UpdateBullets(float delta_time, bool is_firing) {
    fire_rate += delta_time;
    if (is_firing && fire_rate >= FIRE_INTERVAL) {
        float rad = glm::radians(arm_angle);
        Bullet b;
        b.position = GetHand();
        b.velocity = glm::vec3(cos(rad) * 4.5f, sin(rad) * 4.5f, 0.0f);
        b.angle = arm_angle;
        b.active = true;
        bullets.push_back(b);
        fire_rate = 0.0f;
    }
}

void Player::RotateArm(float mouse_x, float mouse_y, float screen_width, float screen_height, glm::vec2 camera_offset, float zoom) {
    float ndc_x = (mouse_x / screen_width) * 2.0f - 1.0f;
    float ndc_y = -((mouse_y / screen_height) * 2.0f - 1.0f);
    
    float world_x = (ndc_x / zoom) - camera_offset.x;
    float world_y = (ndc_y / zoom) - camera_offset.y;
    
    float dx = world_x - position.x;
    float dy = world_y - position.y;
    arm_angle = glm::degrees(atan2(dy, dx));
}

glm::vec3 Player::GetHand() const {
    float rad = glm::radians(arm_angle);
    float barrel_x_offset = 0.08f;
    float barrel_y_offset = -0.02f;

    if (arm_angle > -90.0f && arm_angle < 90.0f) {
        barrel_y_offset = 0.02f;
    }

    float rotate_x = barrel_x_offset * cos(rad) - barrel_y_offset * sin(rad);
    float rotate_y = barrel_x_offset * sin(rad) + barrel_y_offset * cos(rad);
    return glm::vec3(position.x + rotate_x, position.y + rotate_y, 0.0f);
}

void Player::Draw(unsigned int shader_program, glm::mat4 view_matrix) {
    if (!body_shape || !arm_shape) return;
    
    glUniform1i(glGetUniformLocation(shader_program, "is_textured"), false);
    float r = (iframe_timer > 0.0f) ? 1.0f : 0.2f;
    float g = (iframe_timer > 0.0f) ? 1.0f : 0.8f;
    float b = (iframe_timer > 0.0f) ? 1.0f : 0.2f;
    glUniform4f(glGetUniformLocation(shader_program, "set_color"), r, g, b, 1.0f);

    
    glm::mat4 body_model = view_matrix;
    body_model = glm::translate(body_model, position);
    body_model = glm::scale(body_model, glm::vec3(0.08f, 0.16f, 1.0f));
    
    glUniformMatrix4fv(glGetUniformLocation(shader_program, "model"), 1, GL_FALSE, &body_model[0][0]);
    body_shape->Draw();
    
    glUniform1i(glGetUniformLocation(shader_program, "is_textured"), true);
    glUniform4f(glGetUniformLocation(shader_program, "set_color"), 1.0f, 1.0f, 1.0f, 1.0f);
    
    glm::mat4 arm_model = view_matrix;
    arm_model = glm::translate(arm_model, position);
    arm_model = glm::rotate(arm_model, glm::radians(arm_angle), glm::vec3(0.0f, 0.0f, 1.0f));
    //glm reflect?
    float scale_y = 0.124f;
    if (arm_angle > -90.0f && arm_angle < 90.0f) {
        scale_y = -0.124f;
    }
    arm_model = glm::scale(arm_model, glm::vec3(0.15f, scale_y, 1.0f));
    
    glUniformMatrix4fv(glGetUniformLocation(shader_program, "model"), 1, GL_FALSE, &arm_model[0][0]);
    arm_shape->Draw();
}