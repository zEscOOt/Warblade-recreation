#pragma once
#include "raylib.h"
#include "raylib-cpp.hpp"
#include "time.hpp"
#include "entity.hpp"
#include "tools.hpp"
#include "animationHandler.hpp"
#include "doom.hpp"
#include "ini.h"
#include <unordered_map>

enum visible {
    OFF,
    ON
};

struct menu {
    enum selected {
        PLAY,
        SETTING,
        EXIT,
        UNKOWN,
        INVIS // used when you want to make the menu invisible (only going to be used with the settings menu)
    };

    struct settings {
        enum settingSelect {
            RESOLUTION,
            AUDIO,
            DIFF,
            FULL,
            HIGH,
            EXITSETTINGS,
            NONE
        };

        enum diff {
            EASY,
            NORMAL,
            HARD
        };

        enum full {
            FULL_OFF,
            FULL_ON
        };

        enum high {
            HIGH_OFF,
            HIGH_ON

        };

        enum audio {
            AUD0,
            AUD1,
            AUD2,
            AUD3,
            AUD4,
            AUD5,
            AUD6,
            AUD7,
            AUD8
        };

        enum res {
            RES0,
            RES1,
            RES2,
            RES3,
            RES4,
            RES5,
        };
    };
};

menu::selected selectedButton = menu::selected::UNKOWN;
menu::settings::settingSelect settingButton = menu::settings::settingSelect::NONE;

menu::settings::diff difficulty = menu::settings::diff::EASY; // gg fricking easy
menu::settings::full fullscreen = menu::settings::full::FULL_OFF;
menu::settings::high highlights = menu::settings::high::HIGH_ON;
menu::settings::audio audio = menu::settings::audio::AUD0;
menu::settings::res resolution = menu::settings::res::RES1;

visible menuVisible = ON;
visible hideTitle = OFF;

float alphaFade = 1.0f;
const float alphaSpeed = 0.005f; // how much per second will the alpha be reduced

float opacity = 0.0f;

// With all blocking the opacity variable (Need to create an opacity variable for EACH element)

static float settingOpacity = 0.0f;
static float playOpacity = 0.0f;
static float exitOpacity = 0.0f;

static float settingGOpacity = 0.0f;
static float playGOpacity = 0.0f;
static float exitGOpacity = 0.0f;

float fadeSpeed = 0.125f;

static int selectedMenu = menu::selected::PLAY;

// all this going to be used for the config.ini file
static int W;
static int H;
static int iniAUD;
static int iniDIFF;
static int iniFULL;
static int iniHIGH;
//---------------------------------------------------


bool hasEnded = false;
bool exitGame = false;

const float btnFadeInSpeed = 15.0f;
const float glowSpeed = 0.2f;

spriteHandler tex;
timer timedt;
audioHandler audHandler;

void loadSetting() {
    config conf;
    loadConfig("./config.ini", &conf);

    switch (conf.DIFFICULTY)
    {
    case 0:
        difficulty = menu::settings::diff::EASY;
        break;

    case 1:
        difficulty = menu::settings::diff::NORMAL;
        break;

    case 2:
        difficulty = menu::settings::diff::HARD;
        break;

    default:
        difficulty = menu::settings::diff::EASY;
        break;
    }

    switch (conf.WIDTH)
    {
    case 256:
        resolution = menu::settings::res::RES0;
        break;

    case 960:
        resolution = menu::settings::res::RES1;
        break;

    case 1280:
        resolution = menu::settings::res::RES2;
        break;

    case 1366:
        resolution = menu::settings::res::RES3;
        break;

    case 1600:
        resolution = menu::settings::res::RES4;
        break;

    case 1920:
        resolution = menu::settings::res::RES5;
        break;

    default:
        resolution = menu::settings::res::RES1;
        break;
    }

    switch (conf.AUDIO)
    {
    case 0:
        audio = menu::settings::audio::AUD0;
    break;

    case 1:
        audio = menu::settings::audio::AUD1;
        break;

    case 2:
        audio = menu::settings::audio::AUD2;
        break;

    case 3:
        audio = menu::settings::audio::AUD3;
        break;

    case 4:
        audio = menu::settings::audio::AUD4;
        break;

    case 5:
        audio = menu::settings::audio::AUD5;
        break;

    case 6:
        audio = menu::settings::audio::AUD6;
        break;

    case 7:
        audio = menu::settings::audio::AUD7;
        break;

    case 8:
        audio = menu::settings::audio::AUD8;
        break;

    default:
        audio = menu::settings::audio::AUD0;
        break;
    }

    switch (conf.FULLSCREEN)
    {
    case 0:
        fullscreen = menu::settings::full::FULL_OFF;
        break;

    case 1:
        fullscreen = menu::settings::full::FULL_ON;
        break;

    default:
        fullscreen = menu::settings::full::FULL_OFF;
        break;
    }

    switch (conf.HIGHLIGHT)
    {
    case 0:
        highlights = menu::settings::high::HIGH_OFF;
        break;

    case 1:
        highlights = menu::settings::high::HIGH_ON;
        break;

    default:
        highlights = menu::settings::high::HIGH_ON;
        break;
    }
}

void toggleFullSceenWindow(int windowW, int windowH) {
    if (!IsWindowFullscreen() && fullscreen == menu::settings::full::FULL_ON) {

        ToggleFullscreen();
        SetWindowSize(windowW, windowH);
    }
    else if (IsWindowFullscreen() && fullscreen == menu::settings::full::FULL_OFF) {
        ToggleFullscreen();
        SetWindowSize(windowW, windowH);
    }
}

// Check out this wall of pain and if statements //
void checkOpacity() {

    if (alphaFade < 0.0f) {
        alphaFade = 0.0f;
    }

    if (opacity > 1.0f) {
        opacity = 1.0f;
    }
    if (settingOpacity > 1.0f) {
        settingOpacity = 1.0f;
    }

    if (settingGOpacity > 0.05f) {
        settingGOpacity = 0.05f;
    }

    if (exitOpacity > 1.0f) {
        exitOpacity = 1.0f;
    }

    if (exitGOpacity > 0.05f) {
        exitGOpacity = 0.05f;
    }

    if (playOpacity > 1.0f) {
        playOpacity = 1.0f;
    }

    if (playGOpacity > 0.05f) {
        playGOpacity = 0.05f;
    }
}


// Updates all settings //
void update() {
    switch (audio)
    {
    case menu::settings::audio::AUD0:
        DrawTextureEx(tex.loadSprite("aud0.png", AUD), { 450, 125 }, 0.0f, 1.0f, WHITE);
        SetMusicVolume(currM.CURRENT, 0.0f);
        SetSoundVolume(currS.CURRENT, 0.0f);
        iniAUD = 0;

        isSoundUnlocked = unlock::UNLOCKED;
        PlaySound(audHandler.loadAudio("settingselect.mp3", SFX_UI));
        soundConfigLoad();

        break;
    case menu::settings::audio::AUD1:
        DrawTextureEx(tex.loadSprite("aud1.png", AUD), { 450, 125 }, 0.0f, 1.0f, WHITE);
       SetMusicVolume(currM.CURRENT, 0.125f);
       SetSoundVolume(currS.CURRENT, 0.125f);
        iniAUD = 1;

        isSoundUnlocked = unlock::UNLOCKED;
        PlaySound(audHandler.loadAudio("settingselect.mp3", SFX_UI));
        soundConfigLoad();

        break;
    case menu::settings::audio::AUD2:
        DrawTextureEx(tex.loadSprite("aud2.png", AUD), { 450, 125 }, 0.0f, 1.0f, WHITE);
        SetMusicVolume(currM.CURRENT, 0.250f);
        SetSoundVolume(currS.CURRENT, 0.250f);
        iniAUD = 2;

        isSoundUnlocked = unlock::UNLOCKED;
        PlaySound(audHandler.loadAudio("settingselect.mp3", SFX_UI));
        soundConfigLoad();

        break;
    case menu::settings::audio::AUD3:
        DrawTextureEx(tex.loadSprite("aud3.png", AUD), { 450, 125 }, 0.0f, 1.0f, WHITE);
        SetMusicVolume(currM.CURRENT, 0.375f);
        SetSoundVolume(currS.CURRENT, 0.375f);
        iniAUD = 3;

        isSoundUnlocked = unlock::UNLOCKED;
        PlaySound(audHandler.loadAudio("settingselect.mp3", SFX_UI));
        soundConfigLoad();

        break;
    case menu::settings::audio::AUD4:
        DrawTextureEx(tex.loadSprite("aud4.png", AUD), { 450, 125 }, 0.0f, 1.0f, WHITE);
       SetMusicVolume(currM.CURRENT, 0.5f);
       SetSoundVolume(currS.CURRENT, 0.5f);
        iniAUD = 4;

        isSoundUnlocked = unlock::UNLOCKED;
        PlaySound(audHandler.loadAudio("settingselect.mp3", SFX_UI));
        soundConfigLoad();

        break;
    case menu::settings::audio::AUD5:
        DrawTextureEx(tex.loadSprite("aud5.png", AUD), { 450, 125 }, 0.0f, 1.0f, WHITE);
       SetMusicVolume(currM.CURRENT, 0.625f);
       SetSoundVolume(currS.CURRENT, 0.625f);
        iniAUD = 5;

        isSoundUnlocked = unlock::UNLOCKED;
        PlaySound(audHandler.loadAudio("settingselect.mp3", SFX_UI));
        soundConfigLoad();

        break;
    case menu::settings::audio::AUD6:
        DrawTextureEx(tex.loadSprite("aud6.png", AUD), { 450, 125 }, 0.0f, 1.0f, WHITE);
       SetMusicVolume(currM.CURRENT, 0.75f);
       SetSoundVolume(currS.CURRENT, 0.75f);
        iniAUD = 6;

        isSoundUnlocked = unlock::UNLOCKED;
        PlaySound(audHandler.loadAudio("settingselect.mp3", SFX_UI));
        soundConfigLoad();

        break;
    case menu::settings::audio::AUD7:
        DrawTextureEx(tex.loadSprite("aud7.png", AUD), { 450, 125 }, 0.0f, 1.0f, WHITE);
       SetMusicVolume(currM.CURRENT, 0.875f);
       SetSoundVolume(currS.CURRENT, 0.875f);
        iniAUD = 7;

        isSoundUnlocked = unlock::UNLOCKED;
        PlaySound(audHandler.loadAudio("settingselect.mp3", SFX_UI));
        soundConfigLoad();

        break;
    case menu::settings::audio::AUD8:
        DrawTextureEx(tex.loadSprite("aud8.png", AUD), { 450, 125 }, 0.0f, 1.0f, WHITE);
       SetMusicVolume(currM.CURRENT, 1.0f);
       SetSoundVolume(currS.CURRENT, 1.0f);
        iniAUD = 8;

        isSoundUnlocked = unlock::UNLOCKED;
        PlaySound(audHandler.loadAudio("settingselect.mp3", SFX_UI));
        soundConfigLoad();

        break;
    default:
        DrawTextureEx(tex.loadSprite("aud0.png", AUD), { 450, 125 }, 0.0f, 1.0f, WHITE);
       SetMusicVolume(currM.CURRENT, 0.0f);
       SetSoundVolume(currS.CURRENT, 0.0f);
        iniAUD = 0;

        isSoundUnlocked = unlock::UNLOCKED;
        PlaySound(audHandler.loadAudio("settingselect.mp3", SFX_UI));
        soundConfigLoad();

        break;
    }

    switch (difficulty)
    {
    case menu::settings::diff::EASY:
        DrawTextureEx(tex.loadSprite("easy.png", UI), { 400, 255 }, 0.0f, 1.5f, WHITE);
        iniDIFF = 0;

        isSoundUnlocked = unlock::UNLOCKED;
        PlaySound(audHandler.loadAudio("settingselect.mp3", SFX_UI));
        soundConfigLoad();

        break;

    case menu::settings::diff::NORMAL:
        DrawTextureEx(tex.loadSprite("normal.png", UI), { 440, 240 }, 0.0f, 1.5f, WHITE);
        iniDIFF = 1;

        isSoundUnlocked = unlock::UNLOCKED;
        PlaySound(audHandler.loadAudio("settingselect.mp3", SFX_UI));
        soundConfigLoad();

        break;

    case menu::settings::diff::HARD:
        DrawTextureEx(tex.loadSprite("hard.png", UI), { 400, 250 }, 0.0f, 1.5f, WHITE);
        iniDIFF = 2;

        isSoundUnlocked = unlock::UNLOCKED;
        PlaySound(audHandler.loadAudio("settingselect.mp3", SFX_UI));
        soundConfigLoad();

        break;

    default:
        DrawTextureEx(tex.loadSprite("easy.png", UI), { 400, 255 }, 0.0f, 1.5f, WHITE);
        iniDIFF = 0;

        isSoundUnlocked = unlock::UNLOCKED;
        PlaySound(audHandler.loadAudio("settingselect.mp3", SFX_UI));
        soundConfigLoad();

        break;
    }

    config full;
    loadConfig("./config.ini", &full);

    switch (fullscreen)
    {
    case menu::settings::full::FULL_OFF:
        DrawTextureEx(tex.loadSprite("off.png", UI), { 400, 375 }, 0.0f, 1.5f, WHITE);

        if (full.FULLSCREEN == 1) {
            toggleFullSceenWindow(W, H);
        }

        iniFULL = 0;

        break;
    case menu::settings::full::FULL_ON:

        DrawTextureEx(tex.loadSprite("on.png", UI), { 400, 375 }, 0.0f, 1.5f, WHITE);   
        if (full.FULLSCREEN == 0) {
            toggleFullSceenWindow(W, H);
        }
        iniFULL = 1;

        break;
    }

    switch (highlights)
    {
    case menu::settings::high::HIGH_ON:
        DrawTextureEx(tex.loadSprite("on.png", UI), { 400, 500 }, 0.0f, 1.5f, WHITE);
        iniHIGH = 1;

        isSoundUnlocked = unlock::UNLOCKED;
        PlaySound(audHandler.loadAudio("settingselect.mp3", SFX_UI));
        soundConfigLoad();

        break;

    case menu::settings::high::HIGH_OFF:
        DrawTextureEx(tex.loadSprite("off.png", UI), { 400, 500 }, 0.0f, 1.5f, WHITE);
        iniHIGH = 0;

        isSoundUnlocked = unlock::UNLOCKED;
        PlaySound(audHandler.loadAudio("settingselect.mp3", SFX_UI));
        soundConfigLoad();
        break;

    default:
        DrawTextureEx(tex.loadSprite("on.png", UI), { 400, 500 }, 0.0f, 1.5f, WHITE);
        iniHIGH = 1;

        isSoundUnlocked = unlock::UNLOCKED;
        PlaySound(audHandler.loadAudio("settingselect.mp3", SFX_UI));
        soundConfigLoad();
        break;
    }

    switch (resolution)
    {
    case menu::settings::res::RES0:
        DrawTextureEx(tex.loadSprite("256.png", RES), { 480, 10 }, 0.0f, 1.0f, WHITE);
        SetWindowSize(256,144);

        W = 256;
        H = 144;

        isSoundUnlocked = unlock::UNLOCKED;
        PlaySound(audHandler.loadAudio("settingselect.mp3", SFX_UI));
        soundConfigLoad();
        break;
    case menu::settings::res::RES1:
        DrawTextureEx(tex.loadSprite("960.png", RES), { 480, 10 }, 0.0f, 1.0f, WHITE);
        SetWindowSize(960, 540);

        W = 960;
        H = 540;
               
        isSoundUnlocked = unlock::UNLOCKED;
        PlaySound(audHandler.loadAudio("settingselect.mp3", SFX_UI));
        soundConfigLoad();
        break;
    case menu::settings::res::RES2:
        DrawTextureEx(tex.loadSprite("1280.png", RES), { 480, 10 }, 0.0f, 1.0f, WHITE);
        SetWindowSize(1280, 720);

        W = 1280;
        H = 720;

        isSoundUnlocked = unlock::UNLOCKED;
        PlaySound(audHandler.loadAudio("settingselect.mp3", SFX_UI));
        soundConfigLoad();
        break;
    case menu::settings::res::RES3:
        DrawTextureEx(tex.loadSprite("1366.png", RES), { 480, 10 }, 0.0f, 1.0f, WHITE);
        SetWindowSize(1366, 768);

        W = 1366;
        H = 768;

        isSoundUnlocked = unlock::UNLOCKED;
        PlaySound(audHandler.loadAudio("settingselect.mp3", SFX_UI));
        soundConfigLoad();
        break;
    case menu::settings::res::RES4:
        DrawTextureEx(tex.loadSprite("1600.png", RES), { 480, 10 }, 0.0f, 1.0f, WHITE);
        SetWindowSize(1600, 900);

        W = 1600;
        H = 900;

        isSoundUnlocked = unlock::UNLOCKED;
        PlaySound(audHandler.loadAudio("settingselect.mp3", SFX_UI));
        soundConfigLoad();
        break;
    case menu::settings::res::RES5:
        DrawTextureEx(tex.loadSprite("1920.png", RES), { 480, 10 }, 0.0f, 1.0f, WHITE);
        SetWindowSize(1920, 1080);

        W = 1920;
        H = 1080;

        isSoundUnlocked = unlock::UNLOCKED;
        PlaySound(audHandler.loadAudio("settingselect.mp3", SFX_UI));
        soundConfigLoad();
        break;
    default:
        DrawTextureEx(tex.loadSprite("960.png", RES), { 480, 10 }, 0.0f, 1.0f, WHITE);
        SetWindowSize(960, 540);

        W = 960;
        H = 540;

        isSoundUnlocked = unlock::UNLOCKED;
        PlaySound(audHandler.loadAudio("settingselect.mp3", SFX_UI));
        soundConfigLoad();
        break;
    }

    iniSave(W, H, iniAUD, iniDIFF, iniFULL, iniHIGH);
}

void menuInit() {

    checkOpacity();

    if (!hasEnded) {
        loadSetting();

        if (timedt.getDelta() >= 2.0f && timedt.getDelta() <= 3.5f) {
            DrawTextureEx(tex.loadSprite("warbladeWHITE.png", UI), { 750 , 180 }, 0.0f, 2.0f, Fade(WHITE, (timedt.getDelta() - (2.0f)) / 0.8f));
        }
        else if (timedt.getDelta() >= 3.5f && timedt.getDelta() <= 8.0f) {
            DrawTextureEx(tex.loadSprite("recreationWHITE.png", UI), { 580, 200 }, 0.0f, 2.0f, Fade(WHITE, (timedt.getDelta() - (3.5f)) / 1.2f));
            DrawTextureEx(tex.loadSprite("warbladeWHITE.png", UI), { 750, 180 }, 0.0f, 2.0f, WHITE);
        }
        else if (timedt.getDelta() >= 8.0f && timedt.getDelta() <= 18.0f) {
            DrawTextureEx(tex.loadSprite("title.png", UI), { 580, 200 }, 0.0f, 2.0f, Fade(WHITE, opacity += fadeSpeed));
            DrawTextureEx(tex.loadSprite("play.png", UI), { 400, 500 }, 0.0f, 2.0f, Fade(WHITE, opacity += fadeSpeed));
            DrawTextureEx(tex.loadSprite("setting.png", UI), { 1100, 500 }, 0.0f, 2.0f, Fade(WHITE, opacity += fadeSpeed));
            DrawTextureEx(tex.loadSprite("exit.png", UI), { 700, 700 }, 0.0f, 2.0f, Fade(WHITE, opacity += fadeSpeed));
            DrawRectangle(0, 0, virtualResolutions::VIRTUAL_WIDTH, virtualResolutions::VIRTUAL_HEIGHT, Fade(WHITE, alphaFade -= alphaSpeed * (timedt.getDelta() - (8.0f))));

        }
        else if (timedt.getDelta() >= 18.0f) {
            DrawTextureEx(tex.loadSprite("title.png", UI), { 580, 200 }, 0.0f, 2.0f, WHITE);
                DrawTextureEx(tex.loadSprite("playH.png", UI), { 400, 500 }, 0.0f, 2.0f, WHITE);
                DrawTextureEx(tex.loadSprite("setting.png", UI), { 1100, 500 }, 0.0f, 2.0f, WHITE);
                DrawTextureEx(tex.loadSprite("exit.png", UI), { 700, 700 }, 0.0f, 2.0f, WHITE);

            hasEnded = true;
        }
    }
    else {

        // Need to draw it twice to avoid flickering //
        if (hideTitle == OFF) {
            DrawTextureEx(tex.loadSprite("title.png", UI), { 580, 200 }, 0.0f, 2.0f, WHITE);
        }

        if (selectedButton == menu::selected::UNKOWN) {
            soundConfigLoad();

            isSoundUnlocked = unlock::UNLOCKED;
            PlaySound(audHandler.loadAudio("select.mp3", SFX_UI));

            DrawTextureEx(tex.loadSprite("play.png", UI), { 400, 500 }, 0.0f, 2.0f, WHITE);
            DrawTextureEx(tex.loadSprite("setting.png", UI), { 1100, 500 }, 0.0f, 2.0f, WHITE);
            DrawTextureEx(tex.loadSprite("exit.png", UI), { 700, 700 }, 0.0f, 2.0f, WHITE);

            // To make sure the player is not stuck when the menu starts //
            selectedButton = menu::selected::PLAY;
        }


        if (selectedButton == menu::selected::SETTING) {
            soundConfigLoad();

            playOpacity = 0.0f;
            exitOpacity = 0.0f;

            playGOpacity = 0.0f;
            exitGOpacity = 0.0f;

            settingOpacity += GetFrameTime() * btnFadeInSpeed;
            settingGOpacity += GetFrameTime() * glowSpeed;

            isSoundUnlocked = unlock::UNLOCKED;
            PlaySound(audHandler.loadAudio("select.mp3", SFX_UI));

            checkOpacity();

            DrawTextureEx(tex.loadSprite("play.png", UI), { 400, 500 }, 0.0f, 2.0f, WHITE);

            DrawTextureEx(tex.loadSprite("exit.png", UI), { 700, 700 }, 0.0f, 2.0f, WHITE);

            DrawTextureEx(tex.loadSprite("black.png", UI), { 1100, 500 }, 0.0f, 2.0f, Fade(BLACK, settingGOpacity));
            DrawTextureEx(tex.loadSprite("settingH.png", UI), { 1100, 500 }, 0.0f, 2.0f, Fade(WHITE, settingOpacity));
             
        }

        if (settingButton == menu::settings::settingSelect::RESOLUTION) {
            selectedButton = menu::selected::INVIS;
            hideTitle = ON;

            isSoundUnlocked = unlock::UNLOCKED;
            PlaySound(audHandler.loadAudio("select.mp3", SFX_UI));
            

            DrawTextureEx(tex.loadSprite("res.png", UI), { 100, 0 }, 0.0f, 1.5f, WHITE);

            DrawTextureEx(tex.loadSprite("audio.png", UI), { 100, 125 }, 0.0f, 1.5f, WHITE);

            DrawTextureEx(tex.loadSprite("diff.png", UI), { 100, 250 }, 0.0f, 1.5f, WHITE);

            DrawTextureEx(tex.loadSprite("full.png", UI), { 100, 375 }, 0.0f, 1.5f, WHITE);

            DrawTextureEx(tex.loadSprite("high.png", UI), { 100, 500 }, 0.0f, 1.5f, WHITE);

            DrawTextureEx(tex.loadSprite("exit.png", UI), { 100, 700 }, 0.0f, 1.5f, WHITE);

            DrawTextureEx(tex.loadSprite("arrow.png", UI), { 30, 55 }, 0.0f, 0.5f, WHITE);

            update();

        }

        if (settingButton == menu::settings::settingSelect::AUDIO) {

            isSoundUnlocked = unlock::UNLOCKED;
            PlaySound(audHandler.loadAudio("select.mp3", SFX_UI));

            DrawTextureEx(tex.loadSprite("res.png", UI), { 100, 0 }, 0.0f, 1.5f, WHITE);
            DrawTextureEx(tex.loadSprite("audio.png", UI), { 100, 125 }, 0.0f, 1.5f, WHITE);

            DrawTextureEx(tex.loadSprite("diff.png", UI), { 100, 250 }, 0.0f, 1.5f, WHITE);

            DrawTextureEx(tex.loadSprite("full.png", UI), { 100, 375 }, 0.0f, 1.5f, WHITE);

            DrawTextureEx(tex.loadSprite("high.png", UI), { 100, 500 }, 0.0f, 1.5f, WHITE);   

            DrawTextureEx(tex.loadSprite("exit.png", UI), { 100, 700 }, 0.0f, 1.5f, WHITE);

            DrawTextureEx(tex.loadSprite("arrow.png", UI), { 100, 180 }, 0.0f, 0.5f, WHITE);

            update();
        }

        if (settingButton == menu::settings::settingSelect::DIFF) {

            isSoundUnlocked = unlock::UNLOCKED;
            PlaySound(audHandler.loadAudio("select.mp3", SFX_UI));

            DrawTextureEx(tex.loadSprite("res.png", UI), { 100, 0 }, 0.0f, 1.5f, WHITE);
            DrawTextureEx(tex.loadSprite("audio.png", UI), { 100, 125 }, 0.0f, 1.5f, WHITE);

            DrawTextureEx(tex.loadSprite("diff.png", UI), { 100, 250 }, 0.0f, 1.5f, WHITE);

            DrawTextureEx(tex.loadSprite("full.png", UI), { 100, 375 }, 0.0f, 1.5f, WHITE);

            DrawTextureEx(tex.loadSprite("high.png", UI), { 100, 500 }, 0.0f, 1.5f, WHITE);

            DrawTextureEx(tex.loadSprite("exit.png", UI), { 100, 700 }, 0.0f, 1.5f, WHITE);

            DrawTextureEx(tex.loadSprite("arrow.png", UI), { 40, 305 }, 0.0f, 0.5f, WHITE);

            update();
        }

        if (settingButton == menu::settings::settingSelect::FULL) {

            isSoundUnlocked = unlock::UNLOCKED;
            PlaySound(audHandler.loadAudio("select.mp3", SFX_UI));

            DrawTextureEx(tex.loadSprite("res.png", UI), { 100, 0 }, 0.0f, 1.5f, WHITE);
            DrawTextureEx(tex.loadSprite("audio.png", UI), { 100, 125 }, 0.0f, 1.5f, WHITE);

            DrawTextureEx(tex.loadSprite("diff.png", UI), { 100, 250 }, 0.0f, 1.5f, WHITE);

            DrawTextureEx(tex.loadSprite("full.png", UI), { 100, 375 }, 0.0f, 1.5f, WHITE);

            DrawTextureEx(tex.loadSprite("high.png", UI), { 100, 500 }, 0.0f, 1.5f, WHITE);

            DrawTextureEx(tex.loadSprite("exit.png", UI), { 100, 700 }, 0.0f, 1.5f, WHITE);

            DrawTextureEx(tex.loadSprite("arrow.png", UI), { 40, 430 }, 0.0f, 0.5f, WHITE);

            update();

        }

        if (settingButton == menu::settings::settingSelect::HIGH) {

            isSoundUnlocked = unlock::UNLOCKED;
            PlaySound(audHandler.loadAudio("select.mp3", SFX_UI));

            exitOpacity = 0.0f;
            exitGOpacity = 0.0f;

            DrawTextureEx(tex.loadSprite("res.png", UI), { 100, 0 }, 0.0f, 1.5f, WHITE);
            DrawTextureEx(tex.loadSprite("audio.png", UI), { 100, 125 }, 0.0f, 1.5f, WHITE);

            DrawTextureEx(tex.loadSprite("diff.png", UI), { 100, 250 }, 0.0f, 1.5f, WHITE);

            DrawTextureEx(tex.loadSprite("full.png", UI), { 100, 375 }, 0.0f, 1.5f, WHITE);

            DrawTextureEx(tex.loadSprite("high.png", UI), { 100, 500 }, 0.0f, 1.5f, WHITE);

            DrawTextureEx(tex.loadSprite("exit.png", UI), { 100, 700 }, 0.0f, 1.5f, WHITE);

            DrawTextureEx(tex.loadSprite("arrow.png", UI), { 40, 555 }, 0.0f, 0.5f, WHITE);

            update();
        }

        if (settingButton == menu::settings::settingSelect::EXITSETTINGS) {

            exitOpacity += GetFrameTime() * btnFadeInSpeed;
            exitGOpacity += GetFrameTime() * glowSpeed;

            isSoundUnlocked = unlock::UNLOCKED;
            PlaySound(audHandler.loadAudio("select.mp3", SFX_UI));

            checkOpacity();

            DrawTextureEx(tex.loadSprite("res.png", UI), { 100, 0 }, 0.0f, 1.5f, WHITE);
            DrawTextureEx(tex.loadSprite("audio.png", UI), { 100, 125 }, 0.0f, 1.5f, WHITE);

            DrawTextureEx(tex.loadSprite("diff.png", UI), { 100, 250 }, 0.0f, 1.5f, WHITE);

            DrawTextureEx(tex.loadSprite("full.png", UI), { 100, 375 }, 0.0f, 1.5f, WHITE);

            DrawTextureEx(tex.loadSprite("high.png", UI), { 100, 500 }, 0.0f, 1.5f, WHITE);

            DrawTextureEx(tex.loadSprite("black.png", UI), { 100, 700 }, 0.0f, 1.5f, Fade(BLACK, exitGOpacity));
            DrawTextureEx(tex.loadSprite("exitH.png", UI), { 100, 700 }, 0.0f, 1.5f, Fade(WHITE, exitOpacity));

            update();
        }

        if (selectedButton == menu::selected::PLAY) {

            soundConfigLoad();

            settingOpacity = 0.0f;
            exitOpacity = 0.0f;

            settingGOpacity = 0.0f;
            exitGOpacity = 0.0f;

            playOpacity += GetFrameTime() * btnFadeInSpeed;
            playGOpacity += GetFrameTime() * glowSpeed;

            isSoundUnlocked = unlock::UNLOCKED;
            PlaySound(audHandler.loadAudio("select.mp3", SFX_UI));

            checkOpacity();

            DrawTextureEx(tex.loadSprite("black.png", UI), { 400, 500 }, 0.0f, 2.0f, Fade(BLACK, playGOpacity));
            DrawTextureEx(tex.loadSprite("playH.png", UI), { 400, 500 }, 0.0f, 2.0f, Fade(WHITE, playOpacity));

            DrawTextureEx(tex.loadSprite("setting.png", UI), { 1100, 500 }, 0.0f, 2.0f, WHITE);

            DrawTextureEx(tex.loadSprite("exit.png", UI), { 700, 700 }, 0.0f, 2.0f, WHITE);

            if (IsKeyPressed(KEY_ENTER)) {
                menuVisible = visible::OFF;
            }
        }

        if (selectedButton == menu::selected::EXIT) {
            soundConfigLoad();

            settingOpacity = 0.0f;
            playOpacity = 0.0f;

            settingGOpacity = 0.0f;
            playGOpacity = 0.0f;

            exitOpacity += GetFrameTime() * btnFadeInSpeed;
            exitGOpacity += GetFrameTime() * glowSpeed;

            isSoundUnlocked = unlock::UNLOCKED;
            PlaySound(audHandler.loadAudio("select.mp3", SFX_UI));

            checkOpacity();

            DrawTextureEx(tex.loadSprite("play.png", UI), { 400, 500 }, 0.0f, 2.0f, WHITE);

            DrawTextureEx(tex.loadSprite("setting.png", UI), { 1100, 500 }, 0.0f, 2.0f, WHITE);

            DrawTextureEx(tex.loadSprite("black.png", UI), { 700, 700 }, 0.0f, 2.0f, Fade(BLACK, exitGOpacity));
            DrawTextureEx(tex.loadSprite("exitH.png", UI), { 700, 700 }, 0.0f, 2.0f, Fade(WHITE, exitOpacity));
        }

        /////////////////////////////////
        // MAIN CONTROLS FOR SELECTING //
        /////////////////////////////////

        if (IsKeyPressed(KEY_RIGHT) && (selectedButton == menu::selected::PLAY || selectedButton == menu::selected::EXIT)) {
            selectedButton = menu::selected::SETTING;
        }
        else if (IsKeyPressed(KEY_LEFT) && (selectedButton == menu::selected::SETTING || selectedButton == menu::selected::EXIT)) {
            selectedButton = menu::selected::PLAY;
        }
        else if (IsKeyPressed(KEY_UP) && selectedButton == menu::selected::EXIT) {
            selectedButton = menu::selected::PLAY;
        }
        else if (IsKeyPressed(KEY_DOWN) && (selectedButton == menu::selected::PLAY || selectedButton == menu::selected::SETTING)) {
            selectedButton = menu::selected::EXIT;
        }
        else if (IsKeyPressed(KEY_ENTER) && selectedButton == menu::selected::SETTING) {
            settingButton = menu::settings::settingSelect::RESOLUTION;
        }
        else if (IsKeyPressed(KEY_DOWN) && settingButton == menu::settings::settingSelect::RESOLUTION) {
            settingButton = menu::settings::settingSelect::AUDIO;
        }
        else if (IsKeyPressed(KEY_DOWN) && settingButton == menu::settings::settingSelect::AUDIO) {
            settingButton = menu::settings::settingSelect::DIFF;
        }
        else if (IsKeyPressed(KEY_DOWN) && settingButton == menu::settings::settingSelect::DIFF) {
            settingButton = menu::settings::settingSelect::FULL;
        }
        else if (IsKeyPressed(KEY_DOWN) && settingButton == menu::settings::settingSelect::FULL) {
            settingButton = menu::settings::settingSelect::HIGH;
        }
        else if (IsKeyPressed(KEY_DOWN) && settingButton == menu::settings::settingSelect::HIGH) {
            settingButton = menu::settings::settingSelect::EXITSETTINGS;
        }
        else if (IsKeyPressed(KEY_UP) && settingButton == menu::settings::settingSelect::EXITSETTINGS) {
            settingButton = menu::settings::settingSelect::HIGH;
        }
        else if (IsKeyPressed(KEY_UP) && settingButton == menu::settings::settingSelect::HIGH) {
            settingButton = menu::settings::settingSelect::FULL;
        }
        else if (IsKeyPressed(KEY_UP) && settingButton == menu::settings::settingSelect::FULL) {
            settingButton = menu::settings::settingSelect::DIFF;
        }
        else if (IsKeyPressed(KEY_UP) && settingButton == menu::settings::settingSelect::DIFF) {
            settingButton = menu::settings::settingSelect::AUDIO;
        }
        else if (IsKeyPressed(KEY_UP) && settingButton == menu::settings::settingSelect::AUDIO) {
            settingButton = menu::settings::settingSelect::RESOLUTION;
        }
        else if (IsKeyPressed(KEY_RIGHT) && settingButton == menu::settings::settingSelect::DIFF && difficulty == menu::settings::diff::EASY) {
            difficulty = menu::settings::diff::NORMAL;
        }
        else if (IsKeyPressed(KEY_RIGHT) && settingButton == menu::settings::settingSelect::DIFF && difficulty == menu::settings::diff::NORMAL) {
            difficulty = menu::settings::diff::HARD;
        }
        else if (IsKeyPressed(KEY_LEFT) && settingButton == menu::settings::settingSelect::DIFF && difficulty == menu::settings::diff::HARD) {
            difficulty = menu::settings::diff::NORMAL;
        }
        else if (IsKeyPressed(KEY_LEFT) && settingButton == menu::settings::settingSelect::DIFF && difficulty == menu::settings::diff::NORMAL) {
            difficulty = menu::settings::diff::EASY;
        }
        else if (IsKeyPressed(KEY_RIGHT) && settingButton == menu::settings::settingSelect::FULL && fullscreen == menu::settings::full::FULL_OFF) {
            fullscreen = menu::settings::full::FULL_ON;
        }
        else if (IsKeyPressed(KEY_LEFT) && settingButton == menu::settings::settingSelect::FULL && fullscreen == menu::settings::full::FULL_OFF) {
            fullscreen = menu::settings::full::FULL_ON;
        }
        else if (IsKeyPressed(KEY_LEFT) && settingButton == menu::settings::settingSelect::FULL && fullscreen == menu::settings::full::FULL_ON) {
            fullscreen = menu::settings::full::FULL_OFF;
        }
        else if (IsKeyPressed(KEY_RIGHT) && settingButton == menu::settings::settingSelect::FULL && fullscreen == menu::settings::full::FULL_ON) {
            fullscreen = menu::settings::full::FULL_OFF;
        }
        else if (IsKeyPressed(KEY_RIGHT) && settingButton == menu::settings::settingSelect::HIGH && highlights == menu::settings::high::HIGH_ON) {
            highlights = menu::settings::high::HIGH_OFF;
        }
        else if (IsKeyPressed(KEY_LEFT) && settingButton == menu::settings::settingSelect::HIGH && highlights == menu::settings::high::HIGH_ON) {
            highlights = menu::settings::high::HIGH_OFF;
        }
        else if (IsKeyPressed(KEY_LEFT) && settingButton == menu::settings::settingSelect::HIGH && highlights == menu::settings::high::HIGH_OFF) {
            highlights = menu::settings::high::HIGH_ON;
        }
        else if (IsKeyPressed(KEY_RIGHT) && settingButton == menu::settings::settingSelect::HIGH && highlights == menu::settings::high::HIGH_OFF) {
            highlights = menu::settings::high::HIGH_ON;
        }
        else if (IsKeyPressed(KEY_RIGHT) && settingButton == menu::settings::settingSelect::AUDIO && audio == menu::settings::audio::AUD0) {
            audio = menu::settings::audio::AUD1;
        }
        else if (IsKeyPressed(KEY_RIGHT) && settingButton == menu::settings::settingSelect::AUDIO && audio == menu::settings::audio::AUD1) {
            audio = menu::settings::audio::AUD2;
        }
        else if (IsKeyPressed(KEY_RIGHT) && settingButton == menu::settings::settingSelect::AUDIO && audio == menu::settings::audio::AUD2) {
            audio = menu::settings::audio::AUD3;
        }
        else if (IsKeyPressed(KEY_RIGHT) && settingButton == menu::settings::settingSelect::AUDIO && audio == menu::settings::audio::AUD3) {
            audio = menu::settings::audio::AUD4;
        }
        else if (IsKeyPressed(KEY_RIGHT) && settingButton == menu::settings::settingSelect::AUDIO && audio == menu::settings::audio::AUD4) {
            audio = menu::settings::audio::AUD5;
        }
        else if (IsKeyPressed(KEY_RIGHT) && settingButton == menu::settings::settingSelect::AUDIO && audio == menu::settings::audio::AUD5) {
            audio = menu::settings::audio::AUD6;
        }
        else if (IsKeyPressed(KEY_RIGHT) && settingButton == menu::settings::settingSelect::AUDIO && audio == menu::settings::audio::AUD6) {
            audio = menu::settings::audio::AUD7;
        }
        else if (IsKeyPressed(KEY_RIGHT) && settingButton == menu::settings::settingSelect::AUDIO && audio == menu::settings::audio::AUD7) {
            audio = menu::settings::audio::AUD8;
        }
        else if (IsKeyPressed(KEY_LEFT) && settingButton == menu::settings::settingSelect::AUDIO && audio == menu::settings::audio::AUD1) {
            audio = menu::settings::audio::AUD0;
        }
        else if (IsKeyPressed(KEY_LEFT) && settingButton == menu::settings::settingSelect::AUDIO && audio == menu::settings::audio::AUD2) {
            audio = menu::settings::audio::AUD1;
        }
        else if (IsKeyPressed(KEY_LEFT) && settingButton == menu::settings::settingSelect::AUDIO && audio == menu::settings::audio::AUD3) {
            audio = menu::settings::audio::AUD2;
        }
        else if (IsKeyPressed(KEY_LEFT) && settingButton == menu::settings::settingSelect::AUDIO && audio == menu::settings::audio::AUD4) {
            audio = menu::settings::audio::AUD3;
        }
        else if (IsKeyPressed(KEY_LEFT) && settingButton == menu::settings::settingSelect::AUDIO && audio == menu::settings::audio::AUD5) {
            audio = menu::settings::audio::AUD4;
        }
        else if (IsKeyPressed(KEY_LEFT) && settingButton == menu::settings::settingSelect::AUDIO && audio == menu::settings::audio::AUD6) {
            audio = menu::settings::audio::AUD5;
        }
        else if (IsKeyPressed(KEY_LEFT) && settingButton == menu::settings::settingSelect::AUDIO && audio == menu::settings::audio::AUD7) {
            audio = menu::settings::audio::AUD6;
        }
        else if (IsKeyPressed(KEY_LEFT) && settingButton == menu::settings::settingSelect::AUDIO && audio == menu::settings::audio::AUD8) {
            audio = menu::settings::audio::AUD7;
        }
        else if (IsKeyPressed(KEY_RIGHT) && settingButton == menu::settings::settingSelect::RESOLUTION && resolution == menu::settings::res::RES0) {
            resolution = menu::settings::res::RES1;
        }
        else if (IsKeyPressed(KEY_RIGHT) && settingButton == menu::settings::settingSelect::RESOLUTION && resolution == menu::settings::res::RES1) {
            resolution = menu::settings::res::RES2;
        }
        else if (IsKeyPressed(KEY_RIGHT) && settingButton == menu::settings::settingSelect::RESOLUTION && resolution == menu::settings::res::RES2) {
                resolution = menu::settings::res::RES3;
        }
        else if (IsKeyPressed(KEY_RIGHT) && settingButton == menu::settings::settingSelect::RESOLUTION && resolution == menu::settings::res::RES3) {
            resolution = menu::settings::res::RES4;
        }
        else if (IsKeyPressed(KEY_RIGHT) && settingButton == menu::settings::settingSelect::RESOLUTION && resolution == menu::settings::res::RES4) {
                resolution = menu::settings::res::RES5;
        }
        else if (IsKeyPressed(KEY_LEFT) && settingButton == menu::settings::settingSelect::RESOLUTION && resolution == menu::settings::res::RES1) {
            resolution = menu::settings::res::RES0;
        }
        else if (IsKeyPressed(KEY_LEFT) && settingButton == menu::settings::settingSelect::RESOLUTION && resolution == menu::settings::res::RES2) {
            resolution = menu::settings::res::RES1;
        }
        else if (IsKeyPressed(KEY_LEFT) && settingButton == menu::settings::settingSelect::RESOLUTION && resolution == menu::settings::res::RES3) {
                resolution = menu::settings::res::RES2;
        }
        else if (IsKeyPressed(KEY_LEFT) && settingButton == menu::settings::settingSelect::RESOLUTION && resolution == menu::settings::res::RES4) {
            resolution = menu::settings::res::RES3;
        }
        else if (IsKeyPressed(KEY_LEFT) && settingButton == menu::settings::settingSelect::RESOLUTION && resolution == menu::settings::res::RES5) {
                resolution = menu::settings::res::RES4;
        }
        else if (IsKeyPressed(KEY_ENTER) && settingButton == menu::settings::settingSelect::EXITSETTINGS) {
            selectedButton = menu::selected::UNKOWN;
            settingButton = menu::settings::settingSelect::NONE;

            hideTitle = OFF;
        }
        else if (IsKeyPressed(KEY_ENTER) && selectedButton == menu::selected::EXIT) {
            exitGame = true;
        }
    }
}

// little test for doom.hpp
void test() {
    DrawTextureEx(tex.loadSprite("supernova0.png", BACKGROUND), { 0, 0 }, 0.0f, 5.0f, WHITE);
}