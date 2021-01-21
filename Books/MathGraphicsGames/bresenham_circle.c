//
// Bresenham circle algorithm. (centerx, centery, radius.)
//
// From the book - mathmatics for computer graphics and game programming.
//


#include "raylib.h"

void circleBres(int xc, int yc, int r);
void putpixelBres(int xc, int yc, int x, int y);

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
            circleBres(100,100,100);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;


}

void circleBres(int xc, int yc, int r) 
{ 
    int x=0;
    int y=r;
    int p=1-r;
    while(x<=y){
        putpixelBres(xc,yc,x,y);
        if(p<=0){
            x=x+1;
            y=y;
            p=p+(x*2)+3;
        }else if(p>0)
        {
            x=x+1;
            y=y-1;
            p=p+(x*2)-(y*2)+5;
        }
    }
}    
void putpixelBres(int xc, int yc, int x, int y) 
{     
    DrawPixel(xc+x, yc+y, RED); 
    DrawPixel(xc-x, yc+y, RED); 
    DrawPixel(xc+x, yc-y, RED); 
    DrawPixel(xc-x, yc-y, RED); 
    DrawPixel(xc+y, yc+x, RED); 
    DrawPixel(xc-y, yc+x, RED); 
    DrawPixel(xc+y, yc-x, RED); 
    DrawPixel(xc-y, yc-x, RED); 
} 
