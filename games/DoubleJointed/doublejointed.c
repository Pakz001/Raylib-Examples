/*******************************************************************************************
*
*   raylib [textures] example - Texture loading and drawing a part defined by a rectangle
*
*   This example has been created using raylib 1.3 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2014 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"

#define MAX_FRAME_SPEED     15
#define MIN_FRAME_SPEED      1


#define animKick  1
#define animHit 2
#define animUcut 3


// currentframe hold the current position of the cells (typewriter style)
int currentFrame;

int frame_start = 0;
int frame_end = 3;

int frame_kickstart = 1;
int frame_kickend = 3;

int frame_hitstart = 16;
int frame_hitend = 18;

int frame_ucutstart = 31;
int frame_ucutend = 33;


// this sets the frame to start and sets start and end position(loop)
void setanimation(int anim);

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1400;
    const int screenHeight = 768;

    InitWindow(screenWidth, screenHeight, "raylib [texture] example - texture rectangle");

    // NOTE: Textures MUST be loaded after Window initialization (OpenGL context is required)
    Texture2D scarfy = LoadTexture("resources/mrskelly.png");        // Texture loading
    Texture2D backg = LoadTexture("resources/onedragon.png");

    Vector2 position = { 350.0f, 480.0f };
    Rectangle frameRec = { 0.0f, 0.0f, (float)96, (float)96 };
    //int currentFrame = frame_kickstart;

    int framesCounter = 0;
    int framesSpeed = 8;            // Number of spritesheet frames shown by second

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    //setanimation(animKick);
    setanimation(animUcut);

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        framesCounter++;

        if (framesCounter >= (60/framesSpeed))
        {
            framesCounter = 0;
            currentFrame++;

            if (currentFrame > frame_end) currentFrame = frame_start;

            int ypos = currentFrame/15;
            frameRec.y = (float)(currentFrame/15)*(float)96;

            frameRec.x = (float)(currentFrame-ypos*15)*(float)96;
            
        }

        if (IsKeyPressed(KEY_RIGHT)) framesSpeed++;
        else if (IsKeyPressed(KEY_LEFT)) framesSpeed--;

        if (framesSpeed > MAX_FRAME_SPEED) framesSpeed = MAX_FRAME_SPEED;
        else if (framesSpeed < MIN_FRAME_SPEED) framesSpeed = MIN_FRAME_SPEED;
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);
            DrawTexture(backg, 0, 0, WHITE);
            DrawTexture(scarfy, 15, 40, WHITE);
            DrawRectangleLines(15, 40, scarfy.width, scarfy.height, LIME);
            DrawRectangleLines(15 + (int)frameRec.x, 40 + (int)frameRec.y, (int)frameRec.width, (int)frameRec.height, RED);

            DrawText("FRAME SPEED: ", 165, 210, 10, DARKGRAY);
            DrawText(TextFormat("%02i FPS", framesSpeed), 575, 210, 10, DARKGRAY);
            DrawText("PRESS RIGHT/LEFT KEYS to CHANGE SPEED!", 290, 240, 10, DARKGRAY);

            for (int i = 0; i < MAX_FRAME_SPEED; i++)
            {
                if (i < framesSpeed) DrawRectangle(250 + 21*i, 205, 20, 20, RED);
                DrawRectangleLines(250 + 21*i, 205, 20, 20, MAROON);
            }

            DrawTextureRec(scarfy, frameRec, position, WHITE);  // Draw part of the texture

            DrawText("(c) Scarfy sprite by Eiden Marsal", screenWidth - 200, screenHeight - 20, 10, GRAY);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadTexture(scarfy);       // Texture unloading

    CloseWindow();                // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

void setanimation(int anim){
    switch(anim){
            case 1:
            frame_start = frame_kickstart;
            frame_end = frame_kickend;
            
            break;
            case 2:
            frame_start = frame_hitstart;
            frame_end = frame_hitend;
            break;

            case 3:
            frame_start = frame_ucutstart;
            frame_end = frame_ucutend;
            break;

    }
    currentFrame = frame_start;
}
