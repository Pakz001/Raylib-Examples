
#include "raylib.h"

// Here are the enums.

enum flag{UP,DOWN,LEFT,RIGHT};
enum flag2{UP2=5,DOWN2,LEFT2,RIGHT2};

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib example.");
 
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

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
            
            DrawText(FormatText("UP %i",UP)         ,0,0,20,GRAY);
            DrawText(FormatText("DOWN %i",DOWN)     ,0,20,20,GRAY);
            DrawText(FormatText("LEFT %i",LEFT)     ,0,40,20,GRAY);

            DrawText(FormatText("UP2 %i",UP2)       ,0,80,20,GRAY);
            DrawText(FormatText("DOWN2 %i",DOWN2)   ,0,100,20,GRAY);
            DrawText(FormatText("LEFT2 %i",LEFT2)   ,0,120,20,GRAY);


        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;


}
