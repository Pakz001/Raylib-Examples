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
    int num=0;
    for(int y=10;y<14;y++){
    for(int x=15;x<19;x++){
        arr_vfo[num].active = true;
        arr_vfo[num].position.x = x*16;
        arr_vfo[num].position.y = y*16;
        num++;
    }}
 
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
        arr_unit[0].mx += cos(an)*2;
        arr_unit[0].my += sin(an)*2;
        int numvfo = 0;
        float mx=0;
        float my=0;
        for(int i=0;i<MAX_VFO;i++){
            if(distance(arr_unit[0].position.x,arr_unit[0].position.y,arr_vfo[i].position.x,arr_vfo[i].position.y)>200)continue;
            numvfo++;
            float an = getangle(arr_unit[0].position.x,arr_unit[0].position.y,arr_vfo[i].position.x,arr_vfo[i].position.y);           
            mx -= (float)cos(an);
            my -= (float)sin(an);
        }
        if(numvfo>0){
            arr_unit[0].mx += mx/(numvfo/1.2);
            arr_unit[0].my += my/(numvfo/1.2);
        }
        if(distance(arr_unit[0].position.x,arr_unit[0].position.y,arr_unit[0].target.x,arr_unit[0].target.y)>16){
            arr_unit[0].position.x += arr_unit[0].mx;
            arr_unit[0].position.y += arr_unit[0].my;
        }else{
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
        DrawRectangle(arr_unit[i].position.x,arr_unit[i].position.y,arr_unit[i].width,arr_unit[i].height,YELLOW);
        DrawRectangleLines(arr_unit[i].position.x,arr_unit[i].position.y,arr_unit[i].width,arr_unit[i].height,BLACK);
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
