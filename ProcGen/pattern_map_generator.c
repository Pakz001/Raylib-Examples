//
// Pattern map generator.
//

#define MAX_POINT 124

#include "raylib.h"
#include <math.h>

enum flag{UP,DOWN,LEFT,RIGHT};

static int mapWidth=80;
static int mapHeight=80;
static float tileWidth;
static float tileHeight;
static int map[200][200];

typedef struct point{
    bool active;
    Vector2 position;
}point;

static point arr_point[MAX_POINT];

static void generate(void);

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
    //--------------------------------+------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        
        if(IsKeyReleased(KEY_SPACE))generate();
        
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

            }}
            
            DrawText("Press Space..",0,0,20,DARKGRAY);

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
    static int script[100];//= {RIGHT,RIGHT,DOWN,RIGHT,DOWN,DOWN,LEFT,LEFT,LEFT,UP,UP,UP};
    for(int i=0;i<100;i++){
        script[i]=GetRandomValue(0,3);
    }
    int dunlen=GetRandomValue(11,30);
    for(int i=0;i<MAX_POINT;i++){
        arr_point[i].active=false;
    }
    for(int y=0;y<mapHeight;y++){
    for(int x=0;x<mapWidth;x++){
        map[x][y]=0;
    }}
    
    int num=0;
    int x=mapWidth/2;
    int y=mapHeight/2;
    arr_point[num].active = true;
    arr_point[num].position = (Vector2){x,y};
    map[x][y]=1;
    for(int i=0;i<dunlen;i++){
        switch (script[i]){
            case RIGHT:
                x+=7;
                if(x>mapWidth-5)x-=7;
            break;
            case LEFT:
                x-=7;
                if(x<5)x+=7;
            break;
            case UP:
                y-=7;
                if(y<5)y+=7;
            break;
            case DOWN:
                y+=7;
                if(y<mapHeight-5)y-=7;
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
        //if(i==11){
        //    x2 = arr_point[0].position.x;
        //    y2 = arr_point[0].position.y;
        //}
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
