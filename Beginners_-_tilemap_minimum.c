
#include "raylib.h"

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib example.");

    
    int mymap[3][5] = { {1,1,1,1,1},
                        {1,0,0,0,1},
                        {1,1,1,1,1}
                        };    

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
            
            for (int y = 0; y< 3 ; y++)
            {
                for (int x = 0; x< 5 ; x++)
                {
                    if (mymap[y][x] == 1)
                    {
                        DrawRectangle(x*32,y*32,32,32,BLUE);
                    }
                }
            }

            DrawText("Example of a minimal tilemap.", 100, 180, 40, LIGHTGRAY);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
