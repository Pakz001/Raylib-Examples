
#include "raylib.h"

static void drawroundedrect(int x, int y, int w,int h,Color col);
static void drawroundedrectline(int x, int y, int w,int h,Color col);

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
       
            drawroundedrect(10,0,screenWidth-20,20,DARKGRAY);
            drawroundedrect(screenWidth-20,20,20,screenHeight-40,DARKGRAY);
            drawroundedrectline(200,200,320,150,GRAY);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;


}

void drawroundedrect(int x, int y, int w,int h,Color col){

    // our round border size.
    int roundsize = 20;
    
    // if messy then look here..
    if(w<50)roundsize/=2;
    if(h<50)roundsize/=2;
    if(w<20)roundsize/=2;
    if(h<20)roundsize/=2;
    
    // first draw 4 circles at the corners
    DrawCircle(x+roundsize,y+roundsize,roundsize,col);
    DrawCircle(x+w-roundsize,y+roundsize,roundsize,col);
    DrawCircle(x+roundsize,y+h-roundsize,roundsize,col);
    DrawCircle(x+w-roundsize,y+h-roundsize,roundsize,col);

    // Draw two rectangles on top of the circles..
    DrawRectangle(x,y+roundsize,w,h-roundsize*2,col);
    DrawRectangle(x+roundsize,y,w-roundsize*2,h,col);
}

void drawroundedrectline(int x, int y, int w,int h,Color col){
        
    int roundsize = 20;

    // if messy then look here..
    if(w<50)roundsize/=2;
    if(h<50)roundsize/=2;
    if(w<20)roundsize/=2;
    if(h<20)roundsize/=2;

    // first draw 4 circles at the corners
    DrawCircle(x+roundsize,y+roundsize,roundsize,col);    
    DrawCircle(x+w-roundsize,y+roundsize,roundsize,col);
    DrawCircle(x+roundsize,y+h-roundsize,roundsize,col);
    DrawCircle(x+w-roundsize,y+h-roundsize,roundsize,col);
    // Draw the edge lines.    
    DrawCircleLines(x+roundsize,y+roundsize,roundsize,BLACK);    
    DrawCircleLines(x+w-roundsize,y+roundsize,roundsize,BLACK);
    DrawCircleLines(x+roundsize,y+h-roundsize,roundsize,BLACK);
    DrawCircleLines(x+w-roundsize,y+h-roundsize,roundsize,BLACK);

    // Draw two rectangles on top of the circles..
    DrawRectangle(x,y+roundsize,w,h-roundsize*2,col);
    DrawRectangle(x+roundsize,y,w-roundsize*2,h,col);
    // Draw the lines on the outside.
    DrawLine(x,y+roundsize,x,y+h-roundsize,BLACK);
    DrawLine(x+w,y+roundsize,x+w,y+h-roundsize,BLACK);
    DrawLine(x+roundsize,y,x+w-roundsize,y,BLACK);
    DrawLine(x+roundsize,y+h,x+w-roundsize,y+h,BLACK);
}
