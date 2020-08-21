//
// Simple way to generate a map. I learned that this method is called the Hill ALgorithm.'
// You basically drop some sand on the map and enter a loop. Then pick random points
// and if on that spot is sand than drop sand on that spot and around it. This will
// grow that into a hill.
//
//



#include "raylib.h"
#include <math.h>

static int map[300][300];

static int mapWidth = 100;
static int mapHeight = 100;
static float tileWidth;
static float tileHeight;
static const int screenWidth = 640;
static const int screenHeight = 480;

static void makemap();
static void drawmap();

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------

    InitWindow(screenWidth, screenHeight, "raylib example.");

    // We want to have the tile width and height fill the entire screen.
    tileWidth =  ceil((float)screenWidth / (float)mapWidth);
    tileHeight = ceil((float)screenHeight / (float)mapHeight);
    // Here we generate the map using the hill algorithm
    makemap();
 
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        if(IsKeyReleased(KEY_SPACE)){
            makemap();
        }
        
        //----------------------------------------------------------------------------------
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);
            drawmap();  
            DrawRectangle(0,0,screenWidth,24,DARKGRAY);
            DrawText("Press Space to Generate new map",2,2,20,WHITE);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;


}

static void makemap(){

    for(int y=0;y<mapHeight;y++){
    for(int x=0;x<mapWidth;x++){
        map[x][y]=0;
    }}
        

    // First we create a couple of hills in the map
    // We create a random spot and draw a line there somewhat randomly
    int rx,ry;
    for(int i=0;i<mapWidth/3;i++){
        rx = GetRandomValue(5,mapWidth-5);
        ry = GetRandomValue(5,mapHeight-5);
        int mx=GetRandomValue(-1,1);
        int my=GetRandomValue(-1,1);
        int len=GetRandomValue(1,mapWidth/4);
        for(int ii=0;ii<len;ii++){
            if(GetRandomValue(0,10)<2){
                mx=GetRandomValue(-1,1);
                my=GetRandomValue(-1,1);
            }
            int nx=rx+mx;
            int ny=ry+my;
            
            if(nx>0&&nx<mapWidth-1 && ny>0 && ny<mapHeight-1){
                map[nx][ny] = 1;
            }
        }
        
    }
    // Now we want to pile sand ontop or right next to the current hills.
    // This will make them into larger hills. We do this by picking a random
    // spot. If there is something below that we increase the height there.
    // We also throw sand in the direct area around this spot.(+1)
    for(int i=0;i<(mapWidth*mapHeight)*2;i++){
        int nx,ny;
        nx = GetRandomValue(1,mapWidth-2);
        ny = GetRandomValue(1,mapHeight-2);
        if(map[nx][ny]>0){
            for(int y=-1;y<2;y++){
                for(int x=-1;x<2;x++){
                    map[nx+x][ny+y]++;
                    if(map[nx+x][ny+y]>100)map[nx+x][ny+y]=100;
                }
            }
        }
        
    }
};

static void drawmap(){
    for(int y=0;y<mapHeight;y++){
    for(int x=0;x<mapWidth;x++){
        if(map[x][y]==0){ // water
            DrawRectangle(x*tileWidth,y*tileHeight,tileWidth,tileHeight,BLUE);
        }        
        if(map[x][y]>0){
            if(map[x][y]>0){//Draw the beach
            DrawRectangle(x*tileWidth,y*tileHeight,tileWidth,tileHeight,(Color){250,220,0,255});
            }

            if(map[x][y]>3){//If the height is larger than 3 than draw trees
            DrawRectangle(x*tileWidth,y*tileHeight,tileWidth,tileHeight,(Color){0,210,0,255});
            }
            if(map[x][y]>15){ // here we draw some more trees
            DrawRectangle(x*tileWidth,y*tileHeight,tileWidth,tileHeight,(Color){0,170,0,255});
            }

            if(map[x][y]>20){ // draw mountains or rocks
            DrawRectangle(x*tileWidth,y*tileHeight,tileWidth,tileHeight,(Color){110,110,110,255});
            }

        }

    }    
    }    
};
