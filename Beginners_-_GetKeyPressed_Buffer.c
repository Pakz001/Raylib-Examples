
#include "raylib.h"


int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib example.");
 
    SetTargetFPS(5);               // Set our game to run at 5 frames-per-second
    //--------------------------------------------------------------------------------------

    int keypressedcount=0;
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        
        // See if the keyboard has been used.
        int key = GetKeyPressed(); // read/remove key from buffer.
        // Here we count how many keys are inside the buffer. (set framerate to low for effect!)
        int len = 0;
        while(key>0){
            len++;
            key = GetKeyPressed();// get/remove key from buffer.
        }
        
        //----------------------------------------------------------------------------------
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);
            
            // Draw our info.
            DrawText("Quickly hammer on the keyboard to fill up the keyboard buffer.",100,200,20,GRAY);                
            DrawText(FormatText("GetKeyPressed() buffer length : %i",len),100,100,20,RED);
            

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;


}
