

#include "raylib.h"
// string.h
// for strlen()=length of string, strcmp()=compare two strings, 
// strlwr()=convert string to lower case, strupr()=convert string to upper case.
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

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        
        char lenny[10] = "hello"; 
        char com1[] = "abc";
        char com2[] = "abc";
        char com3[] = "abcd";
        char lower[] = "IsThisLow?";
        char higher[] = "IsThisHigh?";
        //----------------------------------------------------------------------------------
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);
                
            DrawText(FormatText("Length of lenny is : %i",strlen(lenny)),100,200,20,DARKGRAY);
            // strlen returns the amount of characters in the char.
            if(strcmp(com1,com2)==0){
                DrawText("chars com1 and com2 have same contents.",100,220,20,DARKGRAY);
            }else{
                DrawText("chars com1 and com2 do not have same contents.",100,220,20,DARKGRAY);
            }
            if(strcmp(com1,com3)==0){
                DrawText("chars com1 and com2 have same contents.",100,240,20,DARKGRAY);
            }else{
                DrawText("chars com1 and com2 do not have same contents.",100,240,20,DARKGRAY);
            }
            // with strcmp it returns <0 or >0 if the char is different. ==0 if the same.
            DrawText(FormatText("char lower in lower case is : %s",strlwr(lower)),100,260,20,DARKGRAY);
            DrawText(FormatText("char higher in higher case is : %s",strupr(higher)),100,280,20,DARKGRAY);
            

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;


}
