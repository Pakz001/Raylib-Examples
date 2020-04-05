
#include "raylib.h"

void floodit();

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib example.");


    int mymap[50][50] = {0};

    for (int i=0;i<20;i++){
    int y=0;
    if (GetRandomValue(0,1) == 1) y=1;
    mymap[10+i][25+y] = 1;
    
    }


    for (int i=0;i<31550;i++)
    {
        int x = GetRandomValue(1,49);
        int y = GetRandomValue(1,49);
        if (mymap[y][x] == 1)
        {
            switch (GetRandomValue(0,3))
            {
                case 0:
                mymap[y-1][x]=1;
                break;
                case 1:
                mymap[y+1][x]=1;
                break;
                case 2:
                mymap[y][x-1]=1;
                break;
                case 3:
                mymap[y][x+1]=1;
                break;
            }
        }
        
    }

 
     // Create a Image in memory
    RenderTexture2D target = LoadRenderTexture(screenWidth, screenHeight);
    
    // Clear our texture(image) before entering the game loop
    BeginTextureMode(target);    
    ClearBackground(BLUE);
    EndTextureMode(); 
    // Draw something on it.
    for (int y=0;y<50;y++)
    {
        for (int x=0;x<50; x++)
        {            
            if (mymap[y][x]==1)
            {
                BeginTextureMode(target);    
                DrawRectangle(x*16,y*8,16,8,BLACK);
                EndTextureMode(); 
            }
        }
    }

    
 
 
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


            // NOTE: Render texture must be y-flipped due to default OpenGL coordinates (left-bottom)
            DrawTextureRec(target.texture, (Rectangle){ 0, 0, target.texture.width, -target.texture.height }, (Vector2){ 0, 0 }, WHITE);
            
 
            DrawText("Switch/array/hillalgo.", 100, 180, 40, LIGHTGRAY);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;


}

