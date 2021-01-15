
#include "raylib.h"
// For rand_float
#include <stdio.h>
#include <stdlib.h>

float float_rand( float min, float max );

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib example.");
 
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    float set[5][19] = {0.0f};
    for(int x=0;x<5;x++){
    for(int y=0;y<19;y++){        
        set[x][y] = float_rand(-1,1);
    }}

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
            DrawText("rand_float(min, max) function.",0,0,20,BLACK);
            for(int x=0;x<5;x++){
            for(int y=0;y<19;y++){        
                DrawText(FormatText("%f",set[x][y]),x*150,y*20+40,20,BLACK);
            }}    

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;


}
float float_rand( float min, float max ){
    float scale = rand() / (float) RAND_MAX; /* [0, 1.0] */
    return min + scale * ( max - min );      /* [min, max] */
}
