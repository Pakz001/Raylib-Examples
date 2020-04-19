
#include "raylib.h"
#include "string.h"

#define MAX_ITEMS 10
#define MAX_ITEMSSSIZE 32

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib example.");
 
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------
    // Maxim number of items here is 9. End with 'End'
    char myitems[MAX_ITEMS][MAX_ITEMSSSIZE] = {
    "Sword",
    "Shield",
    "Axe",
    "Helmet",
    "End"
    };

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
            
            // Draw the items on the screen and exit if the item is "End"
            DrawText("Inventory:",70,80,20,BLACK);
            for(int i=0;i<MAX_ITEMS-1;i++){                
                DrawText(FormatText("%i",i),70,100+i*20,20,BLACK);
                DrawText(myitems[i],100,100+i*20,20,BLACK);
                if(strcmp(myitems[i+1],"End")==0)break;
            }

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;


}
