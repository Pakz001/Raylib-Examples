// 
// Line of sight or raycasting into every direction of player.

// The method I used is by taking an area around the player. In this area each tile is indexed. Than
// a invisible bullet is shot from the player position into the direction of this tile. The area underneath
// this invisible bullet is marked as visible area. If the bullet hits a wall then the line of sight stops.
// I think this method will make sure that every tile is handled!
//
// My previous method I just shot a x amount of rays into every direction which I think might be less
// efficient.

// Controls are : cursors up down left right.


#include "raylib.h"
#include <math.h>

typedef struct player{
    Vector2 position;
}player;

float tilewidth;
float tileheight;

// This is out collision map.
bool colmap[10][20] = {false};
bool losmap[10][20] = {false};
// This is our tile map.
int map[10][20] = { {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
                    {1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,1,1,1,1},
                    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1},
                    {1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,1,1,1,1},
                    {1,1,1,1,1,1,1,1,0,1,1,1,1,0,0,1,1,1,1,1},
                    {1,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,1},
                    {1,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,1},
                    {1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,1},
                    {1,0,0,0,0,0,0,1,0,0,0,1,1,1,1,0,0,0,0,1},
                    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}};    

    // Our player setup
    player myplayer = {0};

static void makelos(void);
static float getangle(float x1,float y1,float x2,float y2);


int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib example.");

    myplayer.position = (Vector2){5,5};

    // Our tile width and height. We use the screen dimension and the map dimension to 
    // get tile dimensions that fill up the entire screen.
    tilewidth = (float)screenWidth/20.0f;
    tileheight = (float)screenHeight/10.0f;

    // Copy the contents of the map into the collision map. 
    for(int y=0;y<10;y++){
        for(int x=0;x<20;x++){
            if(map[y][x]>0)colmap[y][x]=true;
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
        if(IsKeyPressed(KEY_RIGHT)&& myplayer.position.x+1<20 && colmap[(int)myplayer.position.y][(int)myplayer.position.x+1]==false) {
            myplayer.position.x+=1;
            makelos();
        }
        if(IsKeyPressed(KEY_LEFT) && myplayer.position.x-1>-1 && colmap[(int)myplayer.position.y][(int)myplayer.position.x-1]==false) {
            myplayer.position.x-=1;
            makelos();
        }
        if(IsKeyPressed(KEY_UP)&& myplayer.position.y-1>-1 && colmap[(int)myplayer.position.y-1][(int)myplayer.position.x]==false) {
            myplayer.position.y-=1;
            makelos();
        }
        if(IsKeyPressed(KEY_DOWN) && myplayer.position.y+1<20 && colmap[(int)myplayer.position.y+1][(int)myplayer.position.x]==false) {
            myplayer.position.y+=1;
            makelos();
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
                        DrawRectangle(x*tilewidth,y*tileheight,tilewidth,tileheight,BLACK);
                    }
                    if(map[y][x]==2){
                        DrawRectangle(x*tilewidth,y*tileheight,tilewidth,tileheight,YELLOW);
                    }
                    if(map[y][x]==3){
                        DrawRectangle(x*tilewidth,y*tileheight,tilewidth,tileheight,BLUE);
                    }

                }
            }
            // Draw our player
            DrawRectangle(myplayer.position.x*tilewidth,myplayer.position.y*tileheight,tilewidth,tileheight,RED);
            
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

void makelos(){
    for(int y=0;y<10;y++){
        for(int x=0;x<20;x++){
        if(map[y][x]!=1)map[y][x]=0;
    }}
    float x1 = myplayer.position.y;
    float y1 = myplayer.position.x;
    for(int y=y1-8;y<y1+8;y++){
        for(int x=x1-8;x<x1+8;x++){
            float x4 = x1*tilewidth+tilewidth/2;
            float y4 = y1*tileheight+tileheight/2;
            float x2=(float)x*tilewidth;
            float y2=(float)y*tileheight;
            float angle = getangle(x2,y2,x4,y4);
            bool donot=false;
            for(int z=0;z<200;z++){
                x4+=cos(angle)*1;
                y4+=sin(angle)*1;
                int x3 = x4/tilewidth;
                int y3 = y4/tileheight;
                if(x3!=x1 || y3!=y1){
                    if(map[x3][y3]==1)donot=true;
                    
                    if(donot==false){
                    if(x3>-1 && y3>-1){
                    if(x3<10 && y3<20){
                    
                    
                    map[x3][y3]=GetRandomValue(2,3);
                    
                    }}}
                }                    
            }
            
    }}
};

// Return the angle from - to in float
float getangle(float x1,float y1,float x2,float y2){
    return (float)atan2(y2-y1, x2-x1);
}
