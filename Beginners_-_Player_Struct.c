
#include "raylib.h"

// Our player struct
typedef struct Player {
    Vector2 position;
    Vector2 size;
    float speed;
    int energy;
} Player;

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib example.");
 
    // Here we initialise our player.
    Player player = {0};
    player.position = (Vector2){400,200};
    player.size = (Vector2){32,48};
    player.speed = 1.5f;
    player.energy = 10.0f;
 
 
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
            
            // Draw our player.
            DrawRectangle(player.position.x,player.position.y,player.size.x,player.size.y,RED);
            // Draw our player info.
            DrawText(FormatText("Player x: %i ",player.position.x),0,0,20,BLACK);
            DrawText(FormatText("Player y: %i ",player.position.y),0,20,20,BLACK);
            DrawText(FormatText("Player speed: %02.02f ",player.speed),0,40,20,BLACK);
            DrawText(FormatText("Player energy: %i ",player.energy),0,60,20,BLACK);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;


}
