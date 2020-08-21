//
// This map generator works by first creating one random rectangle in the room. This rectangle has walls.
// We then create more rectangles randomly on the map and when one of these its edges touches another
// edge we create another room there.
//
// We only draw walls on empty map places so there will be a floor area created.
//
// Edit:there is still a bug where closed rooms are created.

#include "raylib.h"
#include <math.h> // For the ceil function

const int screenWidth = 800;
const int screenHeight = 450;
static int map[100][100];
static int mapWidth = 100;
static int mapHeight = 100;
static int tileWidth = 100;
static int tileHeight = 100;

static void drawmap();
static void makemap();
static void makerect(int x,int y,int w,int h,bool force);


int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------

    InitWindow(screenWidth, screenHeight, "raylib example.");
 
    // Get the tile width and height based on the amount of tiles hor and vertical on the screen.
    tileWidth = ceil((float)screenWidth/(float)mapWidth);
    tileHeight = ceil((float)screenHeight/(float)mapHeight);
 
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
    // Clear anything that was inside the map array.
    for(int y=0;y<mapHeight;y++){
        for(int x=0;x<mapWidth;x++){
            map[x][y]=0;
    }}
    // Place the first rectangle.
    makerect(mapWidth/2,mapHeight/2,10,10,true);
    for(int i=0;i<1500;i++){
        int x=GetRandomValue(4,mapWidth-4);
        int y=GetRandomValue(4,mapHeight-4);
        makerect(x,y,GetRandomValue(4,18),GetRandomValue(4,18),false);
    }
};

static void makerect(int x,int y,int w,int h,bool force){
    // Check if the rects is inside the map area.
    if(x<0 || x+w>=mapWidth-10 || y<0 || y+h>=mapHeight-10)return;
    //
    if(force==false){
        int edge=0;

        bool good1=false;
        bool good2=true;
        // Check if top is inside another rect and one below is nothing.
        for(int x1=x;x1<x+w;x1++){
            
            if(map[x1][y]==2){
                good1=true;
            }
            if(map[x1][y+1]==0){
                good2=false;
            }
        }
        if(good1==true && good2==true)edge++;

        good1=false;
        good2=true;        
        // Check if bottom is inside another rect and one above is nothing.
        for(int x1=x;x1<x+w;x1++){
            
            if(map[x1][y+h]==2){
                good1=true;
            }
            if(map[x1][y+h-1]==0){
                good2=false;
            }
        }
        if(good1==true && good2==true)edge++;

        good1=false;
        good2=true;        
        // Check if left side is inside another rect and one lefter is nothing.
        for(int y1=y;y1<y+h;y1++){
            
            if(map[x][y1]==2){
                good1=true;
            }
            if(map[x-1][y1]==0){
                good2=false;
            }
        }
        if(good1==true && good2==true)edge++;
        

        
        good1=false;
        good2=true;        
        // Check if right side is inside another rect and one righter is nothing.
        for(int y1=y;y1<y+h;y1++){
            
            if(map[x+w][y1]==2){
                good1=true;
            }
            if(map[x+w-1][y1]==0){
                good2=false;
            }
        }
        if(good1==true && good2==true)edge++;
        
        if(edge!=1)return;        
    }
    
    //Draw walls
    for(int y1=y;y1<y+h;y1++){
        for(int x1=x;x1<x+w;x1++){
            if(map[x1][y1]==0)map[x1][y1]=2;
        }
    }

    //Draw floor
    for(int y1=y+1;y1<y+h-1;y1++){
        for(int x1=x+1;x1<x+w-1;x1++){
            map[x1][y1]=1;
        }
    }

};

static void drawmap(){
    for(int y=0;y<mapHeight;y++){
        for(int x=0;x<mapWidth;x++){
            int dx=x*tileWidth;
            int dy=y*tileHeight;
            if(map[x][y]==0){// Nothing on the map here
                
            }
            if(map[x][y]==1){// Floor tile
                DrawRectangle(dx,dy,tileWidth,tileHeight,GRAY);
            }            
            if(map[x][y]==2){// Wall tile
                DrawRectangle(dx,dy,tileWidth,tileHeight,BROWN);
            }            

    }}
};

