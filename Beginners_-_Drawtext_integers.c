
#include "raylib.h"

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

    int myvar = 0;          // My thing here
    int myvar2 = 1;         // My second var
    int myvar3 = 5;

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            DrawText(FormatText("my var value : %01i" , myvar), 190, 200, 20, LIGHTGRAY);
            DrawText(FormatText("my var2 value : %05i" , myvar2), 190, 220, 20, LIGHTGRAY); // Here I changed it so it draws 5 numbers.
            DrawText(FormatText("my var2 + var3 value : %05i" , myvar2+myvar3), 190, 240, 20, LIGHTGRAY); // Here I changed it does some math.
            DrawText(FormatText("my var2 + var 3 + 1 value : %05i" , myvar2+myvar3+1), 190, 260, 20, LIGHTGRAY); // Here I changed it does some math.

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
