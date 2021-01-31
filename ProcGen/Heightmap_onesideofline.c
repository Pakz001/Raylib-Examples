
#include "raylib.h"
#include <math.h>

#define MAP_SIZE 256

int map[MAP_SIZE][MAP_SIZE];
int heightmap[MAP_SIZE][MAP_SIZE];

void mapline(int x1, int y1, int x2, int y2);
void scanlinebufferfill(int x, int y, int value);

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 512;

    InitWindow(screenWidth, screenHeight, "raylib example.");
 
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Initialise a empty heightmap!
    for(int y=0;y<MAP_SIZE;y++){
    for(int x=0;x<MAP_SIZE;x++){
        heightmap[x][y]=0;
    }}

    //
    // The code here below creates the heightmap. We want the height to be with a max height
    // of 255 and loop the code until this happens. We do this by finding 2 points on a empty
    // map array. This line will start at an edge and end at an other edge thus dividing the
    // map into two areas. We then pick a random spot on the map and fill this area. This
    // filled area is then used to add to the heightmap. (one part of a map divided into two parts
    // every time) This creates the heightmap.
    //
    int highest=0;
    while(highest<256){
    //for(int i=0;i<1;i++){
        for(int y=0;y<MAP_SIZE;y++){
        for(int x=0;x<MAP_SIZE;x++){
            map[x][y]=0;
        }}
        int side1=GetRandomValue(0,3);
        int sl[3];
        int cnt=0;
        for(int j=0;j<4;j++){
            if(j==side1)continue;
            sl[cnt]=j;
            cnt++;
        }
        int side2 = sl[GetRandomValue(0,3)];
        int x1=0;
        int y1=0;
        int x2=0;
        int y2=0;
        // get coordinates side 1
        if(side1 == 0){
            x1 = 0;
            y1 = GetRandomValue(0,MAP_SIZE-1);
        }
        if(side1 == 1){
            x1 = GetRandomValue(0,MAP_SIZE-1);
            y1 = 0;
        }
        if(side1 == 2){
            x1 = MAP_SIZE-1;
            y1 = GetRandomValue(0,MAP_SIZE-1);
        }
        if(side1 == 3){
            x1 = GetRandomValue(0,MAP_SIZE-1);
            y1 = MAP_SIZE-1;
        }
        // get coordinates side 2
        if(side2 == 0){
            x2 = 0;
            y2 = GetRandomValue(0,MAP_SIZE-1);
        }
        if(side2 == 1){
            x2 = GetRandomValue(0,MAP_SIZE-1);
            y2 = 0;
        }
        if(side2 == 2){
            x2 = MAP_SIZE-1;
            y2 = GetRandomValue(0,MAP_SIZE-1);
        }
        if(side2 == 3){
            x2 = GetRandomValue(0,MAP_SIZE-1);
            y2 = MAP_SIZE-1;
        }
        mapline(x1,y1,x2,y2);
        scanlinebufferfill(GetRandomValue(0,MAP_SIZE-1),GetRandomValue(0,MAP_SIZE-1),1);
        
        for(int y=0;y<MAP_SIZE;y++){
        for(int x=0;x<MAP_SIZE;x++){
           // Add this line below and remove the other one below that one for a slightly noisy heightmap!
           //if(map[x][y]==1 && GetRandomValue(0,100)<99)heightmap[x][y]++; 
           if(map[x][y]==1)heightmap[x][y]++; 
           // Here we create the heightmap
           if(heightmap[x][y]>highest)highest=heightmap[x][y];
        }}

        

    }



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
            int highest=0;

            // Draw the heightmap.
            for(int y=0;y<MAP_SIZE;y++){
            for(int x=0;x<MAP_SIZE;x++){
                if(heightmap[x][y]>highest)highest=heightmap[x][y];
                if(heightmap[x][y]>0 && heightmap[x][y]<256){
                    int col=heightmap[x][y];
                    //col=255;
                    DrawRectangle(x*2,y*2,2,2,(Color){col,col,col,255});
                }
            }
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

// A bresenham line function that draws a line inside a mapp[][] array.
//
void mapline(int x1, int y1, int x2, int y2){
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
        map[x1][y1]=2;
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



void scanlinebufferfill(int x, int y, int value){
    Vector2 list[MAP_SIZE*MAP_SIZE] = {(Vector2){-1,-1}};
    int listsize=0;
    list[listsize] = (Vector2){x,y};
    while(listsize>-1){
        Vector2 pos = list[listsize];
        listsize--;
        
        map[(int)pos.x][(int){pos.y}]=1;
        if(pos.y-1>-1 && map[(int)pos.x][(int)pos.y-1]==0){
            listsize++;
            list[listsize] = (Vector2){pos.x,pos.y-1};
        }
        if(pos.y+1<MAP_SIZE && map[(int)pos.x][(int)pos.y+1]==0){
            listsize++;
            list[listsize] = (Vector2){pos.x,pos.y+1};
        }
        if(pos.x-1>-1 && map[(int)pos.x-1][(int)pos.y]==0){
            listsize++;
            list[listsize] = (Vector2){pos.x-1,pos.y};
        }
        if(pos.x+1<MAP_SIZE && map[(int)pos.x+1][(int)pos.y]==0){
            listsize++;
            list[listsize] = (Vector2){pos.x+1,pos.y};
        }

    }
}
