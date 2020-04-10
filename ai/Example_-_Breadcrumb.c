
#define MAX_PATH 32

#include "raylib.h"
#include <math.h>

static int mapWidth = 20;
static int mapHeight = 10;
static float tileWidth;
static float tileHeight;

// This is our tile map. Note that [y][x]!!
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

typedef struct player{
    bool active;
    int x;
    int y;
    int pathx[MAX_PATH];
    int pathy[MAX_PATH];
}player;

static player p;
static void dropbreadcrumb(void);

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib example.");
 
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    p.active = true;
    p.x = 5;
    p.y = 5;
    for(int i=MAX_PATH-1;i>-1;i-=1){
        p.pathx[i]=-1;
        p.pathy[i]=-1;
    }
    dropbreadcrumb();
    tileWidth = abs((float)screenWidth/(float)mapWidth);
    tileHeight = abs((float)screenHeight/(float)mapHeight);

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        if(IsKeyReleased(KEY_UP) &&     map[p.y-1][p.x]==0){
            p.y-=1;
            dropbreadcrumb();
        }
        if(IsKeyReleased(KEY_DOWN) &&   map[p.y+1][p.x]==0){
            p.y+=1;
            dropbreadcrumb();
        }
        if(IsKeyReleased(KEY_LEFT) &&   map[p.y][p.x-1]==0){
            p.x-=1;
            dropbreadcrumb();
        }
        if(IsKeyReleased(KEY_RIGHT) &&  map[p.y][p.x+1]==0){
            p.x+=1;
            dropbreadcrumb();
        }
        //----------------------------------------------------------------------------------
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);
            
            //
            // Draw our tilemap.
            for(int y=0;y<10;y++){
                for(int x=0;x<20;x++){
                    if(map[y][x]==1){
                        DrawRectangle(x*tileWidth,y*tileHeight,tileWidth,tileHeight,BLACK);
                    }
                }
            }
            // Draw our breadcrumb
            for(int i=0;i<MAX_PATH;i++){
                if(p.pathx[i]>-1)
                DrawRectangle(  p.pathx[i]*tileWidth+tileWidth/4,p.pathy[i]*tileHeight+tileHeight/4,
                                tileWidth-tileWidth/2,tileHeight-tileHeight/2,(Color){50,50,50,255-(255.0f/32.0f)*i});
            }
            // Draw our player
            DrawRectangle(p.x*tileWidth,p.y*tileHeight,tileWidth,tileHeight,BLUE);
            
            //
            //
            DrawText("BreadCrumb Example. Use cursor keys to move around.",0,0,20,GRAY);
            
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;


}

void dropbreadcrumb(){
    // first shift the path one step down.
    for(int i=MAX_PATH-1;i>0;i--){
        p.pathx[i] = p.pathx[i-1];
        p.pathy[i] = p.pathy[i-1];
    }
    // insert the current position to the top of the path array.
    p.pathx[0] = p.x;
    p.pathy[0] = p.y;
}
