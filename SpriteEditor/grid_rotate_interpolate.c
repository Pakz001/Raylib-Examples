//
// Interpolate seems to be like a blur function. Get the avarage color from the surrounding cells if that center cell is unused.
// With palette's take the most present palette from the surrounding cell when cell got lost in rotation(empty)
//
//


#include "raylib.h"
#include <math.h>

//
//
float angle=0; // radians
int debug=0;
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib example.");

    // Put some tiles in the map..
    int map[32][32] = {0};
    
    for (int y = 10; y < 32-10; y++)
    {
        for (int x = 10; x < 32-10; x++)
        {
            map[x][y]=1;
    }}

 
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        if(IsKeyDown(KEY_RIGHT)){
            angle+=0.1;
        }
        if(IsKeyDown(KEY_LEFT)){
            angle-=0.1;
        }
        //----------------------------------------------------------------------------------
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
            ClearBackground(RAYWHITE);

            int rotated[32][32]={-1};    
            for(int y=0;y<32;y++){
                for(int x=0;x<32;x++){
                    int x2=floor((cos(angle)*(x-16) - sin(angle)*(y-16)))+16;
                    int y2=floor((sin(angle)*(x-16) + cos(angle)*(y-16)))+16;
                    if(x2>-1 && y2>-1 && x2<32 && y2<32){
                        if(map[x][y]>0)rotated[x2][y2]=map[x][y];
                    }
                }
            }
            // interpolate
            for(int y=0;y<32;y++){
                for(int x=0;x<32;x++){
                    if(rotated[x][y]==0){
                        int cnt=0;
                        for(int y2=-1;y2<2;y2++){
                        for(int x2=-1;x2<2;x2++){
                            if(x2+x<0 || x2+x>31 || y2+y<0 || y2+y >31)continue;
                                if(rotated[x2+x][y2+y]==1)cnt++;                                
                        }}
                        
                        if(cnt>3){
                            rotated[x][y]=1;
                            debug = 99;
                        }
                    }
                    
            }}
            
            for(int y=0;y<32;y++){
                for(int x=0;x<32;x++){
                    if(rotated[x][y]==1){
                        DrawRectangle(x*16,y*8,16,8,RED);
                    }
            }}            

            DrawText(FormatText("%i",debug),0,0,30,BLACK);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;


}
