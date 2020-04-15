//
// Vector Force object test.. Wo\rk in progress
//

#define MAX_UNITS 64
#define MAX_VFO 64

#include "raylib.h"
#include <math.h>

static float tileWidth,tileHeight;

typedef struct vfo{ // force vector object (repulse objects from this point.
    bool active;
    Vector2 point;
};

typedef struct unit{
    bool active;
    bool controlled;
    float x,y;
    int targetx,targety;    
};


static struct unit arr_unit[MAX_UNITS];
static struct vfo arr_vfo[MAX_VFO];

static float getangle(float x1,float y1,float x2, float y2);
static float distance(float x1,float y1,float x2, float y2);

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;
    const int mapWidth = 20;
    const int mapHeight = 15;

    tileWidth = abs((float)screenHeight/(float)mapWidth);
    tileHeight = abs((float)screenHeight/(float)mapHeight);

    InitWindow(screenWidth, screenHeight, "raylib example.");
 
    //
    // initiate a number of units.
    for(int i=0;i<MAX_UNITS;i++){ // all units to false
        arr_unit[i].active = false;
    }
    int num=0;
    for(int y=8;y<12;y++){ // create a number of units.
    for(int x=8;x<12;x++){
        arr_unit[num].active = true;
        arr_unit[num].controlled = false;
        arr_unit[num].x = 8*tileWidth;
        arr_unit[num].y = 8*tileHeight+GetRandomValue(-5,5);
        arr_unit[num].targetx = -1;
        arr_unit[num].targety = -1;
        num++;        
    }}
    arr_unit[0].controlled = true;
    
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        if(IsMouseButtonPressed(0)){
            arr_vfo[0].active = true;
            arr_vfo[0].point = GetMousePosition();
        }
        if(IsKeyDown(KEY_UP))arr_unit[0].y-=1;
        if(IsKeyDown(KEY_DOWN))arr_unit[0].y+=1;
        if(IsKeyDown(KEY_LEFT))arr_unit[0].x-=1;
        if(IsKeyDown(KEY_RIGHT))arr_unit[0].x+=1;

        //----------------------------------------------------------------------------------
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);
            
            // draw units
            for(int i=0;i<MAX_UNITS;i++){
            if(arr_unit[i].active){
                DrawRectangle(arr_unit[i].x,arr_unit[i].y,tileWidth,tileHeight,BLACK);
                DrawRectangle(arr_unit[i].x+1,arr_unit[i].y+1,tileWidth-2,tileHeight-2,YELLOW);
            }
            }
            
            // Move units away from each other.
            for(int i=0;i<MAX_UNITS;i++){
            //if(arr_unit[i].controlled)continue;
            for(int ii=0;ii<MAX_UNITS;ii++){
                if(arr_unit[i].active==false || arr_unit[ii].active==false || i==ii)continue;
                //if(arr_unit[ii].controlled)continue;
                float d = distance(arr_unit[i].x,arr_unit[i].y,arr_unit[ii].x,arr_unit[ii].y);
                if(d>64)continue;
                float an=getangle(arr_unit[i].x,arr_unit[i].y,arr_unit[ii].x,arr_unit[ii].y);
                if(arr_unit[i].controlled==false)arr_unit[i].x -= cos(an)*2;
                if(arr_unit[i].controlled==false)arr_unit[i].y -= sin(an)*2;
                if(arr_unit[ii].controlled==false)arr_unit[ii].x += cos(an)*2;
                if(arr_unit[ii].controlled==false)arr_unit[ii].y += sin(an)*2;
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

//' Return the angle from - to in float
float getangle(float x1,float y1,float x2,float y2){
    return (float)atan2(y2-y1, x2-x1);
}

// Manhattan Distance (less precise)
float distance(float x1,float y1,float x2,float y2){
    return (float)abs(x2-x1)+abs(y2-y1);
}
