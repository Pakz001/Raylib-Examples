
#include "raylib.h"

void drawpillar01(int x, int y, int w, int h, int thick, Color col);
// Raylibs built in Clamp seems not to work?
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

    int time=0;
    Color col=(Color){GetRandomValue(0,255),GetRandomValue(0,255),GetRandomValue(0,255),255};
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        time++;
        if(time>60){
            time=0;
            col=(Color){GetRandomValue(0,255),GetRandomValue(0,255),GetRandomValue(0,255),255};
        }
        
        //----------------------------------------------------------------------------------
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

    
            Color mycol = (Color){  Clamp((col.r/100)*110,0,255),
                                    Clamp((col.g/100)*110,0,255),
                                    Clamp((col.b/100)*110,0,255),255};
            
            ClearBackground(mycol);
            for(int x=0;x<screenWidth;x+=200){
            for(int y=0;y<screenHeight;y+=256){
            drawpillar01(x,y,96,256,8,col);
            
            }
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

void drawpillar01(int x, int y, int w, int h,int thick, Color col){
    Color col1 = (Color){63,30,3,255};
    Color col2 = (Color){81,40,0,255};
    Color col3 = (Color){119,61,7,255};
    Color col4 = (Color){159,116,2,255};
    Color col5 = (Color){198,181,0,255};

    int gc=0;
    
    gc=(col1.r+col1.g+col1.b)/3;
    col1 = (Color){ Clamp(gc/2+(col.r/100)*50,0,255),
                    Clamp(gc/2+(col.g/100)*50,0,255),
                    Clamp(gc/2+(col.b/100)*50,0,255),255};
    gc=(col2.r+col2.g+col2.b)/3;    
    col2 = (Color){ Clamp(gc/2+(col.r/100)*50,0,255),
                    Clamp(gc/2+(col.g/100)*50,0,255),
                    Clamp(gc/2+(col.b/100)*50,0,255),255};
    gc=(col3.r+col3.g+col3.b)/3;    
    col3 = (Color){ Clamp(gc/2+(col.r/100)*50,0,255),
                    Clamp(gc/2+(col.g/100)*50,0,255),
                    Clamp(gc/2+(col.b/100)*50,0,255),255};
    gc=(col4.r+col4.g+col4.b)/3;    
    col4 = (Color){ Clamp(gc/2+(col.r/100)*50,0,255),
                    Clamp(gc/2+(col.g/100)*50,0,255),
                    Clamp(gc/2+(col.b/100)*50,0,255),255};
    gc=(col5.r+col5.g+col5.b)/3;    
    col5 = (Color){ Clamp(gc/2+(col.r/100)*50,0,255),
                    Clamp(gc/2+(col.g/100)*50,0,255),
                    Clamp(gc/2+(col.b/100)*50,0,255),255};

    
    // TOP OF PILLAR
    DrawRectangle(x,y,w,thick*8,col1);
    DrawRectangle(x+w/2-thick*3,y,thick*6,4,col2);
    DrawRectangle(x+thick,y+thick,w-thick*2,thick*5,col2);
    DrawRectangle(x+w/2-thick*4,y+thick*2,thick*8,thick*3,col3);
    DrawRectangle(x+w/2-thick*2,y+thick*3,thick*4,thick,col4);
    DrawRectangle(x+w/2-thick*3,y+thick*5,thick,thick,col3);
    DrawRectangle(x+w/2-thick*2,y+thick*5,thick*4,thick,col4);
    DrawRectangle(x+w/2+thick*2,y+thick*5,thick,thick,col3);
    y+=thick*6;
    //
    // MIDDLE PILLAR
    // 
    h-=thick*2;
    // Inner pillar
    DrawRectangle(x+thick,y,w/2,h,col2);
    DrawRectangle(x+w/2,y,w/2,h,col2);

    // Outer pillar line
    DrawRectangle(x,y,thick,h,col1);
    DrawRectangle(x+w-thick,y,thick,h,col1);
    // Pillar inner edge
    DrawRectangle(x+w/2-thick*3,y,thick,h,col3);
    DrawRectangle(x+w/2-thick*2,y,thick,h,col4);
    DrawRectangle(x+w/2-thick,y,thick*2,h,col5);
    DrawRectangle(x+w/2+thick,y,thick,h,col4);
    DrawRectangle(x+w/2+thick*2,y,thick,h,col3);
    h+=thick*2;
    //
    // BOTTOM PILLAR
    //
    DrawRectangle(x,y+h-thick*2,w,thick*2,col1);
    
    DrawRectangle(x+w/2-thick*5,y+h-thick*2,thick*10,thick,col2);
    DrawRectangle(x+w/2-thick*6,y+h-thick,thick*12,thick,col2);
    
    DrawRectangle(x+w/2-thick*3,y+h-thick,thick*2,thick*2,col3);
    DrawRectangle(x+w/2-thick*2,y+h-thick*2,thick,thick*2,col3);
    DrawRectangle(x+w/2-thick,y+h-thick*2,thick*2,thick*2,col4);
    DrawRectangle(x+w/2+thick*1,y+h-thick*2,thick,thick*2,col3);
    DrawRectangle(x+w/2+thick*2,y+h-thick,thick,thick*2,col3);

}


// Clamp float value
float Clamp(float value, float min, float max)
{
    const float res = value < min ? min : value;
    return res > max ? max : res;
}
