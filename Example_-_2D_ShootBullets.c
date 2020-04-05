
#include "raylib.h"
#include <stdlib.h>
// math.h is needed for cos and sin.
#include <math.h>

#define MAX_BULLETS 100
#define MAX_BATCH_ELEMENTS 8192

typedef struct Bullets{
    Vector2 position;
    bool active;
    float angle;
} Bullets;

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib example.");

    Bullets *mybullets = (Bullets *)malloc(MAX_BULLETS*sizeof(Bullets));    // Bullets array
    mybullets[0].active=false;
    

     // Create a Image in memory
    RenderTexture2D target = LoadRenderTexture(32, 32);
    
    int sprite[8][8] = {
    {0,2,1,0,0,1,2,0},
    {2,1,0,2,2,0,1,2},
    {1,0,2,1,1,2,0,1},
    {0,2,1,1,1,1,2,0},
    {0,2,1,1,1,1,2,0},
    {1,0,2,1,1,2,0,1},
    {2,1,0,2,2,0,1,2},
    {0,2,1,0,0,1,2,0}};
    
    // Clear our texture(image) before entering the game loop
    BeginTextureMode(target);    
    ClearBackground(BLANK); // Make the entire Sprite Transparent.
    EndTextureMode(); 
    // Draw something on it.
    for (int y=0;y<8;y++)
    {
        for (int x=0;x<8; x++)
        {            
            // Our sprite color 1
            if (sprite[y][x]==1)
            {
                BeginTextureMode(target);    
                DrawRectangle(x*4,y*4,4,4,WHITE);
                EndTextureMode(); 
            }
            // Our sprite color 2
            if (sprite[y][x]==2)
            {
                BeginTextureMode(target);    
                DrawRectangle(x*4,y*4,4,4,RED);
                EndTextureMode(); 
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
        // Update the bullets
        // Deactivate if outside window.
        for(int i=0;i<MAX_BULLETS;i++){
            if(mybullets[i].active){
                mybullets[i].position.x+=(float)cos(mybullets[i].angle)*3;
                mybullets[i].position.y+=(float)sin(mybullets[i].angle)*3;
                if(mybullets[i].position.x<-50 || mybullets[i].position.x>screenWidth+50 || mybullets[i].position.y<-50 || mybullets[i].position.y>screenHeight+50){
                    mybullets[i].active = false;
                }                
            }
        }
       // Shoot a bullet
        if(IsMouseButtonReleased(0)){
            for(int i=0;i<MAX_BULLETS;i++){    
                if(mybullets[i].active==false){
                    mybullets[i].active=true;
                    mybullets[i].angle = GetRandomValue(0,359);
                    mybullets[i].position = GetMousePosition();
                    break;
                }
            }
        }
        //----------------------------------------------------------------------------------
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);
                
            DrawText("Press the left Mouse Button to Shoot button from mouse Pos.",0,100,20,BLACK);
                
            for(int i=0;i<MAX_BULLETS;i++){
                if(mybullets[i].active){
                    DrawTexture(target.texture,mybullets[i].position.x,mybullets[i].position.y,WHITE);
            }}
            

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    free(mybullets);              // Unload points data array
    UnloadRenderTexture(target);    // Unload render texture    
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;


}
