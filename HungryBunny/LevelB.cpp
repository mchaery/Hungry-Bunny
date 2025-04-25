// LevelB.cpp
/**
* Author: Kristie Lee
* Assignment: Rise of the AI
* Date due: 2025-04-05, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#include "LevelB.h"
#include "Utility.h"

#define LEVELB_WIDTH 16
#define LEVELB_HEIGHT 15

constexpr char SPRITESHEET_FILEPATH[] = "assets/bunny.png",
PLATFORM_FILEPATH[] = "assets/spritesheet_grass.png",
GUARD_FILEPATH[] = "assets/spritesheet_greenpig.png",
WALKER_FILEPATH[] = "assets/spritesheet_flame.png",
BOUNCER_FILEPATH[] = "assets/spritesheet_pig.png",
CARROT_FILEPATH[] = "assets/carrot.png",
HEART_FILEPATH[] = "assets/heart.png";

unsigned int LEVELB_DATA[] =
{
0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
0,  0,  0,  0,  0,  0,  0,  0,  0, 11, 12, 12, 12, 13,  0,  0,
0,  0,  0,  0,  0,  0,  0,  0,  0, 22, 23, 23, 82, 24,  0,  0,
0, 11, 12, 12, 12, 13,  0,  0,  0, 22, 82, 23, 23, 24,  0,  0,
0, 22, 67, 23, 23, 60, 56, 56, 56, 61, 23, 71, 23, 38, 13,  0,
0, 22, 23, 23, 67, 24,  0,  0,  0, 33, 34, 34, 34, 34, 35,  0,
0, 22, 23, 68, 27, 35,  0,  0,  0,  0, 65, 65,  0,  0,  0,  0,
0, 33, 34, 34, 35,  0,  0,  0,  0,  0, 76, 76,  0,  0,  0,  0,
0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 87, 87,  0,  0,  0,  0,
0,  0,  0,  0,  0,  0,  0, 11, 12, 12, 12, 12, 13,  0,  0,  0,
0,  0,  0,  0,  0,  0,  0, 22, 78, 23, 23, 79, 24,  0,  0,  0,
0,  0,  0,  0,  0,  0, 11, 39, 79, 23, 23, 27, 35,  0,  0,  0,
0,  0,  0,  0,  0,  0, 22, 79, 23, 78, 79, 24,  0,  0,  0,  0,
0,  0,  0,  0,  0,  0, 33, 34, 34, 34, 34, 35,  0,  0,  0,  0,
0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
};

unsigned int LEVB_CARROT_DATA[] =
{
0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,
0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  0,  0,  0,
0,  0,  0,  0,  0,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
0,  0,  0,  0,  0,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
0,  0,  0,  0,  0,  1,  0,  0,  0,  0,  0,  1,  0,  0,  0,  0,
0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  0,  0,  0,  0,
0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  0,  0,  0,
0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  0,  0,  0,  0,  0,
0,  0,  0,  0,  0,  0,  0,  0,  1,  0,  0,  1,  0,  0,  0,  0,
0,  0,  0,  0,  0,  0,  0,  1,  0,  1,  0,  0,  0,  0,  0,  0,
0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
};

LevelB::~LevelB()
{
    delete[] m_game_state.enemies;
    delete    m_game_state.player;
    delete    m_game_state.map;
    delete    m_game_state.heart;
    Mix_FreeChunk(m_game_state.collect_sfx);
    //Mix_FreeMusic(m_game_state.bgm);
}

void LevelB::initialise()
{
    if (m_game_state.enemies != nullptr) delete[] m_game_state.enemies;
    if (m_game_state.map != nullptr) delete m_game_state.map;

    GLuint map_texture_id = Utility::load_texture(PLATFORM_FILEPATH);
    m_game_state.map = new Map(LEVELB_WIDTH, LEVELB_HEIGHT, LEVELB_DATA, map_texture_id, 1.0f, 11, 8);

    GLuint player_texture_id = Utility::load_texture(SPRITESHEET_FILEPATH);

    int player_walking_animation[4][4] =
    {
    { 8, 9, 10, 11 },  // to move to the left,
    { 12, 13, 14, 15}, // to move to the right,
    { 4, 5, 6, 7 },  // to move upwards,
    { 0, 1, 2, 3 }   // to move downwards
    };

    glm::vec3 acceleration = glm::vec3(0.0f, 0.0f, 0.0f);

    m_game_state.player = new Entity(
        player_texture_id,         // texture id
        5.0f,                      // speed
        acceleration,              // acceleration
        3.2f,                      // jumping power
        player_walking_animation,  // animation index sets
        0.0f,                      // animation time
        4,                         // animation frame amount
        0,                         // current animation index
        4,                         // animation column amount
        4,                         // animation row amount
        1.0f,                      // width
        1.0f,                       // height
        PLAYER
    );
    m_game_state.player->set_position(init_position);

    /* CARROTS stuff */
    COLLECTED_CARROT = 0;
    GLuint carrot_texture_id = Utility::load_texture(CARROT_FILEPATH);
    m_game_state.carrots = new Entity[CARROT_COUNT];

    for (int i = 0; i < CARROT_COUNT; i++) {
        m_game_state.carrots[i] = Entity(carrot_texture_id, 0.0f, 0.3f, 0.3f, COLLECTIBLE);
    }
    Utility::set_position_from_data(m_game_state.carrots, CARROT_COUNT, LEVB_CARROT_DATA, LEVELB_WIDTH, LEVELB_HEIGHT, 1.0f);

    /* ENEMIES stuff */
    GLuint guard_texture_id = Utility::load_texture(GUARD_FILEPATH);
    GLuint walker_texture_id = Utility::load_texture(WALKER_FILEPATH);
    GLuint bouncer_texture_id = Utility::load_texture(BOUNCER_FILEPATH);

    m_game_state.enemies = new Entity[ENEMY_COUNT];

    int enemy_walking_animation[4][4] =
    {
        { 2, 6, 10, 14 },   // LEFT
        { 3, 7, 11, 15 },   // RIGHT
        { 1, 5, 9, 13 },   // UP
        { 0, 4, 8, 12 }    // DOWN
    };


    m_game_state.enemies[0] = Entity(guard_texture_id, 0.85f, 0.4f, 0.4f, ENEMY, GUARD, IDLE);
    m_game_state.enemies[1] = Entity(walker_texture_id, 1.0f, 0.4f, 0.4f, ENEMY, WALKER_UD, IDLE);
    m_game_state.enemies[2] = Entity(walker_texture_id, 1.0f, 0.4f, 0.4f, ENEMY, WALKER_LR, IDLE);
    m_game_state.enemies[3] = Entity(bouncer_texture_id, 1.0f, 0.4f, 0.4f, ENEMY, BOUNCER, IDLE);

    for (int i = 0; i < ENEMY_COUNT; i++)
    {
        m_game_state.enemies[i].set_animation_rows(7);
        m_game_state.enemies[i].set_animation_cols(4);
        m_game_state.enemies[i].set_animation_frames(4);
        m_game_state.enemies[i].set_walking(enemy_walking_animation);
        m_game_state.enemies[i].face_left();
    }
    m_game_state.enemies[0].set_position(glm::vec3(12.7f, -3.5f, 0.0f));
    m_game_state.enemies[1].set_position(glm::vec3(4.5f, -5.0f, 0.0f));
    m_game_state.enemies[2].set_position(glm::vec3(11.0f, -9.4f, 0.0f));
    m_game_state.enemies[3].set_position(glm::vec3(9.0f, -11.0f, 0.0f));

    /* HEART stuff */
    GLuint heart_texture_id = Utility::load_texture(HEART_FILEPATH);
    m_game_state.heart = new Entity(heart_texture_id, 0.0f, 0.45f, 0.45f, HEART);
    m_game_state.heart->set_position(glm::vec3(12.0f, -2.0f, 0.0f));

    /**
     BGM and SFX
     */
    m_game_state.collect_sfx = Mix_LoadWAV("assets/coin.wav");
    Mix_VolumeChunk(m_game_state.collect_sfx, 48);
}

void LevelB::update(float delta_time)
{
    m_game_state.player->update(delta_time, m_game_state.player, m_game_state.enemies, ENEMY_COUNT, m_game_state.map);

    /* CARROT */
    for (int i = 0; i < CARROT_COUNT; i++)
    {
        m_game_state.carrots[i].update(delta_time, m_game_state.player, NULL, NULL, m_game_state.map);
    }

    for (int i = 0; i < CARROT_COUNT; i++) {
        if (m_game_state.carrots[i].get_entity_type() == COLLECTIBLE && m_game_state.carrots[i].get_is_active()) {
            if (m_game_state.player->check_collision(&m_game_state.carrots[i])) {
                m_game_state.carrots[i].deactivate();
                COLLECTED_CARROT++;
                Mix_PlayChannel(-1, m_game_state.collect_sfx, 0);
            }
        }
    }

    /* ENEMIES */
    for (int i = 0; i < ENEMY_COUNT; i++)
    {
        m_game_state.enemies[i].update(delta_time, m_game_state.player, NULL, NULL, m_game_state.map);
    }

    /* HEART */
    m_game_state.heart->update(delta_time, m_game_state.player, NULL, NULL, m_game_state.map);

    if (m_game_state.heart->get_is_active() &&
        m_game_state.player->check_collision(m_game_state.heart)) {

        m_game_state.heart->deactivate();
        if (g_lives < 3) g_lives++;
        Mix_PlayChannel(-1, m_game_state.collect_sfx, 0); 
    }
}


void LevelB::render(ShaderProgram* g_shader_program)
{
    m_game_state.map->render(g_shader_program);
    m_game_state.player->render(g_shader_program);
    m_game_state.heart->render(g_shader_program);
    for (int i = 0; i < ENEMY_COUNT; i++)
        m_game_state.enemies[i].render(g_shader_program);
    for (int i = 0; i < CARROT_COUNT; i++) {
        m_game_state.carrots[i].render(g_shader_program);
    }

    std::string count_text = std::to_string(COLLECTED_CARROT) + "/" + std::to_string(CARROT_COUNT);
    glm::vec3 text_position = m_game_state.player->get_position() + glm::vec3(-0.25f, 0.5f, 0.0f);
    GLuint font_texture_id = Utility::load_texture("assets/font1.png");
    Utility::draw_text(g_shader_program, font_texture_id, count_text, 0.35f, -0.15f, text_position);

}
