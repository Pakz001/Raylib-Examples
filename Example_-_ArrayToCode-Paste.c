
#include "raylib.h"
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
    int spritewidth = 8;
    int spriteheight = 8;

    
    int sprite[8][8] = {{1,1,1,1,0,0,0,9},
                        {0,1,1,1,0,0,0,0},
                        {0,0,1,1,1,2,2,2},
                        {0,0,0,1,1,2,2,2},
                        {0,0,0,1,1,2,2,2},
                        {1,1,1,1,1,2,2,2},
                        {0,0,1,1,1,2,2,2},
                        {0,0,0,0,0,2,2,2}};

    // Here we create a copy and paste ready
    // text that can be pasted into a code editor.
    // It shows how to convert a array into c language code.
    char output1[1000] = "int sprite[";
    char output2[32];
    sprintf(output2, "%d", spritewidth);    
    strcat(output1,output2);
    strcat(output1,"][");
    sprintf(output2, "%d", spriteheight);    
    strcat(output1,output2);
    strcat(output1,"] = {\n");
    
    int x=0;int y=0;
    for (y=0;y<spriteheight;y++){

        for(x=0;x<spritewidth-1;x++){
            if(x==0)strcat(output1,"{");
            char num[16];
            sprintf(num, "%d", sprite[y][x]);    
            strcat(output1,num);
            strcat(output1,",");
        }
        char num2[16];
        sprintf(num2, "%d", sprite[y][spritewidth-1]);    
        strcat(output1,num2);
        if(y<spriteheight-1){
            strcat(output1,"},\n");
        }else{
        strcat(output1,"}};\n");
        }
    }

    SetClipboardText(output1);
    
    
    

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

            DrawText("Check your clipboard buffer CTRL+V",50,100,20,DARKGRAY);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;


}
