#include "support/environment_setup.hpp"
#include "support/build_shapes.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "classes/Shader.hpp"
#include "classes/Player.hpp"
#include "classes/Enemy.hpp"
#include <vector>
#include <cstdlib>
#include <ctime>

/// \brief Processes user input for the GLFW window.
void processInput(GLFWwindow *window, float delta_time);
void mouse_callback(GLFWwindow* window, double x_pos, double y_pos);

// settings
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 600;

// globalb
glm::vec4 clear_color (0.2f, 0.2f, 0.2f, 1.0f);
glm::mat4 identity (1.0f);
glm::mat4 model (1.0f);
bool space_pressed = false;

bool leftclick_pressed = false;
bool game_over = false;

// camera
glm::vec2 cam_offset(0.0f, 0.0f);
float cam_zoom = 1.4f;

// controls
double mouse_x = SCR_WIDTH / 2.0;
double mouse_y = SCR_HEIGHT / 2.0;

// player
Player* player;

// bullets
struct Bullet {
    glm::vec3 position;
    glm::vec3 velocity;
    float angle;
    bool active;
};
std::vector<Bullet> bullets;
float fire_rate = 0.0f;
const float INTERVAL = 0.5f;
const float ENEMY_SPAWN_INTERVAL = 2.0f;


// enemies
std::vector<Enemy*> enemies;
float enemy_spawn_timer = 0.0f;

// power core
glm::vec3 power_core_position(0.0f, -0.2f, 0.0f);
int shield_health = 4;
float shield_scale = 1.0f;

int main()
{
    std::string title = "Shield Defense 2D";
    GLFWwindow* window = InitializeEnvironment(SCR_WIDTH, SCR_HEIGHT, title);
    if(window == NULL) {
        std::cout << "Failed to initialize environment" << std::endl;
        return -1;
    }
    
    glfwSetCursorPosCallback(window, mouse_callback);
    
    Shader shader_program("shaders/shader.vertex.glsl",
                          "shaders/shader.fragment.glsl");

    unsigned int core_texture = GetTexture("images/core.jpg");
    unsigned int grass_texture = GetTexture("images/grass.jpg");
    unsigned int gun_texture = GetTexture("images/gun.png");
    unsigned int sky_texture = GetTexture("images/sky.jpg");


    VAOStruct vao;
    
    glGenVertexArrays(1, &(vao.id));
    AttributePointer position_attr = BuildAttribute(3, GL_FLOAT, false, 3*sizeof(float), 0);
    vao.attributes.push_back(position_attr);

    VAOStruct texture_vao;
    glGenVertexArrays(1, &(texture_vao.id));
    AttributePointer texture_position_attr = BuildAttribute(3, GL_FLOAT, false, 5*sizeof(float), 0);
    AttributePointer texture_coord_attr = BuildAttribute(2, GL_FLOAT, false, 5*sizeof(float), 3*sizeof(float));
    texture_vao.attributes.push_back(texture_position_attr);
    texture_vao.attributes.push_back(texture_coord_attr);

    // shapes

    BasicShape sky = GetTexturedRectangle(texture_vao, glm::vec3(-4.0f, -1.0f, 0.0f), 8.0f, 4.0f, 1.0f);
    BasicShape grass = GetTexturedRectangle(texture_vao,glm::vec3(-4.0f, -1.3f, 0.0f), 8.0f, 0.6f, 1.0f);
    
    BasicShape power_core = GetTexturedCircle(texture_vao, 0.1f, 40, power_core_position, 1.0f);
    BasicShape shield = GetCircle(vao, 0.25f, 60, glm::vec3(0.0f, 0.0f, 0.0f));
    
    BasicShape player_body = GetTexturedRectangle(texture_vao, glm::vec3(-0.5f, -1.0f, 0.0f), 1.0f, 2.0f, 1.0f);
    BasicShape player_arm = GetTexturedRectangle(texture_vao, glm::vec3(-0.2f, -0.4f, 0.0f), 1.0f, 1.0f, 1.0f);

    BasicShape bullet_shape = GetTexturedRectangle(texture_vao, glm::vec3(0.0f, -0.5f, 0.0f), 1.0f, 1.0f, 1.0f);

    BasicShape enemy_shape = GetTexturedRectangle(texture_vao, glm::vec3(-0.5f, -1.0f, 0.0f), 1.0f, 2.0f, 1.0f);

    player = new Player(glm::vec3(0.4f, -0.5f, 0.0f));
    player->SetShapes(&player_body, &player_arm);
    
    for (int i = 0; i < 5; i++) {
        Enemy* enemy = new Enemy();
        enemy->SetShape(&enemy_shape);
        enemies.push_back(enemy);
    }

    if (!enemies.empty()) {
        float spawn_x = (rand() % 2 == 0) ? -4.0f : 4.0f;
        enemies[0]->Spawn(spawn_x, -0.57f);
    }

    float delta_time = 0.0f;
    float last_frame = 0.0f;

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // time
        // ----
        float current_frame = glfwGetTime();
        delta_time = current_frame - last_frame;
        last_frame = current_frame;
        
        // input
        // -----
        processInput(window, delta_time);

        // camera
        // ------
        cam_offset.x = -player->GetPosition().x;
        cam_offset.y = 0.7f - 0.4f / cam_zoom;
        
        int actual_width, actual_height;
        glfwGetFramebufferSize(window, &actual_width, &actual_height);
        float aspect = (float)actual_width / (float)actual_height;

        glm::mat4 view = identity;
        view = glm::scale(view, glm::vec3(1.0f / aspect, 1.0f, 1.0f));
        view = glm::scale(view, glm::vec3(cam_zoom, cam_zoom, 1.0f));
        view = glm::translate(view, glm::vec3(cam_offset.x, cam_offset.y, 0.0f));

        // player
        // ----------
        player->RotateArm(mouse_x, mouse_y, actual_width, actual_height, cam_offset, cam_zoom);
        player->Update(delta_time);


        // shooting and collisons
        // --------
        fire_rate += delta_time;
        if (leftclick_pressed && fire_rate >= INTERVAL) {
            Bullet bullet;
            glm::vec3 hand_pos = player->GetHand();
            bullet.position = hand_pos;
            bullet.angle = player->GetArmAngle();
            float angle_rad = glm::radians(bullet.angle);
                bullet.velocity = glm::vec3(cos(angle_rad) * 4.5f, sin(angle_rad) * 4.5f , 0.0f);
            bullet.active = true;
            bullets.push_back(bullet);
            fire_rate = 0.0f;
        }

        for (int i = bullets.size() - 1; i >= 0; i--) {
            if (bullets[i].active) {
                bullets[i].position += bullets[i].velocity * delta_time;
                if (bullets[i].position.x > 4.0f || bullets[i].position.x < -4.0f ||
                    bullets[i].position.y > 4.0f || bullets[i].position.y < -4.0f ||
                    bullets[i].position.y <= -0.68f) {
                    bullets[i].active = false;
                }
            }
            if (!bullets[i].active) {
                bullets.erase(bullets.begin() + i);
            }
        }

        for (int i = bullets.size() - 1; i >= 0; i--) {
            if (bullets[i].active) {
                for (auto& enemy : enemies) {
                    if (enemy->IsActive()) {
                        glm::vec3 enemy_pos = enemy->GetPosition();
                        float collision_distance = 0.035f + enemy->GetSize() * 0.5f;
                        float dist = glm::distance(bullets[i].position, enemy_pos);
                        
                        if (dist < collision_distance) {
                            bullets[i].active = false;
                            enemy->TakeDamage(bullets[i].position, 40.0f);
                            break;
                        }
                    }
                }
            }
        }

        // enemies
        // -------
        for (auto& enemy : enemies) {
            if (enemy->IsActive()) {
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


        enemy_spawn_timer += delta_time;
        if (enemy_spawn_timer >= ENEMY_SPAWN_INTERVAL) {
            enemy_spawn_timer = 0.0f;
            for (auto& enemy : enemies) {
                if (!enemy->IsActive()) {
                    float spawn_x = (rand() % 2 == 0) ? -4.0f : 4.0f;
                    enemy->Spawn(spawn_x, -0.57f);
                    break;
                }
            }
        }

        // render
        // ------
        if (game_over) {
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
        } else {      
            glClearColor(clear_color.r, clear_color.g, clear_color.b, clear_color.a);
            glClear(GL_COLOR_BUFFER_BIT);

            shader_program.use();

            // draw sky
            shader_program.setBool("is_textured", true);
            shader_program.setMat4("model", view);
            glBindTexture(GL_TEXTURE_2D, sky_texture);
            sky.Draw();

            // draw grass
            shader_program.setBool("is_textured", true);
            shader_program.setMat4("model", view); 
            glBindTexture(GL_TEXTURE_2D, grass_texture);
            grass.Draw();
            
            // draw power core
            glm::mat4 power_core_model = view;
            power_core_model = glm::translate(power_core_model, power_core_position);
            power_core_model = glm::scale(power_core_model, glm::vec3(1.0f, 1.0f, 1.0f));
            shader_program.setMat4("model", power_core_model);
            glBindTexture(GL_TEXTURE_2D, core_texture);
            power_core.Draw();
            

            // draw shield
            shader_program.setBool("is_textured", false);
            if (shield_scale != 0.0f) {
                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                shader_program.setVec4("set_color", glm::vec4(0.8f, 0.2f, 0.8f, 0.6f));
                glm::mat4 shield_model = view;
                shield_model = glm::translate(shield_model, power_core_position * 2.0f);
                shield_model = glm::scale(shield_model, glm::vec3(shield_scale, shield_scale, 1.0f));
                shader_program.setMat4("model", shield_model);
                shield.Draw();
                glDisable(GL_BLEND);
            }

            // draw player
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            shader_program.setVec4("set_color", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)); 
            glBindTexture(GL_TEXTURE_2D, gun_texture);shader_program.setVec4("set_color", glm::vec4(0.2, 0.8, 0.2, 1.0));
            player->Draw(shader_program.ID, view);
            glDisable(GL_BLEND);
            shader_program.setBool("is_textured", false);

            // draw bullets
            shader_program.setVec4("set_color", glm::vec4(0.9, 0.9, 0.2, 1.0));
            for (auto& bullet : bullets) {
                if (bullet.active) {
                    glm::mat4 bullet_model = view;
                    bullet_model = glm::translate(bullet_model, bullet.position);
                    bullet_model = glm::rotate(bullet_model, glm::radians(bullet.angle), glm::vec3(0.0f, 0.0f, 1.0f));
                    bullet_model = glm::scale(bullet_model, glm::vec3(0.02f, 0.02f, 1.0f));
                    shader_program.setMat4("model", bullet_model);
                    bullet_shape.Draw();
                }
            }
            
            // draw enemies
            shader_program.setVec4("set_color", glm::vec4(0.9, 0.2, 0.2, 1.0));
            for (auto& enemy: enemies) {
                if (enemy->IsActive()) {
                    enemy->Draw(shader_program.ID, view);
                }
            }
        }
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    sky.DeallocateShape();
    grass.DeallocateShape();
    power_core.DeallocateShape();
    shield.DeallocateShape();
    player_body.DeallocateShape();
    player_arm.DeallocateShape();
    enemy_shape.DeallocateShape();
    bullet_shape.DeallocateShape();

    delete player;
    for (auto& enemy : enemies) {
        delete enemy;
    }

    glDeleteProgram(shader_program.ID);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window, float delta_time)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        player->Move(-1.5f * delta_time);
    }
    
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        player->Move(1.5f * delta_time);
    }

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        if (!space_pressed) {
            player->Jump();
            space_pressed = true;
        }
    } else {
        space_pressed = false;
    }

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        leftclick_pressed = true;
    } else {
        leftclick_pressed = false;
    }

    if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS) {
        cam_zoom += 0.001f;
        if (cam_zoom > 2.5f) cam_zoom = 2.5f;
    }
    
    if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS) {
        cam_zoom -= 0.001f;
        if (cam_zoom < 1.0f) cam_zoom = 1.0f;
    }
} 

void mouse_callback(GLFWwindow* window, double x_pos, double y_pos) {
    mouse_x = x_pos;
    mouse_y = y_pos;
}

