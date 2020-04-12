// Work in progress!!

#define MAPWIDTH 30
#define MAPHEIGHT 30
#define MAX_PATH 10024 //; On large or/and complex maps increase this value.

#include "raylib.h"
#include <math.h>

typedef struct pathnode{
    int x;
    int y;
    
}pathnode;

static int map[MAPWIDTH][MAPHEIGHT];
static pathnode arr_path[MAX_PATH];


static int mapWidth = MAPWIDTH;
static int mapHeight = MAPHEIGHT;
static float tileWidth ;
static float tileHeight;
// Start and end of our path.
static int startx;
static int starty;
static int endx;
static int endy;

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
            // Draw our map.
            for(int y=0;y<MAPHEIGHT;y++){
            for(int x=0;x<MAPWIDTH;x++){
                if(map[x][y]==-1){
                    DrawRectangle(x*tileWidth,y*tileHeight,tileWidth,tileHeight,BLACK);
                }
                if(map[x][y]>0){
                    DrawText(FormatText("%i",map[x][y]),x*tileWidth,y*tileHeight,10,BLACK);
                }
            }}
            // Draw start and end position.
            DrawRectangle(startx*tileWidth,starty*tileHeight,tileWidth,tileHeight,RED);
            DrawRectangle(endx*tileWidth,endy*tileHeight,tileWidth,tileHeight,RED);
            
            //DrawText(FormatText("%f",tileWidth),0,0,20,RED);
            //DrawText(FormatText("%i",MAPHEIGHT),0,20,20,RED);
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
    
    //
    // Generate random map....
    //
    
    // First make sure every map value is 0.
    for(int y=0;y<mapHeight;y++){
    for(int x=0;x<mapWidth;x++){
        map[x][y] = 0;
    }}
  	// Draw some lines on the map.
   	for(int x=0;x< mapWidth;x+=5){
        if(GetRandomValue(0,3)<2){
            for(int y=2;y< mapHeight-2;y++){
                map[x][y] = -1;
            }
        }
    }
    //
    //draw some rectangles on the map
   	for(int i=0;i<mapWidth;i++){
   		int x1=GetRandomValue(0,mapWidth-5);
   		int y1=GetRandomValue(0,mapHeight-5);
   		for(int x2=0;x2<4;x2++){
   		for(int y2=0;y2<4;y2++){
   			map[x1+x2][y1+y2] = -1;
   		}}
   	} 
    // Draw some passages on the map.
   	for(int y=0;y<mapHeight;y+=5){
 	for(int x=0;x<mapWidth;x++){
 		map[x][y] = 0;
 	}}
    
    
    //
    // find a start and end location.
    // 
    bool found=false;
    int failed=0;
    while(found==false){
        found = true;
        startx = GetRandomValue(1,mapWidth-1);
        starty = GetRandomValue(1,mapHeight-1);
        endx = GetRandomValue(1,mapWidth-1);
        endy = GetRandomValue(1,mapHeight-1);
        // If below any start or end position than try again.
        if(map[startx][starty]!=0 || map[endx][endy]!=0)found=false;
        failed+=1;
        if(failed>500000)return; // If we just can not find any start and end position then exit.
    }
    
    //
    // Flood the map with distances from the start.
    //
    
    struct pathnode list[MAX_PATH];
    //
    // We store the distance on each map cell if there is no wall there.
    //
    int listlen=0;    
    list[listlen].x=startx;
    list[listlen].y=starty;
    listlen+=1;        
    failed=0;
    // 4 way search!
    int dx[4]={ 0,1,0,-1};
    int dy[4]={-1,0,1,0};    
    // While we have a list to work with
    while(listlen>0){
        // Take the first value from the array.
        int x1=list[0].x;
        int y1=list[0].y;
        // shift all down
        for(int i=0;i<listlen;i++){
            list[i].x = list[i+1].x;
            list[i].y = list[i+1].y;
        }
        // Decrease list length
        listlen-=1;
        //
        // Here we check around our current position.
        for(int i=0;i<4;i++){
            int nx = x1+dx[i];
            int ny = y1+dy[i];
            if(nx<0 || ny<0 || nx>= mapWidth || ny>= mapHeight)continue;
            // If we can get there then put the new distance there and add this position
            // to the list.
            if(map[nx][ny]==0){
                map[nx][ny]=map[x1][y1]+1;
                // add to last
                //
                list[listlen].x = nx;
                list[listlen].y = ny;
                listlen++;
                //                
            }
        }
        // Error?
        failed+=1;
        if(failed>160000)return;
    }
}
