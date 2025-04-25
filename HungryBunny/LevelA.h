// LevelA.h
#include "Scene.h"

class LevelA : public Scene {
public:
    const int CARROT_COUNT = 10;
    const int ENEMY_COUNT = 9;
    int COLLECTED_CARROT = 0;
    glm::vec3 init_position = glm::vec3(2.5f, -4.5f, 0.0f);

    ~LevelA();

    void initialise() override;
    void update(float delta_time) override;
    void render(ShaderProgram* program) override;
    const int get_number_of_enemies() const override { return ENEMY_COUNT; }
    bool is_complete() const override { return COLLECTED_CARROT >= CARROT_COUNT; }
    glm::vec3 get_init_pos() const override { return init_position; }
};