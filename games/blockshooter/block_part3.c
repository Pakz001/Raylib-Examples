
#include "raylib.h"
#include <math.h>

int map[100][100] = {0};



// Get our angle between two points.
static float getangle(float x1,float y1,float x2,float y2);
// Our rectsoverlap function. Returns true/false.
static bool rectsoverlap(int x1,int y1,int w1,int h1,int x2,int y2,int w2,int h2);



int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib example.");
 
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    for(int i=0;i<100;i++){
        map[i][0]=1;
    }
    map[5][1]=1;

    float posx = 320;
    float posy = 400;
    
    float animx;
    float animy;
    float animtargetx;
    float animtargety;
    bool startanim = false;

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        if(startanim==false){
            if(IsMouseButtonDown(0)){
                
                float x = GetMouseX()/32;
                float y = GetMouseY()/32;
                
                    if(map[(int)x][(int)y]==0){
                    if( map[(int)x][(int)y-1]==1 || map[(int)x-1][(int)y]==1 || map[(int)x+1][(int)y]==1){
                        //DrawRectangle(x*32,y*32,32,32,DARKGRAY);
                        startanim=true;
                        animtargetx = x*32;
                        animtargety = y*32;
                        animx = posx;
                        animy = posy;
                    }}

                    
                    
                }
        }
        if(startanim){
            float an = getangle(animx,animy,animtargetx,animtargety);
            animx += cos(an)*8;
            animy += sin(an)*8;
            if(rectsoverlap(animx-1,animy-1,32+2,32+2,animtargetx-1,animtargety-1,32+2,32+2)){
                startanim=false;
                map[(int)animtargetx/32][(int)animtargety/32]=1;
            }
            
        }
        
        //----------------------------------------------------------------------------------
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);
 
            for(int y=0;y<10;y++){
            for(int x=0;x<100;x++){
                if(map[x][y]==1){
                    DrawRectangle(x*32,y*32,32,32,RED);
                    DrawRectangle(x*32+1,y*32+1,32-2,32-2,BLUE);
                }
            }}
 
            if(startanim){
            DrawRectangle(animx,animy,32,32,RED);
            }
 
            float x = GetMouseX()/32;
            float y = GetMouseY()/32;
            
            if(map[(int)x][(int)y]==0){
            if( map[(int)x][(int)y-1]==1 || map[(int)x-1][(int)y]==1 || map[(int)x+1][(int)y]==1){
                DrawRectangle(x*32,y*32,32,32,DARKGRAY);
            }}
            
            float an = getangle(posx,posy,x*32,y*32);
 
            DrawLine(posx,posy,posx+cos(an)*32,posy+sin(an)*32,DARKGRAY);
 

            

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;


}
// Return the angle from - to in float
float getangle(float x1,float y1,float x2,float y2){
    return (float)atan2(y2-y1, x2-x1);
}
// Rectangles overlap
bool rectsoverlap(int x1,int y1,int w1,int h1,int x2,int y2,int w2,int h2){
    if(x1 >= (x2 + w2) || (x1 + w1) <= x2) return false;
    if(y1 >= (y2 + h2) || (y1 + h1) <= y2) return false;
    return true;
}
