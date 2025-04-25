// Menu.cpp
/**
* Author: Kristie Lee
* Assignment: Rise of the AI
* Date due: 2025-04-05, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#include "Menu.h"
#include "Utility.h"

#define LEVEL_WIDTH 10
#define LEVEL_HEIGHT 9

constexpr char MENU_TEXTURE_PATH[] = "assets/start_screen.png",
PLATFORM_FILEPATH[] = "assets/spritesheet_hills.png";

unsigned int MENU_DATA[] =
{
 0,  0,  0, 14,  0, 14,  0,  0, 11, 13,  
 0,  0,  0, 25,  0, 25,  0,  0, 33, 35,  
 0,  0,  0, 25,  0, 25,  0,  0,  0,  0, 
 0,  0, 11, 41, 12, 41, 13,  0,  0,  0, 
 0,  0, 22, 79, 23, 78, 24,  0,  0,  0,  
 0, 11, 39, 23, 67, 80, 24,  0, 84,  0, 
 0, 33, 34, 34, 34, 34, 35,  0,  0, 83,  
 0,  0,  0,  0,  0,  0,  0, 83,  0,  0,  
 0,  0,  0,  0,  0,  0,  0,  0,  0,  0
};
Menu::~Menu()
{
    delete    m_game_state.player;
    delete    m_game_state.map;
    Mix_FreeChunk(m_game_state.collect_sfx);
    //Mix_FreeMusic(m_game_state.bgm);
}


void Menu::initialise()
{
    if (m_game_state.enemies != nullptr) delete[] m_game_state.enemies;
    if (m_game_state.map != nullptr) delete m_game_state.map;

    GLuint map_texture_id = Utility::load_texture(PLATFORM_FILEPATH);
    m_game_state.map = new Map(LEVEL_WIDTH, LEVEL_HEIGHT, MENU_DATA, map_texture_id, 1.0f, 11, 8);

    int player_walking_animation[4][4] =
    {
    { 4, 5, 6, 7 },  // to move to the left,
    { 8, 9, 10, 11}, // to move to the right,
    { 0, 0, 0, 0 },  // to move upwards,
    { 0, 0, 0, 0 }   // to move downwards
    };

    glm::vec3 acceleration = glm::vec3(0.0f, -4.905f, 0.0f);

    m_game_state.player = new Entity();

    /**
     BGM and SFX
     */
    m_background_texture = Utility::load_texture(MENU_TEXTURE_PATH);
}

void Menu::update(float delta_time)
{

}


void Menu::render(ShaderProgram* program)
{
    m_game_state.map->render(program);

    glm::mat4 model_matrix = glm::mat4(1.0f);
    model_matrix = glm::translate(model_matrix, glm::vec3(5.0f, -3.4f, 0.0f));
    model_matrix = glm::scale(model_matrix, glm::vec3(10.0f, 8.0f, 1.0f));

    program->set_model_matrix(model_matrix);

    float vertices[] = {
        -0.5f, -0.5f,   0.5f, -0.5f,   0.5f,  0.5f,
        -0.5f, -0.5f,   0.5f,  0.5f,  -0.5f,  0.5f
    };

    float tex_coords[] = {
        0.0f, 1.0f,   1.0f, 1.0f,   1.0f, 0.0f,
        0.0f, 1.0f,   1.0f, 0.0f,   0.0f, 0.0f
    };

    glBindTexture(GL_TEXTURE_2D, m_background_texture);

    glVertexAttribPointer(program->get_position_attribute(), 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->get_position_attribute());

    glVertexAttribPointer(program->get_tex_coordinate_attribute(), 2, GL_FLOAT, false, 0, tex_coords);
    glEnableVertexAttribArray(program->get_tex_coordinate_attribute());

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableVertexAttribArray(program->get_position_attribute());
    glDisableVertexAttribArray(program->get_tex_coordinate_attribute());
}
