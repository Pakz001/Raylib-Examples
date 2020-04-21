
#include "raylib.h"

struct inventory{
    bool pepper;
    bool salt;
    bool potion;
};

struct aiMushroom{
    int x;
    int y;
    int energy;
    struct inventory leftPocket;
};

static struct aiMushroom myaiMushroom = {10,20,99,{true,false,true}};

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
                
            DrawText("The mushroom is here at :",100,180,20,BLACK);    
            DrawText(FormatText("x: %i y: %i Energy : %i",myaiMushroom.x,myaiMushroom.y,myaiMushroom.energy),100,200,20,BLACK);
            if(myaiMushroom.leftPocket.pepper){
                DrawText("The mushroom has pepper in his left pocket..",100,220,20,BLACK);
            }else{
                DrawText("The mushroom has no pepper in his left pocket..",100,220,20,BLACK);
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
