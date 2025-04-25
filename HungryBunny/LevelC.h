// LevelC.h
#include "Scene.h"

class LevelC : public Scene {
public:
    const int CARROT_COUNT = 25;
    const int ENEMY_COUNT = 5;
    int COLLECTED_CARROT = 0;
    glm::vec3 init_position = glm::vec3(1.5f, -1.75f, 0.0f);

    ~LevelC();

    void initialise() override;
    void update(float delta_time) override;
    void render(ShaderProgram* program) override;
    const int get_number_of_enemies() const override { return ENEMY_COUNT; }
    bool is_complete() const override { return COLLECTED_CARROT >= CARROT_COUNT; }
    glm::vec3 get_init_pos() const override { return init_position; }

};