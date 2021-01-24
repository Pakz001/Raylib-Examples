//
// Copper bars. An background effect. Put a title or some text in front of it.
//
//
#include "raylib.h"
#include <math.h>

void copperbars(float val, int y, int height,int screenWidth,  Color col);
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

    float val=0;
    Color col=RED;
    col.a=GetRandomValue(100,255);
    int sw=0;
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        
        // val is the speed of which the copperbars bounce on the screen.
        val+=.076;
        if(val>PI*2.0f)val=0;
        // Every 120 frames change to a different color
        sw++;
        if(sw>120){
            sw=0;
            col=(Color){GetRandomValue(0,255),GetRandomValue(0,255),GetRandomValue(0,255),GetRandomValue(100,255)};
            // Make sure we have a bright color.
            if(col.r<150 && col.g<150 && col.b<150)col.r=255;
        }
        
        //----------------------------------------------------------------------------------
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(BLACK);
            //
            // value(0..PI*2.0f)
            // y location
            // height(height of screen taken)
            // width of screen
            // Color
            copperbars(val,0,240,screenWidth,col);    
            
            for(int y=-3;y<3;y++){
            for(int x=-3;x<3;x++){
                DrawText("COPPER BARS",x+50,y+20,90,BLACK);
            }}
            DrawText("COPPER BARS",50,20,90,YELLOW);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;


}
// value(0..PI*2.0f)
// y location
// height(height of screen taken)
// width of screen
// Color  
void copperbars(float val, int y, int height, int screenWidth,  Color col){
    // Convert the height to fit the screen.
    height/=2.3;
    // Draw 8 bars
    for(int i=0;i<8;i++){
        // Draw the top of the bar
        for(int j=0;j<16;j++){            
            int py=y+((sin(val)*height+height));
            DrawRectangle(0,py+j,screenWidth,1,(Color){    Clamp((col.r/16)*j,0,255),
                                                           Clamp((col.g/16)*j,0,255),
                                                           Clamp((col.b/16)*j,0,255),col.a});
        }
        // Draw the bottom of the bar
        for(int j=16;j>0;j--){
            int py=y+sin(val)*height+height+15;
            DrawRectangle(0,py+16-j,screenWidth,1,(Color){  Clamp((col.r/16)*j,0,255),
                                                            Clamp((col.g/16)*j,0,255),
                                                            Clamp((col.b/16)*j,0,255),col.a});
        }
        // Distance between the bars.
        val+=.35;        
    }
}

// Clamp float value
float Clamp(float value, float min, float max)
{
    const float res = value < min ? min : value;
    return res > max ? max : res;
}
