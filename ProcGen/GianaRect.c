
#include "raylib.h"

void c64rectangle(int x, int y, int w, int h,int thick);

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
            for(int y=0;y<screenHeight;y+=64){
            for(int x=0;x<screenWidth;x+=64){
                c64rectangle(x,y,64,64,4); 
            }}            

            for(int y=128;y<screenHeight-128;y+=32){
            for(int x=128;x<screenWidth-128;x+=32){
                c64rectangle(x,y,32,32,2); 
            }}            

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;


}

void c64rectangle(int x, int y, int w, int h,int thick){
    
    Color col1 = (Color){91,91,91,255};
    DrawRectangle(x,y,thick,thick,col1);
    DrawRectangle(x+w-thick,y,thick,thick,col1);
    Color col2 = (Color){120,120,120,255};
    DrawRectangle(x+thick,y,w-thick*2,thick,col2);
    Color col3 = (Color){105,105,105,255};
    DrawRectangle(x,y+thick,thick,h-thick*2,col3);
    Color col4 = (Color){114,114,114,255};
    DrawRectangle(x+thick*1,y+thick,thick,thick,col4);
    DrawRectangle(x+thick*1,y+h-thick*3,thick,thick,col4);
    Color col5 = (Color){79,79,79,255};
    DrawRectangle(x+thick*2,y+thick,w-thick*4,thick,col5);
    DrawRectangle(x+thick*1,y+thick*2,thick,h-thick*3,col5);
    Color col6 = (Color){0,0,0,255};
    // outside bottom and right black
    DrawRectangle(x,y+h-thick,w,thick,col6);
    DrawRectangle(x+w-thick,y+thick,thick,h-thick,col6);
    // inside black 
    Color col7 = (Color){58,58,58,255};
    DrawRectangle(x+thick*2,y+thick*2,w-thick*4,thick,col6);
    DrawRectangle(x+thick*2,y+thick*2,thick,h-thick*4,col6);
    DrawRectangle(x+w-thick*2,y+thick*1,thick,thick,col7);
    
    DrawRectangle(x+thick*2,y+h-thick*2,thick,thick,col7);
    DrawRectangle(x+w-thick*2,y+h-thick*2,thick,thick,col7);
    // Inside area gray
    DrawRectangle(x+thick*3,y+thick*3,w-thick*6,h-thick*5,col1);
    // 
    DrawRectangle(x+thick*3,y+h-thick*2,w-thick*5,thick,col3);
    DrawRectangle(x+w-thick*3,y+thick*3,thick,h-thick*5,col2);
    //
    DrawRectangle(x+w-thick*2,y+thick*2,thick,h-thick*4,col5);
}   
