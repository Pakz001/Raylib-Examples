
#include "raylib.h"


int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib example.");

    
    // The last number of the char array[5] contains the ending code.
    char messy[6] = "hello";
    // We need to set up the char that contains the text with a lenght.    
    int lenofmessy = 6;
 
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
          
            // Here we draw the char!
            DrawText(messy,100,100,20,BLACK);

            // Draw the string(the char array) backwards.
            for(int i=lenofmessy-1;i>-1;i--){
                DrawText(FormatText("%01c",messy[i]),100+(((lenofmessy-2)*20)-(i*20)),120,20,BLACK);
                
            }
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;


}
