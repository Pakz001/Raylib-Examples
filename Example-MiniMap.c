//
// MiniMap example



#include "raylib.h"
#include <math.h>

static int map[300][300];

static int mapWidth = 100;
static int mapHeight = 100;
static float tileWidth;
static float tileHeight;
static const int screenWidth = 640;
static const int screenHeight = 480;

static int mapx,mapy;

static void makemap();
static void drawminimap();
static void drawmap();
static bool rectsoverlap(int x1,int y1,int w1,int h1,int x2,int y2,int w2,int h2);

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------

    InitWindow(screenWidth, screenHeight, "raylib example.");

    // We want to have the tile width and height fill the entire screen.
    tileWidth = 32 ;
    tileHeight = 32;
    // Here we generate the map using the hill algorithm
    makemap();
    mapx = 0;
    mapy = 0;
 
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
        
        if(rectsoverlap(GetMouseX(),GetMouseY(),1,1,screenWidth-105,32,100,100)){
            mapx = GetMouseX()-(screenWidth-105);
            mapy = GetMouseY()-32;
            if(mapx>100-17)mapx=100-17;
            if(mapy>100-14)mapy=100-14;
        }
        
        //----------------------------------------------------------------------------------
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);
            drawmap();
            drawminimap();  
            DrawRectangle(0,3,screenWidth,24,DARKGRAY);
            DrawText("Move mouse over minimap.",2,6,20,WHITE);

            DrawRectangle(0,screenHeight-30,screenWidth,24,DARKGRAY);
            DrawText("Press Space to Generate new map",2,screenHeight-28,20,WHITE);
            

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
    // We create a line of some sort to create more realistic landmasses. :Experiment:
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
    // Now we want to pile sand ontop and right next to the current hills.
    // This will make them into larger hills. We do this by picking a random
    // spot. If there is something below that we increase the height there.
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
    for(int y=0;y<14;y++){
    for(int x=0;x<17;x++){
        if(x+mapx>=100 && y+mapy>=100)continue;
        int m = map[x+mapx][y+mapy];
        if(m==0){ // water
            DrawRectangle(x*tileWidth,y*tileHeight+32,tileWidth,tileHeight,BLUE);
        }        
        if(m>0){
            if(m>0){//Draw the beach
            DrawRectangle(x*tileWidth,y*tileHeight+32,tileWidth,tileHeight,(Color){250,220,0,255});
            }

            if(m>3){//If the height is larger than 3 than draw trees
            DrawRectangle(x*tileWidth,y*tileHeight+32,tileWidth,tileHeight,(Color){0,210,0,255});
            }
            if(m>15){ // here we draw some more trees
            DrawRectangle(x*tileWidth,y*tileHeight+32,tileWidth,tileHeight,(Color){0,170,0,255});
            }

            if(m>20){ // draw mountains or rocks
            DrawRectangle(x*tileWidth,y*tileHeight+32,tileWidth,tileHeight,(Color){110,110,110,255});
            }

        }
    }    
    }    
}

static void drawminimap(){
    DrawRectangle(0,0,screenWidth,32,BLACK);
    DrawRectangle(screenWidth-110,32,110,screenHeight-64,BLACK);
    DrawRectangle(0,screenHeight-32,screenWidth,32,BLACK);
    for(int y=0;y<mapHeight;y++){
    for(int x=0;x<mapWidth;x++){
        
        if(map[x][y]==0){ // water
            DrawRectangle(screenWidth-105+x,y+32,1,1,BLUE);
        }        
        if(map[x][y]>0){
            if(map[x][y]>0){//Draw the beach
            DrawRectangle(screenWidth-105+x,y+32,1,1,(Color){250,220,0,255});
            }

            if(map[x][y]>3){//If the height is larger than 3 than draw trees
            DrawRectangle(screenWidth-105+x,y+32,1,1,(Color){0,210,0,255});
            }
            if(map[x][y]>15){ // here we draw some more trees
            DrawRectangle(screenWidth-105+x,y+32,1,1,(Color){0,170,0,255});
            }

            if(map[x][y]>20){ // draw mountains or rocks
            DrawRectangle(screenWidth-105+x,y+32,1,1,(Color){110,110,110,255});
            }

        }

    }    
    }    
    //draw selection
    DrawRectangleLines(screenWidth-105+mapx,32+mapy,17,14,WHITE);
};

// Rectangles overlap
bool rectsoverlap(int x1,int y1,int w1,int h1,int x2,int y2,int w2,int h2){
    if(x1 >= (x2 + w2) || (x1 + w1) <= x2) return false;
    if(y1 >= (y2 + h2) || (y1 + h1) <= y2) return false;
    return true;
}
