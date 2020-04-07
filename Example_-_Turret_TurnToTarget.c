
#include "raylib.h"
#include <math.h> // for cos and sin

// Return on which side of the line the point is. l-1 0= r=1
// lineback x,y linefront x,y point x,y
int orientation(int ax,int ay,int bx, int by, int cx, int cy);

typedef struct turret{
    float isactive;
    float angle;
    Vector2 position;
    Vector2 barreltip; // tip of the cannon
    Vector2 target;
}turret;

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib example.");
 
    turret myturret={0};
    myturret.isactive = true;
    myturret.angle = 0.0f;
    myturret.position = (Vector2){400,200};
 
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        
        // The turret section.
        if(myturret.isactive){
            // Get the target coordinates.
            myturret.target = GetMousePosition();
            // Figure out if the target is left or right or center of the barrel.
            int o = orientation(myturret.position.x,myturret.position.y,myturret.barreltip.x,myturret.barreltip.y,myturret.target.x,myturret.target.y);            
            if(o==-1){ // target is left of the barrel so turn left.
                myturret.angle-=0.01f;            
            }else if(o==1){//target is right of the barrel so turn right.
                myturret.angle+=0.01f;
            }else{
            }
            // Keep the angle value between 0..360
            if(myturret.angle<0)myturret.angle=myturret.angle+360;
            if(myturret.angle>360)myturret.angle=0+myturret.angle;
            // Get the new barreltip vector.
            myturret.barreltip = (Vector2){myturret.position.x+(cos(myturret.angle)*16),myturret.position.y+(sin(myturret.angle)*16)};
        }
        
        
        //----------------------------------------------------------------------------------
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);
                
            //draw the turret.
            DrawCircle(myturret.position.x,myturret.position.y,10,DARKGRAY);            
            DrawLineEx(myturret.position,myturret.barreltip,5,RED);

            DrawText("Move the mouse around the turret to see it turn.",0,0,30,DARKGRAY);
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
