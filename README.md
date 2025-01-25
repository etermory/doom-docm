# DooM-docm

![LOGO](screenshots/0.png)

**DOOM in Microsoft Word!**

Source-port of [doomgeneric](https://github.com/ozkl/doomgeneric). Does not have sound.

Requires Windows with an x64 CPU architecture and the latest version of Microsoft Word.

See a video of it [here](https://youtu.be/G3XoOCMnSNg)!

## Usage

1. Download `doom.docm` from the latest [github release](https://github.com/wojciech-graj/doom-docm/releases)
2. Right click on the file, select **Properties**
3. Select the **General** tab
4. Select **Unblock**
5. Open the file in Microsoft Word
6. Select **Enable Content** in the security warning bar

Additionally, expect to have to fight Windows Defender, as Microsoft is (rightfully) not too keen on having Word documents extracting and running executables.

## How it works

The Word document contains the library `doomgeneric_docm.dll` and `doom1.wad` game data encoded in base 64, which a VBA macro extracts onto the disk and then loads. Every game tick, `doomgeneric_docm.dll` creates a bmp image containing the current frame and uses `GetAsyncKeyState` to read the keyboard state. The main VBA macro's game loop runs a tick in doom, then replaces the image in the document with the latest frame.

## Controls

| Action        | Default Keybind |
|---------------|-----------------|
| UP            | ARROW UP        |
| DOWN          | ARROW DOWN      |
| LEFT          | ARROW LEFT      |
| RIGHT         | ARROW RIGHT     |
| FIRE          | CTRL            |
| USE           | SPACE           |
| WEAPON SELECT | 1-7             |

## Build
Requires Make and a MinGW compiler. Creates `bin/doomgeneric_docm.dll`
```
cd doomgeneric
make
```

`bin/doomgeneric_docm.dll` and `doom1.wad` can be converted into VBA macros using `util/codegen.py`, and saved as `vba/Data.vba`.

The contents of the source files in the `vba` directory have been saved as modules in the Word document.
