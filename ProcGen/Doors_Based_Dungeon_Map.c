
#include "raylib.h"
#include <math.h>

enum mapflag{mleft,mright,mup,mdown};


static int mapWidth = 50;
static int mapHeight = 50;
static float tileWidth;
static float tileHeight;

static int map[200][200];

static void generatemap(void);
static bool roomfits(int x,int y,int w,int h);
static void insertroom(int x,int y,int w,int h);
static void makeroom(int x,int y);

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib example.");
 
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------


    generatemap();
    
    tileWidth = abs((float)screenWidth/(float)mapWidth);
    tileHeight = abs((float)screenHeight/(float)mapHeight);

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        
        if(IsKeyReleased(KEY_SPACE) || (IsMouseButtonReleased(0))){
            int z=GetRandomValue(30,100);            
            mapWidth=z;
            mapHeight=z;
            tileWidth = abs((float)screenWidth/(float)mapWidth);
            tileHeight = abs((float)screenHeight/(float)mapHeight);
            
            generatemap();
        }
        //----------------------------------------------------------------------------------
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);
            // Draw the map..
            for(int y=0;y<mapHeight;y++){
            for(int x=0;x<mapWidth;x++){
                if(map[x][y]==1){ // Wall
                    DrawRectangle(x*tileWidth,y*tileHeight,tileWidth,tileHeight,BLACK);
                }
                if(map[x][y]==2){ // Floor
                    DrawRectangle(x*tileWidth,y*tileHeight,tileWidth,tileHeight,DARKGRAY);
                }
                if(map[x][y]==3){ // Door
                    DrawRectangle(x*tileWidth,y*tileHeight,tileWidth,tileHeight,RED);
                }

            }    
            }
            DrawText("Press Space or Left Mouse Button for new map.",0,0,30,RED);
            
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;

}



void generatemap(){
    
    // erase old map;
    for(int y=0;y<mapHeight;y++){
    for(int x=0;x<mapWidth;x++){
        map[x][y] = 0;
    }}        
    map[mapWidth/2][mapHeight/2]=3;
    //
    int timeout=0;
    while (timeout<(mapWidth*mapHeight)*20){
        timeout+=1;
        int x=GetRandomValue(11,mapWidth-11);
        int y=GetRandomValue(11,mapHeight-11);
        if(map[x][y] == 3){
            makeroom(x,y);
        }
        if(timeout>12345612)return;
    }
    //'here we turn doors into walls
    //'if they should be walls
    
    for(int y1=1;y1<mapHeight-1;y1++){
    for(int x1=1;x1<mapWidth-1;x1++){
        if(map[x1][y1] == 3){
            int cnt=0;
            for(int y2=y1-1;y2<y1+1;y2++){
            for(int x2=x1-1;x2<x1+1;x2++){
                if(map[x2][y2] == 2)cnt+=1;
            }}
            if(cnt>3)map[x1][y1] = 2;
        }
    }}

   //here we turn doors into walls if it is blocked
    for(int y1=1;y1<=mapHeight-1;y1++){
    for(int x1=1;x1<=mapWidth-1;x1++){
        if(map[x1][y1] == 3){
            int cnt=0;
            for(int y2=y1-1;y2<=y1+1;y2++){
            for(int x2=x1-1;x2<=x1+1;x2++){
                if(map[x2][y2]==2)cnt+=2;
            }}
            if(cnt>6)map[x1][y1]=2;
        }       
    }}
    //'here we turn doors into walls if they
    //' touch tiles that are nothing (0)
    for(int y1=1;y1<=mapHeight-1;y1++){
    for(int x1=1;x1<=mapWidth-1;x1++){
        if(map[x1][y1] == 3){
            int cnt=0;
            for(int y2=y1-1;y2<=y1+1;y2++){
            for(int x2=x1-1;x2<=x1+1;x2++){
                if(map[x2][y2] == 0)cnt+=1;
            }}
            if(cnt>0)map[x1][y1] = 2;
        }
    }}
    //'here we turn the doors into floors
    
   for(int y1=0;y1<=mapHeight;y1++){
   for(int x1=0;x1<=mapWidth;x1++){
        if(map[x1][y1] == 3)map[x1][y1] = 1;
   }}

}
    
bool roomfits(int x,int y,int w,int h){
    for(int y1=y;y1<=y+h;y1++){
    for(int x1=x;x1<=x+w;x1++){
        if(x1<0 || x1>=mapWidth || y1<0 || y1>=mapHeight)return false;
        if(map[x1][y1] == 1)return false;
    }
    }
    return true;
}
void insertroom(int x,int y,int w,int h){
    for(int y2=y;y2<y+h;y2++){
    for(int x2=x;x2<x+w;x2++){
        if(x2<0 || x2>=mapWidth || y2<0 || y2>=mapHeight)return;
        if( map[x2][y2] != 3 )map[x2][y2] = 2;
    }}
    
    for(int y2=y+1;y2<y+h-1;y2++){
    for(int x2=x+1;x2<x+w-1;x2++){
        if(x2<0 || x2>=mapWidth || y2<0 || y2>=mapHeight)return;
        map[x2][y2] = 1;
    }}
}

void makeroom(int x,int y){
    int side=0;
    if(map[x][y-1] == 0){
        side=mup;
    }else if(map[x+1][y] == 0){
        side=mright;
    }else if(map[x][y+1] == 0){
        side=mdown;
    }else if(map[x-1][y] == 0){
        side=mleft;
    }	
    int w=GetRandomValue(5,10);
    int h=GetRandomValue(5,10);
    // Sometimes create a big room
    if(GetRandomValue(0,20)<2){
        w*=2;
        h*=2;
        // sometimes it is wider or higher
        if(GetRandomValue(0,5)==1){
            if(GetRandomValue(0,1)==1){
                w/=2;
            }else{
                h/=2;
            }
        }
    }
    // Here we create the room based on where the door is.
    if(side==mup){
        int x1=x-w/2;
        int y1=y-h;
        if(roomfits(x1,y1,w,h)){
            insertroom(x1,y1,w,h+1);
            //'door up
            map[x1+GetRandomValue(2,w-3)][y1] = 3;
            //' door right
            map[x1+w-1][y1+GetRandomValue(2,h-3)] = 3;
            //'door left
            map[x1][y1+GetRandomValue(2,h-3)] = 3;
        }
        
    }
    if(side==mright){
        int x1=x+1;
        int y1=y-h/2;
        if(roomfits(x1,y1,w,h)){
            insertroom(x1-1,y1,w,h);
            //'door up
            map[x1+GetRandomValue(2,w-3)][y1] = 3;
            //'door down
            map[x1+GetRandomValue(2,w-3)][y1+h-1] = 3;
            //' door right
            map[x1+w-2][y1+GetRandomValue(2,h-3)] = 3;
        }
    }
    if(side==mleft){
        int x1=x-w;
        int y1=y-h/2;
        if(roomfits(x1,y1,w,h)){
            insertroom(x1,y1,w+1,h);
            //'door up
            map[x1+GetRandomValue(2,w-3)][y1] = 3;
            //'door down
            map[x1+GetRandomValue(2,w-3)][y1+h-1] = 3;
            //'door left
            map[x1][y1+GetRandomValue(2,h-3)] = 3;

        }
    }
    if(side==mdown){
        int x1=x-w/2;
        int y1=y+1;
        if(roomfits(x1,y1,w,h)){
            insertroom(x1,y1-1,w,h);
            //'door down
            map[x1+GetRandomValue(2,w-3)][y1+h-2] = 3;
            //'door left
            map[x1][y1+GetRandomValue(2,h-3)] = 3;		
            //' door right
            map[x1+w-1][y1+GetRandomValue(2,h-3)] = 3;

        }	
    }

}
