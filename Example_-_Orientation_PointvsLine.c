
#include "raylib.h"

int orientation(int ax,int ay,int bx, int by, int cx, int cy);

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib example.");
 
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    Vector2 line1a=(Vector2){200,100};
    Vector2 line1b=(Vector2){100,200};
    Vector2 point = (Vector2){300,150};

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // Put the point below the mouse.
        point = GetMousePosition();
        //----------------------------------------------------------------------------------
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);
                
            // Here we draw the first line.
            DrawLine(line1a.x,line1a.y,line1b.x,line1b.y,RED);
            DrawText("Back",line1a.x,line1a.y,10,BLACK);
            DrawText("Front",line1b.x,line1b.y,10,BLACK);
            // Draw the Point under the mouse.
            DrawCircle(point.x,point.y,10,RED);

            // Here we check if the mouse if left or right or on the same path of the line.
            // line starts at a.xy to b.xy.
            // This orientation function could be useful for turrets etc.
            int orien = orientation(line1a.x,line1a.y,line1b.x,line1b.y,point.x,point.y);
            if(orien==-1)DrawText("left",0,0,20,RED);
            if(orien==0)DrawText("Same",0,0,20,RED);
            if(orien==1)DrawText("right",0,0,20,RED);



        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;


}

//
// This is the orientation function. It returns -1 if the point is left of the inputted line.
// 0 if on the same and 1 if on the right of the line.
// aa,bb,point
int orientation(int ax,int ay,int bx, int by, int cx, int cy){
	if(((bx-ax)*(cy-ay)-(by-ay)*(cx-ax))<0)return -1;
    if(((bx-ax)*(cy-ay)-(by-ay)*(cx-ax))>0)return 1;
    return 0;
}
