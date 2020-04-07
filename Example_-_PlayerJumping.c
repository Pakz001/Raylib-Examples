#include "raylib.h"
#include "math.h"


// our player struct
typedef struct player{
    Vector2 position;
    int width;
    int height;
    float jumpforce;
    bool canjump;
}player;

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib example.");

    const float ground = 200.0f;
 
    player myplayer = {0};
    myplayer.position = (Vector2){400,200};
    myplayer.canjump = true;
    myplayer.width = 32;
    myplayer.height = 48;
    myplayer.jumpforce = 0.0f;
 
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        
        // If we are currently not jumping and if the player presses space.
        if(IsKeyDown(KEY_SPACE) && myplayer.canjump){
            myplayer.canjump = false; // We are now jumping.
            myplayer.jumpforce = -4.0f; // How high we jump.
        }
        // If we are currently in a jump.
        if(myplayer.canjump==false){
            // Here we update the player y position.
            // We want to move 1 pixel at a time. This for collision detection(Not going through tiles.)
            for(int i=0;i<abs((int)myplayer.jumpforce);i++){
                // If we are still jumping then update the position.
                if(myplayer.canjump==false)myplayer.position.y += myplayer.jumpforce;
                // if we touch the ground then reset out position and set canjump to true.
                if(myplayer.position.y>ground){
                    myplayer.position.y = ground;
                    myplayer.canjump = true;
                    break;
                }                
            }
            // Here we add to the jumpforce. It will go from - to +.
            myplayer.jumpforce += 0.2f;            
        }
        //----------------------------------------------------------------------------------
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);
            
            DrawText("Press space to Jump.",200,240,40,DARKGRAY);
            // Draw our player.
            DrawRectangle(myplayer.position.x,myplayer.position.y,myplayer.width,myplayer.height,BLACK);    
            DrawCircle(myplayer.position.x+4,myplayer.position.y+6,4,WHITE);    
            DrawCircle(myplayer.position.x+myplayer.width-4,myplayer.position.y+6,4,WHITE);    

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;


}
