// Utility.h
#pragma once
#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <vector>
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include "Entity.h"

//extern ShaderProgram g_view_matrix;

class Utility {
public:
    static GLuint load_texture(const char* filepath);
    
    static void draw_text(ShaderProgram* program, GLuint font_texture_id, std::string text, float screen_size, float spacing, glm::vec3 position);

    static void draw_textured_quad(GLuint texture, const glm::mat4& model_matrix, ShaderProgram& shader_program);

    static void place_entities_from_data(Entity* entities, int& count,
        unsigned int* data, int map_width, int map_height,
        float tile_size, GLuint texture);

    static void set_position_from_data(Entity* entities,
        int entity_count,
        unsigned int* data,
        int map_width,
        int map_height,
        float tile_size);
};