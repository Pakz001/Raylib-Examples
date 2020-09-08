//
// Modified example of raylib.com !!
//
// Will change later on..
//
// I was watching a video of the game 'Gaia beyond' a space rpg where there were tiny space ships
// and rockets with trails. I wanted to head into the direction of coding a small version of this.
// Here I looked into how to do the smoke trails.. I'm not using a external image for the smoke
// but created one in the code here.
//

#include "raylib.h"

#define MAX_PARTICLES 200

// Particle structure with basic data
typedef struct {
    Vector2 position;
    Color color;
    float alpha;
    float size;
    float rotation;
    bool active;        // NOTE: Use it to activate/deactive particle
} Particle;

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - generate random values");


    // Particles pool, reuse them!
    Particle mouseTail[MAX_PARTICLES] = { 0 };

    // Initialize particles
    for (int i = 0; i < MAX_PARTICLES; i++)
    {
        mouseTail[i].position = (Vector2){ 0, 0 };
        mouseTail[i].color = GRAY;//(Color){ GetRandomValue(0, 255), GetRandomValue(0, 255), GetRandomValue(0, 255), 255 };
        mouseTail[i].alpha = 1.0f;
        mouseTail[i].size = (float)GetRandomValue(1, 30)/20.0f;
        mouseTail[i].rotation = (float)GetRandomValue(0, 360);
        mouseTail[i].active = false;
    }

    float gravity = 3.0f;


     // Create a Image in memory
    RenderTexture2D smoke = LoadRenderTexture(32, 32);
    
    BeginTextureMode(smoke);    
    ClearBackground(BLANK);
    // Draw a base image(dark edges and white inside) for the additive blend mode smoke trail.
    for(int i=0;i<16;i++){
        BeginTextureMode(smoke); 
        int c=255/16*i;
        DrawCircle(16,16,16-i/2,(Color){c,c,c,255});
        EndTextureMode();               // This needs to be called after every different draw command used. Do not forget to use begintexture also..
    }

    EndTextureMode();               // This needs to be called after every different draw command used. Do not forget to use begintexture also..
 

    // Start at blend additive mode for the white smoke trial.
    int blending = BLEND_ADDITIVE;
    
 
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

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
            
 


        // Update
        //----------------------------------------------------------------------------------

        // Activate one particle every frame and Update active particles
        // NOTE: Particles initial position should be mouse position when activated
        // NOTE: Particles fall down with gravity and rotation... and disappear after 2 seconds (alpha = 0)
        // NOTE: When a particle disappears, active = false and it can be reused.
        for (int i = 0; i < MAX_PARTICLES; i++)
        {
            if (!mouseTail[i].active)
            {
                mouseTail[i].active = true;
                mouseTail[i].alpha = 1.0f;
                mouseTail[i].position = GetMousePosition();
                i = MAX_PARTICLES;
            }
        }

        for (int i = 0; i < MAX_PARTICLES; i++)
        {
            if (mouseTail[i].active)
            {
                mouseTail[i].position.y += gravity/2;
                mouseTail[i].alpha -= 0.005f;

                if (mouseTail[i].alpha <= 0.0f) mouseTail[i].active = false;

                mouseTail[i].rotation += 2.0f;
            }
        }

        if (IsKeyPressed(KEY_SPACE))
        {
            if (blending == BLEND_ALPHA) blending = BLEND_ADDITIVE;
            else blending = BLEND_ALPHA;
        }
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(DARKGRAY);

            BeginBlendMode(blending);

                // Draw active particles
                for (int i = 0; i < MAX_PARTICLES; i++)
                {
                    if (mouseTail[i].active) DrawTexturePro(smoke.texture, (Rectangle){ 0.0f, 0.0f, (float)smoke.texture.width, (float)smoke.texture.height },
                                                           (Rectangle){ mouseTail[i].position.x, mouseTail[i].position.y, smoke.texture.width*mouseTail[i].size, smoke.texture.height*mouseTail[i].size },
                                                           (Vector2){ (float)(smoke.texture.width*mouseTail[i].size/2.0f), (float)(smoke.texture.height*mouseTail[i].size/2.0f) }, mouseTail[i].rotation,
                                                           Fade(mouseTail[i].color, mouseTail[i].alpha));
                }

            EndBlendMode();

            DrawText("PRESS SPACE to CHANGE BLENDING MODE", 180, 20, 20, BLACK);

            if (blending == BLEND_ALPHA) DrawText("ALPHA BLENDING", 290, screenHeight - 40, 20, BLACK);
            else DrawText("ADDITIVE BLENDING", 280, screenHeight - 40, 20, RAYWHITE);










        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadRenderTexture(smoke);    // Unload render texture
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
