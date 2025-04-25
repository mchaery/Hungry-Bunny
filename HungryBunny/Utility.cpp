﻿// Utility.cpp
/**
* Author: Kristie Lee
* Assignment: Rise of the AI
* Date due: 2025-04-05, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#define LOG(argument) std::cout << argument << '\n'
#define STB_IMAGE_IMPLEMENTATION
#define NUMBER_OF_TEXTURES 1 // to be generated, that is
#define LEVEL_OF_DETAIL 0    // base image level; Level n is the nth mipmap reduction image
#define TEXTURE_BORDER 0     // this value MUST be zero
#define FONTBANK_SIZE 16

#include "Utility.h"
#include <SDL_image.h>
#include "stb_image.h"

GLuint Utility::load_texture(const char* filepath) {
    // STEP 1: Loading the image file
    int width, height, number_of_components;
    unsigned char* image = stbi_load(filepath, &width, &height, &number_of_components, STBI_rgb_alpha);

    if (image == NULL)
    {
        LOG("Unable to load image. Make sure the path is correct.");
        assert(false);
    }

    // STEP 2: Generating and binding a texture ID to our image
    GLuint texture_id;
    glGenTextures(NUMBER_OF_TEXTURES, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexImage2D(GL_TEXTURE_2D, LEVEL_OF_DETAIL, GL_RGBA, width, height, TEXTURE_BORDER, GL_RGBA, GL_UNSIGNED_BYTE, image);

    // STEP 3: Setting our texture filter modes
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // STEP 4: Setting our texture wrapping modes
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // the last argument can change depending on what you are looking for
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // STEP 5: Releasing our file from memory and returning our texture id
    stbi_image_free(image);

    return texture_id;
}

void Utility::draw_text(ShaderProgram* program, GLuint font_texture_id, std::string text, float screen_size, float spacing, glm::vec3 position)
{
    // Scale the size of the fontbank in the UV-plane
    // We will use this for spacing and positioning
    float width = 1.0f / FONTBANK_SIZE;
    float height = 1.0f / FONTBANK_SIZE;

    // Instead of having a single pair of arrays, we'll have a series of pairs—one for each character
    // Don't forget to include <vector>!
    std::vector<float> vertices;
    std::vector<float> texture_coordinates;

    // For every character...
    for (int i = 0; i < text.size(); i++) {
        // 1. Get their index in the spritesheet, as well as their offset (i.e. their position
        //    relative to the whole sentence)
        int spritesheet_index = (int)text[i];  // ascii value of character
        float offset = (screen_size + spacing) * i;

        // 2. Using the spritesheet index, we can calculate our U- and V-coordinates
        float u_coordinate = (float)(spritesheet_index % FONTBANK_SIZE) / FONTBANK_SIZE;
        float v_coordinate = (float)(spritesheet_index / FONTBANK_SIZE) / FONTBANK_SIZE;

        // 3. Inset the current pair in both vectors
        vertices.insert(vertices.end(), {
            offset + (-0.5f * screen_size), 0.5f * screen_size,
            offset + (-0.5f * screen_size), -0.5f * screen_size,
            offset + (0.5f * screen_size), 0.5f * screen_size,
            offset + (0.5f * screen_size), -0.5f * screen_size,
            offset + (0.5f * screen_size), 0.5f * screen_size,
            offset + (-0.5f * screen_size), -0.5f * screen_size,
            });

        texture_coordinates.insert(texture_coordinates.end(), {
            u_coordinate, v_coordinate,
            u_coordinate, v_coordinate + height,
            u_coordinate + width, v_coordinate,
            u_coordinate + width, v_coordinate + height,
            u_coordinate + width, v_coordinate,
            u_coordinate, v_coordinate + height,
            });
    }

    // 4. And render all of them using the pairs
    glm::mat4 model_matrix = glm::mat4(1.0f);
    model_matrix = glm::translate(model_matrix, position);

    program->set_model_matrix(model_matrix);
    glUseProgram(program->get_program_id());

    glVertexAttribPointer(program->get_position_attribute(), 2, GL_FLOAT, false, 0, vertices.data());
    glEnableVertexAttribArray(program->get_position_attribute());
    glVertexAttribPointer(program->get_tex_coordinate_attribute(), 2, GL_FLOAT, false, 0, texture_coordinates.data());
    glEnableVertexAttribArray(program->get_tex_coordinate_attribute());

    glBindTexture(GL_TEXTURE_2D, font_texture_id);
    glDrawArrays(GL_TRIANGLES, 0, (int)(text.size() * 6));

    glDisableVertexAttribArray(program->get_position_attribute());
    glDisableVertexAttribArray(program->get_tex_coordinate_attribute());
}

void Utility::draw_textured_quad(GLuint texture, const glm::mat4& model_matrix, ShaderProgram& shader_program)
{
    float vertices[] = {
        -0.5f, -0.5f,  0.5f, -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,  0.5f, -0.5f,  0.5f
    };

    float tex_coords[] = {
        0.0f, 1.0f,  1.0f, 1.0f,  1.0f, 0.0f,
        0.0f, 1.0f,  1.0f, 0.0f,  0.0f, 0.0f
    };

    shader_program.set_model_matrix(model_matrix);
    glBindTexture(GL_TEXTURE_2D, texture);

    glVertexAttribPointer(shader_program.get_position_attribute(), 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(shader_program.get_position_attribute());

    glVertexAttribPointer(shader_program.get_tex_coordinate_attribute(), 2, GL_FLOAT, false, 0, tex_coords);
    glEnableVertexAttribArray(shader_program.get_tex_coordinate_attribute());

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableVertexAttribArray(shader_program.get_position_attribute());
    glDisableVertexAttribArray(shader_program.get_tex_coordinate_attribute());
}


void Utility::place_entities_from_data(Entity* entities, int& count,
    unsigned int* data, int map_width, int map_height,
    float tile_size, GLuint texture) 
{
    count = 0;

    float x_offset = -(tile_size / 2.0f);
    float y_offset = (tile_size / 2.0f);

    for (int y = 0; y < map_height; y++) {
        for (int x = 0; x < map_width; x++) {
            int index = y * map_width + x;
            if (data[index] == 1) {
                float pos_x = x_offset + (x * tile_size);
                float pos_y = y_offset + (-y * tile_size);

                entities[count].set_position(glm::vec3(pos_x, pos_y, 0));
                entities[count].set_entity_type(COLLECTIBLE);
                entities[count].set_texture_id(texture);
                entities[count].set_width(tile_size * 0.8f); 
                entities[count].set_height(tile_size * 0.8f);
                entities[count].activate();
                count++;
            }
        }
    }
}

void Utility::set_position_from_data(Entity* entities,
    int entity_count,
    unsigned int* data,
    int map_width,
    int map_height,
    float tile_size)
{
    int placed = 0;

    float x_offset = -(tile_size / 2.0f);
    float y_offset = (tile_size / 2.0f);

    for (int y = 0; y < map_height; y++) {
        for (int x = 0; x < map_width; x++) {
            int index = y * map_width + x;
            if (data[index] == 1) {
                if (placed < entity_count) {
                    float pos_x = x_offset + (x * tile_size);
                    float pos_y = y_offset + (-y * tile_size);

                    entities[placed].set_position(glm::vec3(pos_x, pos_y, 0));
                    placed++;
                }
            }
        }
    }
}