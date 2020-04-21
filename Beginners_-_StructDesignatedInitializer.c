
#include "raylib.h"

struct unit{
    int empty;
    bool active;
    int x;
    int y;
    int damage;
    char name[10];
};

static struct unit myUnit = {.active = true, .x = 100, .y = 200, .name="hello"};

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
                
            if(myUnit.active){
                DrawText("Active:true",100,200,20,BLACK);
            }
            DrawText(FormatText("This should be empty : %i",myUnit.empty),100,180,20,BLACK);
            DrawText(FormatText("%i , %i",myUnit.x,myUnit.y),100,220,20,BLACK);
            DrawText(FormatText("%s",myUnit.name),100,240,20,BLACK);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;


}
