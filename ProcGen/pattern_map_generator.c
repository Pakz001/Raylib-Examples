//
// Pattern map generator.
//

#define MAX_POINT 124

#include "raylib.h"
#include <math.h>

enum flag{UP,DOWN,LEFT,RIGHT};

static int screenWidth = 800;
static int screenHeight = 450;
static int mapWidth=80;
static int mapHeight=80;
static float tileWidth;
static float tileHeight;
static int map[512][512];

typedef struct point{
    bool active;
    Vector2 position;
}point;

static point arr_point[MAX_POINT];

static void generate(void); // generate the map.
static void makenice(); // add walls.

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    tileWidth = abs((float)screenWidth/(float)mapWidth);
    tileHeight = abs((float)screenHeight/(float)mapHeight);

    InitWindow(screenWidth, screenHeight, "raylib example.");

    generate();
    makenice();
 
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------+------------------------------------------------------
    static int time=0;
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        
        if(IsKeyReleased(KEY_SPACE)){
            generate();
            makenice();
        }
        time++;
        if(time>100){
            time=0;
            generate();
            makenice();
        }
        //----------------------------------------------------------------------------------
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);
            // Draw the map
            for(int y=0;y<mapHeight;y++){
            for(int x=0;x<mapWidth;x++){
                if(map[x][y]==1){
                    DrawRectangle(x*tileWidth,y*tileHeight,tileWidth,tileHeight,BLACK);
                }
                if(map[x][y]==2){
                    DrawRectangle(x*tileWidth,y*tileHeight,tileWidth,tileHeight,RED);
                }
                if(map[x][y]==3){
                    DrawRectangle(x*tileWidth,y*tileHeight,tileWidth,tileHeight,BLUE);
                }
                if(map[x][y]==4){
                    DrawRectangle(x*tileWidth,y*tileHeight,tileWidth,tileHeight,YELLOW);
                }


            }}
            
            DrawRectangle(0,0,screenWidth,20,(Color){50,50,50,150});
            DrawText("Press Space to generate new map..(Or wait)",0,0,20,RAYWHITE);

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
    mapWidth=80;
    mapHeight=80;
    static int script[100];//= {RIGHT,RIGHT,DOWN,RIGHT,DOWN,DOWN,LEFT,LEFT,LEFT,UP,UP,UP};
    for(int i=0;i<100;i++){
        script[i]=GetRandomValue(0,3);
    }
    int dunlen=GetRandomValue(11,30);
    for(int i=0;i<MAX_POINT;i++){
        arr_point[i].active=false;
    }
    for(int y=0;y<512;y++){
    for(int x=0;x<512;x++){
        map[x][y]=0;
    }}
    
    int num=0;
    int x=mapWidth/2;
    int y=mapHeight/2;
    arr_point[num].active = true;
    arr_point[num].position = (Vector2){x,y};
    map[x][y]=1;
    for(int i=0;i<dunlen-1;i++){
        switch (script[i]){
            case RIGHT:
                x+=7;
                if(x>mapWidth-6)x-=7;
            break;
            case LEFT:
                x-=7;
                if(x<6)x+=7;
            break;
            case UP:
                y-=7;
                if(y<6)y+=7;
            break;
            case DOWN:
                y+=7;
                if(y<mapHeight-6)y-=7;
            break;
        }
        num++;
        map[x][y] = 1;
        arr_point[num].active = true;
        arr_point[num].position.x = x;
        arr_point[num].position.y = y;
    }
    for(int i=0;i<dunlen;i++){
        if(arr_point[i].active==false)continue;
        int x=arr_point[i].position.x;
        int y=arr_point[i].position.y;
        int offx=-2;
        int offy=-2;
        int w=GetRandomValue(4,6);
        int h=GetRandomValue(4,6);
        for(int y2=y+offy;y2<y+offy+h;y2++){
        for(int x2=x+offx;x2<x+offx+w;x2++){
            if(x2<0 || y2<0 || x2>mapWidth-1 || y2>mapHeight-1)continue;
            map[x2][y2]=1;
        }}
    }
    for(int i=0;i<dunlen-1;i++){
        int x1 = arr_point[i].position.x;
        int y1 = arr_point[i].position.y;
        int x2 = arr_point[i+1].position.x;
        int y2 = arr_point[i+1].position.y;
        if(x2>x1){
            while(x1<x2){
                x1++;
                if(x1<0 || y1<0 || x1>mapWidth-1 || y1>mapHeight-1)continue;
                map[x1][y1]=2;
            }
        }
        if(x1>x2){
            while(x1>x2){
                x1--;
                if(x1<0 || y1<0 || x1>mapWidth-1 || y1>mapHeight-1)continue;
                map[x1][y1]=2;
            }
        
        }
        if(y2>y1){
            while(y1<y2){
                y1++;
                if(x1<0 || y1<0 || x1>mapWidth-1 || y1>mapHeight-1)continue;
                map[x1][y1]=2;
            }
        }
        if(y1>y2){
            while(y1>y2){
                y1--;
                if(x1<0 || y1<0 || x1>mapWidth-1 || y1>mapHeight-1)continue;
                map[x1][y1]=2;
            }
        }
    }
 }
 
 void makenice(){
     // add walls to rooms
     for(int y=1;y<mapHeight-1;y++){
     for(int x=1;x<mapWidth-1;x++){
        int cnt=0;
        if(map[x][y]==1 || map[x][y]==2)continue;
        
        for(int y2=y-1;y2<y+2;y2++){
        for(int x2=x-1;x2<x+2;x2++){
            if(map[x2][y2]==1)cnt++;
        }
        }
        if(cnt>0)map[x][y]=3;
     }
     }
     // add walls to halls.
     for(int y=1;y<mapHeight-1;y++){
     for(int x=1;x<mapWidth-1;x++){
        int cnt=0;
        if(map[x][y]==0){
        
        for(int y2=y-1;y2<y+2;y2++){
        for(int x2=x-1;x2<x+2;x2++){
            if(map[x2][y2]==2)cnt++;
        }
        }
        if(cnt>0)map[x][y]=3;
        }
     }
     }
     // turn door lines into floor
     for(int y=1;y<mapHeight-1;y++){
     for(int x=1;x<mapWidth-1;x++){
         if(map[x][y]==2)map[x][y]=1;
     }}
     
     //find top most
     int top=0;
     for(int y=0;y<mapHeight;y++){
     bool ex=false;
     for(int x=0;x<mapWidth;x++){
         if(map[x][y]>0){
            top=y;
            ex=true;
            break;
         }
     }
     if(ex)break;
     }
     
     
     //find left most
     int left=0;
     for(int x=0;x<mapWidth;x++){
     bool ex=false;
     for(int y=0;y<mapHeight;y++){          
         if(map[x][y]>0){
            left=x;
            ex=true;
            break;
         }
     }
     if(ex)break;
     }


    // Copy map into buffer map from where it begins on the map top and left
    // and then copy this all back to the main map.
    int m2[200][200];
    for(int y=0;y<mapHeight;y++){
    for(int x=0;x<mapWidth;x++){
        m2[x][y]=map[x+left][y+top];
    }}
    for(int y=0;y<512;y++){//erase map
    for(int x=0;x<512;x++){
        map[x][y]=0;
    }}
    for(int y=0;y<mapHeight;y++){//copy temp back into main map at left top
    for(int x=0;x<mapWidth;x++){
        map[x][y] = m2[x][y];
    }}
 
    // find right part
    int right=0;
    for(int x=mapWidth-1;x>0;x--){
    bool ex=false;
    for(int y=0;y<mapHeight;y++){
        if(map[x][y]>0){
            right = x;
            ex=true;
            break;
        }
    }
    if(ex)break;
    }
    // find most bottom part
    int bottom=0;
    for(int y=mapHeight-1;y>0;y--){
    bool ex=false;
    for(int x=0;x<mapWidth;x++){        
        if(map[x][y]>0){
            bottom = y;
            ex=true;
            break;
        }
    }
    if(ex)break;
    }
 
    mapWidth = right+1;
    mapHeight = bottom+1;
    tileWidth = abs((float)screenWidth/(float)mapWidth);
    tileHeight = abs((float)screenHeight/(float)mapHeight);
  
 
 }
