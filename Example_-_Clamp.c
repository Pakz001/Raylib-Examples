
#include "raylib.h"


// These are the circlerect collision function. The clamp one is needed for the circlerectcollide.
static bool circlerectcollide(int cx,int cy,int cr, int rx, int ry,int rw,int rh);
static float Clamp(float value, float min, float max);
    
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
        
        Rectangle r=(Rectangle){GetMouseX(),GetMouseY(),128,64};
        Vector2 c=(Vector2){400,200};
        float cr=50;
        //----------------------------------------------------------------------------------
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);
 
            DrawRectangle(r.x,r.y,r.width,r.height,BLACK);
            DrawCircle(c.x,c.y,cr,BLUE);

            if(circlerectcollide(c.x,c.y,cr,r.x,r.y,r.width,r.height)){
                DrawText("COLLISION..",20,0,30,RED);
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

bool circlerectcollide(int cx,int cy,int cr, int rx, int ry,int rw,int rh){
    float closestx = Clamp(cx, rx, rx+rw);
    float closesty = Clamp(cy, ry, ry+rh);
    float distancex = cx - closestx;
    float distancey = cy - closesty;
    float distancesquared = (distancex * distancex) + (distancey * distancey);
    return distancesquared < (cr * cr);
}

// Clamp float value
float Clamp(float value, float min, float max)
{
    const float res = value < min ? min : value;
    return res > max ? max : res;
}
