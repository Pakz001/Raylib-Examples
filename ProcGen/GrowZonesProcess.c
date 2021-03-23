//
// Create zones - this version kind of creates those military camouflage patterns.
//
#include "raylib.h"

int numzones = 30;
int map[100][100];

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib example.");
 
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // First put the id's on the map
    for(int i=0;i<numzones;i++){
        int x = GetRandomValue(0,100);
        int y = GetRandomValue(0,100);
        map[x][y] = i;
    }
    // grow the zones. pick a random spot. with that value grow one around it.
    for(int i=0;i<100*100*100;i++){
        int x = GetRandomValue(1,99);
        int y = GetRandomValue(1,99);
        int z = map[x][y];
        if(z>0){
            int y1=GetRandomValue(-1,1);
            int x1=GetRandomValue(-1,1);
            if(map[x+x1][y+y1]==0){
                map[x+x1][y+y1]=z;
            }
        }
    }
    // pick random spot. With that value draw a rect there (3*3) (blockize)
    for(int i=0;i<100*100*100;i++){
        int x = GetRandomValue(1,99);
        int y = GetRandomValue(1,99);
        int z = map[x][y];
        if(z>0){
            for(int y1=-1;y1<2;y1++){
            for(int x1=-1;x1<2;x1++){
                if(x+x1<0 || y+y1<0 || x+x1>99 || y+y1>99) continue;
                map[x+x1][y+y1]=z;
            }}
        }
        
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
            
            for(int y=0;y<100;y++){
            for(int x=0;x<100;x++){
                int m=map[x][y];
                float col = (255.0f/(float)numzones)*(float)m;
                Color c = (Color){col,col,col,255};
                
                DrawRectangle(x*7,y*4,7,4,c);
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
