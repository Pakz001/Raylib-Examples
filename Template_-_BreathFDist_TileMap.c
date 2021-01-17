//
// Genetic Algorithm Example 

#include "raylib.h"
#include <math.h>

#define MAX_PATH 1024


enum terrain{TREE=1,GROUND=0,AIPLAYER1=2,AIPLAYER2=3,GOAL=4,GOALREACHED=5};
enum flag{UP,DOWN,LEFT,RIGHT,WAIT};

//
// You can set any number of 3 and 2 on the map. more 4's untested. keep the borders(1) intact(memory errors)
//
//1=border,2=ai unit,3=defensiveunit,4=objective
int map1[10][10] = {    {1,1,1,1,1,1,1,1,1,1},
                        {1,0,0,0,0,1,0,0,3,1},
                        {1,0,0,0,0,1,0,0,0,1},
                        {1,3,0,0,0,1,0,0,0,1},
                        {1,0,0,0,0,1,1,0,1,1},
                        {1,0,0,0,0,1,0,0,0,1},
                        {1,1,1,0,1,1,0,0,0,1},
                        {1,0,0,0,0,0,2,0,0,1},
                        {1,0,0,0,0,0,0,0,0,1},
                        {1,1,1,1,1,1,1,1,1,1}
                        };  
int map[10][10] = {0};

int dijkstramap[10][10]={0};


typedef struct pathnode{
    int x;
    int y;    
}pathnode;



void makedijkstramap();
static float distance(float x1,float y1,float x2,float y2);
static float edistance(float x1,float y1,float x2,float y2);

int topdijkstralen=0;
int screenWidth;
int screenHeight;
int tileWidth,tileHeight,mapWidth,mapHeight;



int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    screenWidth = 800;
    screenHeight = 600;
    mapWidth = 10;
    mapHeight = 10;
    tileWidth = ceil((float)screenWidth/(float)mapWidth);
    tileHeight = ceil((float)screenHeight/(float)mapHeight);
    
    InitWindow(screenWidth, screenHeight, "raylib example.");
 
    SetTargetFPS(200);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------
    //makedijkstramap(); // The 4 = the goal that gets flooded from...

    for(int y=0;y<mapHeight;y++){
    for(int x=0;x<mapHeight;x++){
        map[y][x]= map1[y][x];
    }}
    
    int playposition=0;
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
            //drawmap
            for(int y=0;y<10;y++){
                for(int x =0;x<10;x++){
                    int m=map[y][x];
                    if(m==TREE){
                        
                        DrawRectangle(x*tileWidth,y*tileHeight,tileWidth,tileHeight,(Color){40,90,20,255});
                        DrawCircle(x*tileWidth+tileWidth/1.5,y*tileHeight+tileHeight/1.5,tileWidth/5,(Color){20,50,20,255});
                        DrawRectangle(x*tileWidth+tileWidth/2.2,y*tileHeight+tileHeight/2,tileWidth/8,tileHeight/3,BROWN);
                        
                        DrawCircle(x*tileWidth+tileWidth/2,y*tileHeight+tileHeight/3,tileWidth/4,(Color){120,250,20,255});
                        DrawCircle(x*tileWidth+tileWidth/2.2,y*tileHeight+tileHeight/4,tileWidth/9,(Color){220,255,220,155});
                    }
                    if(m==GROUND){
                        DrawRectangle(x*tileWidth,y*tileHeight,tileWidth,tileHeight,DARKGREEN);                        
                        DrawRectangle(x*tileWidth+5,y*tileHeight+10,2,1,GREEN);                        
                        DrawRectangle(x*tileWidth+tileWidth/6,y*tileHeight+tileHeight/6,2,1,GREEN);                        
                        DrawRectangle(x*tileWidth+tileWidth/1.5,y*tileHeight+tileHeight/1.5,2,1,GREEN);                        
                        DrawRectangle(x*tileWidth+tileWidth/2,y*tileHeight+tileHeight/2,2,1,GREEN);                        
                        
                    }
                    if(m==AIPLAYER1){
                        DrawRectangle(x*tileWidth,y*tileHeight,tileWidth,tileHeight,BLUE);                        
                        DrawText("Player",x*tileWidth,y*tileHeight+tileHeight/4,16,BLACK);                        
                    }
                    if(m==AIPLAYER2){
                        DrawRectangle(x*tileWidth,y*tileHeight,tileWidth,tileHeight,RED); 
                        DrawText("Ai_Player",x*tileWidth,y*tileHeight+tileHeight/4,16,WHITE);
                    }
                    if(m==GOAL){
                       DrawRectangle(x*tileWidth,y*tileHeight,tileWidth,tileHeight,WHITE);      
                       DrawText("City",x*tileWidth+tileWidth/4,y*tileHeight+tileHeight/4,26,BLACK);                        
                    }
                    if(m==GOALREACHED){
                        DrawRectangle(x*tileWidth,y*tileHeight,tileWidth,tileHeight,YELLOW);                        
                        DrawText("Captured",x*tileWidth+4,y*tileHeight+tileHeight/4,16,BLACK);                        
                    }
                    DrawText(FormatText("%i",dijkstramap[y][x]),x*tileWidth,y*tileHeight,20,BLACK);

            }
            }
            DrawText("Press space for new simulation.(Autorun=on)",10,10,26,BLACK);
            DrawText("Press space for new simulation.(Autorun=on)",9,9,26,WHITE);
            
            
            

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;


}



void makedijkstramap(){
    
    int startx;
    int starty;
    // First make sure every map value is 0.
    for(int y=0;y<mapHeight;y++){
    for(int x=0;x<mapWidth;x++){
        dijkstramap[y][x] = 0;
        if(map1[y][x]==1)dijkstramap[y][x]=-1;
        if(map1[y][x]==4){
            startx = x;
            starty = y;
        }
    }}
    
    //
    
    //
    // Flood the map with distances from the start.
    //
    
    // We store the distance on each map cell if there is no wall there.
    //
    struct pathnode list[MAX_PATH];
    dijkstramap[starty][startx]=1;
    int listlen=0;    
    list[listlen].x=startx;
    list[listlen].y=starty;
    listlen+=1;        
    int failed=0;
    // 4 way search! left/up/down/right
    int dx[8]={ -1,0,1,-1,1,-1,0,1};
    int dy[8]={-1,-1,-1,0,0,1,1,1};    
    // While we have a list to work with
    while(listlen>0){
        // Take the first value from the array.
        int x1=list[0].x;
        int y1=list[0].y;
        
        // shift all up.
        for(int i=0;i<listlen;i++){
            list[i].x = list[i+1].x;
            list[i].y = list[i+1].y;
        }
        // Decrease list length
        listlen-=1;
        //
        // Here we check around our current position.
        for(int i=0;i<8;i++){
            int nx = x1+dx[i];
            int ny = y1+dy[i];
            if(nx<0 || ny<0 || nx>= mapWidth || ny>= mapHeight)continue;            
            // If we can get there then put the new distance there and add this position
            // to the list.
            if(dijkstramap[ny][nx]==0 && map[ny][ny]!=1){
                dijkstramap[ny][nx]=dijkstramap[y1][x1]+1;
                if(dijkstramap[ny][nx]>topdijkstralen)topdijkstralen=dijkstramap[ny][nx];
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

// Manhattan Distance (less precise)
float distance(float x1,float y1,float x2,float y2){
    return (float)abs(x2-x1)+abs(y2-y1);
}

// Euclidean distance (more precise)
float edistance(float x1,float y1,float x2,float y2){
    return sqrt( (x1-x2)*(x1-x2)+(y1-y2)*(y1-y2) );
}
