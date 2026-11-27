#include "raylib-cpp.hpp"
#include "src/entity.hpp"
#include "src/tools.hpp"
#include "src/menu.hpp"
#include "src/doom.hpp"
#include "src/time.hpp"
#include "src/ini.h"
#include "src/level_loader.hpp"
#include <vector>
#include <iostream>
#include <algorithm>

enum class Scene
{
    MAIN_MENU,
    LEVEL0,
    LEVEL1
};


//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------

    config conf;
    currentMusic music;

    loadConfig("./config.ini", &conf);

    bool loadConfigOnce = true;
    bool startNewLevel = false;
    bool exitFromMenu = false;

    const int screenWidth = conf.WIDTH;
    const int screenHeight = conf.HEIGHT;
    const float playerAccel = 1.0f;

    // Texture/Sprite initializations ALWAYS goes AFTER window initialization
    InitWindow(screenWidth, screenHeight, "raylib [core] example - input keys");

    InitAudioDevice();

    HideCursor();

    if (!IsAudioDeviceReady()) {
        TraceLog(LOG_ERROR, "Audio device failed to initialize!");
    }

    Scene currentScene = Scene::MAIN_MENU;
    Dmelt doomTransition;
    musicStream musicHandler;

    // Don't be a dumbass like me and put the background or ANY initializations before the window initialization...
    RenderTexture2D bgRender = LoadRenderTexture(
        virtualResolutions::VIRTUAL_WIDTH,
        virtualResolutions::VIRTUAL_HEIGHT
    );

    doomTransition.init(virtualResolutions::VIRTUAL_WIDTH, virtualResolutions::VIRTUAL_HEIGHT);

    // Fullscreen after initialization from config
    if (conf.FULLSCREEN == 1 && loadConfigOnce) {
        ToggleFullscreen();
        SetWindowSize(conf.WIDTH, conf.HEIGHT);
        loadConfigOnce = false;
    }

    // temp music (later will change it)
    PlayMusicStream(musicHandler.loadMusic("title0.mp3"));

    

    Player player;
    // TODO: create a global entity vector with type unique_ptr for easier application of .applyVelocity() and .Draw()
    // it would be able to store every polymorth of entity such as player, enemy, projectile etc. in 1 vector


    timer timedt;

    //Texture2D bg = LoadTexture("media/textures/background/space0.png");

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second

    // Main game loop
    while (!exitGame)    // Detect window close button or ESC key
    {

        if (IsKeyPressed(KEY_T) && currentScene == Scene::LEVEL0) {
            startNewLevel = true;
        }

        if (IsKeyPressed(KEY_ESCAPE) && currentScene == Scene::MAIN_MENU) {
            exitGame = true;
        }

        // Later I will create function for this exact issue
        if (IsKeyPressed(KEY_ESCAPE) && (currentScene == Scene::LEVEL0 || currentScene == Scene::LEVEL1)) {
            menuVisible = visible::ON;
            currentScene = Scene::MAIN_MENU;
        }

        UpdateMusicStream(currM.CURRENT); // Updates any current sound that is palying through the "Music Streaming" pipeline
        audioConfigLoad();

        float scale = std::min(
            (float)GetScreenWidth() / virtualResolutions::VIRTUAL_WIDTH,
            (float)GetScreenHeight() / virtualResolutions::VIRTUAL_HEIGHT
        );

        int scaledWidth = virtualResolutions::VIRTUAL_WIDTH * scale;
        int scaledHeight = virtualResolutions::VIRTUAL_HEIGHT * scale;

        int offsetX = (GetScreenWidth() - scaledWidth) / 2;
        int offsetY = (GetScreenHeight() - scaledHeight) / 2;

        // Update
 	    //----------------------------------------------------------------------------------
        if (menuVisible == visible::OFF) { // Makes sure the player can't move around while the menu is visible
            if (IsKeyDown(KEY_RIGHT)) player.updateVelocity(playerAccel, 0.0f);
            if (IsKeyDown(KEY_LEFT)) player.updateVelocity(-playerAccel, 0.0f);
            if (IsKeyDown(KEY_UP)) player.updateVelocity(0.0f, -playerAccel);
            if (IsKeyDown(KEY_DOWN)) player.updateVelocity(0.0f, playerAccel);
        

            player.applyVelocity();

        }
        //----------------------------------------------------------------------------------
        // Draw
        //----------------------------------------------------------------------------------

        BeginDrawing();

        ClearBackground(BLACK);

        // Check out the documentation inside doom.hpp to understand how to set this up
        if (currentScene == Scene::MAIN_MENU &&
            menuVisible == visible::OFF)
        {
            doomTransition.start(
                [&](){
                    DrawTextureEx(
                        tex.loadSprite("space0.png", BACKGROUND),
                        { 0, 0 },
                        0.0f,
                        3.0f,
                        WHITE
                    );

                    menuInit();
                },

                [&](){
                    test();
                    player.Draw();
                }
            );

            currentScene = Scene::LEVEL0;
        }
        else if (currentScene == Scene::LEVEL0 &&
            menuVisible == visible::OFF && startNewLevel) {
            startNewLevel = false;

            doomTransition.start(
                [&](){
                    test();
                    player.Draw();
                },

                [&](){
                    
                    DrawTextureEx(
                        tex.loadSprite("supernova1.png", BACKGROUND),
                        { 0, 0 },
                        0.0f,
                        7.0f,
                        WHITE);

                    player.Draw();
                }
            );
            currentScene = Scene::LEVEL1;
        }

        BeginTextureMode(bgRender);

        ClearBackground(BLACK);

        if (doomTransition.isActive())
        {
            doomTransition.update(GetFrameTime());
            doomTransition.draw();
        }
        else
        {
            switch (currentScene)
            {
            case Scene::MAIN_MENU:

                DrawTextureEx(
                    tex.loadSprite("space0.png", BACKGROUND),
                    { 0, 0 },
                    0.0f,
                    3.0f,
                    WHITE
                );

                menuInit();
                break;

            case Scene::LEVEL0:

                test();
                player.Draw();
                break;

            case Scene::LEVEL1:

                DrawTextureEx(
                    tex.loadSprite("supernova1.png", BACKGROUND),
                    { 0, 0 },
                    0.0f,
                    7.0f,
                    WHITE);

                player.Draw();
                break;
            }

        }

        EndTextureMode();

        // virtual resolutions used for proper scaling //

        Rectangle source = {
            0.0f,
            0.0f,
            (float)virtualResolutions::VIRTUAL_WIDTH,
            -(float)virtualResolutions::VIRTUAL_HEIGHT
        };

        Rectangle dest = {
            (float)offsetX,
            (float)offsetY,
            (float)scaledWidth,
            (float)scaledHeight
        };

        DrawTexturePro(
            bgRender.texture,
            source,
            dest,
            { 0, 0 },
            0.0f,
            WHITE
        );

        EndDrawing();

        //----------------------------------------------------------------------------------
    }
    return 0;
}
