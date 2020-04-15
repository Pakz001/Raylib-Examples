//
// Vector Force object test.. Wo\rk in progress
//
// In the book Pro Html5 Games one of the games learned to program there is a rts. To be honest I do not really understand
// much of how to code a complete rts like shown in that book but I was looking through it again and had learned more about
// vectors. (search: the coding train)
// Force vector object in this book are used to be placed on a path to push away units. I tried to code this myself here but only
// used a couple of these vfo's myself.
// 
// What the code does is create a set of units on the map. One routine is to move all units apart from each other. The Force
// vector object code moves away units from that force vector object position. I find the angle from vector vs unit and move
// the unit in the opposite direction of that point.
//
// These vfo's can (one) be placed under the mouse. One is positioned in the movement direction of the player controlled
// unit.

// It seems to work..
//

#define MAX_UNITS 64
#define MAX_VFO 64

#include "raylib.h"
#include <math.h>

static float tileWidth,tileHeight;

typedef struct vfo{ // force vector object (repulse objects from this point.
    bool active;
    Vector2 position;
}vfo;

typedef struct unit{
    bool active;
    bool controlled;
    float x,y;
    int targetx,targety;    
}unit;


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
    const int mapHeight = 20;

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
    // Our Force Vector Object [0] is the one in front of our player.
    arr_vfo[0].active = true;
    for(int i=1;i<MAX_VFO;i++){
        arr_vfo[i].active=false;
    }
    
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        
        // Force Vector Object [0] is in front of our player contoller unit.
        arr_vfo[0].position.x = arr_unit[0].x+tileWidth/2;
        arr_vfo[0].position.y = arr_unit[0].y+tileHeight/2;
        // Move our unit and position the force vector object.
        if(IsKeyDown(KEY_UP)){
            arr_unit[0].y-=1;
            arr_vfo[0].position.y -= tileHeight;
        }
        if(IsKeyDown(KEY_DOWN)){
            arr_unit[0].y+=1;
            arr_vfo[0].position.y += tileHeight;        
        }
        if(IsKeyDown(KEY_LEFT)){
            arr_unit[0].x-=1;
            arr_vfo[0].position.x -= tileWidth/2+tileWidth/2;
        }
        if(IsKeyDown(KEY_RIGHT)){
            arr_unit[0].x+=1;
            arr_vfo[0].position.x += tileWidth;
        }        

        // Position a Force Vector Object under the mouse!
        if(IsMouseButtonPressed(0)){
            arr_vfo[1].active = true;
            arr_vfo[1].position.x = GetMouseX();
            arr_vfo[1].position.y = GetMouseY();
        }


        //----------------------------------------------------------------------------------
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);
            
            // draw units
            for(int i=0;i<MAX_UNITS;i++){
            if(arr_unit[i].active){
                Color MYCOL=(Color){255,255,0,255};
                if(i==0)MYCOL=(Color){255,0,0,255};
                DrawRectangle(arr_unit[i].x,arr_unit[i].y,tileWidth,tileHeight,BLACK);
                DrawRectangle(arr_unit[i].x+1,arr_unit[i].y+1,tileWidth-2,tileHeight-2,MYCOL);
            }
            }
            
            //
            // Move units away from each other.
            for(int i=0;i<MAX_UNITS;i++){            
            for(int ii=0;ii<MAX_UNITS;ii++){
                if(arr_unit[i].active==false || arr_unit[ii].active==false || i==ii)continue;                
                float d = distance(arr_unit[i].x,arr_unit[i].y,arr_unit[ii].x,arr_unit[ii].y);
                if(d>64)continue;
                float an=getangle(arr_unit[i].x,arr_unit[i].y,arr_unit[ii].x,arr_unit[ii].y);
                if(arr_unit[i].controlled==false)arr_unit[i].x -= cos(an)*2;
                if(arr_unit[i].controlled==false)arr_unit[i].y -= sin(an)*2;
                if(arr_unit[ii].controlled==false)arr_unit[ii].x += cos(an)*2;
                if(arr_unit[ii].controlled==false)arr_unit[ii].y += sin(an)*2;
            }
            }
            
            // Move units away from force vector object
            for(int i=1;i<MAX_VFO;i++){ //force vector object 0 is for user controlled
            for(int ii=0;ii<MAX_UNITS;ii++){
                if(arr_unit[ii].active==false || arr_unit[ii].controlled==true)continue;
                if(distance(arr_vfo[i].position.x,arr_vfo[i].position.y,arr_unit[ii].x+tileWidth/2,arr_unit[ii].y+tileHeight/2)>64)continue;
                float an=getangle(arr_vfo[i].position.x,arr_vfo[i].position.y,arr_unit[ii].x+tileWidth/2,arr_unit[ii].y+tileHeight/2);
                arr_unit[ii].x += cos(an)*2;
                arr_unit[ii].y += sin(an)*2;                
            }
            }
            
            
           // Draw vfo points.
           for(int i=0;i<MAX_VFO;i++){
               if(arr_vfo[i].active==false)continue;
               DrawCircle(arr_vfo[i].position.x,arr_vfo[i].position.y,1,RED);
           }
           
           //
           DrawText("Use Cursor keys to control unit. Press Mouse to Scatter units.",0,0,20,DARKGRAY);
            
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
