
#include "raylib.h"


int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib example.");

    // We set up our 3 dimensional array here. It is a int array.
    int map[10][10][10] = {0}; // the ={} means all the starting values are set to 0.
    
    // Here we put some values inside the array.
    map[0][0][0] = 10;
    map[9][0][0] = 20; // With a setup size of 10 the maximum slot to be used is 9!
    map[0][0][9] = 30;
    map[9][0][9] = 40;
 
    // Boolean map. Only true or false.
    bool boolmap[10][10][10] = {false};
    boolmap[1][1][2] = true;

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
                
            // Here we show the contents of the 3 dimensional array on the screen.    
            DrawText(FormatText("map[0][0][0]: %i",map[0][0][0]),100,100,20,BLACK);
            DrawText(FormatText("map[9][0][0]: %i",map[9][0][0]),100,120,20,BLACK);
            DrawText(FormatText("map[0][0][9]: %i",map[0][0][9]),100,140,20,BLACK);
            DrawText(FormatText("map[9][0][9]: %i",map[9][0][9]),100,160,20,BLACK);
            DrawText(FormatText("default ={0} slot of map[1][1][1]: %i",map[1][1][1]),100,180,20,BLACK);
            
            // Check if boolmap[1][1][2] = true or false.
            if(boolmap[1][1][2]==true)
            {
                DrawText("boolmap[1][1][2] set to true.",100,200,20,BLACK);
            }else{
                DrawText("boolmap[1][1][2] set to false.",100,200,20,BLACK);
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
