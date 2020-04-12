
#include "raylib.h"
#include "string.h"

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

            char a[40];
            int b=100;
            long c=100000;
            double d=183.1237d;
            float e=2138.234f;
  
            DrawText(FormatText("Size of char   = %i",sizeof(a)),100,120,20,BLACK);
            DrawText(FormatText("Size of int    = %i",sizeof(b)),100,140,20,BLACK);
            DrawText(FormatText("Size of long   = %i",sizeof(c)),100,160,20,BLACK);
            DrawText(FormatText("Size of double = %i",sizeof(d)),100,180,20,BLACK);
            DrawText(FormatText("Size of float  = %i",sizeof(e)),100,200,20,BLACK);
            
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;


}
