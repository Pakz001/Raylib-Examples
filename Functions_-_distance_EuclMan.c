
#include "raylib.h"
#include <math.h>

static float distance(float x1,float y1,float x2,float y2);
static float edistance(float x1,float y1,float x2,float y2);

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
        
        int x1 = GetMouseX();
        int y1 = GetMouseY();
        int x2 = 400;
        int y2 = 200;
        
        //----------------------------------------------------------------------------------
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);
            
            DrawLine(x1,y1,x2,y2,RED);
            
            DrawText(FormatText("Manhattan distance : %f ",distance(x1,y1,x2,y2)),0,0,20,DARKGRAY);
            DrawText(FormatText("Euclidean distance : %f ",edistance(x1,y1,x2,y2)),0,20,20,DARKGRAY);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;


}

// Manhattan Distance (less precise)
float distance(float x1,float y1,float x2,float y2){
    return (float)abs(x2-x1)+abs(y2-y1);
}

// Euclidean distance (more precise)
float edistance(float x1,float y1,float x2,float y2){
    return sqrt( (x1-x2)*(x1-x2)+(y1-y2)*(y1-y2) );
}
