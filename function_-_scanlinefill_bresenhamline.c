
#include "raylib.h"

#define MAP_WIDTH 100
#define MAP_HEIGHT 100

int map[MAP_HEIGHT][MAP_WIDTH];

void scanlinefill(int x, int y,int value);
void mapline(int x1, int y1, int x2, int y2,int value);


int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib example.");
 
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------
    mapline(0,0,MAP_WIDTH,MAP_HEIGHT,1);
    scanlinefill(50,48,1);

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        
        
        //----------------------------------------------------------------------------------
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);
                
            for(int y=0;y<MAP_HEIGHT;y++){
            for(int x=0;x<MAP_WIDTH;x++){
                if(map[y][x]==1){
                    DrawRectangle(x*2,y*2,2,2,RED);
                }
            }}

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;


}

//
// scan line fill(flood) the map array if the value is 0 with value.
//
//
void scanlinefill(int x, int y,int value){
    if(x<0 || x>=MAP_WIDTH || y<0 || y>=MAP_HEIGHT)return;
    if(map[y][x]==value)return;
    int x2=x;
    while(x2>-1 && map[y][x2]==0){
        map[y][x2]=value;
        scanlinefill(x2,y-1,value);
        scanlinefill(x2,y+1,value);
        x2--;
    }
    x2=x+1;
    while(x2<MAP_WIDTH && map[y][x2]==0){
        map[y][x2]=value;
        scanlinefill(x2,y-1,value);
        scanlinefill(x2,y+1,value);
        x2++;
    }
    
}

//
// Bresenham line inside map[][] array.
//
void mapline(int x1, int y1, int x2, int y2,int value){
    int dx;
    int dy;
    int sx;
    int sy;
    int e;
    dx = abs(x2 - x1);
    sx = -1;
    if(x1 < x2)sx = 1;
    dy = abs(y2 - y1);
    sy = -1;
    if(y1 < y2)sy = 1;
    if(dx < dy){
      e = dx / 2; 
    }else{ 
      e = dy / 2;          
    }
    bool exitloop=false;
    while(exitloop == false){
        //SetColor 255,255,255
        //DrawPoint x1,y1
        if(x1>-1 && x1<MAP_WIDTH && y1>-1 && y1<MAP_HEIGHT)map[x1][y1]=value;
 
        if(x1 == x2){ 
            if(y1 == y2){
                exitloop = true;
            }
        }
        if(dx > dy){
            x1 += sx;
            e -= dy; 
            if (e < 0){
                e += dx;
                y1 += sy;
            }
        }else{
            y1 += sy; 
            e -= dx;
            if(e < 0){
              e += dy;
              x1 += sx;
            }
        }
    }
}
