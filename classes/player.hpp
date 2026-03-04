#ifndef PlAYER_HPP
#define PlAYER_HPP

#include <glm/glm.hpp>
#include "../support/basic_shape.hpp"

struct Bullet {
    glm::vec3 position;
    glm::vec3 velocity;
    float angle;
    bool active;
};

class Player {
    private:
        glm::vec3 position;
        float arm_angle;
        float velocity_y;
        float health;
        float regen_timer = 0.0f;
        float iframe_timer = 0.0f;
        bool is_grounded;
        const float MAX_HEALTH = 5.0f;
        const float REGEN_RATE = 0.5f;
        const float REGEN_DELAY = 3.0f;
        const float IFRAME_DURATION = 0.2f;
        const float GROUND_LEVEL = -0.54f;
        BasicShape* body_shape;
        BasicShape* arm_shape;

        float fire_rate = 0.0f;
        const float FIRE_INTERVAL = 0.5f;

        glm::vec3 kb_velocity;

    public:
        Player(glm::vec3 start_pos = glm::vec3(0.0f));
        void SetShapes(BasicShape* body, BasicShape* arm);
        void Move(float delta_x);
        void Jump();
        void TakeDamage(glm::vec3 enemy_position, float damage);
        bool IsAlive() const { return health > 0; }
        float GetHealth() const { return health; }
        float GetMaxHealth() const { return MAX_HEALTH; }
        void Update(float delta_time, bool is_firing);
        void UpdateBullets(float delta_time, bool is_firing);
        void RotateArm(float mouse_x, float mouse_y, float screen_width, float screen_height, glm::vec2 cam_offset, float zoom);
        glm::vec3 GetPosition() const { return position; };
        float GetArmAngle() const { return arm_angle; };
        glm::vec3 GetHand() const;
        void Draw(unsigned int shader_program, glm::mat4 view_matrix);

        std::vector<Bullet> bullets;
};

#endif