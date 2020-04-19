
#include "raylib.h"
#include "string.h"

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib example.");
 
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------
    
    // We create a char with some text in it. It must end with '\0'
    char savethis[100]="is this allright?\0";
    // Save this string as test.txt to the current folder.
    SaveFileText("test.txt",savethis);
    // Create a char to let our pointer use.
    static char myloadstring[128];
    // Create a pointer to our char.
    static char *loadthis = myloadstring;
    // Load the text we saved previously into the loadthis.
    loadthis = LoadFileText("test.txt");

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
            
            // Draw the text from pointer char 'loadthis' to the screen.
            DrawText(loadthis,50,50,20,BLACK);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;


}
