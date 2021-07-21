//
// Loses pixels in the progress. Trying to learn/do the reverse rotation.
//


#include "raylib.h"
#include <stdlib.h>           // Required for: free()

//
// How to draw into a image and then read the image data and use that. 
//
float angle=0;
float reverseangle=0;
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib example.");

    // Create a Perlin Image
    Image tempImage = GenImageColor(32,32,BLACK); 
    ImageDrawRectangleV(&tempImage,(Vector2){16-5,16-5},(Vector2){10,10},RED);
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
        if(IsKeyDown(KEY_RIGHT)){
            angle+=0.1;
            reverseangle=angle+3.142;
        }
        if(IsKeyDown(KEY_LEFT)){
            angle-=0.1;
            reverseangle=angle+3.142;
        }

        //----------------------------------------------------------------------------------
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
            //angle+=.1;
            ClearBackground(RAYWHITE);

            int temp[32][32]={0};    
            for(int y=0;y<32;y++){
                for(int x=0;x<32;x++){
                    float x2=(cos(angle)*(x-16) - sin(angle)*(y-16));
                    float y2=(sin(angle)*(x-16) + cos(angle)*(y-16));
                    //if(map[x+16][y+16]==1)DrawRectangle(x2*16+320,y2*8+240,16,8,RED);
                    x2+=16;
                    y2+=16;
                    if(x2>-1 && y2>-1 && x2<32 && y2<32){
                    temp[(int)x2][(int)y2]=map[x][y];
                    }
                }
            }
            for(int y=0;y<32;y++){
                for(int x=0;x<32;x++){
                    if(temp[x][y]!=0){
                        DrawRectangle(x*16,y*8,16,8,RED);
                    }
            }}            

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
