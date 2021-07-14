
#include "raylib.h"
#include <math.h>

// Get our angle between two points.
static float getangle(float x1,float y1,float x2,float y2);


int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib example.");
 
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    float posx = 320;
    float posy = 400;

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
 
            float x = GetMouseX()/32*32;
            float y = GetMouseY()/32*32;
            DrawRectangle(x,y,32,32,DARKGRAY);
 
            float an = getangle(posx,posy,x,y);
 
            DrawLine(posx,posy,posx+cos(an)*32,posy+sin(an)*32,DARKGRAY);
 

            DrawText(FormatText("Angle between x1,y1,x2,y2 : %f",getangle(GetMouseX(),GetMouseY(),posx,posy)),0,0,20,DARKGRAY);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;


}
// Return the angle from - to in float
float getangle(float x1,float y1,float x2,float y2){
    return (float)atan2(y2-y1, x2-x1);
}
