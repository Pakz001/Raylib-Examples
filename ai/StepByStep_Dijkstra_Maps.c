#define MAX_PATH 1048
#define MAPWIDTH 15
#define MAPHEIGHT 15

#include "raylib.h"
#include <math.h>


typedef struct pathnode{
    int x;
    int y;
    
}pathnode;

static struct pathnode list[MAX_PATH];

static int map[MAPWIDTH][MAPHEIGHT];
static int mapWidth = MAPWIDTH;
static int mapHeight = MAPHEIGHT;
static float tileWidth ;
static float tileHeight;
static int startx;
static int starty;
static int endx;
static int endy;
static int listlen=0;    
static bool startfound=false;


static void newmap(void);
static void animatedijkstramap(void);
    
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib example.");
 
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------
    newmap();

    tileWidth = abs((float)screenWidth/(float)mapWidth);
    tileHeight = abs((float)screenHeight/(float)mapHeight);
     
    listlen = 0;
    map[startx][starty]=1;
    list[listlen].x=startx;
    list[listlen].y=starty;
    listlen+=1;        

    
    
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        
        if(IsKeyPressed(KEY_SPACE)){
            if(startfound==false){
                animatedijkstramap();
            }else{
                startfound=false;
                newmap();
                listlen = 0;
                map[startx][starty]=1;
                list[listlen].x=startx;
                list[listlen].y=starty;
                listlen+=1;        
                
            }
            
        }
        
        //----------------------------------------------------------------------------------
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);
            for(int y=0;y<mapHeight;y++){
            for(int x=0;x<mapWidth;x++){
                if(map[x][y]<0){                    
                    DrawRectangleGradientEx((Rectangle){x*tileWidth,y*tileHeight,tileWidth,tileHeight},BLACK,GRAY,WHITE,WHITE);
                }else if(map[x][y]>0){
                    DrawRectangle(x*tileWidth,y*tileHeight,tileWidth,tileHeight,BLUE);
                    DrawText(FormatText("%i",map[x][y]),x*tileWidth,y*tileHeight,30,YELLOW);
                }else{
                    DrawText(FormatText("%i",map[x][y]),x*tileWidth,y*tileHeight,30,BLACK);
                }
                
            }
            }
            DrawRectangle(startx*tileWidth,starty*tileHeight,tileWidth,tileHeight,(Color){0,200,0,200});
            DrawRectangle(endx*tileWidth,endy*tileHeight,tileWidth,tileHeight,(Color){200,0,0,100});
            
            if(startfound){
                DrawRectangle(0,0,screenWidth,60,(Color){10,10,10,200});
                DrawText("Start Position Found!.",0,0,50,(Color){255,255,255,200});
            }else{
                DrawRectangle(0,0,screenWidth,20,(Color){100,100,100,200});
                DrawText("Press space to search.",0,0,20,(Color){255,255,255,255});
            }

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;


}


void animatedijkstramap(){
    
    
    //
    // Flood the map with distances from the start.
    //
  
    if(listlen==0)return;
    
//    int failed=0;
    // 4 way search! left/up/down/right
    int dx[4]={ 0,1,0,-1};
    int dy[4]={-1,0,1,0};    
    // While we have a list to work with
//    while(listlen>0){
        // Take the first value from the array.
        int x1=list[0].x;
        int y1=list[0].y;
        
        // shift all up.
        for(int i=0;i<listlen;i++){
            list[i].x = list[i+1].x;
            list[i].y = list[i+1].y;
        }
        if(x1==endx && y1==endy){            
            listlen=0;
            startfound=true;
            return;
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
//        failed+=1;
//        if(failed>160000)return;
        
//    }
    
 
}

void newmap(){
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

}
