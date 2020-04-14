
#include "raylib.h"


int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 400;

    InitWindow(screenWidth,screenHeight, "raylib example.");
    

     // Create a Image in memory
    RenderTexture2D target = LoadRenderTexture(320,200);
    
    // Clear our texture(image) before entering the game loop
    BeginTextureMode(target);    
    ClearBackground(BLUE);
    // Draw something on it.
    DrawRectangle(10,10,300,180,BLACK);

    EndTextureMode();               // This needs to be called after every different draw command used. Do not forget to use begintexture also..
 

 
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
            
            // Draw a texture from rectangle to target rectangle. Note we use the - for opengl reasons!
            DrawTexturePro(target.texture,  (Rectangle){0,0,target.texture.width,-target.texture.height},
                                            (Rectangle){0,0,screenWidth,screenHeight},
                                            (Vector2){0,0},0,WHITE);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadRenderTexture(target);    // Unload render texture
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;


}
