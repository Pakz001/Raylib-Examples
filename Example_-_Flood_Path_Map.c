
#include "raylib.h"

// Our flood path function
void floodmap(int map[][10],int x, int y);

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;


    InitWindow(screenWidth, screenHeight, "raylib example.");



 
     // Create a Image in memory
    RenderTexture2D target = LoadRenderTexture(32, 32);
    
    int sprite[8][8] = {
    {0,1,0,0,0,0,1,0},
    {0,0,1,0,0,1,0,0},
    {0,0,0,2,2,0,0,0},
    {0,0,1,2,2,1,0,0},
    {0,1,0,2,2,0,1,0},
    {0,0,0,1,1,0,0,0},
    {0,0,2,3,3,2,0,0},
    {0,0,1,1,1,1,0,0}};
    // Clear our texture(image) before entering the game loop
    BeginTextureMode(target);    
    ClearBackground(BLANK); // Make the entire Sprite Transparent.
    EndTextureMode(); 
    // Draw something on it.
    for (int y=0;y<8;y++)
    {
        for (int x=0;x<8; x++)
        {            
            // Our sprite color 1
            if (sprite[y][x]==1)
            {
                BeginTextureMode(target);    
                DrawRectangle(x*4,y*4,4,4,BLUE);
                EndTextureMode(); 
            }
            // Our sprite color 2
            if (sprite[y][x]==2)
            {
                BeginTextureMode(target);    
                DrawRectangle(x*4,y*4,4,8,LIGHTGRAY);
                EndTextureMode(); 
            }            
            // Our sprite color 12
            if (sprite[y][x]==12)
            {
                BeginTextureMode(target);    
                DrawRectangle(x*4,y*4,4,8,BLACK);
                EndTextureMode(); 
            }            

        }
    }

    int map[10][10] = {0};
    floodmap(map,3,3);
    


    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        Vector2 mousePos = GetMousePosition();
        if(IsMouseButtonPressed(0)){
            int x = GetMouseX()/20;
            int y = GetMouseY()/20;
            if(x<10 && y<10)floodmap(map,x,y);
        }
        //----------------------------------------------------------------------------------
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            DrawText("Simple Floodfill pathmap.", 100, 180, 40, LIGHTGRAY);

            for(int y=0;y<10;y++){
            for(int x=0;x<10;x++){
                DrawText(TextFormat("%i",map[x][y]),x*20,y*20,20,BLACK);
            }}
            
           


            // NOTE: Render texture must be y-flipped due to default OpenGL coordinates (left-bottom)
            DrawTexture(target.texture,mousePos.x, mousePos.y, WHITE);
            
 

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadRenderTexture(target);    // Unload render texture    
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;


}

// Here we flood the map from the destination to the other parts of the map.
void floodmap(int map[][10],int x,int y){
    // Clear the old map
    for(int y=0;y<10;y++){
        for(int x=0;x<10;x++){
            map[x][y]=0;
    }}
    // set our destination
    map[x][y]=1;
    // flood map
    int low = 1;
    bool end=false;
    while(end==false){
        end = true;
        for(int y=0;y<10;y++){
        for(int x=0;x<10;x++){
            if(map[x][y]==low){                               
                if(map[x-1][y]==0){map[x-1][y]=low+1;end = false;}
                if(map[x+1][y]==0){map[x+1][y]=low+1;end = false;}
                if(map[x][y-1]==0){map[x][y-1]=low+1;end = false;}
                if(map[x][y+1]==0){map[x][y+1]=low+1;end = false;}
            }
        }        
        }
        low++;              
    }    
}
