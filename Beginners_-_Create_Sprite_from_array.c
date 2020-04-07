
#include "raylib.h"

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib example.");



 
     // Create a Image in memory
    RenderTexture2D target = LoadRenderTexture(32, 32);
    
    int sprite[8][8] = {{1,1,1,1,0,0,0,0},
                        {0,1,1,1,0,0,0,0},
                        {0,0,1,1,1,2,2,2},
                        {0,0,0,1,1,2,2,2},
                        {0,0,0,1,1,2,2,2},
                        {1,1,1,1,1,2,2,2},
                        {0,0,1,1,1,2,2,2},
                        {0,0,0,0,0,2,2,2}};
    
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
                DrawRectangle(x*4,y*4,4,4,RED);
                EndTextureMode(); 
            }
            // Our sprite color 2
            if (sprite[y][x]==2)
            {
                BeginTextureMode(target);    
                DrawRectangle(x*4,y*4,4,8,BLACK);
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
        Vector2 mousePos = GetMousePosition();
        //----------------------------------------------------------------------------------
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            DrawText("Sprite Creation from data.", 100, 180, 40, LIGHTGRAY);

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

