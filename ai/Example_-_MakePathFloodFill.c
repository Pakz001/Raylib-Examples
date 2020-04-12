// Work in progress!!

#define MAPWIDTH 50
#define MAPHEIGHT 50
#define MAX_PATH 1024

#include "raylib.h"
#include <math.h>

typedef struct pathnode{
    int x;
    int y;
    
}pathnode;

static int map[MAPWIDTH][MAPHEIGHT];
static pathnode arr_path[MAX_PATH];


static float tileWidth = 50.0f;
static float tileHeight = 50.0f;
static int mapWidth = MAPWIDTH;
static int mapHeight = MAPHEIGHT;

static void newmapandpath(void);

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib example.");
    
    // Make sure the tile dimensions fill up the entire map if the entire map is drawn.    

    tileWidth = abs((float)screenWidth/(float)mapWidth);
    tileHeight = abs((float)screenHeight/(float)mapHeight);
    
    newmapandpath();
 
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

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
            
            for(int y=0;y<MAPHEIGHT;y++){
            for(int x=0;x<MAPWIDTH;x++){
                if(map[x][y]==-1){
                    DrawRectangle(x*tileWidth,y*tileHeight,tileWidth,tileHeight,BLACK);
                }
            }}
            DrawText(FormatText("%f",tileWidth),0,0,20,RED);
            DrawText(FormatText("%i",MAPHEIGHT),0,20,20,RED);
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;


}

void newmapandpath(){
    
    
    for(int y=0;y<mapHeight;y++){
    for(int x=0;x<mapWidth;x++){
        map[x][y] = 0;
    }}
  	//draw some contents on the map
   	for(int x=0;x< mapWidth;x+=5){
        if(GetRandomValue(0,3)<2){
            for(int y=2;y< mapHeight-2;y++){
                map[x][y] = -1;
            }
        }
    }
    
   	for(int i=0;i<mapWidth;i++){
   		int x1=GetRandomValue(0,mapWidth-5);
   		int y1=GetRandomValue(0,mapHeight-5);
   		for(int x2=0;x2<4;x2++){
   		for(int y2=0;y2<4;y2++){
   			map[x1+x2][y1+y2] = -1;
   		}}
   	}
   	for(int y=0;y<mapHeight;y+=5){
 	for(int x=0;x<mapWidth;x++){
 		map[x][y] = 0;
 	}}
    
}
