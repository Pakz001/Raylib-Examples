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


#define animIdle 0
#define animKick  1
#define animHit1 2
#define animUcut 3
#define animHit2 4
#define animWalk 5
#define animDamage 6
#define animFlying 7

#define MAX_PLAYERS 1 
#define MAX_ENTITIES 1 

typedef struct entity{
    bool active;
    float facing; //-1 left, 1 right
    Vector2 position;
    int w;
    int h;
    int health;
    int currentFrame;
    int currentAnim; // which animation is currenty active
    int lastAnim;
    int framesCounter;
    int frame_start;
    int frame_end;
    int lastFiretime;
    int lastFireAnim;
    Rectangle frameRec;
    int mod; //collumn of sprites (+5 = enemy1 +9 = enemy headless)
}entity;

static struct entity e[MAX_ENTITIES];


typedef struct player{
    bool active;
    float facing; //-1 left, 1 right
    Vector2 position;
    int w;
    int h;
    int score;
    int health;
    int currentFrame;
    int currentAnim; // which animation is currenty active
    int lastAnim;
    int framesCounter;
    int frame_start;
    int frame_end;
    int keynothingtime; // when key is down this is zero (used to disable animations..)
    int lastFiretime;
    int lastFireAnim;
}player;

static struct player p[MAX_PLAYERS];


// controls for player (true/false) ,,add function to read keyboard/joypad/touchscreen
bool LEFT;
bool RIGHT;
bool UP;
bool DOWN;
bool FIRE1;
bool FIRE2;

// currentframe hold the current position of the cells (typewriter style)
int currentFrame;
int framesSpeed = 8;            // Number of spritesheet frames shown by second

Rectangle frameRec = { 0.0f, 0.0f, (float)96, (float)96 };


int frame_start = 0;
int frame_end = 0;

int frame_kickstart = 1;
int frame_kickend = 3;

int frame_hit1start = 16;
int frame_hit1end = 18;

int frame_ucutstart = 31;
int frame_ucutend = 33;

int frame_hit2start = 46;
int frame_hit2end = 48;

int frame_walkstart = 61;
int frame_walkend = 62;

int frame_damagestart = 76;
int frame_damageend = 77;

int frame_flyingstart = 92;
int frame_flyingend = 93;

int frame_idlestart = 1;
int frame_idleend = 1;


// this sets the frame to start and sets start and end position(loop)
void setanimation(int anim);
void drawplayers();
void updateplayer(int player);
void setplayeranimation(int player, int anim);
void playercontrols(int player);
void drawentities();
void setentityanimation(int entity, int anim);
void updateentity(int entity);


Texture2D scarfy;
Texture2D backg;

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1400;
    const int screenHeight = 768;

    InitWindow(screenWidth, screenHeight, "raylib [texture] example - texture rectangle");

    // NOTE: Textures MUST be loaded after Window initialization (OpenGL context is required)
    //Texture2D scarfy = LoadTexture("resources/mrskelly.png");        // Texture loading
    //Texture2D backg = LoadTexture("resources/onedragon.png");
    scarfy = LoadTexture("resources/mrskelly.png");        // Texture loading
    backg = LoadTexture("resources/onedragon.png");

    Vector2 position = { 350.0f, 480.0f };
    //Rectangle frameRec = { 0.0f, 0.0f, (float)96, (float)96 };
    //int currentFrame = frame_kickstart;

    int framesCounter = 0;

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    //setanimation(animKick);
    //setanimation(animFlying);
    e[0].frameRec = (Rectangle){ 0.0f, 0.0f, (float)96, (float)96 };
    e[0].facing=1;
    e[0].position.y = 470;
    e[0].position.x = 320;
    e[0].mod = 5;
    setentityanimation(0,animIdle);
    
    
    p[0].facing=1;
    p[0].position.y = 370;
    p[0].position.x = 320;
    setplayeranimation(0,animIdle);

    //other sprites - 
    int mod=0;
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        
        playercontrols(0);
        updateplayer(0);
        updateentity(0);

        /*
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
        // temp controls
        if (IsKeyDown(KEY_RIGHT)) position.x+=2;
        else if (IsKeyDown(KEY_LEFT)) position.x-=2;
        if (IsKeyDown(KEY_DOWN)) position.y+=2;
        else if (IsKeyDown(KEY_UP)) position.y-=2;
        */


        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);
            DrawTexture(backg, 0, 0, WHITE);
            //DrawTexture(scarfy, 15, 40, WHITE);


            drawplayers();
            drawentities();
            
            /*
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
            DrawTexturePro(scarfy,  (Rectangle){frameRec.x,frameRec.y,-96,96},// the -96 (-)means mirror on x axis
                                            (Rectangle){position.x,position.y,96,96},
                                            (Vector2){0,0},0,WHITE);
            DrawText("(c) Scarfy sprite by Eiden Marsal", screenWidth - 200, screenHeight - 20, 10, GRAY);
            */
            DrawRectangle(0,2,screenWidth,23,WHITE);
            DrawText("Cursor LEFT/RIGHT/UP/DOWN Z(FIRE1)/X(FIRE2)", 0, 0, 30, BLACK);

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


void drawentities(){
    if(e[0].facing==1){
    DrawTextureRec(scarfy, e[0].frameRec, (Vector2){e[0].position.x,e[0].position.y}, WHITE);  // Draw part of the texture
    }else{
    DrawTexturePro(scarfy,  (Rectangle){e[0].frameRec.x,e[0].frameRec.y,-96,96},// the -96 (-)means mirror on x axis
                                    (Rectangle){e[0].position.x,e[0].position.y,96,96},
                                    (Vector2){0,0},0,WHITE);
    }
    
}

void drawplayers(){
    if(p[0].facing==1){
    DrawTextureRec(scarfy, frameRec, (Vector2){p[0].position.x,p[0].position.y}, WHITE);  // Draw part of the texture
    }else{
    DrawTexturePro(scarfy,  (Rectangle){frameRec.x,frameRec.y,-96,96},// the -96 (-)means mirror on x axis
                                    (Rectangle){p[0].position.x,p[0].position.y,96,96},
                                    (Vector2){0,0},0,WHITE);
    }
    
}


void setentityanimation(int entity, int anim){
    e[entity].lastAnim = anim;
    switch(anim){
            case animIdle:
            e[entity].frame_start = frame_idlestart+e[entity].mod;
            e[entity].frame_end = frame_idleend+e[entity].mod;
            e[entity].currentFrame = e[entity].frame_start;    
            break;

            case animKick:
            e[entity].frame_start = frame_kickstart+e[entity].mod;
            e[entity].frame_end = frame_kickend+e[entity].mod;
            e[entity].currentFrame = e[entity].frame_start;
            break;
            case animHit1:
            e[entity].frame_start = frame_hit1start+e[entity].mod;
            e[entity].frame_end = frame_hit1end+e[entity].mod;
            e[entity].currentFrame = e[entity].frame_start;
            break;

            case animUcut:
            e[entity].frame_start = frame_ucutstart+e[entity].mod;
            e[entity].frame_end = frame_ucutend+e[entity].mod;
            e[entity].currentFrame = e[entity].frame_start;
            break;

            case animHit2:
            e[entity].frame_start = frame_hit2start+e[entity].mod;
            e[entity].frame_end = frame_hit2end+e[entity].mod;
            e[entity].currentFrame = e[entity].frame_start;
            break;
            case animWalk:
            if(p[0].currentAnim!=animWalk){
            e[entity].frame_start = frame_walkstart+e[entity].mod;
            e[entity].frame_end = frame_walkend+e[entity].mod;
            e[entity].currentFrame = e[entity].frame_start;            
            }
            break;
            case animDamage:
            e[entity].frame_start = frame_damagestart+e[entity].mod;
            e[entity].frame_end = frame_damageend+e[entity].mod;
            e[entity].currentFrame = e[entity].frame_start;
            break;
            case animFlying:
            e[entity].frame_start = frame_flyingstart+e[entity].mod;
            e[entity].frame_end = frame_flyingend+e[entity].mod;
            e[entity].currentFrame = e[entity].frame_start;
            
            break;

    
    }
    e[entity].currentAnim = anim;
}

void setplayeranimation(int player, int anim){
    p[player].lastAnim = anim;
    switch(anim){
            case animIdle:
            p[player].frame_start = frame_idlestart;
            p[player].frame_end = frame_idleend;
            p[player].currentFrame = p[player].frame_start;    
            break;

            case animKick:
            p[player].frame_start = frame_kickstart;
            p[player].frame_end = frame_kickend;
            p[player].currentFrame = p[player].frame_start;
            break;
            case animHit1:
            p[player].frame_start = frame_hit1start;
            p[player].frame_end = frame_hit1end;
            p[player].currentFrame = p[player].frame_start;
            break;

            case animUcut:
            p[player].frame_start = frame_ucutstart;
            p[player].frame_end = frame_ucutend;
            p[player].currentFrame = p[player].frame_start;
            break;

            case animHit2:
            p[player].frame_start = frame_hit2start;
            p[player].frame_end = frame_hit2end;
            p[player].currentFrame = p[player].frame_start;
            break;
            case animWalk:
            if(p[0].currentAnim!=animWalk){
            p[player].frame_start = frame_walkstart;
            p[player].frame_end = frame_walkend;
            p[player].currentFrame = p[player].frame_start;            
            }
            break;
            case animDamage:
            p[player].frame_start = frame_damagestart;
            p[player].frame_end = frame_damageend;
            p[player].currentFrame = p[player].frame_start;
            break;
            case animFlying:
            p[player].frame_start = frame_flyingstart;
            p[player].frame_end = frame_flyingend;
            p[player].currentFrame = p[player].frame_start;
            
            break;

    
    }
    p[player].currentAnim = anim;
}

void updateentity(int entity){
        // animation
        e[entity].lastFiretime++;
        e[entity].framesCounter++;

        

        if (e[0].framesCounter >= (60/framesSpeed))
        {
            e[0].framesCounter = 0;
            e[0].currentFrame++;

            if (e[0].currentFrame > e[0].frame_end) e[0].currentFrame = e[0].frame_start;

            int ypos = e[0].currentFrame/15;
            e[entity].frameRec.y = (float)(e[0].currentFrame/15)*(float)96;

            e[entity].frameRec.x = (float)((e[0].currentFrame)-ypos*15)*(float)96;
            
        }
 

        // player movement
        if(e[entity].currentAnim == animWalk){
            setentityanimation(entity,animIdle);
        }
        if(e[entity].currentAnim == animHit1){
            
            setentityanimation(entity,animIdle);
        }
        if(e[entity].currentAnim == animHit2){
            setentityanimation(entity,animIdle);
            
        }

        if(e[entity].currentAnim == animKick){
            setentityanimation(entity,animIdle);
        }
        
        
}


void updateplayer(int player){
        // animation
        p[player].lastFiretime++;
        p[player].keynothingtime++;
        p[player].framesCounter++;

        

        if (p[0].framesCounter >= (60/framesSpeed))
        {
            p[0].framesCounter = 0;
            p[0].currentFrame++;

            if (p[0].currentFrame > p[0].frame_end) p[0].currentFrame = p[0].frame_start;

            int ypos = p[0].currentFrame/15;
            frameRec.y = (float)(p[0].currentFrame/15)*(float)96;

            frameRec.x = (float)((p[0].currentFrame)-ypos*15)*(float)96;
            
        }
 

        // player movement
        if(p[player].currentAnim == animWalk && p[player].keynothingtime>1){
            setplayeranimation(player,animIdle);
        }
        if(p[player].currentAnim == animHit1 && p[player].keynothingtime>15){
            
            setplayeranimation(player,animIdle);
        }
        if(p[player].currentAnim == animHit2 && p[player].keynothingtime>15){
            setplayeranimation(player,animIdle);
            
        }

        if(p[player].currentAnim == animKick && p[player].keynothingtime>15){
            setplayeranimation(player,animIdle);
        }
        
        
        
        if(LEFT==false && RIGHT==false && UP==false && DOWN==false && FIRE1){
            if(p[player].lastFireAnim==animHit2){
                setplayeranimation(player,animHit1);
                p[player].lastFireAnim=animHit1;
            }else{
                setplayeranimation(player,animHit2);
                p[player].lastFireAnim=animHit2;
            }
            FIRE1=false;
        }
        if(LEFT==false && RIGHT==false && UP==false && DOWN==false && FIRE2){
            setplayeranimation(player,animKick);
            FIRE2=false;
        }
        if (RIGHT){
            p[player].lastFiretime=50;
            p[0].facing = 1;
            p[0].position.x+=2;
            RIGHT=false;
            setplayeranimation(player,animWalk);
        }
        else if (LEFT){
            p[player].lastFiretime=50;
            p[0].facing=-1;
            p[0].position.x-=2;
            LEFT=false;
            setplayeranimation(player,animWalk);
        }
        if (DOWN){
            p[player].lastFiretime=50;
            p[0].position.y+=2;
            DOWN=false;
            setplayeranimation(player,animWalk);
        }
        else if (UP){
            p[player].lastFiretime=50;
            p[0].position.y-=2;
            UP=false;
            setplayeranimation(player,animWalk);
        }
}

void playercontrols(int player){
        if (p[player].lastFiretime>30 && IsKeyDown(KEY_Z) && FIRE1==false && FIRE2==false){
            FIRE1 = true;
            p[player].lastFiretime=0;
            p[player].keynothingtime = 0;
        }
        if (p[player].lastFiretime>30 && IsKeyDown(KEY_X) && FIRE1==false && FIRE2==false){
            FIRE2 = true;
            p[player].lastFiretime=0;
            p[player].keynothingtime = 0;
        }
        
        if(p[player].lastFiretime<20)return; //WATCH this return!!
        if (IsKeyDown(KEY_RIGHT)){
            RIGHT = true;
            LEFT = false;
            p[player].keynothingtime = 0;
        }
        else if (IsKeyDown(KEY_LEFT)){
            LEFT = true;            
            RIGHT = false;
            p[player].keynothingtime = 0;
        }
        if (IsKeyDown(KEY_DOWN)){
            DOWN = true;
            UP = false;
            p[player].keynothingtime = 0;
        }
        else if (IsKeyDown(KEY_UP)){
            UP = true;
            DOWN = false;
            p[player].keynothingtime = 0;
        }
    
}
