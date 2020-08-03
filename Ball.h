#ifndef BALL_H
#define BALL_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "game_object.h"
#include "texture.h"

const glm::vec2 INITIAL_BALL_VELOCITY(100.0f, -350.0f);

class Ball : public GameObject
{
public:	
    float   Radius;
    bool    Stuck;
    Ball();
    Ball(glm::vec2 pos, float radius, glm::vec2 velocity, Texture2D sprite);
    glm::vec2 Move(float dt, unsigned int window_width);
    void      Reset(glm::vec2 position, glm::vec2 velocity);
    void DoBounce(GameObject* player);
};

#endif