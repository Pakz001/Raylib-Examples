
#include "raylib.h"

    
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib example.");

     // Create a Image in memory
    RenderTexture2D player = LoadRenderTexture(16, 16);
    
    
    // Draw our ship..
    BeginTextureMode(player);    
    ClearBackground(BLANK);
    for(int y=player.texture.height;y>0;y--){
        BeginTextureMode(player);
        DrawLine(0+y/2,player.texture.height-y,player.texture.width-y/2,player.texture.height-y,WHITE);
        EndTextureMode();
    }
    
 
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Several variables we use for ship rotation speed and our current angle.
    int shipTurnSpeed=3;
    int shipAngle=0;

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        
        for(int i=0;i<shipTurnSpeed;i++){
            if(IsKeyDown(KEY_LEFT)){
                shipAngle--;
            }
            if(IsKeyDown(KEY_RIGHT)){
                shipAngle++;
            }
            if(shipAngle<0)shipAngle=359;
            if(shipAngle>359)shipAngle=0;
        }
        //----------------------------------------------------------------------------------
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(BLACK);
            DrawText("Use Cursor Left and Right to Rotate Ship - escape to exit.",2,2,20,DARKGRAY);

            // Draw a texture from rectangle to target rectangle. Note we use the - for opengl reasons!
            DrawTexturePro(player.texture,  (Rectangle){0,0,player.texture.width,-player.texture.height},
                                            (Rectangle){320,200,player.texture.width,player.texture.height},
                                            (Vector2){player.texture.width/2,player.texture.height/2},shipAngle,WHITE);            

            //(Rectangle){0,0,player.texture.width,-player.texture.height} == << what part of texture to draw
            // (Rectangle){320,200,player.texture.width,player.texture.height} == << where to draw it to on the screen
            //(Vector2){player.texture.width/2,player.texture.height/2} == << rotation center
            //shipAngle == << rotation in degrees
            //

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    UnloadRenderTexture(player);    // Unload render texture
 
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;


}
