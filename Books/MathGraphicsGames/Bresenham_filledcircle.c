//
// Bresenham circle FILL algorithm. (centerx, centery, radius.)
//
// From the book - mathmatics for computer graphics and game programming.
//


#include "raylib.h"

void circleBres(int xc, int yc, int r,Color col);
void putpixelBres(int xc, int yc, int x, int y,Color col);

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
                circleBres(x,y,r,RED);
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

void circleBres(int xc, int yc, int r,Color col) 
{ 
    int x=0;
    int y=r;
    int p=1-r;
    while(x<=y){
        putpixelBres(xc,yc,x,y,col);
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
void putpixelBres(int xc, int yc, int x, int y,Color col) 
{     
    DrawLine(xc+x, yc+y,xc-x, yc+y, col); 
    DrawLine(xc-x, yc+y,xc-x, yc-y, col); //**
    DrawLine(xc+x, yc-y,xc+x, yc+y, col); 
    DrawLine(xc+y, yc+x,xc-y, yc+x, col); 
    DrawLine(xc+y, yc-x,xc-y, yc-x, col); 
} 

