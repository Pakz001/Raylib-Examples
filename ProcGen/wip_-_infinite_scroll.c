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
static void drawmap2();
static void genmap();
static void edgemap();

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
    int scrolly=0;
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        if(IsKeyDown(KEY_SPACE)){
            genmap();
        }
        //if(IsKeyPressed(KEY_A)){

        for(int y=3;y<mapHeight;y++){
        for(int x=0;x<mapWidth;x++){
            map[x][y-3] = map[x][y];
        }    
        }

        scrolly++;
        if(scrolly>16){
            
            edgemap();
            scrolly=0;
        }
        //}
        //----------------------------------------------------------------------------------
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);
            drawmap2();  

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
    for(int i=0;i<40/4;i++){
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
              if(nx+px<mapWidth && nx+px>-1 &&  ny+py<mapHeight && ny+py>-1){
                map[nx+px][ny+py]++;
              }
            //}
        }
        
    }
    // Now we want to pile sand ontop and right next to the current hills.
    // This will make them into larger hills. We do this by picking a random
    // spot. If there is something below that we increase the height there.
    int clunk=(50*50)*5;//GetRandomValue(2,3);
    for(int i=0;i<clunk;i++){
        int nx,ny;
        nx = GetRandomValue(-100,100-2);
        ny = GetRandomValue(-100,100-2);
        if(map[nx+px][ny+py]>0){
            for(int y=-1;y<2;y++){
                for(int x=-1;x<2;x++){
                    if(nx+x+px>-1 && nx+x+px<mapWidth && ny+y+py>-1 && ny+y+py<mapHeight){
                    map[nx+x+px][ny+y+py]++;
                    }
                }
            }
        }
        
    }
    
    
};

static void drawmap2(){
    int tileWidth2 = tileWidth*2;
    int tileHeight2 = tileHeight*2;
    int mapWidth2 = mapWidth/2;
    int mapHeight2 = mapHeight/2;
    for(int y=0;y<mapHeight2;y++){
    for(int x=0;x<mapWidth2+20;x++){
        int value = map[x+96][y];
        if(value==0){ // water
            DrawRectangle(x*tileWidth2,y*tileHeight2,tileWidth2,tileHeight2,BLUE);
        }        
        if(value>0){
            if(value>0){//Draw the beach
            DrawRectangle(x*tileWidth2,y*tileHeight2,tileWidth2,tileHeight2,(Color){250,220,0,255});
            }

            if(value>3){//If the height is larger than 3 than draw trees
            DrawRectangle(x*tileWidth2,y*tileHeight2,tileWidth2,tileHeight2,(Color){0,210,0,255});
            }
            if(value>10){ // here we draw some more trees            
            DrawRectangle(x*tileWidth2,y*tileHeight2,tileWidth2,tileHeight2,(Color){0,140,0,255});
            }

            if(value>22){ // draw mountains or rocks
            int col=(value-22)*4;
            DrawRectangle(x*tileWidth2,y*tileHeight2,tileWidth2,tileHeight2,(Color){110+col,110+col,110+col,255});
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
            if(map[x][y]>10){ // here we draw some more trees            
            DrawRectangle(x*tileWidth,y*tileHeight,tileWidth,tileHeight,(Color){0,140,0,255});
            }

            if(map[x][y]>22){ // draw mountains or rocks
            int col=(map[x][y]-22)*4;
            DrawRectangle(x*tileWidth,y*tileHeight,tileWidth,tileHeight,(Color){110+col,110+col,110+col,255});
            }

        }

    }    
    }    
};

//
// Here we generate a new map part at the bottom part of the map
// we shift the map up in memory to make place for that new part.
// For scrolling back to the origal part we would need to save the map and
// reload it.
//
//
static void edgemap(){
    
    for(int y=8;y<9;y++){
    for(int x=2;x<9;x++){
        makemap(x*50,y*50);
    }}
}

static void genmap(){
    // Here we generate the map using the hill algorithm
    for(int y=0;y<mapHeight;y++){
    for(int x=0;x<mapWidth;x++){
        map[x][y]=0;
    }}

    for(int y=2;y<9;y++){
    for(int x=2;x<9;x++){
        makemap(x*50,y*50);
    }}
};
