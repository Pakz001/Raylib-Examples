// !! needs files 0x.gif to 09.gif in the same folder as this example.
// 

//
// gif to sprite sheet Script (work in progress)
//
// I had some sprites that needed converting. I have a folder filled with
// gif files. Each gif file is a tiny sprite. They are named 01..99+.gif
// I wanted to load each of these gifs and make a sprite sheet of them and then
// save this sheet as a image file.
//
// I was working on doing this but discovered the export function of the
// tool I used had a bug where only the first sprite of a set was saved as
// a gif. I decided not to continue at this point.

// What I have here is a loader that creates x amount of textures. Then
// the loadtexture command loads the first 10 of these gifs into
// this array. It shows how to create a load function which can load
// procedural generated filenames.

#include "raylib.h"
#include <string.h>

// create the container for the gifs
Texture2D im[150];

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib example.");
    // create our char that wil contain the filename.
    char lfn[6]; // '0x.gif'
    // create the first set of names
    for(int i=1;i<10;i++){    
        lfn[0] = 0;
        lfn[1] = i; // here we add the number in the filename
        lfn[2] = '.'; // gif extension
        lfn[3] = 'g';
        lfn[4] = 'i';
        lfn[5] = 'f';
        // load the texture using the FormatText command
        im[i] = LoadTexture(FormatText("0%i.gif",i));
    }
    //im1 = LoadTexture("01.gif"); // test case
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------
            //im1 = LoadTexture(lfn);
                //DrawTexture(im1, 190+i*16, 190, WHITE);




    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        
        
        //----------------------------------------------------------------------------------
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
            // draw the texture(gifs) on the canvas
            int cnt=0;
            for(int y=0;y<10;y++){
            for(int x=0;x<20;x++){
                if(cnt<10){
                    DrawTexture(im[cnt],x*16,y*16,WHITE);
                }
                cnt++;
            }}
            // test case
            //DrawText(lfn,100,80,20,BLACK);
            //DrawText(FormatText("0%i.gif",i),100,80,20,BLACK);
            ClearBackground(RAYWHITE);
            

        EndDrawing();
        //----------------------------------------------------------------------------------
    }
    for(int i=0;i<50;i++){
        UnloadTexture(im[i]);
    }    
    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
    

}
