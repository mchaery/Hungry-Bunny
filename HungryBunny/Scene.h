// Scene.h
#pragma once
#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <SDL_mixer.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include "Utility.h"
#include "Entity.h"
#include "Map.h"

extern int g_lives;

struct GameState
{
    Map* map;
    Entity* player;
    Entity* enemies;
    Entity* carrots;
    Entity* heart;

    Mix_Music* bgm;
    Mix_Chunk* collect_sfx;

    int next_scene_id;
};

class Scene {
public:
    //const int ENEMY_COUNT = 0;

    GameState m_game_state;

    virtual void initialise() = 0;
    virtual void update(float delta_time) = 0;
    virtual void render(ShaderProgram* program) = 0;
    virtual bool is_complete() const = 0;
    virtual glm::vec3 get_init_pos() const { return(glm::vec3(3.0f, -3.0f, 0.0f)); }

    GameState const get_state()             const { return m_game_state; }
    virtual const int get_number_of_enemies() const { return 0; }
    Entity* const get_player()              const { return m_game_state.player; }
};
