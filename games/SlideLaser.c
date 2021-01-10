//
// Sliding laser.

// The idea here is to have a player slide a laser below a enemy or enemy weapon. When the laser
// stops sliding a laser is shot upwards and it should destroy the threat.
//
// Going on a break.. Maybe I will expand and make it more playable later.
//
//

#include "raylib.h"
#include <math.h>

#define MAX_CARS 100
#define MAX_WHISKERS 8

#define MAX_SLIDEBOMBS 10
   
int myMap[10][11] =  {  {1,1,1,1,1,1,1,1,1,1,1},
                        {1,1,1,1,1,1,1,1,1,1,1},
                        {1,1,1,1,1,1,1,1,1,1,1},
                        {1,1,1,1,1,1,1,1,1,1,1},
                        {1,1,1,0,0,0,0,1,1,1,1},
                        {1,1,1,0,0,0,0,1,0,0,1},
                        {1,1,1,0,0,0,0,1,0,0,1},
                        {1,1,1,0,0,0,0,1,0,0,1},
                        {1,0,0,0,0,0,0,0,0,0,1},
                        {1,1,1,1,1,1,1,1,1,1,1}
                    };  
int mapWidth = 11;
int mapHeight = 10;
float tileWidth;
float tileHeight;

typedef struct player{
    bool active;
    Vector2 position;    
    int direction; // -1 left, 2 - right
    int w;
    int h;
}player;

static player myplayer = {0};

static struct slidelaser{
    bool active;
    int state;
    Vector2 position;
    Vector2 inc;
    Vector2 incdec;
    int ceilingloc;
    int w;
    int h;
}slidelaser;

static struct slidelaser arr_slidelaser[MAX_SLIDEBOMBS];


//Unit collide with solid blocks true/false
bool recttilecollide(int x, int y, int w, int h, int offsetx,int offsety);
// Our rectsoverlap function. Returns true/false.
static bool rectsoverlap(int x1,int y1,int w1,int h1,int x2,int y2,int w2,int h2);
                        
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 600;
    tileWidth = ceil((float)(float)screenWidth/(float)mapWidth);
    tileHeight = ceil((float)screenHeight/(float)mapHeight);

    InitWindow(screenWidth, screenHeight, "raylib example.");
 
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    Vector2 position;
    position.x = 100;
    position.y = 100;
    int time=0;
    Color collisionColor = GREEN;
    
    arr_slidelaser[0].active = false;
    arr_slidelaser[0].state = 0;
    arr_slidelaser[0].position = (Vector2){160,530};
    arr_slidelaser[0].w = 16;
    arr_slidelaser[0].h = 10;
    arr_slidelaser[0].inc.x = 3;
    arr_slidelaser[0].incdec.x = 0.02;
    
    myplayer.active = true;
    myplayer.w = 16;
    myplayer.h = 24;
    myplayer.position = (Vector2){100,516};
    myplayer.direction = 2;
    
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        time++;
        if(time>30){ // every time time is at 300
            time=0;//reset time
            position.x = GetRandomValue(32,screenWidth-32);
            position.y = GetRandomValue(32,screenHeight-32);
        }
        
        if(recttilecollide(position.x,position.y,tileWidth/2, tileHeight/2, 0,0)){
            collisionColor = RED;
        }else{
            collisionColor = GREEN;
        }

        // Update the slidelasers
        for(int i=0;i<MAX_SLIDEBOMBS;i++){
            if(arr_slidelaser[i].active==false)continue;
            if(arr_slidelaser[i].state==0){
                arr_slidelaser[i].inc.x-=arr_slidelaser[i].incdec.x;
                if((arr_slidelaser[i].inc.x>-0.2 && arr_slidelaser[i].inc.x<0.2) || recttilecollide(arr_slidelaser[i].position.x,arr_slidelaser[i].position.y,arr_slidelaser[i].w,arr_slidelaser[i].h,0,0)){
                    arr_slidelaser[i].state = 1;
                    arr_slidelaser[i].inc.x = 0;                    
                    // find ceiling(top of the laser beam;)
                    int ceilingloc;
                    for(int y=arr_slidelaser[i].position.y;y>0;y--){
                        if(recttilecollide(arr_slidelaser[i].position.x+4,y,1,1,0,0)){
                            ceilingloc=y+1;
                            break;
                        }
                    }
                    arr_slidelaser[i].ceilingloc = ceilingloc;

                }
                arr_slidelaser[i].position.x+=arr_slidelaser[i].inc.x;
            }
            
        }


        // Update the player..
        if(myplayer.active==true){
            Vector2 oldpos = myplayer.position;
            int fast=1;
            if(IsKeyDown(KEY_LEFT_SHIFT)){
                fast++;
            }
            if(IsKeyDown(KEY_RIGHT)){
                myplayer.position.x += fast;
                myplayer.direction = 2;
            }
            if(IsKeyDown(KEY_LEFT)){
                myplayer.position.x -= fast;
                myplayer.direction = 1;
            }
            if(recttilecollide(myplayer.position.x,myplayer.position.y,myplayer.w,myplayer.h,0,0)){
                myplayer.position = oldpos;
            }
            if(IsKeyPressed(KEY_Z) && arr_slidelaser[0].active==false){ // Slide the laser weapon
                arr_slidelaser[0].active = true;
                arr_slidelaser[0].state = 0;
                arr_slidelaser[0].position.x = myplayer.position.x;
                arr_slidelaser[0].position.y = myplayer.position.y+arr_slidelaser[0].h+4;
                if(myplayer.direction==1){
                    arr_slidelaser[0].inc.x = -2*fast;
                }else{
                    arr_slidelaser[0].inc.x = 1.5f*fast;
                }
            }
        }
        
        //----------------------------------------------------------------------------------
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);
            
            // Draw map
            for (int y = 0; y< mapHeight ; y++)
            {
                for (int x = 0; x< mapWidth ; x++)
                {
                    if (myMap[y][x] == 1)
                    {
                        DrawRectangle(x*tileWidth,y*tileHeight,tileWidth,tileHeight,BLUE);
                    }
                }
            }
            
            DrawRectangle(position.x,position.y,tileWidth/2,tileHeight/2,collisionColor);
          

            // draw the player
            if(myplayer.active==true){
                DrawRectangle(myplayer.position.x,myplayer.position.y,myplayer.w,myplayer.h,RED);
            }
          
            // Draw the slidelasers
            for(int i=0;i<MAX_SLIDEBOMBS;i++){
                if(arr_slidelaser[i].active==false)continue;                
                DrawRectangle(arr_slidelaser[i].position.x,arr_slidelaser[i].position.y,arr_slidelaser[i].w,arr_slidelaser[i].h,GRAY);
                if(arr_slidelaser[i].state==1){
                    DrawRectangle(arr_slidelaser[i].position.x+3,arr_slidelaser[i].ceilingloc,arr_slidelaser[i].w-6,arr_slidelaser[i].position.y-arr_slidelaser[i].ceilingloc,YELLOW);
                }
            }
            
            // some screen info
            DrawText("Cursor Left and Right. Left Shift = Run. Z key is slide laser weapon.",2,2,22,BLACK);


        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;


}



//Unit collide with solid blocks true/false
bool recttilecollide(int x, int y,int w, int h, int offsetx,int offsety){
    int cx = (x+offsetx)/tileWidth;
    int cy = (y+offsety)/tileHeight;
    for(int y2=cy-1; y2<cy+2;y2++){//Note that the - and + are to be set differently with differently sized players
    for(int x2=cx-1; x2<cx+2;x2++){
        if(x2>=0 && x2<mapWidth && y2>=0 && y2<mapHeight){
            if(myMap[y2][x2] == 1){
                int x3 = (x2)*tileWidth;
                int y3 = (y2)*tileHeight;
                if(rectsoverlap(x+offsetx,y+offsety,w,h,x3,y3,tileWidth,tileHeight)){
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
