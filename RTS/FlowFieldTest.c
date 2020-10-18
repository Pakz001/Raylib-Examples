//
// FLOW fields - work in progress
//  

#define MAX_AGENTS 50

#include "raylib.h"
#include <math.h>

// This is our tile map.
int map[10][20] = { {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
                    {1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,1,1,1,1},
                    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1},
                    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                    {1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,1},
                    {1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,1},
                    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}};    

static float tileWidth;
static float tileHeight;
static int mapWidth;
static int mapHeight;

typedef struct player{
    Vector2 position;
    int width;
    int height;
}player;

static player myplayer = {0};

static float flowfieldmap[10][20] = {0};

typedef struct agent{
    Vector2 position;
    int width;
    int height;
    Vector2 target;
    float speed;
    int pausetime;
}agent;

static struct agent arr_agent[MAX_AGENTS];

// uses the myplayer.position coordinates.
// If offsetx is 1 then it checks if the player x position PLus 1 is colliding with a tile.
static bool playertilecollide(int offsetx,int offsety);
static bool rectsoverlap(int x1,int y1,int w1,int h1,int x2,int y2,int w2,int h2);
static float getangle(float x1,float y1,float x2,float y2);



int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;
    mapWidth = 20;
    mapHeight = 10;
    // Our tile width and height. We use the screen dimension and the map dimension to 
    // get tile dimensions that fill up the entire screen.
    tileWidth = (float)screenWidth/(float)mapWidth;
    tileHeight = (float)screenHeight/(float)mapHeight;


    InitWindow(screenWidth, screenHeight, "raylib example.");


    // Our player setup    
    myplayer.position = (Vector2){4*tileWidth,6*tileHeight};
    myplayer.width = tileWidth/2;
    myplayer.height = tileHeight/2;

    for(int y=0;y<mapHeight;y++){
    for(int x=0;x<mapWidth;x++){
        flowfieldmap[y][x]=GetRandomValue(0,3);
    }
    }

    // add units to the screen
    int y=0;
    int x=0;
    int sq=0;
    for(int i=0;i<MAX_AGENTS;i++){
        
        
        arr_agent[i].position = (Vector2){100+x,100+y};
        arr_agent[i].width = 10;
        arr_agent[i].height = 10;
        arr_agent[i].speed = 1.0f;
        arr_agent[i].target = (Vector2){400,200};
        arr_agent[i].pausetime = 0;
        x+=12;
        sq++;
        if(sq == 4){
            sq=0;
            y+=12;
            x=0;
        }
    }

    
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        
        // Below here we read the key. If the l/r/u/d cursor is pressed then we check the new position is inside
        // the array bounds and then we check if the collision map on the new position is not set to flag true on that tile.
        if(IsKeyDown(KEY_RIGHT)&& myplayer.position.x+1<(mapWidth-1)*tileWidth){
            if(playertilecollide(1,0)==false){
                myplayer.position.x+=1;
            }
        }
        if(IsKeyDown(KEY_LEFT) && myplayer.position.x-1>-1){
            if(playertilecollide(-1,0)==false){
                myplayer.position.x-=1;
            }
        }
        if(IsKeyDown(KEY_UP)&& myplayer.position.y-1>-1){
            if(playertilecollide(0,-1)==false){
                myplayer.position.y-=1;
            }
        }
        if(IsKeyDown(KEY_DOWN) && myplayer.position.y+1<(mapHeight-1)*tileHeight){
            if(playertilecollide(0,1)==false){
                myplayer.position.y+=1;
            }
        }
        
        if(IsMouseButtonPressed(0)){
            for(int i=0;i<MAX_AGENTS;i++){
                int posx = GetMouseX()/tileWidth;
                int posy = GetMouseY()/tileHeight;
                //if(map[posy][posx]==0){
                    arr_agent[i].target = GetMousePosition();
                //}
            }
        }

        //reset flowfieldmap
        if(GetRandomValue(0,20)==1){
        for(int y=0;y<mapHeight;y++){
        for(int x=0;x<mapWidth;x++){
            flowfieldmap[y][x]=0;
        }}
        }
        // Move the units.
        for(int i=0;i<MAX_AGENTS;i++){
            if(arr_agent[i].pausetime>0){
                arr_agent[i].pausetime--;                
            }
            if(arr_agent[i].pausetime>0)continue;
            float angle = getangle(arr_agent[i].position.x,arr_agent[i].position.y,arr_agent[i].target.x,arr_agent[i].target.y);
            Vector2 oldPos = arr_agent[i].position;
            int posx=arr_agent[i].position.x/tileWidth;
            int posy=arr_agent[i].position.y/tileHeight;
            
            arr_agent[i].position.x+= cos(angle);
            arr_agent[i].position.y+= sin(angle);
            if(flowfieldmap[posy][posx]!=0){
            arr_agent[i].position.x+= cos(flowfieldmap[posy][posx])/1.5;
            arr_agent[i].position.y+= sin(flowfieldmap[posy][posx])/1.5;
            }
            for(int j=0;j<MAX_AGENTS;j++){
                if(j==i)continue;
                if(rectsoverlap(arr_agent[i].position.x,arr_agent[i].position.y,arr_agent[i].width,arr_agent[i].height,
                                arr_agent[j].position.x,arr_agent[j].position.y,arr_agent[j].width,arr_agent[j].height)){
                    arr_agent[i].position = oldPos;
                    arr_agent[i].pausetime = GetRandomValue(0,5);
                    float an2=getangle(arr_agent[i].position.x,arr_agent[i].position.y,arr_agent[j].position.x,arr_agent[j].position.y);
                    int px = arr_agent[j].position.x/tileWidth;
                    int py = arr_agent[j].position.y/tileHeight;
                    flowfieldmap[py][px]=an2+GetRandomValue(-1.5f,1.5f);
                }
            }
        }

        //----------------------------------------------------------------------------------
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);
                
            // Draw our tilemap.
            for(int y=0;y<10;y++){
                for(int x=0;x<20;x++){
                    if(map[y][x]==1){
                        DrawRectangle(x*tileWidth,y*tileHeight,tileWidth,tileHeight,BLACK);
                    }
                }
            }
            
            // Draw out agents
            for(int i=0;i<MAX_AGENTS;i++){
                DrawRectangle(arr_agent[i].position.x,arr_agent[i].position.y,arr_agent[i].width,arr_agent[i].height,BLUE);
            }
            
            
            // Draw our player
            DrawRectangle(myplayer.position.x,myplayer.position.y,myplayer.width,myplayer.height,RED);
            
            DrawText("Use Cursor Keys to move around..",0,0,20,WHITE);
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
bool playertilecollide(int offsetx,int offsety){
    int cx = (myplayer.position.x+offsetx)/tileWidth;
    int cy = (myplayer.position.y+offsety)/tileHeight;
    for(int y2=cy-1; y2<cy+2;y2++){//Note that the - and + are to be set differently with differently sized players
    for(int x2=cx-1; x2<cx+2;x2++){
        if(x2>=0 && x2<mapWidth && y2>=0 && y2<mapHeight){
            if(map[y2][x2] == 1){
                int x3 = (x2)*tileWidth;
                int y3 = (y2)*tileHeight;
                if(rectsoverlap(myplayer.position.x+offsetx,myplayer.position.y+offsety,myplayer.width,myplayer.height,x3,y3,tileWidth,tileHeight)){
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

// Return the angle from - to in float
float getangle(float x1,float y1,float x2,float y2){
    return (float)atan2(y2-y1, x2-x1);
}

