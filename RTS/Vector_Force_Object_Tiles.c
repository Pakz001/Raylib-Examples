

#define MAX_UNITS 64
#define MAX_VFO 64

#include "raylib.h"
#include <math.h>

// 0 is passable 1 is blocked
static int map[20][20] =    {   {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
                                {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                                {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                                {1,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,1},
                                {1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                                {1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                                {1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                                {1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                                {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                                {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                                {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                                {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},                                {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                                {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                                {1,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1},
                                {1,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,1},
                                {1,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,1},
                                {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
                                {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
                                {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}};

static float tileWidth,tileHeight;
static int mapWidth,mapHeight;

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
static bool utc(int unit,int offsetx,int offsety);
static bool rectsoverlap(int x1,int y1,int w1,int h1,int x2,int y2,int w2,int h2);

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;
    mapWidth = 20;
    mapHeight = 20;
    tileWidth = abs((float)screenWidth/(float)mapWidth);
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

            //
            // Move units away from each other.
            for(int i=0;i<MAX_UNITS;i++){            
            for(int ii=0;ii<MAX_UNITS;ii++){
                if(arr_unit[i].active==false || arr_unit[ii].active==false || i==ii)continue;                
                float d = distance(arr_unit[i].x,arr_unit[i].y,arr_unit[ii].x,arr_unit[ii].y);
                if(d>64)continue;
                float an=getangle(arr_unit[i].x,arr_unit[i].y,arr_unit[ii].x,arr_unit[ii].y);
                // Store positions of units to be moved here
                int oldx1 = arr_unit[i].x;
                int oldy1 = arr_unit[i].y;
                int oldx2 = arr_unit[ii].x;
                int oldy2 = arr_unit[ii].y;
                // Move the units here.
                if(arr_unit[i].controlled==false)arr_unit[i].x -= cos(an)*2;
                if(arr_unit[i].controlled==false)arr_unit[i].y -= sin(an)*2;
                if(arr_unit[ii].controlled==false)arr_unit[ii].x += cos(an)*2;
                if(arr_unit[ii].controlled==false)arr_unit[ii].y += sin(an)*2;
                
                // if collide with map then restore old position.
                if(utc(i,0,0)){
                    
                arr_unit[i].x = oldx1;
                arr_unit[i].y = oldy1;
                }

                if(utc(ii,0,0)){
                    
                arr_unit[ii].x = oldx2;
                arr_unit[ii].y = oldy2;
                }

            }
            }
            
            // Move units away from force vector object
            for(int i=1;i<MAX_VFO;i++){ //force vector object 0 is for user controlled
            for(int ii=0;ii<MAX_UNITS;ii++){
                if(arr_unit[ii].active==false || arr_unit[ii].controlled==true)continue;
                if(distance(arr_vfo[i].position.x,arr_vfo[i].position.y,arr_unit[ii].x+tileWidth/2,arr_unit[ii].y+tileHeight/2)>64)continue;
                float an=getangle(arr_vfo[i].position.x,arr_vfo[i].position.y,arr_unit[ii].x+tileWidth/2,arr_unit[ii].y+tileHeight/2);
                // store old unit position
                int oldx=arr_unit[ii].x;
                int oldy=arr_unit[ii].y;
                // Move unit
                arr_unit[ii].x += cos(an)*2;
                arr_unit[ii].y += sin(an)*2;                
                // if collide with map then restore old position.
                if(utc(ii,0,0)){ // unit tile collision
                    
                arr_unit[ii].x = oldx;
                arr_unit[ii].y = oldy;
                }
            }
            }
            
            
        //----------------------------------------------------------------------------------
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);
            
            // Draw Map
            for(int y=0;y<mapHeight;y++){
            for(int x=0;x<mapWidth;x++){
                if(map[y][x]==1){
                    DrawRectangle(x*tileWidth,y*tileHeight,tileWidth,tileHeight,GREEN);
                }
            }
            }
            
            // draw units
            for(int i=0;i<MAX_UNITS;i++){
            if(arr_unit[i].active){
                Color MYCOL=(Color){255,255,0,255};
                if(i==0)MYCOL=(Color){255,0,0,255};
                DrawRectangle(arr_unit[i].x,arr_unit[i].y,tileWidth,tileHeight,BLACK);
                DrawRectangle(arr_unit[i].x+1,arr_unit[i].y+1,tileWidth-2,tileHeight-2,MYCOL);
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

//Unit collide with solid blocks true/false
bool utc(int unit,int offsetx,int offsety){
    int cx = (arr_unit[unit].x+offsetx)/tileWidth;
    int cy = (arr_unit[unit].y+offsety)/tileHeight;
    for(int y2=cy-1; y2<cy+5;y2++){
    for(int x2=cx-1; x2<cx+5;x2++){
        if(x2>=0 && x2<mapWidth && y2>=0 && y2<mapHeight){
            if(map[y2][x2] == 1){
                int x3 = (x2)*tileWidth-tileWidth;
                int y3 = (y2)*tileHeight;
                if(rectsoverlap(arr_unit[unit].x-tileWidth+offsetx,arr_unit[unit].y+offsety,tileWidth,tileHeight,x3,y3,tileWidth,tileHeight)){
                    return true;
                }
            }
        }
    }}
    return false;
}

// Rectangles overlap
bool rectsoverlap(int x1,int y1,int w1,int h1,int x2,int y2,int w2,int h2){
    if(x1 >= (x2 + w2) || (x1 + w1) <= x2) return false;
    if(y1 >= (y2 + h2) || (y1 + h1) <= y2) return false;
    return true;
}
