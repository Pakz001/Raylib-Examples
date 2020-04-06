
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
            
            // Here we test is mouse button 0 or 1 or 2 is down.
            if(IsMouseButtonDown(0)){
                DrawText("Mouse Button 0 is Down.",50,100,20,BLACK);
            }else{
                DrawText("Mouse Button 0 is NOT Down.",50,100,20,BLACK);
            }
            if(IsMouseButtonDown(1)){
                DrawText("Mouse Button 1 is Down.",50,120,20,BLACK);
            }else{
                DrawText("Mouse Button 1 is NOT Down.",50,120,20,BLACK);
            }
            if(IsMouseButtonDown(2)){
                DrawText("Mouse Button 2 is Down.",50,140,20,BLACK);
            }else{
                DrawText("Mouse Button 2 is NOT Down.",50,140,20,BLACK);
            }

            // Here we test if the space key is down.
            if(IsKeyDown(KEY_SPACE)){
                DrawText("KEY_SPACE is Down.",370,100,20,BLACK);
            }else{
                DrawText("KEY_SPACE is NOT Down.",370,100,20,BLACK);
            }
            
            // Here we test if any key is down.
            // Note that the value i<x might be higher than here.
            bool onedown=false;
            for(int i=0;i<355;i++){
                if(IsKeyDown(i)){
                    DrawText(FormatText("IsKeyDown : %i",i),370,120,20,BLACK);
                    onedown = true;
                }
            }
            if (onedown==false)DrawText("IsKeyDown : Nothing",370,120,20,BLACK);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;


}
