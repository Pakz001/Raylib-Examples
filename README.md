# Raylib-Examples
My collection of raylib examples. ( https://www.raylib.com/index.html )

For Raylib Version of 4 april 2020 ( Notepad++ windows installer version )

So far every different file in the repo are standalone. GFX and maybe sound wil be included inside the code files as array data.

I might end up storing and creating a few hundred game related code examples in this repo over the next couple of years.

Points of interest for me and this repo are :

+learning C with Raylib
+Learning more about how Rts/Fps/Rpg/Tbs/Shooter/Action/Puzzle games work.
+Tools for eq. Sprite editing(For direct pasting into code)
+Main focus - Minecraft and Civilization 1/2 style games.

Dotate to author (book and motivation funds) https://www.paypal.me/RudyvanEtten/5

.
.
.

How to use these examples :

Each example in here is a standalone .c file. I myself just copy and paste the one I need into notepad++ and save it as a new .c file and it just works. You should also be able to save this repo as a zip and unzip it and be able to load each example and run it.

Possibly a .h file like raymath.h sometimes is not found, this happens with certain examples in the official examples. You can copy that .h files from the /src files into the folder with the example that needs it. That seemed to have worked for me.


Below here is a Windows sprite editor tool I programmed that can be used to create sprites and copy that sprite data to the clipboard buffer. I use this for some of the example in this repo. I might rewrite it for Raylib in the future. The spritedata copied in the clipboard by the sprite tool are inside a array. It uses the c64 palette or the db32 palette. The color values can be found online.

The editor is based on the Pico8 sprite and map editor.

(use the c key inside the editor to export the current cell data into the clipboard buffer)
https://cromdesi.home.xs4all.nl/tools/raylibspriteed.zip
