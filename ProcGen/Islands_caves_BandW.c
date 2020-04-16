// create a number of points on the map. Select a random spot. neighboor to
// closest point or random direction.


#include "raylib.h"
#include <math.h>

static int mapWidth = 100;
static int mapHeight = 100;
static float tileWidth;
static float tileHeight;
static int map[100][100];

static void generate();
static float distance(float x1,float y1,float x2,float y2);

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    tileWidth = abs((float)screenWidth/(float)mapWidth);
    tileHeight = abs((float)screenHeight/(float)mapHeight);

    InitWindow(screenWidth, screenHeight, "raylib example.");
 
    generate();
 
    
 
 
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        
        if(IsKeyPressed(KEY_SPACE) || IsMouseButtonPressed(0))generate();
        
        //----------------------------------------------------------------------------------
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);
            
            for(int y=0;y<mapHeight;y++){
            for(int x=0;x<mapWidth;x++){
                if(map[x][y]==1){
                    DrawRectangle(x*tileWidth,y*tileHeight,tileWidth,tileHeight,BLACK);
                }
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

void generate(){
    // We generate our map here.
    for(int y=0;y<mapHeight;y++){
    for(int x=0;x<mapWidth;x++){
        map[x][y]=0;
    }}
    
    // First put some random points on the map
    for(int i=0;i<(mapWidth*mapHeight)/100;i++){
        map[GetRandomValue(0,mapWidth-1)][GetRandomValue(0,mapHeight-1)] = 1;
    }
    // Go connect and expand.
    for(int i=0;i<((mapWidth*mapHeight)*40);i++){
        int x = GetRandomValue(0,mapWidth-1);
        int y = GetRandomValue(0,mapHeight-1);
        if(map[x][y]==0)continue;
        int dir;
        int z=4;
        if(GetRandomValue(0,100)<15)z=GetRandomValue(0,3);
        switch (z){
            case 0:
            if(x-1>-1){
            if(map[x-1][y]==0){
                map[x-1][y]=1;
            }
            }
            break;
            case 1:
            if(x+1<mapWidth){
            if(map[x+1][y]==0){
                map[x+1][y]=1;
            }
            }
            break;
            case 2:
            if(y+1<mapHeight){
            if(map[x][y+1]==0){
                map[x][y+1]=1;
            }
            }
            break;
            case 3:
            if(y-1>-1){
            if(map[x][y-1]==0){
                map[x][y-1]=1;
            }
            }
            break;
            case 4:
            {
            dir=-1;
            int d=99999;            
            for(int y2=y-8;y2<y+8;y2++){
            for(int x2=x-8;x2<x+8;x2++){            
                if(x2<0 || y2<0 || x2>mapWidth-1 || y2>mapHeight-1)continue;
                if(map[x2][y2]==0)continue;
                int nd=distance(x,y,x2,y2);
                if(nd<d){
                    // if neighboor count empty is >3
                    int ncnt=0;
                    for(int y3=-1;y3<2;y3++){
                    for(int x3=-1;x3<2;x3++){
                        int x4=x2+x3;
                        int y4=y2+y3;
                        if(x4<0 || y4<0 || x4>mapWidth-1 || y4>mapHeight-1)continue;
                        if(map[x4][y4]==0)ncnt+=1;
                    }}
                    if(ncnt>6){
                        d=nd;
                        if(y2<y)dir=0;
                        if(y2>y)dir=2;
                        if(x2<x)dir=3;
                        if(x2>x)dir=1;
                    }
                }
            }
            }
            }
            break;
        }
        switch (dir){
        case 0:
            if(y-1>-1){
            if(map[x][y-1]==0){
                map[x][y-1]=1;
            }
            }
        
        break;
        case 1:
            if(x+1<mapWidth){
            if(map[x+1][y]==0){
                map[x+1][y]=1;
            }
            }

        break;
        case 2:
            if(y+1<mapHeight){
            if(map[x][y+1]==0){
                map[x][y+1]=1;
            }
            }

        break;
        case 3:
            if(x-1>-1){
            if(map[x-1][y]==0){
                map[x-1][y]=1;
            }
            }

        break;
        }
    }
 
 
}

// Manhattan Distance (less precise)
float distance(float x1,float y1,float x2,float y2){
    return (float)abs(x2-x1)+abs(y2-y1);
}

