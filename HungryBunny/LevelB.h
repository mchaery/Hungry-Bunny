// LevelB.h
#include "Scene.h"

class LevelB : public Scene {
public:
    const int CARROT_COUNT = 18;
    const int ENEMY_COUNT = 4;
    int COLLECTED_CARROT = 0;
    glm::vec3 init_position = glm::vec3(3.0f, -3.0f, 0.0f);

    ~LevelB();

    void initialise() override;
    void update(float delta_time) override;
    void render(ShaderProgram* program) override;
    const int get_number_of_enemies() const override { return ENEMY_COUNT; }
    bool is_complete() const override { return COLLECTED_CARROT >= CARROT_COUNT; }
    glm::vec3 get_init_pos() const override { return init_position; }

};