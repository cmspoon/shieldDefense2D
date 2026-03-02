#ifndef PlAYER_HPP
#define PlAYER_HPP

#include <glm/glm.hpp>
#include "../support/basic_shape.hpp"

class Player {
    private:
        glm::vec3 position;
        float arm_angle;
        float velocity_y;
        bool is_grounded;
        const float GROUND_LEVEL = -0.54f;
        BasicShape* body_shape;
        BasicShape* arm_shape;

    public:
        Player(glm::vec3 start_pos = glm::vec3(0.0f));
        void SetShapes(BasicShape* body, BasicShape* arm);
        void Move(float delta_x);
        void Jump();
        void Update(float delta_time);
        void RotateArm(float mouse_x, float mouse_y, float screen_width, float screen_height, glm::vec2 cam_offset, float zoom);
        glm::vec3 GetPosition() const { return position; };
        float GetArmAngle() const { return arm_angle; };
        glm::vec3 GetHand() const;
        void Draw(unsigned int shader_program, glm::mat4 view_matrix);
};

#endif