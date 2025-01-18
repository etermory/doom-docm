//
// Copyright(C) 2025 Wojciech Graj
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// DESCRIPTION:
//     Search for and locate an IWAD file, and initialize according
//     to the IWAD type.
//

#include <stdio.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "d_iwad.h"
#include "doomkeys.h"
#include "doomtype.h"
#include "i_system.h"
#include "m_argv.h"

#include "doomgeneric.h"

#include "bmp.h"

#define FILENAME "\\frame.bmp"

struct KeyMap {
	unsigned char key;
	int pressed;
	int vKey;
};

struct KeyEvent {
	unsigned char key;
	int pressed;
};

pixel_t DG_ScreenBuffer[DOOMGENERIC_RESX * DOOMGENERIC_RESY];
static char bmp_buf[BMP_SIZE(DOOMGENERIC_RESX, DOOMGENERIC_RESY)];
static unsigned key_queue_len = 0;
static boolean key_queue_emptied = true;
static struct KeyEvent key_queue[16];
static struct KeyMap key_map[] = {
	(struct KeyMap){ .key = KEY_RIGHTARROW, .vKey = VK_RIGHT },
	(struct KeyMap){ .key = KEY_LEFTARROW, .vKey = VK_LEFT },
	(struct KeyMap){ .key = KEY_UPARROW, .vKey = VK_UP },
	(struct KeyMap){ .key = KEY_DOWNARROW, .vKey = VK_DOWN },
	(struct KeyMap){ .key = KEY_USE, .vKey = VK_SPACE },
	(struct KeyMap){ .key = KEY_ENTER, .vKey = VK_RETURN },
	(struct KeyMap){ .key = KEY_FIRE, .vKey = VK_CONTROL },
	(struct KeyMap){ .key = KEY_ESCAPE, .vKey = VK_ESCAPE },
	(struct KeyMap){ .key = '1', .vKey = '1' },
	(struct KeyMap){ .key = '2', .vKey = '2' },
	(struct KeyMap){ .key = '3', .vKey = '3' },
	(struct KeyMap){ .key = '4', .vKey = '4' },
	(struct KeyMap){ .key = '5', .vKey = '5' },
	(struct KeyMap){ .key = '6', .vKey = '6' },
	(struct KeyMap){ .key = '7', .vKey = '7' },
};

void D_DoomMain(void);
void AddIWADDir(char *dir);

static char *out_filepath;

void DLL_EXPORT doomgeneric_Create(const char *const dir_p)
{
	myargc = 0;
	myargv = NULL;
	const size_t out_filepath_len = strlen(dir_p) + strlen(FILENAME) + 1;
	out_filepath = malloc(out_filepath_len);
	if (!out_filepath)
		I_Error("malloc error");

	snprintf(out_filepath, out_filepath_len, "%s" FILENAME, dir_p);
	AddIWADDir(strdup(dir_p));

	bmp_init(bmp_buf, DOOMGENERIC_RESX, DOOMGENERIC_RESY);

	M_FindResponseFile();

	D_DoomMain();
}

uint32_t DG_GetTicksMs(void)
{
	__int64 wintime;
	GetSystemTimeAsFileTime((FILETIME *)&wintime);
	return (wintime - 116444736000000000ll) / 10000ll;
}

void DG_SleepMs(const uint32_t ms)
{
	(void)ms;
	// Sleep(ms);
}

void DG_DrawFrame(void)
{
	unsigned x, y;
	for (y = 0; y < DOOMGENERIC_RESY; y++)
		for (x = 0; x < DOOMGENERIC_RESX; x++)
			bmp_set(bmp_buf, x, y, DG_ScreenBuffer[y * DOOMGENERIC_RESX + x]);

	FILE *file = fopen(out_filepath, "w");
	if (!file)
		I_Error("Failed to open file '%s'", out_filepath);
	int written = fwrite(bmp_buf, 1, sizeof(bmp_buf), file);
	if (written != sizeof(bmp_buf))
		I_Error("Failed to write to file '%s'", out_filepath);
	int close_ret = fclose(file);
	if (close_ret)
		I_Error("Failed to close file '%s'", out_filepath);
}

int DG_GetKey(int *const pressed, unsigned char *const key)
{
	if (key_queue_len > 0) {
		key_queue_len--;
		*pressed = key_queue[key_queue_len].pressed;
		*key = key_queue[key_queue_len].key;
		return 1;
	}
	if (!key_queue_emptied) {
		key_queue_emptied = true;
		return 0;
	}
	key_queue_emptied = false;
	key_queue_len = 0;
	unsigned i;
	for (i = 0; i < sizeof(key_map) / sizeof(struct KeyMap); i++) {
		int pressed = !!(GetAsyncKeyState(key_map[i].vKey) & 0x8000);
		if (pressed == key_map[i].pressed || key_queue_len == sizeof(key_queue) / sizeof(struct KeyEvent))
			continue;
		key_map[i].pressed = pressed;
		key_queue[key_queue_len].pressed = pressed;
		key_queue[key_queue_len].key = key_map[i].key;
		key_queue_len++;
	}
	return DG_GetKey(pressed, key);
}
