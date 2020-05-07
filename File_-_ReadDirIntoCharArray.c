
#include "raylib.h"
#include <dirent.h>
#include <stdio.h>
#include <limits.h> 
#include <string.h>

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib example.");
 
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Out char where we store the filenames.
    char files[10024][PATH_MAX];
    int numfiles=0;
    // Here we read the current dir(where this example is run)
    // and copy the filename into the files char array.
    DIR *d;
    struct dirent *dir;
    d = opendir(".");
    if (d)
    {
        
        while ((dir = readdir(d)) != NULL)
        {
            strcpy(files[numfiles],dir->d_name);
            numfiles++;
        }
        closedir(d);
    }    

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
                
            DrawText(FormatText("Number of files in dir : %i",numfiles),10,10,20,BLACK);
            // Draw the first chunk of the dir to the screen.
            if(numfiles>0){
                int x=0;
                int y=20;
                for(int i=0;i<numfiles;i++){
                    DrawText(FormatText("%s",files[i]),10+x,10+y,10,BLACK);
                    y+=10;
                    if(y>screenHeight-50){
                        x+=200;
                        y=20;
                    }
                }
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
