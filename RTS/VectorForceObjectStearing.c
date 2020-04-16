//
// Looking through a book trying to understand code. 
// This, points on the map that repulse a unit traveling to a target.
//

#define MAX_UNIT 64
#define MAX_VFO 64


#include "raylib.h"
#include <math.h>

typedef struct unit{
    bool active;
    int width;
    int height;
    float mx;
    float my;
    Vector2 target;
    Vector2 position;
}unit;

typedef struct vfo{
    bool active;
    Vector2 position;
}vfo;

static unit arr_unit[MAX_UNIT];
static vfo arr_vfo[MAX_VFO];

static void drawunits();
static void drawvfo();
static float getangle(float x1,float y1,float x2,float y2);
static float distance(float x1,float y1,float x2,float y2);

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib example.");
 
    arr_unit[0].active = true;
    arr_unit[0].position = (Vector2){500,200};
    arr_unit[0].width = 16;
    arr_unit[0].height = 16;
    arr_unit[0].target = (Vector2){10*16,12*16};
    
    for(int i=0;i<15;i++){
        arr_vfo[i].active = true;
        arr_vfo[i].position = (Vector2){GetRandomValue(50,screenWidth-50),GetRandomValue(50,screenHeight-50)};
     }
 
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        arr_unit[0].mx = 0;
        arr_unit[0].my = 0;        
        float an = getangle(arr_unit[0].position.x,arr_unit[0].position.y,arr_unit[0].target.x,arr_unit[0].target.y);
        arr_unit[0].mx += cos(an)*1.2;
        arr_unit[0].my += sin(an)*1.2;
        int numvfo = 0;
        float mx=0;
        float my=0;
        for(int i=0;i<MAX_VFO;i++){
            float d = distance(arr_unit[0].position.x,arr_unit[0].position.y,arr_vfo[i].position.x,arr_vfo[i].position.y);
            if(d>64)continue; //preferable distance to keep from the vfo's
            numvfo++;
            float an = getangle(arr_unit[0].position.x,arr_unit[0].position.y,arr_vfo[i].position.x,arr_vfo[i].position.y);           
            mx -= (float)cos(an);
            my -= (float)sin(an);
            if(d<32){//if closer than this stear away extra
                mx -= (float)cos(an);
                my -= (float)sin(an);
                numvfo+=1;
            }
        }
        if(numvfo>0){
            arr_unit[0].mx += mx/(numvfo);
            arr_unit[0].my += my/(numvfo);
        }
        if(distance(arr_unit[0].position.x,arr_unit[0].position.y,arr_unit[0].target.x,arr_unit[0].target.y)>16){
            //
            // Keep the movement speed between -1.0f and 1.0f
            // We get the angle using atan2 and then recreate the mx and my using cos and sin
            // this puts the speed back between -1 and 1.
            float an=atan2(arr_unit[0].my,arr_unit[0].mx);
            arr_unit[0].mx = cos(an);
            arr_unit[0].my = sin(an);
            // move our unit
            arr_unit[0].position.x += arr_unit[0].mx;
            arr_unit[0].position.y += arr_unit[0].my;

        }else{// Here we have arived and now we create another target and new vfo's
            arr_unit[0].target = (Vector2){GetRandomValue(50,screenWidth-50),GetRandomValue(50,screenHeight-50)};
            for(int i=0;i<MAX_VFO;i++){
                if(arr_vfo[i].active){
                    arr_vfo[i].position = (Vector2){GetRandomValue(50,screenWidth-50),GetRandomValue(50,screenHeight-50)};
                }
            }
        }
        //----------------------------------------------------------------------------------
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);
            
            drawvfo();
            drawunits();
            
            DrawRectangle(10,screenHeight-40,screenWidth-20,30,(Color){100,100,100,100});
            DrawCircle(64,screenHeight-34+8,8,RED);
            DrawCircleLines(64,screenHeight-34+8,8,BLACK);
            DrawText("- Vector Force Object.",80,screenHeight-34,20,BLACK);

            DrawRectangle(400,screenHeight-34,arr_unit[0].width,arr_unit[0].height,YELLOW);
            DrawRectangleLines(400,screenHeight-34,arr_unit[0].width,arr_unit[0].height,BLACK);
            DrawText("- CPU Player.",440,screenHeight-34,20,BLACK);
            
            DrawText("Press Escape to End.",0,0,15,DARKGRAY);
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;


}

void drawunits(){
    for(int i=0;i<MAX_UNIT;i++){
        if(arr_unit[i].active==false)continue;        
        int w = arr_unit[i].width;
        int h = arr_unit[i].height;
        DrawRectangle(arr_unit[i].position.x-w/2,arr_unit[i].position.y-h/2,arr_unit[i].width,arr_unit[i].height,YELLOW);
        DrawRectangleLines(arr_unit[i].position.x-w/2,arr_unit[i].position.y-h/2,arr_unit[i].width,arr_unit[i].height,BLACK);
        // target
        DrawCircle(arr_unit[i].target.x,arr_unit[i].target.y,8,DARKGRAY);
        DrawText("Target",arr_unit[i].target.x,arr_unit[i].target.y,10,BLACK);
    }
}

void drawvfo(){
    for(int i=0;i<MAX_VFO;i++){
        if(arr_vfo[i].active==false)continue;
        DrawCircle(arr_vfo[i].position.x,arr_vfo[i].position.y,8,RED);
        DrawCircleLines(arr_vfo[i].position.x,arr_vfo[i].position.y,8,BLACK);
    }
}

//' Return the angle from - to in float
float getangle(float x1,float y1,float x2,float y2){
    return (float)atan2(y2-y1, x2-x1);
}

// Manhattan Distance (less precise)
float distance(float x1,float y1,float x2,float y2){
    return (float)abs(x2-x1)+abs(y2-y1);
}
