
#include "raylib.h"
#define MAX_BULLETS 10

struct blueBullet{
    bool active;
    Vector2 position;
    Vector2 movement;
}blueBullet;

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib example.");
 
    // This is one way to create a new array of struct!
    struct blueBullet arr_blueBullet[2] =       {  
                                                {true,(Vector2){400,200},(Vector2){2.5f,0.0f}},
                                                {true,(Vector2){400,200},(Vector2){-2.5f,0.0f}}
                                                };
 
    // This is another way of creating a array of struct.
    struct blueBullet arr_blueBullet2[MAX_BULLETS];
    
    for(int i=0;i<MAX_BULLETS;i++){
        arr_blueBullet2[i].active = true;
        arr_blueBullet2[i].position = (Vector2){400,200};
        arr_blueBullet2[i].movement = (Vector2){(float)GetRandomValue(-5,5),(float)GetRandomValue(-5,5)};
    }
 
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // Update blueBullets
        for(int i=0;i<2;i++){
            if(arr_blueBullet[i].active){
                arr_blueBullet[i].position = (Vector2){ arr_blueBullet[i].position.x+arr_blueBullet[i].movement.x,
                                                        arr_blueBullet[i].position.y+arr_blueBullet[i].movement.y};
                // If outside the screen reset position.
                if(     arr_blueBullet[i].position.x<-10 || 
                        arr_blueBullet[i].position.x>screenWidth+10 || 
                        arr_blueBullet[i].position.y<-10 || 
                        arr_blueBullet[i].position.y>screenHeight-10){                    
                    arr_blueBullet[i].position = (Vector2){400,200};
                }
            }
        }

        // Update bluebullets2
        for(int i=0;i<MAX_BULLETS;i++){
            if(arr_blueBullet2[i].active){
                arr_blueBullet2[i].position = (Vector2){    arr_blueBullet2[i].position.x+arr_blueBullet2[i].movement.x,
                                                            arr_blueBullet2[i].position.y+arr_blueBullet2[i].movement.y};
                // If outside the screen reset position.
                if(     arr_blueBullet2[i].position.x<-10 || 
                        arr_blueBullet2[i].position.x>screenWidth+10 || 
                        arr_blueBullet2[i].position.y<-10 || 
                        arr_blueBullet2[i].position.y>screenHeight-10){                    
                    arr_blueBullet2[i].position = (Vector2){400,200};
                }

            }
        }
        
        //----------------------------------------------------------------------------------
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);
            
            // Draw blueBullets
            for(int i=0;i<2;i++){
                if(arr_blueBullet[i].active){
                    DrawRectangle(arr_blueBullet[i].position.x,arr_blueBullet[i].position.y,5,5,BLUE);
                }
            }

            // Draw blueBullets2
            for(int i=0;i<MAX_BULLETS;i++){
                if(arr_blueBullet2[i].active){
                    DrawRectangle(arr_blueBullet2[i].position.x,arr_blueBullet2[i].position.y,5,5,BLUE);
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
