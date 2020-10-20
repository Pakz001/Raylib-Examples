//
// Quickly programmed/coded and not tested much.

// Floodfill/seed fill - 

//
// Around the start position use a for loop(s) with x and y and grow the edges that touch with
// walkable area. Each loop itteration we use a larger area to loop from(optimalisation!)
//
// We start at the start position and stop when we reach a end position - not a real flood fil here but more usefull for checking walkability.

#include "raylib.h"
#include <math.h>

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib example.");
 
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------    
    int mapWidth = 10;
    int mapHeight = 5;
    float tileWidth = ceil((float)screenWidth/(float)mapWidth);
    float tileHeight = ceil((float)screenHeight/(float)mapHeight);
    int map[5][10] = {  {1,1,1,1,1,1,1,1,1,1},
                        {1,0,0,0,1,1,0,0,3,1},
                        {1,0,0,0,1,1,0,1,1,1},
                        {1,2,0,0,0,0,0,0,0,1},
                        {1,1,1,1,1,1,1,1,1,1}};
    bool visitedmap[5][10] = {false};
    
    bool pathfound = false;
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        if(IsKeyPressed(KEY_SPACE)){
            // Erase the visited map
            // find start position (2)
            // find end position (3)
            Vector2 posstart;
            Vector2 posend;
            for(int y=0;y<mapHeight;y++){
            for(int x=0;x<mapWidth;x++){
                visitedmap[y][x] = false;
                if(map[y][x]==2)posstart = (Vector2){x,y};
                if(map[y][x]==3)posend = (Vector2){x,y};
            }}
            
            // Flood the map until we find the destination cell/position
            
            int mx[] = {0,1,0,-1};//These are the values we use to find the above left and right cells.
            int my[] = {-1,0,1,0};
            visitedmap[(int)posstart.x][(int)posstart.y]=true; // we start by setting the vistedmap position where the flood starts true
            for(int size=1;size<10;size++){ // we check up to a maximum of -10 and 10 size area for the flood.
                for(int y=posstart.y-size;y<posstart.y+size;y++){
                for(int x=posstart.x-size;x<posstart.x+size;x++){
                  if(map[y][x]==0 && visitedmap[y][x]==true){ // if we are on a walkable terrain and we have visited this area before
                    for(int j=0;j<4;j++){ // loop through the neighbouring cells
                        int x2=x+mx[j]; // get cell position next to current visited cell
                        int y2=y+my[j];
                        if(x2<0 || y2<0 || x2>mapWidth || y2>mapHeight)continue; // if we are outside of bounds then exit and continue with next loop(j)
                        if(x2==posend.x && y2==posend.y){ // if we find the end position than set a flag
                            pathfound=true;
                        }

                        if(map[y2][x2]!=0)continue; // if this cell is not walkable then exit and continue with next loop itteration(j)
                        visitedmap[y2][x2]=true; // if we are here it means this current cell position is walkable so set the visited map to true
                    }
                  }
                }}
            }
        }
        
        //----------------------------------------------------------------------------------
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(GREEN);
                
            //drawmap
            for(int y=0;y<5;y++){
            for(int x=0;x<10;x++){
                if(map[y][x]==1){ // wall
                    DrawRectangle(x*tileWidth,y*tileHeight,tileWidth,tileHeight,BLACK);
                }
                if(map[y][x]==2){ // start position
                    DrawRectangle(x*tileWidth,y*tileHeight,tileWidth,tileHeight,RED);
                    DrawText(FormatText("S"),x*tileWidth+tileWidth/8,y*tileHeight+tileHeight/8,tileWidth,WHITE);
                }
                if(map[y][x]==3){ // end position
                    DrawRectangle(x*tileWidth,y*tileHeight,tileWidth,tileHeight,YELLOW);
                    DrawText(FormatText("X"),x*tileWidth+tileWidth/8,y*tileHeight+tileHeight/8,tileWidth,WHITE);
                }
                if(visitedmap[y][x]==true){
                    DrawText(FormatText("."),x*tileWidth,y*tileHeight,tileWidth/2,WHITE);
                }
            }}
            if(pathfound)DrawText(FormatText("PAth was found/is reachable.."),0,0,20,WHITE);
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;


}
