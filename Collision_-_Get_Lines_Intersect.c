// Segmented line vs circle collision.

#include "raylib.h"

bool get_line_intersect(float p0_x, float p0_y, float p1_x, float p1_y, float p2_x ,float p2_y, float p3_x, float p3_y);

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib example.");

    // Create 4 vectors for 2 lines..
    Vector2 line1a=(Vector2){100,100};
    Vector2 line1b=(Vector2){200,200};
    Vector2 line2a;
    Vector2 line2b;
 
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        
        // The second line is below the mouse pointer.
        line2a.x = GetMouseX();
        line2a.y = GetMouseY();
        line2b.x = line2a.x - 100;
        line2b.y = line2a.y + 100;
        
        //----------------------------------------------------------------------------------
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);
                    
               
            // Here we draw the first line.
            DrawLine(line1a.x,line1a.y,line1b.x,line1b.y,RED);
            // Here we draw the second line, the one below the mouse pointer.
            DrawLine(line2a.x,line2a.y,line2b.x,line2b.y,GREEN);
            
            // Here we check if the two lines on the screen touch each other.
            if(get_line_intersect(line1a.x,line1a.y,line1b.x,line1b.y,line2a.x,line2a.y,line2b.x,line2b.y)){
                DrawText("Collision between lines!!",0,0,40,RED); // Let the user know there has been a collision.
            }else{ // If no lines touch then show a message.
                DrawText("Move the mouse line on the other line!!",0,0,20,BLACK);
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

// This function was originally by andre la moth.
bool get_line_intersect(float p0_x, float p0_y, float p1_x, float p1_y, float p2_x ,float p2_y, float p3_x, float p3_y){
    float s1_x;
    float s1_y; 
    float s2_x; 
    float s2_y;
    
    s1_x = p1_x - p0_x;     
    s1_y = p1_y - p0_y;
    s2_x = p3_x - p2_x;
    s2_y = p3_y - p2_y;

    float s;
    float t;
    
    s = (-s1_y * (p0_x - p2_x) + s1_x * (p0_y - p2_y)) / (-s2_x * s1_y + s1_x * s2_y);
    t = ( s2_x * (p0_y - p2_y) - s2_y * (p0_x - p2_x)) / (-s2_x * s1_y + s1_x * s2_y);

    if(s >= 0 && s <= 1 && t >= 0 && t <= 1) return true;
    
    return false; // No collision    
}
