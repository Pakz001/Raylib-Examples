
  
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "raylib.h"

static	int spritelibmap[80*4][32][32]; // numspritelib,


int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib example.");
 
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------
    
    // Put some values in the array.
    spritelibmap[0][0][0] = 1; 
    spritelibmap[20][10][10] = 1;        
    // Create out file pointer.
    FILE *fp;
    // Write the array to disk.
    fp=fopen("spritedata.spr","w");
    fwrite(spritelibmap,sizeof(spritelibmap),1,fp); /* Write to File */
    fclose(fp);
    // Reset the values in the array.
    spritelibmap[0][0][0] = 0; 
    spritelibmap[20][10][10] = 0;
    // Read the array from disk..
    fp=fopen("spritedata.spr","r");
    fread(spritelibmap,sizeof(spritelibmap),1,fp); /* read the entire file into the array */    
    fclose(fp);


    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        
        
        //----------------------------------------------------------------------------------
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);
            
            DrawText(FormatText("%i",spritelibmap[0][0][0]),200,200,20,BLACK);
            DrawText(FormatText("%i",spritelibmap[20][10][10]),200,220,20,BLACK);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;


}
