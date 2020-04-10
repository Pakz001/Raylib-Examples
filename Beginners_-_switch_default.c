
enum flag{rub,adub,bub};

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
            //
            // Here we show how to use a switch statement.
            //
            // Note that you can not initialize arrays inside switch cases.
            int what=3;
            switch (what){
                case rub:
                    break;
                case adub:
                    //int i=0; <<<<<<< You can not initialize things like this in a case label :
                    //for(int i=0;i<10;i++){//<<<<<<<<< You can do these things in the switch statements.
                        //int a=10; <<<<<< Here you can initialize ints.
                    //}; 
                    break;
                case bub:
                    break;
                default:
                    DrawText("default switch case.",100,200,30,GRAY);
                break;
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
