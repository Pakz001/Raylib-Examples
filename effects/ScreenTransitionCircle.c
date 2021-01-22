//
// Circle screen transition effect using Bresenham circle.
//
//

#include "raylib.h"

//
// Our function for transitioning the screen.
void circletransition(int radius, int screenWidth, int screenHeight, Color col);
// Helper functions for curcletransition....
void circleBres(int xc, int yc, int r,int screenWidth, int screenHeight, Color col);
void putpixelBres(int xc, int yc, int x, int y,int screenWidth, int screenHeight,  Color col);


int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    int screenWidth = 800;
    int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib example.");
 
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // transition setup(start with nothing to grow)
    int radius=0;
    int state=0;

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        
        //
        // Here our transition effects is done.
        //
        // Draw our transition
        circletransition(radius,screenWidth,screenHeight,BLACK);
        // Grow our circle
        if(state==0 && radius<screenWidth/1.5){
            radius+=6;
        }else{
            state=1;
        }
        // Shrink our circle
        if(state==1 && radius>0){
            radius-=6;
        }else{
            state=0;
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
            
            

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;


}

void circletransition(int radius, int screenWidth, int screenHeight,Color col){
    int cx = screenWidth/2;
    int cy = screenHeight/2;
    for(int a=0;a<cy-radius;a++){
        DrawLine(0,a,screenWidth,a,col);
    }        
    circleBres(cx,cy, radius,screenWidth,screenHeight,BLACK);
    for(int a=cy+radius;a<=screenHeight;a++){
        DrawLine(0,a,screenWidth,a,col);
    }

}

void circleBres(int xc, int yc, int r, int screenWidth, int screenHeight,  Color col) 
{ 
    int x=0;
    int y=r;
    int p=1-r;
    while(x<=y){
        putpixelBres(xc,yc,x,y,screenWidth, screenHeight, col);
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
//
// Here we draw from the circle outwards to the edges of the screen,.
void putpixelBres(int xc, int yc, int x, int y, int screenWidth, int screenHeight, Color col) {     
    DrawLine(screenWidth,yc+y,xc+x, yc+y, col);
    DrawLine(0,yc+y,xc-x, yc+y, col); 
    DrawLine(screenWidth,yc-y,xc+x, yc-y, col); 
    DrawLine(0,yc-y,xc-x, yc-y, col); 
    DrawLine(0,yc-x,xc-y, yc-x, col); 
    DrawLine(screenWidth,yc-x,xc+y, yc-x, col); 
    DrawLine(0,yc+x,xc-y, yc+x, col); 
    DrawLine(screenWidth,yc+x,xc+y, yc+x, col); 
    
} 

