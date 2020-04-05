
#include "raylib.h"

// Let the compiler know we are using a function outside the main function.
// We need to pass the size of the last dimension of the array here.
void changemap(int map[][10],int x,int y);


int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib example.");

    int map[10][10] = {0};
    changemap(map,9,9);
 
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
                
            for(int y=0;y<10;y++){
                for(int x=0;x<10;x++){
                    DrawText(FormatText("%i",map[x][y]),x*20,y*20,20,BLACK);
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

// This is our function passed with a multi dim array
// It is outside the main function.
// void means it returns nothing.
// We need to pass it the last dimension size
void changemap(int map[][10],int x,int y){
    map[x][y] = 99;
}
