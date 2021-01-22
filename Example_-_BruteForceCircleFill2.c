//
// Draw a Filled circle using brute force.
//

#include "raylib.h"

void bruteforcecircle(int xc, int yc, int r,Color col);

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib example.");
 
    SetTargetFPS(10);               // Set our game to run at 60 frames-per-second
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
            for(int i=0;i<20;i++){
                int x = GetRandomValue(0,screenWidth);
                int y = GetRandomValue(0,screenHeight);
                int r = GetRandomValue(10,100);
                bruteforcecircle(x,y,r,RED);
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

void bruteforcecircle(int xc, int yc, int radius,Color col) { 
//    for(int y=-radius; y<=radius; y++){
//    for(int x=-radius; x<=radius; x++){
//        if(x*x+y*y <= radius*radius)
//            DrawPixel(xc+x, yc+y,col);
//    }}

// Single Loop version ; Said to be 2x speed than double loop method.
int r2 = radius * radius;
int area = r2 << 2;
int rr = radius << 1;

for (int i = 0; i < area; i++)
{
    int tx = (i % rr) - radius;
    int ty = (i / rr) - radius;

    if (tx * tx + ty * ty <= r2)
        DrawPixel(xc + tx, yc + ty, col);
}
}    
