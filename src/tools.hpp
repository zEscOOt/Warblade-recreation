#pragma once
#include "raylib-cpp.hpp"
#include "raylib.h"
#include "ini.h"
#include <cstring>
#include <string>
#include <unordered_map>

// Use this to declare what "type" the sprite is //
enum spriteType {
	BACKGROUND,
	ENTITY,
	ANIMATION,
	UI,
	AUD,
	RES
};

enum audioType {
	OST,
	SFX_ENTITY,
	SFX_UI
};

// Used for temporarly locking sound effects so they don't repeat 100 times a ms.
enum unlock {
	UNLOCKED,
	LOCKED
};

int isSoundUnlocked = unlock::UNLOCKED;

struct currentMusic {
	Music CURRENT;
};

struct currentSound {
	Sound CURRENT;
};

currentMusic currM;
currentSound currS;
config audioConf;

// Virtual resolution for scaling //
struct virtualResolutions{
	static const int VIRTUAL_WIDTH = 1920;
	static const int VIRTUAL_HEIGHT = 1080;
};

struct spriteHandler {

	const char* prefix = "media/textures/";
	std::unordered_map<const char*, Texture2D> textures;

	// Load the texture and cahces it inside an unordered map for faster lookup
	// And to not cause memory leaks.

	 Texture2D& loadSprite(const char* name, int spriteType) {

		switch (spriteType) {
		case BACKGROUND:
			prefix = "media/textures/background/";
		break;

		case ENTITY:
			prefix = "media/textures/entity/";
		break;

		case ANIMATION:
			prefix = "media/textures/anim/";
		break;

		case UI:
			prefix = "media/textures/UI/";
		break;

		case AUD:
			prefix = "media/textures/UI/menu/audio/";
		break;

		case RES:
			prefix = "media/textures/UI/menu/res/";
			break;

		default:
			prefix = "media/textures/";
		break;
		}

		std::string result(std::string(prefix) + name);

		if (!textures.contains(name)) {
			textures[name] = LoadTexture(result.c_str());
		}

		return textures.at(name);
	};
};


// Same thing as the sprite handler but it loads and caches audio //
struct audioHandler {

	const char* audPrefix = "media/audio/";
	std::unordered_map<const char*, Sound> audio;

	Sound& loadAudio(const char* name, int audioType) {
		if (isSoundUnlocked == unlock::UNLOCKED) {
			isSoundUnlocked = unlock::LOCKED;

			switch (audioType) {
			case OST:
				audPrefix = "media/audio/ost/";
				break;

			case SFX_ENTITY:
				audPrefix = "media/audio/sfx/entity/";
				break;

			case SFX_UI:
				audPrefix = "media/audio/sfx/UI/";
				break;

			default:
				audPrefix = "media/audio/";
				break;
			}

			std::string audResult(std::string(audPrefix) + name);

			if (!audio.contains(name)) {
				audio[name] = LoadSound(audResult.c_str());
			}

			currS.CURRENT = audio.at(name);
			return audio.at(name);
		}
	};
};

// Only used for ingame music (not sound effects)
struct musicStream {
	const char* musicPrefix = "media/audio/ost/";
	std::unordered_map<const char*, Music> music;

	Music& loadMusic(const char* name) {

		std::string musicResult(std::string(musicPrefix) + name);

		if (!music.contains(name)) {
			music[name] = LoadMusicStream(musicResult.c_str());
		}

		currM.CURRENT = music.at(name);
		return music.at(name);
	}
};

// LOADS CONFIG TO UPDATE THE AUDIO LEVELS //

void audioConfigLoad() {

	loadConfig("./config.ini", &audioConf);

	switch (audioConf.AUDIO){
	case 0:
		SetMusicVolume(currM.CURRENT, 0.0f);
		break;

	case 1:
		SetMusicVolume(currM.CURRENT, 0.125f);
		break;

	case 2:
		SetMusicVolume(currM.CURRENT, 0.250f);
		break;

	case 3:
		SetMusicVolume(currM.CURRENT, 0.375f);
		break;

	case 4:
		SetMusicVolume(currM.CURRENT, 0.5f);
		break;

	case 5:
		SetMusicVolume(currM.CURRENT, 0.625f);
		break;

	case 6:
		SetMusicVolume(currM.CURRENT, 0.75f);
		break;

	case 7:
		SetMusicVolume(currM.CURRENT, 0.875f);
		break;

	case 8:
		SetMusicVolume(currM.CURRENT, 1.0f);
		break;

	default:
		SetMusicVolume(currM.CURRENT, 0.0f);
		break;
	}
}

void soundConfigLoad() {

	loadConfig("./config.ini", &audioConf);

	switch (audioConf.AUDIO) {
	case 0:
		SetSoundVolume(currS.CURRENT, 0.0f);
		break;

	case 1:
		SetSoundVolume(currS.CURRENT, 0.125f);
		break;

	case 2:
		SetSoundVolume(currS.CURRENT, 0.250f);
		break;

	case 3:
		SetSoundVolume(currS.CURRENT, 0.375f);
		break;

	case 4:
		SetSoundVolume(currS.CURRENT, 0.5f);
		break;

	case 5:
		SetSoundVolume(currS.CURRENT, 0.625f);
		break;

	case 6:
		SetSoundVolume(currS.CURRENT, 0.75f);
		break;

	case 7:
		SetSoundVolume(currS.CURRENT, 0.875f);
		break;

	case 8:
		SetSoundVolume(currS.CURRENT, 1.0f);
		break;

	default:
		SetSoundVolume(currS.CURRENT, 0.0f);
		break;
	}
}


// Use this to declare what "type" the sprite is //
enum spriteType {
	BACKGROUND = 0,
	ENTITY = 1,
	ANIMATION = 2,
	UI = 3
};

// Virtual resolution for scaling //
struct virtualResolutions{
	static const int VIRTUAL_WIDTH = 1920;
	static const int VIRTUAL_HEIGHT = 1080;
};

struct spriteHandler {

	std::unordered_map<const char*, Texture2D> textures;

	// Load the texture and cahces it inside an unordered map for faster lookup
	// And to not cause memory leaks.

	 Texture2D& loadSprite(const char* name, int spriteType) {
		const char* prefix = "media/textures/";

		switch (spriteType) {
		case BACKGROUND:
			prefix = "media/textures/background/";
		break;

		case ENTITY:
			prefix = "media/textures/entity/";
		break;

		case ANIMATION:
			prefix = "media/textures/anim/";
		break;

		case UI:
			prefix = "media/textures/UI/";
		break;

		default:
			prefix = "media/textures/";
		break;
		}

		std::string result(std::string(prefix) + name);

		if (!textures.contains(name)) {
			textures[name] = LoadTexture(result.c_str());
		}

		return textures.at(name);
	};
};

// file for helper functions that don't exist in raylib and would clutter Warblade.cpp
