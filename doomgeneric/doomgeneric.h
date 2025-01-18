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

#ifndef DOOM_GENERIC
#define DOOM_GENERIC

#include <stdint.h>
#include <stdlib.h>

#ifndef DOOMGENERIC_RESX
#define DOOMGENERIC_RESX 640
#endif // DOOMGENERIC_RESX

#ifndef DOOMGENERIC_RESY
#define DOOMGENERIC_RESY 400
#endif // DOOMGENERIC_RESY

#define DLL_EXPORT __declspec(dllexport)

#ifdef CMAP256

typedef uint8_t pixel_t;

#else // CMAP256

typedef uint32_t pixel_t;

#endif // CMAP256

extern pixel_t DG_ScreenBuffer[DOOMGENERIC_RESX * DOOMGENERIC_RESY];

void DLL_EXPORT doomgeneric_Create(const char *dir_p);
void DLL_EXPORT doomgeneric_Tick();

void DG_DrawFrame(void);
void DG_SleepMs(uint32_t ms);
int DG_GetKey(int *, unsigned char *);
uint32_t DG_GetTicksMs();

#endif //DOOM_GENERIC
