
#include "raylib.h"
#include <stdlib.h>           // Required for: free()

//
// How to draw into a image and then read the image data and use that. 
//

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib example.");

    // Create a Perlin Image
    Image tempImage = GenImageColor(32,32,BLACK); 
    ImageDrawCircle(&tempImage,16,16,10,RED);
    Color *mapPixels = GetImageData(tempImage);

    // We will put the image data from the rectangle in this map.
    int map[32][32] = {0};
    
    for (int y = 0; y < tempImage.height; y++)
    {
        for (int x = 0; x < tempImage.width; x++)
        {
            if ((mapPixels[y*tempImage.width + x].r != 0)){       // Collision: if other than 0.
            map[x][y] = 1;
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
                
            for(int y=0;y<32;y++){
                for(int x=0;x<32;x++){
                    if(map[x][y]==1)DrawRectangle(x*16,y*8,16,8,RED);
                }
            }

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    free(mapPixels);            // Unload color array
    UnloadImage(tempImage);             // Unload image from RAM
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;


}
