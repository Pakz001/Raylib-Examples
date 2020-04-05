
#include "raylib.h"

void floodit();

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Raylib examples.");



 
 
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

            DrawText("Creating RGB colors.", 100, 180, 40, LIGHTGRAY);

            // (Color){Red[0..255],Green[0..255],Blue[0.255],Alpha[0.255(non transparent)]}
            DrawRectangle(100   +0  *1,100,64,32,(Color){255,0,0,255});
            DrawRectangle(100   +64 *2,100,64,32,(Color){0,255,0,255});
            DrawRectangle(100   +64 *4,100,64,32,(Color){0,0,255,255});
 

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;


}

