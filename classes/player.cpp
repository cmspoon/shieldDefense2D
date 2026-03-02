#include "Player.hpp"
#include <glm/gtc/matrix_transform.hpp>

Player::Player(glm::vec3 start_pos) {
    position = start_pos;
    arm_angle = 0.0f;
    velocity_y = 0.0f;
    is_grounded = true;
    body_shape = nullptr;
    arm_shape = nullptr;
}

void Player::SetShapes(BasicShape* body, BasicShape* arm) {
    body_shape = body;
    arm_shape = arm;
}

void Player::Move(float dx) {
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

void Player::Update(float delta_time) {
    velocity_y -= 1.5f * delta_time;
    position.y += velocity_y * delta_time;
    if (position.y <= GROUND_LEVEL) {
        position.y = GROUND_LEVEL;
        velocity_y = 0.0f;
        is_grounded = true;
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
    glUniform4f(glGetUniformLocation(shader_program, "set_color"), 0.2f, 0.8f, 0.2f, 1.0f);
    
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