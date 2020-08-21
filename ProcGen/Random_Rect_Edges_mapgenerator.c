//
// This map generator works by first creating one random rectangle in the room. This rectangle has walls.
// We then create more rectangles randomly on the map and when one of these its edges touches another
// edge we create another room there.
//
// We only draw walls on empty map places so there will be a floor area created.
//


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
static bool maprectcheck(int x,int y,int w,int h);
static void edgeenhance();

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

            ClearBackground((Color){20,20,20,255});
            drawmap();
            edgeenhance();
            DrawRectangle(0,0,screenWidth,20,DARKGRAY);
            DrawText("Press space to generate new map.",3,0,20,WHITE);

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
        makerect(x,y,GetRandomValue(5,18),GetRandomValue(5,18),false);
    }
};

static void makerect(int x,int y,int w,int h,bool force){
    // Check if the rects is inside the map area.
    if(x<0 || x+w>=mapWidth-10 || y<0 || y+h>=mapHeight-10)return;
    //
    if(force==false){
        int edge=0;
        int cnt=0;
        bool good1=false;
        // Check if top is inside another rect and one below is nothing.
        for(int x1=x;x1<x+w;x1++){
            
            if(map[x1][y]==2){
                good1=true;
                cnt++;
            }
        }
        if(good1==true && maprectcheck(x,y+1,w,h) && cnt>3)edge++;

        good1=false;  
        cnt=0;        
        // Check if bottom is inside another rect and one above is nothing.
        for(int x1=x;x1<x+w;x1++){
            
            if(map[x1][y+h-1]==2){
                good1=true;
                cnt++;
            }
        }
        if(good1==true && maprectcheck(x,y,w,h-1) && cnt>3)edge++;

        good1=false;    
        cnt=0;
        // Check if left side is inside another rect and one lefter is nothing.
        for(int y1=y;y1<y+h;y1++){
            
            if(map[x][y1]==2){
                good1=true;
                cnt++;
            }
        }
        if(good1==true && maprectcheck(x+1,y,w,h) && cnt>3)edge++;
        

        
        good1=false;
        cnt=0;
            
        // Check if right side is inside another rect and one righter is nothing.
        for(int y1=y;y1<y+h;y1++){
            
            if(map[x+w-1][y1]==2){
                good1=true;
                cnt++;
            }
        }
        if(good1==true && maprectcheck(x,y,w-1,h) && cnt>3)edge++;
        
        if(edge!=1)return;        
    }
    
    //Draw walls
    for(int y1=y;y1<y+h;y1++){
        for(int x1=x;x1<x+w;x1++){          
            if(map[x1][y1]==2)map[x1][y1]=1;        
            if(map[x1][y1]==0)map[x1][y1]=2;
        }
    }

    //Draw floor
    for(int y1=y+1;y1<y+h-1;y1++){
        for(int x1=x+1;x1<x+w-1;x1++){
            map[x1][y1]=1;
        }
    }
    
    // bugremoval - it appears there can be holes in the walls. removed here
    for(int y1=y;y1<y+h;y1++){
        for(int x1=x;x1<x+w;x1++){          
        if(map[x1][y1]==1){
            if(map[x1+1][y1]==0||map[x1-1][y1]==0||map[x1][y1-1]==0||map[x1][y1+1]==0){
                map[x1][y1]=2;
            }
            if(map[x1+1][y1+1]==0||map[x1-1][y1-1]==0||map[x1+1][y1-1]==0||map[x1-1][y1+1]==0){
                map[x1][y1]=2;
            }                        
        }

    }}    

};

static bool maprectcheck(int x,int y,int w,int h){
    for(int y1=y;y1<y+h;y1++){
        for(int x1=x;x1<x+w;x1++){
            if(map[x1][y1]!=0)return false;
        }
    }
    return true;
}

static void drawmap(){
    int swx=0;
    
    for(int y=0;y<mapHeight;y++){
        
        for(int x=0;x<mapWidth;x++){
            int dx=x*tileWidth;
            int dy=y*tileHeight;
  
            if(map[x][y]==0){// Nothing on the map here
                
            }
            if(map[x][y]==1){// Floor tile
                
                if(swx==0)DrawRectangle(dx,dy,tileWidth,tileHeight,(Color){150,150,150,255});
                else DrawRectangle(dx,dy,tileWidth,tileHeight,(Color){120,120,120,255});                
            }            
            if(map[x][y]==2){// Wall tile
                DrawRectangle(dx,dy,tileWidth,tileHeight,BROWN);
            }            

            swx++;
            if(swx>1)swx=0;

        }
        swx++;
        if(swx>1)swx=0;
    }
};

static void edgeenhance(){
    for(int y=0;y<mapHeight;y++){
        for(int x=0;x<mapWidth;x++){
            int dx=x*tileWidth;
            int dy=y*tileHeight;
            //outer edges
            if(map[x][y]==2 && map[x-1][y]==0){
                DrawRectangle(dx-tileWidth/2,dy,tileWidth/2,tileHeight,(Color){200,150,0,255});
            }
            if(map[x][y]==2 && map[x][y-1]==0){
                DrawRectangle(dx,dy-tileHeight/2,tileWidth,tileHeight/2,(Color){200,150,0,255});
            }
            if(map[x][y]==2 && map[x+1][y]==0){
                DrawRectangle(dx+tileWidth,dy,tileWidth/2,tileHeight,(Color){80,50,50,255});
            }
            if(map[x][y]==2 && map[x][y+1]==0){
                DrawRectangle(dx,dy+tileHeight,tileWidth,tileHeight/2,(Color){80,50,50,255});
            }
            //inner edges
            if(map[x][y]==2 && map[x][y+1]==1){
                DrawRectangle(dx,dy+tileHeight,tileWidth,tileHeight/2,DARKGRAY);
            }
            if(map[x][y]==2 && map[x+1][y]==1){
                DrawRectangle(dx+tileWidth,dy,tileWidth/2,tileHeight,DARKGRAY);
            }

    }}    
}
