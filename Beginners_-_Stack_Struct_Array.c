
#include "raylib.h"
#include <stdlib.h>

#define MAX_POINTS 1000
#define MAX_BATCH_ELEMENTS 8192

typedef struct Points{
    Vector2 position;
} Points;

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib example.");

    Points *mypoint = (Points *)malloc(MAX_POINTS*sizeof(Points));    // Points array
    
    int pointscount = 10;
    
    for (int i=0;i<pointscount;i++){
        mypoint[i].position.x = GetRandomValue(0,100);
        mypoint[i].position.y = GetRandomValue(0,100);
    }
 
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

            for(int i=0;i<pointscount;i++){
                DrawText(FormatText("x: %03f",mypoint[i].position.x), 50, 100+i*30, 20, BLACK);    
                DrawText(FormatText("y: %03f",mypoint[i].position.y), 200, 100+i*30, 20, BLACK);    
           }    
            
 
            

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    free(mypoint);              // Unload points data array
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;


}

