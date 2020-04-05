
#include "raylib.h"

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - generate random values");


    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------
        Vector2 mousePos = GetMousePosition();
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            
            DrawText("Getting the mouse and y position!",10,10,40,BLUE);
            DrawText(FormatText("x. %03f",mousePos.x), 100, 180, 40, LIGHTGRAY); // Mouse x and y are floats!!
            DrawText(FormatText("x. %03f",mousePos.y), 100, 220, 40, LIGHTGRAY); // Mouse x and y are floats!!
            

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
