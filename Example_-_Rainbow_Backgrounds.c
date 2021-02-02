//
// Rainbows (backgrounds)
//

#include "raylib.h"
#include <math.h>

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib example.");
 
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------
    float debug=0;
    Color col1 = (Color){100,0  ,200,255};
    Color col2 = (Color){255,0  ,100,255};
    float size=1;
    int fresh=0;
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------

        fresh++;
        if(IsKeyPressed(KEY_SPACE)==true || fresh>30){
            fresh=0;
            col1 = (Color){GetRandomValue(0,255),GetRandomValue(0,255),GetRandomValue(0,255),255};
            col2 = (Color){GetRandomValue(0,255),GetRandomValue(0,255),GetRandomValue(0,255),255};
            size = GetRandomValue(1,32);
        }
        
        float stepr = abs(col1.r-col2.r)/(float)(screenHeight/size);
        float stepg = abs(col1.g-col2.g)/(float)(screenHeight/size);
        float stepb = abs(col1.b-col2.b)/(float)(screenHeight/size);
        if(col1.r>col2.r)stepr=-stepr;
        if(col1.g>col2.g)stepg=-stepg;
        if(col1.b>col2.b)stepb=-stepb;
        
        
        //----------------------------------------------------------------------------------
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);
            
            Color col=col1;
            float r=col.r;
            float g=col.g;
            float b=col.b;
            int cnt=0;
            for(int i=0;i<screenHeight;i++){
                DrawLine(0,i,screenWidth,i,col);
                if(cnt>=size){
                    r+=stepr;
                    g+=stepg;
                    b+=stepb;
                    col.r = r;
                    col.g = g;
                    col.b = b;                
                    cnt=0;
                }
                cnt++;
            }

        
            DrawText("Rainbow Backgrounds..",0,0,20,WHITE);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;


}
