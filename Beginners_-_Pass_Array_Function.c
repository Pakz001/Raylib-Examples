
#include "raylib.h"

void passarray(Vector2 pol[]);

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
            //
            // Create a Vector2 array and create some points in it
            //
            Vector2 pol[3];
            pol[0] = (Vector2){100,100};
            pol[1] = (Vector2){20,300};
            pol[2] = (Vector2){180,300};
            //
            // Here we pass the Vector2 array into the function.
            passarray(pol);    

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;


}

//
// Fonction that takes a array of Vector2 for use in the function.
void passarray(Vector2 pol[]){
    // Draw a triangle.
    DrawTriangle(pol[0],pol[1],pol[2],RED);
}
