
#include "raylib.h"
#include <math.h> // needed for cos and sin.

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib example.");

    Vector2 position;
    position.x = screenWidth/2;
    position.y = screenHeight/2;
    float angle=GetRandomValue(0,359);
 
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        
        // cos and sin here.
        position.x += (float)cos(angle)*4;
        position.y += (float)sin(angle)*4;
        
        if(GetRandomValue(0,100)<5)angle=GetRandomValue(0,359);
        if(position.x>screenWidth || position.x<0 || position.y<0 || position.y>screenHeight){
            position.x=screenWidth/2;
            position.y=screenHeight/2;
            }
        
        //----------------------------------------------------------------------------------
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);
                

            DrawCircle(position.x,position.y,20,RED);
            

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;


}
