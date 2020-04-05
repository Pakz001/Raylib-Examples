
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
        Vector2 mousePos = GetMousePosition();
        //----------------------------------------------------------------------------------
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);
            
            // Create our first rectangle
            Rectangle r1;
            r1.x = 100;
            r1.y = 100;
            r1.width = 80;
            r1.height = 50;
            // Create our second rectangle
            Rectangle r2;
            r2.x = mousePos.x;
            r2.y = mousePos.y;
            r2.width = 50;
            r2.height = 50;

            // Draw our rectangles
            DrawRectangleRec(r1,BLUE);
            DrawRectangleRec(r2,RED);
            
            // Check collision between rectangles.
            if (CheckCollisionRecs(r1,r2))
            {
                DrawText("Collision!!",0,0,20,RED);
            }
 
            DrawText("Example of how use rect collision.", 100, 180, 40, LIGHTGRAY);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
