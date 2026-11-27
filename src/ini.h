#pragma once
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// NEVER FUCKING INCLUDE A HPP FILE HERE
// IT BREAKS EVERYTHING!!!!

typedef struct config
{
	int WIDTH;
	int HEIGHT;
	int AUDIO;
	int DIFFICULTY;
	int FULLSCREEN;
	int HIGHLIGHT;
} config;

void loadConfig(const char* path, config* config) {

	// DEFAULT CONFIG VALUES //
	config->WIDTH = 960;
	config->HEIGHT = 540;
	config->AUDIO = 8;
	config->DIFFICULTY = 1;
	config->FULLSCREEN = 0;
	config->HIGHLIGHT = 1;

	FILE* file = fopen("./config.ini", "r");

	if (file == NULL) { return; }

	char buffer[256];

	while (fgets(buffer, sizeof(buffer), file)) {
		char* seperator = strchr(buffer, '=');

		if (seperator == NULL) { continue; }

		*seperator = '\0';

		char* key = buffer;
		char* value = seperator + 1;

		int num = atoi(value);

		if (strcmp(key, "WIDTH") == 0) {
			config->WIDTH = num;
		}
		else if (strcmp(key, "HEIGHT") == 0) {
			config->HEIGHT = num;
		}
		else if (strcmp(key, "AUDIO") == 0) {
			config->AUDIO = num;
		}
		else if (strcmp(key, "DIFFICULTY") == 0) {
			config->DIFFICULTY = num;
		}
		else if (strcmp(key, "FULLSCREEN") == 0) {
			config->FULLSCREEN = num;
		}
		else if (strcmp(key, "HIGHLIGHT") == 0) {
			config->HIGHLIGHT = num;
		}
	}
	fclose(file);
}


// This should work in both windows and unix systems. (hopefully)

void iniSave(int W, int H, int AUD, int DIFF, int FULL, int HIGH) {
	FILE* path = NULL;
	char numBuffer[16];
	fopen_s(&path, "./config.ini" , "wb"); // the mode "wb" write the file in binary //

	if (path) {
		char fileBuffer[256];
			*fileBuffer = '\0';
			strcat_s(fileBuffer, 256, "WIDTH=");
			strcat_s(fileBuffer, 256, itoa(W, numBuffer, 10));
			strcat_s(fileBuffer, 256, "\n");

			strcat_s(fileBuffer, 256, "HEIGHT=");
			strcat_s(fileBuffer, 256, itoa(H, numBuffer, 10));
			strcat_s(fileBuffer, 256, "\n");

			strcat_s(fileBuffer, 256, "AUDIO=");
			strcat_s(fileBuffer, 256, itoa(AUD, numBuffer, 10));
			strcat_s(fileBuffer, 256, "\n");

			strcat_s(fileBuffer, 256, "DIFFICULTY=");
			strcat_s(fileBuffer, 256, itoa(DIFF, numBuffer, 10));
			strcat_s(fileBuffer, 256, "\n");

			strcat_s(fileBuffer, 256, "FULLSCREEN=");
			strcat_s(fileBuffer, 256, itoa(FULL, numBuffer, 10));
			strcat_s(fileBuffer, 256, "\n");

			strcat_s(fileBuffer, 256, "HIGHLIGHT=");
			strcat_s(fileBuffer, 256, itoa(HIGH, numBuffer, 10));
			strcat_s(fileBuffer, 256, "\n");

			size_t fileLength = strlen(fileBuffer);
			fwrite(fileBuffer, fileLength, 1, path);

		fclose(path);
	}
}

void appendBuffer(char* buffer, char c) {
	if (!iscntrl(c)) {
		char str[2] = {c, '\0'};
		strcat_s(buffer, 256, str);
	}
}