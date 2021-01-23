
#include "raylib.h"
#include <math.h>


int map[512][512];
int mapWidth = 128;
int mapHeight = 128;
int screenWidth;
int screenHeight;
float tileWidth;
float tileHeight;


void makerandommap();
    
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    screenWidth = 800;
    screenHeight = 450;
    tileWidth = ceil((float)screenWidth/(float)mapWidth);
    tileHeight = ceil((float)screenHeight/(float)mapHeight);

    InitWindow(screenWidth, screenHeight, "raylib example.");
    int d=GetRandomValue(-1,1);


 
 
 
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------
    
    int newmaptime=0;

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        newmaptime--;
        if(newmaptime<=0){
            makerandommap();
            newmaptime=60;
        }
        
        //----------------------------------------------------------------------------------
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(BLUE);
            
            for(int y=0;y<mapHeight;y++){
            for(int x=0;x<mapWidth;x++){
                if(map[y][x]>0 && map[y][x]<50){
                    DrawRectangle(x*tileWidth,y*tileHeight,tileWidth,tileHeight,(Color){map[y][x]+150,150,0,255});
                }
                if(map[y][x]>=50 && map[y][x]<200){
                    DrawRectangle(x*tileWidth,y*tileHeight,tileWidth,tileHeight,(Color){map[y][x],map[y][x],0,255});
                }
                if(map[y][x]>=200){
                    DrawRectangle(x*tileWidth,y*tileHeight,tileWidth,tileHeight,(Color){map[y][x],map[y][x],map[y][x],255});
                }
            }}

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;


}

void makerandommap(){    
    for(int y=0;y<mapHeight;y++){
    for(int x=0;x<mapWidth;x++){
        map[y][x]=0;
    }}
    // Create Drunken anty walk map
    int size=2000;
    Vector2 path[size+1];
    int pos=0;
    int px=50;
    int py=50;
    int npx=px;
    int npy=py;
    int lastx=0; // our last direction.
    int lasty=0;
    int currentx;
    int currenty;
    path[pos]=(Vector2){px,py};
    for(int len=0;len<size;len++){        
        currentx = GetRandomValue(-1,1);
        currenty = GetRandomValue(-1,1);
        // Keep running into same diretion..
        if(GetRandomValue(0,10)<5){
            currentx = lastx;
            currenty = lasty;
        }
        npx=px+currentx;
        npy=py+currenty;
        if(npx<0)npx=0;
        if(npy<0)npy=0;
        if(npx>=mapWidth-1)npx=mapWidth-1;
        if(npy>=mapHeight-1)npy=mapHeight-1;

        lastx = currentx;
        lasty = currenty;
        
        for(int cnt=0;cnt<6;cnt++){
            for(int i=0;i<pos;i++){               
                if((int)path[i].x == npx && (int)path[i].y==npy){
                    npx=px+GetRandomValue(-1,1);
                    npy=py+GetRandomValue(-1,1);   
                    if(npx<0)npx=0;
                    if(npy<0)npy=0;
                    if(npx>=mapWidth-1)npx=mapWidth-1;
                    if(npy>=mapHeight-1)npy=mapHeight-1;
                    break;
                }
            }
        }
        pos++;
        
        path[pos] = (Vector2){npx,npy};

        map[npy][npx]=1;
        px=npx;
        py=npy;
    }
 
    // Grow edges
    for(int i=0;i<mapWidth*30;i++){
        int x = GetRandomValue(2,mapWidth-3);
        int y = GetRandomValue(2,mapHeight-3);
        if(map[y][x]==1){
            for(int y1=y-1;y1<y+2;y1++){
            for(int x1=x-1;x1<x+2;x1++){
                map[y1][x1]=1;
            }}
        }
    }
 
    // raise land
    int modder=GetRandomValue(15,20);
    for(int i=0;i<(mapWidth*mapHeight)*modder;i++){
        int x = GetRandomValue(1,mapWidth-2);
        int y = GetRandomValue(1,mapHeight-2);
        if(map[y][x]>0){
            int h=0;
            for(int y1=y-1;y1<y+2;y1++){
            for(int x1=x-1;x1<x+2;x1++){
                if(map[y1][x1]>0)h++;
            }}
            if(h>5 && map[y][x]<240)map[y][x]+=h;
        }
        
    }
}    
 
