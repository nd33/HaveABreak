#include "game.h"
#include "resource_manager.h"
#include "sprite_renderer.h"
#include "ball.h"
#include "game_object.h"

const glm::vec2 PLAYER_SIZE(100.0f, 20.0f);
const float PLAYER_VELOCITY(500.0f);
const float BALL_RADIUS = 12.5f;

SpriteRenderer* Renderer;
GameObject* Player1;
GameObject* Player2;
Ball* ball;

Game::Game(unsigned int width, unsigned int height)
    : State(GAME_ACTIVE), Keys(), Width(width), Height(height)
{

}

Game::~Game()
{
    delete Renderer;
    delete Player1;
    delete Player2;
    delete ball;
}

void Game::Init()
{
    ResourceManager::LoadShader("sprite.vs", "sprite.fs", "sprite");
    // configure shaders
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->Width),
        static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);
    ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
    ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
    // set render-specific options
    Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));
    // load textures
    ResourceManager::LoadTexture("background.jpg", false, "background");
    ResourceManager::LoadTexture("ball.png", true, "face");
    ResourceManager::LoadTexture("paddle.png", true, "paddle");
    // configure game objects
    glm::vec2 player1Pos = glm::vec2(this->Width / 2.0f - PLAYER_SIZE.x / 2.0f, this->Height - PLAYER_SIZE.y);
    Player1 = new GameObject(player1Pos, PLAYER_SIZE, ResourceManager::GetTexture("paddle"));
    glm::vec2 player2Pos = glm::vec2(this->Width / 2.0f - PLAYER_SIZE.x / 2.0f, 0);
    Player2 = new GameObject(player2Pos, PLAYER_SIZE, ResourceManager::GetTexture("paddle"));

    glm::vec2 ballPos = player1Pos + glm::vec2(PLAYER_SIZE.x / 2.0f - BALL_RADIUS,
        -BALL_RADIUS * 2.0f);
    ball = new Ball(ballPos, BALL_RADIUS, INITIAL_BALL_VELOCITY,
        ResourceManager::GetTexture("face"));
}

void Game::Update(float dt)
{
    ball->Move(dt, this->Width);
    this->ResolveCollisions();
    if (ball->Position.y >= this->Height || ball->Position.y <= 0)
    {
        this->ResetPlayers();
    }
}

void Game::ProcessInput(float dt)
{
    if (this->State == GAME_ACTIVE)
    {
        float velocity = PLAYER_VELOCITY * dt;
        // move player's paddle
        if (this->Keys[GLFW_KEY_A])
        {
            if (Player1->Position.x >= 0.0f)
            {
                Player1->Position.x -= velocity;
                if (ball->Stuck)
                    ball->Position.x -= velocity;
            }
        }
        if (this->Keys[GLFW_KEY_D])
        {
            if (Player1->Position.x <= this->Width - Player1->Size.x)
            {
                Player1->Position.x += velocity;
                if (ball->Stuck)
                    ball->Position.x += velocity;
            }
        }
        if (this->Keys[GLFW_KEY_LEFT])
        {
            if (Player2->Position.x >= 0.0f)
            {
                Player2->Position.x -= velocity;
            }
        }
        if (this->Keys[GLFW_KEY_RIGHT])
        {
            if (Player2->Position.x <= this->Width - Player2->Size.x)
            {
                Player2->Position.x += velocity;
            }
        }
        if (this->Keys[GLFW_KEY_SPACE])
            ball->Stuck = false;
    }
}

void Game::Render()
{
    if (this->State == GAME_ACTIVE)
    {
        Renderer->DrawSprite(ResourceManager::GetTexture("background"), glm::vec2(0.0f, 0.0f), glm::vec2(this->Width, this->Height), 0.0f);
        Player1->Draw(*Renderer);
        Player2->Draw(*Renderer);
        ball->Draw(*Renderer);
    }
}

// check collision between circle (ball) and rectangle (player)
bool CheckCollision(Ball& one, GameObject& two)
{
    glm::vec2 center(one.Position + one.Radius);
    glm::vec2 rectangle_half_extents(two.Size.x / 2.0f, two.Size.y / 2.0f);
    glm::vec2 rectangle_center(
        two.Position.x + rectangle_half_extents.x,
        two.Position.y + rectangle_half_extents.y
    );
    glm::vec2 difference = center - rectangle_center;
    glm::vec2 clamped = glm::clamp(difference, -rectangle_half_extents, rectangle_half_extents);
    glm::vec2 closest = rectangle_center + clamped;
    difference = closest - center;
    return glm::length(difference) < one.Radius;
}

void Game::ResolveCollisions()
{
    bool result = CheckCollision(*ball, *Player1);
    if (result) {
        ball->DoBounce(Player1);
    }
    result = CheckCollision(*ball, *Player2);
    if (result) {
        ball->DoBounce(Player2);
    }
}

void Game::ResetPlayers()
{
    // reset player & ball on score
    Player1->Position = glm::vec2(this->Width / 2.0f - PLAYER_SIZE.x / 2.0f, this->Height - PLAYER_SIZE.y);
    Player2->Position = glm::vec2(this->Width / 2.0f - PLAYER_SIZE.x / 2.0f, 0);
    ball->Reset(Player1->Position + glm::vec2(PLAYER_SIZE.x / 2.0f - BALL_RADIUS, -(BALL_RADIUS * 2.0f)), INITIAL_BALL_VELOCITY);
}

