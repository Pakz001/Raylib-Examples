//
// POINTTILECOLLIDE function
//
// Here is a example of how to check collision with a point and a tile on a tilemap.
//

#include "raylib.h"
#include <math.h>

#define MAX_CARS 100
#define MAX_WHISKERS 8

   
int myMap[10][11] =  {  {1,1,1,1,1,1,1,1,1,1,1},
                        {1,0,0,0,0,0,0,0,0,0,1},
                        {1,0,1,1,1,1,1,1,1,0,1},
                        {1,0,1,1,1,1,1,0,0,0,1},
                        {1,0,1,0,0,0,1,0,1,1,1},
                        {1,0,1,0,1,0,1,0,0,0,1},
                        {1,0,1,0,1,0,1,1,1,0,1},
                        {1,0,1,0,1,0,1,1,1,0,1},
                        {1,0,0,0,1,0,0,0,0,0,1},
                        {1,1,1,1,1,1,1,1,1,1,1}
                    };  
int mapWidth = 11;
int mapHeight = 10;
float tileWidth;
float tileHeight;


//Unit collide with solid blocks true/false
bool recttilecollide(int x, int y, int w, int h, int offsetx,int offsety);
// Our rectsoverlap function. Returns true/false.
static bool rectsoverlap(int x1,int y1,int w1,int h1,int x2,int y2,int w2,int h2);
                        
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 600;
    tileWidth = ceil((float)(float)screenWidth/(float)mapWidth);
    tileHeight = ceil((float)screenHeight/(float)mapHeight);

    InitWindow(screenWidth, screenHeight, "raylib example.");
 
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    Vector2 position;
    position.x = 100;
    position.y = 100;
    int time=0;
    Color collisionColor = GREEN;
    
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        time++;
        if(time>30){ // every time time is at 300
            time=0;//reset time
            position.x = GetRandomValue(32,screenWidth-32);
            position.y = GetRandomValue(32,screenHeight-32);
        }
        
        if(recttilecollide(position.x,position.y,tileWidth/2, tileHeight/2, 0,0)){
            collisionColor = RED;
        }else{
            collisionColor = GREEN;
        }
        
        //----------------------------------------------------------------------------------
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);
            
            // Draw map
            for (int y = 0; y< mapHeight ; y++)
            {
                for (int x = 0; x< mapWidth ; x++)
                {
                    if (myMap[y][x] == 1)
                    {
                        DrawRectangle(x*tileWidth,y*tileHeight,tileWidth,tileHeight,BLUE);
                    }
                }
            }
            
            DrawRectangle(position.x,position.y,tileWidth/2,tileHeight/2,collisionColor);
                

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;


}



//Unit collide with solid blocks true/false
bool recttilecollide(int x, int y,int w, int h, int offsetx,int offsety){
    int cx = (x+offsetx)/tileWidth;
    int cy = (y+offsety)/tileHeight;
    for(int y2=cy-1; y2<cy+2;y2++){//Note that the - and + are to be set differently with differently sized players
    for(int x2=cx-1; x2<cx+2;x2++){
        if(x2>=0 && x2<mapWidth && y2>=0 && y2<mapHeight){
            if(myMap[y2][x2] == 1){
                int x3 = (x2)*tileWidth;
                int y3 = (y2)*tileHeight;
                if(rectsoverlap(x+offsetx,y+offsety,w,h,x3,y3,tileWidth,tileHeight)){
                    return true;
                }
            }
        }
    }}
    return false;
}

// Rectangles overlap
bool rectsoverlap(int x1,int y1,int w1,int h1,int x2,int y2,int w2,int h2){
    if(x1 >= (x2 + w2) || (x1 + w1) <= x2) return false;
    if(y1 >= (y2 + h2) || (y1 + h1) <= y2) return false;
    return true;
}
