//
// Circle screen transition effect (CLEAN)
//
// Draw a circle on the screen but draw from the edges of this circle
// to the edges of the screen.
//
//

#include "raylib.h"

void circletransition(int r, int screenWidth, int screenHeight, Color col);

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib example.");
 
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    int radius = 0;
    int state = 0;

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        
        if(state==0 && radius<screenWidth/1.5){
            radius+=10;
        }else{
            state = 1;
        }
        if(state==1 && radius>0){
            radius-=10;
        }else{
            state = 0;
        }
        
        
        //----------------------------------------------------------------------------------
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);
            
            
            for(int y=0;y<screenHeight;y+=30){
            for(int x=0;x<screenWidth;x+=250){
                DrawText("Hello this is a fade..",x,y,25,BLACK);
            }}
            
            circletransition(radius,screenWidth,screenHeight,BLACK);
            
            
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;


}

// 
// Our circle transition effect.
//
void circletransition(int radius, int screenWidth, int screenHeight, Color col) { 
    int xc = screenWidth/2;
    int yc = screenHeight/2;
    // Draw the line above and below the circle...
    for(int a=0;a<yc-radius;a++){
        DrawLine(0,a,screenWidth,a,BLACK);
    }
    for(int a=yc+radius;a<=screenHeight;a++){
        DrawLine(0,a,screenWidth,a,BLACK);
    }
    // Draw from the edges of the circle to the edges of the screen.
    int largestX = radius;
    for (int y = 0; y <= radius; ++y) {
        for (int x = largestX; x >= 0; --x) {
            if ((x * x) + (y * y) <= (radius * radius)) {
                DrawLine(0,yc-y,xc-x,yc-y,col);
                DrawLine(0,yc+y,xc-x,yc+y,col);
                DrawLine(xc+x,yc-y,screenWidth,yc-y,col);
                DrawLine(xc+x,yc+y,screenWidth,yc+y,col);
                largestX = x;
                break; // go to next y coordinate
            }
        }
    }
}    
