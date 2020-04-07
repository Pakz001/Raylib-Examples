
#include "raylib.h"


typedef struct player{
    Vector2 position;
}player;

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib example.");

    // Our player setup
    player myplayer = {0};
    myplayer.position = (Vector2){5,5};

    // Our tile width and height. We use the screen dimension and the map dimension to 
    // get tile dimensions that fill up the entire screen.
    float tilewidth = (float)screenWidth/20.0f;
    float tileheight = (float)screenHeight/10.0f;

    // This is out collision map.
    bool colmap[20][20] = {false};
    // This is our tile map.
    int map[10][20] = { {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
                        {1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,1,1,1,1},
                        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1},
                        {1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,1,1,1,1},
                        {1,1,1,1,1,1,1,1,0,1,1,1,1,0,0,1,1,1,1,1},
                        {1,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,1},
                        {1,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,1},
                        {1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,1},
                        {1,0,0,0,0,0,0,1,0,0,0,1,1,1,1,0,0,0,0,1},
                        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}};    
    // Copy the contents of the map into the collision map. 
    for(int y=0;y<10;y++){
        for(int x=0;x<20;x++){
            if(map[y][x]>0)colmap[y][x]=true;
        }
    }
    
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        
        // Below here we read the key. If the l/r/u/d cursor is pressed then we check the new position is inside
        // the array bounds and then we check if the collision map on the new position is not set to flag true on that tile.
        if(IsKeyPressed(KEY_RIGHT)&& myplayer.position.x+1<20 && colmap[(int)myplayer.position.y][(int)myplayer.position.x+1]==false) {
            myplayer.position.x+=1;
        }
        if(IsKeyPressed(KEY_LEFT) && myplayer.position.x-1>-1 && colmap[(int)myplayer.position.y][(int)myplayer.position.x-1]==false) {
            myplayer.position.x-=1;
        }
        if(IsKeyPressed(KEY_UP)&& myplayer.position.y-1>-1 && colmap[(int)myplayer.position.y-1][(int)myplayer.position.x]==false) {
            myplayer.position.y-=1;
        }
        if(IsKeyPressed(KEY_DOWN) && myplayer.position.y+1<20 && colmap[(int)myplayer.position.y+1][(int)myplayer.position.x]==false) {
            myplayer.position.y+=1;
        }

        //----------------------------------------------------------------------------------
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);
                
            // Draw our tilemap.
            for(int y=0;y<10;y++){
                for(int x=0;x<20;x++){
                    if(map[y][x]==1){
                        DrawRectangle(x*tilewidth,y*tileheight,tilewidth,tileheight,BLACK);
                    }
                }
            }
            // Draw our player
            DrawRectangle(myplayer.position.x*tilewidth,myplayer.position.y*tileheight,tilewidth,tileheight,RED);
            
            DrawText("Use Cursor Keys to move around..",0,0,20,WHITE);
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;


}
