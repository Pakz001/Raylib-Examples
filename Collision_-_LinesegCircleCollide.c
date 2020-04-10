
#include "raylib.h"
#include <math.h>

static bool linecirclecollide(int sx1, int sy1, int sx2, int sy2, int cx, int cy, float cr);

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
        
        Vector2 linea=(Vector2){GetMouseX(),GetMouseY()};
        Vector2 lineb=(Vector2){linea.x-100,linea.y+50};
        Vector2 circle=(Vector2){400,200};
        float circleradius = 50;
        
        //----------------------------------------------------------------------------------
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);
            
            DrawLine(linea.x,linea.y,lineb.x,lineb.y,RED);
            DrawCircle(circle.x,circle.y,circleradius,YELLOW);
            
            if(linecirclecollide(linea.x,linea.y,lineb.x,lineb.y,circle.x,circle.y,circleradius))
            DrawText("Something is a foot..",10,10,30,BLUE);
        

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;


}


//'
//' Line(segment) to Circle Collision//
//'
bool linecirclecollide(int x1, int y1, int x2, int y2, int xc, int yc, float r){

    float xd = 0.0f;
    float yd = 0.0f;
    float t = 0.0f;
    float d = 0.0f;
    float dx = 0.0f;
    float dy = 0.0f;

    dx = x2 - x1;
    dy = y2 - y1;

    t = ((yc - y1) * dy + (xc - x1) * dx) / (dy * dy + dx * dx);

    if(0 <= t && t <= 1)
    {
        xd = x1 + t * dx;
        yd = y1 + t * dy;

        d = sqrt((xd - xc) * (xd - xc) + (yd - yc) * (yd - yc));
        return d <= r;
    }
    else
    {
        d = sqrt((xc - x1) * (xc - x1) + (yc - y1) * (yc - y1));
        if(d <= r)
        {
            return true;
        }
        else
        {
            d = sqrt((xc - x2) * (xc - x2) + (yc - y2) * (yc - y2));
            if(d <= r)
            {
                return true;
            }
            else
            {
                return false;
            }
            
        }
    }

}
