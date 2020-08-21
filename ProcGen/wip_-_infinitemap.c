//
// Trying to figure out how to do larger terrain by generating at the edges - work in progress
//
//



#include "raylib.h"
#include <math.h>

static int map[600][600];

static int mapWidth = 600;
static int mapHeight = 600;
static float tileWidth;
static float tileHeight;
static const int screenWidth = 640;
static const int screenHeight = 480;

static void makemap(int px, int py);
static void drawmap();
static void genmap();

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------

    InitWindow(screenWidth, screenHeight, "raylib example.");

    // We want to have the tile width and height fill the entire screen.
    tileWidth =  1;//ceil((float)screenWidth / (float)mapWidth);
    tileHeight = 1;//ceil((float)screenHeight / (float)mapHeight);
    genmap();

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        if(IsKeyDown(KEY_SPACE)){
            genmap();
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

static void makemap(int px,int py){


    // First we create a couple of hills in the map
    // We create a random spot and draw a line there somewhat randomly
    // We create a line of some sort to create more realistic landmasses. :Experiment:
    int rx,ry;
    for(int i=0;i<40/3;i++){
        rx = GetRandomValue(-10,60-5);
        ry = GetRandomValue(-10,60-5);
        int mx=GetRandomValue(-1,1);
        int my=GetRandomValue(-1,1);
        int len=GetRandomValue(1,50/4);
        for(int ii=0;ii<len;ii++){
            if(GetRandomValue(0,10)<2){
                mx=GetRandomValue(-1,1);
                my=GetRandomValue(-1,1);
            }
            int nx=rx+mx;
            int ny=ry+my;
            
            //if(nx>0&&nx<55-1 && ny>0 && ny<55-1){
                map[nx+px][ny+py] = 1;
            //}
        }
        
    }
    // Now we want to pile sand ontop and right next to the current hills.
    // This will make them into larger hills. We do this by picking a random
    // spot. If there is something below that we increase the height there.
    int clunk=(50*50)*GetRandomValue(2,3);
    for(int i=0;i<clunk;i++){
        int nx,ny;
        nx = GetRandomValue(-20,70-2);
        ny = GetRandomValue(-20,70-2);
        if(map[nx+px][ny+py]>0){
            for(int y=-1;y<2;y++){
                for(int x=-1;x<2;x++){
                    map[nx+x+px][ny+y+py]++;
                    
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

static void genmap(){
    // Here we generate the map using the hill algorithm
    for(int y=0;y<mapHeight;y++){
    for(int x=0;x<mapWidth;x++){
        map[x][y]=0;
    }}

    for(int y=1;y<7;y++){
    for(int x=1;x<8;x++){
        makemap(x*50,y*50);
    }}
};
