
#include "raylib.h"

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib example.");

    // Create a Perlin Image
    Image perlinImage = GenImagePerlinNoise(screenWidth, screenHeight,0,0,1.5f); 
    
    // Images are inside the CPU and not fast. Use Textures(GPU) for quick manipulation.
    
    
    Texture2D texture = { 0 };

    texture = LoadTextureFromImage(perlinImage);

    // Unload image data (CPU RAM)
    UnloadImage(perlinImage);

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
                
            DrawTexture(texture, 0, 0, WHITE);           

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadTexture(texture);       // Unload texture from VRAM
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;


}

