#include "raylib.h"

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
                
            DrawText("FormatText Example",50,180,60,GRAY);    
                
            float val1 = 10.3821;
            // only draw 2 numbers before and after the .
            DrawText(FormatText("val1: %02.02f",val1),100,100,20,BLACK);
            int val2 = 12345;
            // 10 x 0 before the int is drawn.
            DrawText(FormatText("val2: %010i",val2),100,120,20,BLACK);
            // We can put multiple things inside the formattext function.
            DrawText(FormatText("val1 : %f val2 : %i",val1,val2),100,140,20,BLACK);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;


}
