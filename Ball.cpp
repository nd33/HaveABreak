#include "ball.h"

Ball::Ball()
    : GameObject(), Radius(12.5f), Stuck(true) { }

Ball::Ball(glm::vec2 pos, float radius, glm::vec2 velocity, Texture2D sprite)
    : GameObject(pos, glm::vec2(radius * 2.0f, radius * 2.0f), sprite, glm::vec3(1.0f), velocity), Radius(radius), Stuck(true) { }

glm::vec2 Ball::Move(float dt, unsigned int window_width)
{
    if (!this->Stuck)
    {
        this->Position += this->Velocity * dt;
        // bounce off window walls
        if (this->Position.x <= 0.0f)
        {
            this->Velocity.x = -this->Velocity.x;
            this->Position.x = 0.0f;
        }
        else if (this->Position.x + this->Size.x >= window_width)
        {
            this->Velocity.x = -this->Velocity.x;
            this->Position.x = window_width - this->Size.x;
        }
    }
    return this->Position;
}

void Ball::Reset(glm::vec2 position, glm::vec2 velocity)
{
    this->Position = position;
    this->Velocity = velocity;
    this->Stuck = true;
}

void Ball::DoBounce(GameObject* player) {
    if (!this->Stuck)
    {
        // check where it hit the player's paddle, and change velocity based on where it hit the board
        float centerBoard = player->Position.x + player->Size.x / 2.0f;
        float distance = (this->Position.x + this->Radius) - centerBoard;
        float percentage = distance / (player->Size.x / 2.0f);
        float strength = 2.0f;
        glm::vec2 oldVelocity = this->Velocity;
        this->Velocity.x = INITIAL_BALL_VELOCITY.x * percentage * strength;
        this->Velocity.y = -this->Velocity.y;
        this->Velocity = glm::normalize(this->Velocity) * glm::length(oldVelocity) * 1.02f; // increse ball's velocity with small amount at each bounce
    }
}