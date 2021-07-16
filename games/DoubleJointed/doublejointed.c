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
#define animHit1 2
#define animUcut 3
#define animHit2 4
#define animWalk 5
#define animDamage 6
#define animFlying 7


// controls for player (true/false) ,,add function to read keyboard/joypad/touchscreen
bool LEFT;
bool RIGHT;
bool UP;
bool DOWN;
bool FIRE1;
bool FIRE2;

// currentframe hold the current position of the cells (typewriter style)
int currentFrame;

int frame_start = 0;
int frame_end = 3;

int frame_kickstart = 1;
int frame_kickend = 3;

int frame_hit1start = 16;
int frame_hit1end = 18;

int frame_ucutstart = 31;
int frame_ucutend = 33;

int frame_hit2start = 46;
int frame_hit2end = 47;

int frame_walkstart = 61;
int frame_walkend = 62;

int frame_damagestart = 76;
int frame_damageend = 77;

int frame_flyingstart = 92;
int frame_flyingend = 93;



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
    setanimation(animFlying);

    //other sprites - 
    int mod=0;
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        framesCounter++;

        // Switch between characters 
        if(GetRandomValue(0,100)==1){
            setanimation(GetRandomValue(1,7));
            switch(GetRandomValue(0,2)){
                case 0:
                mod=0;
                break;
                case 1:
                mod=5;
                break;
                case 2:
                mod = 9;
                break;
            }
        }

        

        if (framesCounter >= (60/framesSpeed))
        {
            framesCounter = 0;
            currentFrame++;

            if (currentFrame > frame_end) currentFrame = frame_start;

            int ypos = currentFrame/15;
            frameRec.y = (float)(currentFrame/15)*(float)96;

            frameRec.x = (float)((currentFrame+mod)-ypos*15)*(float)96;
            
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

            //DrawTextureRec(scarfy, frameRec, position, WHITE);  // Draw part of the texture
            DrawTextureRec(scarfy, frameRec, (Vector2){position.x+96,position.y}, WHITE);  // Draw part of the texture
            DrawTexturePro(scarfy,  (Rectangle){frameRec.x,frameRec.y,-96,96},
                                            (Rectangle){position.x,position.y,96,96},
                                            (Vector2){0,0},0,WHITE);
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
            case animKick:
            frame_start = frame_kickstart;
            frame_end = frame_kickend;
            
            break;
            case animHit1:
            frame_start = frame_hit1start;
            frame_end = frame_hit1end;
            break;

            case animUcut:
            frame_start = frame_ucutstart;
            frame_end = frame_ucutend;
            break;

            case animHit2:
            frame_start = frame_hit2start;
            frame_end = frame_hit2end;
            break;
            case animWalk:
            frame_start = frame_walkstart;
            frame_end = frame_walkend;
            break;
            case animDamage:
            frame_start = frame_damagestart;
            frame_end = frame_damageend;
            break;
            case animFlying:
            frame_start = frame_flyingstart;
            frame_end = frame_flyingend;
            break;


    }
    currentFrame = frame_start;
}
