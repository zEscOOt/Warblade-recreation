#pragma once
#include "raylib-cpp.hpp"
#include "time.hpp"
#include <vector>
#include <random>
#include <algorithm>

/*  DOCUMENTATION
    If you want to transition from one "scene" to another
    use the function doomTransition.start() under an if statment, otherwise you'd be stuck in
    the same animation loop forever.

    doomTransition.start() takes in 2 void functions, but either they have to be standardized (no parameter functions)
    or use lambdas, something like this:

    doomTransition.start(
                [&](){
                    Scene0();
                },

                [&](){
                    Scene1();
                }
            );

    Later you need to set the variable "currentScene" by adding a new enum inside the "enum class Scene", or using any available
    which depends what do you want to do with it.

    Now you need to set up the "currentScene" inside a switch statement, whatever is inside the switch statement
    will be the thing that you'll see on the screen after the transition has ended.

*/

// In all seriousness don't touch anything here

class Dmelt
{
private:

    timer timedt;

    struct column
    {
        float y = 0.0f;
        float delay = 0.0f;
        float speed = 100.0f;
    };

    std::vector<column> dColumns;

    int screenWidth{};
    int screenHeight{};

    RenderTexture2D prevScreen{};
    RenderTexture2D currentScreen{};

    bool active = false;
    bool initialized = false;

    float elapsedTime = 0.0f;

    static constexpr int columnWidth = 4;

public:

    void init(int width, int height)
    {
        screenWidth = width;
        screenHeight = height;

        prevScreen = LoadRenderTexture(width, height);
        currentScreen = LoadRenderTexture(width, height);

        int count = (width + columnWidth - 1) / columnWidth;

        dColumns.resize(count);

        initialized = true;
    }

    template<typename OldScene, typename NewScene>
    void start(OldScene prevScene, NewScene currScene)
    {
        if (!initialized || active)
            return;

        BeginTextureMode(prevScreen);
        ClearBackground(BLACK);

        prevScene();

        EndTextureMode();

        BeginTextureMode(currentScreen);
        ClearBackground(BLACK);

        currScene();

        EndTextureMode();

        std::mt19937 rng(std::random_device{}());

        std::uniform_real_distribution<float> dist(0.0f, 0.35f);

        for (auto& col : dColumns)
        {
            col.y = 0.0f;
            col.delay = dist(rng);
            col.speed = 100.0f;
        }

        elapsedTime = 0.0f;
        active = true;
    }

    void update(float dt)
    {
        if (!active) {
            return;
        }

        dt = std::clamp(dt, 0.0f, 0.05f);

        elapsedTime += dt;

        bool finished = true;

        for (auto& col : dColumns)
        {
            if (elapsedTime >= col.delay && col.y < screenHeight)
            {
                col.speed += 3000.0f * dt;
                col.y += col.speed * dt;
            }

            if (col.y < screenHeight) {
                finished = false;
            }
        }

        if (finished) {
            active = false;
        }
    }

    void draw()
    {
        DrawTexturePro(
            currentScreen.texture,
            {
                0.0f,
                0.0f,
                (float)screenWidth,
                -(float)screenHeight
            },
            {
                0.0f,
                0.0f,
                (float)screenWidth,
                (float)screenHeight
            },
            { 0.0f, 0.0f },
            0.0f,
            WHITE
        );

        for (int i = 0; i < (int)dColumns.size(); ++i)
        {
            const auto& col = dColumns[i];

            float x = (float)i * columnWidth;

            float width = std::min(
                (float)columnWidth,
                (float)screenWidth - x
            );

            if (col.y >= screenHeight) {
                continue;
            }

            DrawTexturePro(
                prevScreen.texture,
                {
                    x,
                    0.0f,
                    width,
                    -(float)screenHeight
                },
                {
                    x,
                    col.y,
                    width,
                    (float)screenHeight
                },
                { 0.0f, 0.0f },
                0.0f,
                WHITE
            );
        }
    }

    bool isActive() const
    {
        return active;
    }

    void unload()
    {
        if (!initialized) {
            return;
        }

        UnloadRenderTexture(prevScreen);
        UnloadRenderTexture(currentScreen);

        dColumns.clear();

        initialized = false;
        active = false;
    }
};
