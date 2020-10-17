// Based on my old game project "Dude Space Mining " : 
// https://github.com/Pakz001/Monkey2examples/blob/master/games/dudespacemining5.monkey27
// (Playable) https://cromdesi.home.xs4all.nl/emscripten/dudespacemining/Main.html
//
//
// Create tilemap system of 20x100by20x100 tiles. Player starts at position 10x10. In the Tilemap struct
// a map is created for this start position(10x10) When the map scrolls near the edge then a new section
// is created. In each map section a minable asteroid is created.


#include "raylib.h"

#define MAX_TILEMAPS 200

static int map[20][20];

typedef struct{
    Vector2 mapPosition;
}Player;


typedef struct {
    int map[100][100]; //20x100by20x100 tiles.
}Tilemap;

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib example.");
 
    // Create all the tilemaps, they will be generated when needed.
    Tilemap playerMaps[MAX_TILEMAPS] = { 0 };

 
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
                

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;


}
