/*******************************************************************************************
*
*   
*
********************************************************************************************/

#include "raylib.h"
#include <math.h>

#define MAXASTEROIDS 5
#define MAXSHIPS 1

// Variables for storing and restoring the current game to and from memory
bool savedactive[MAXASTEROIDS];
Vector2 savedposition[MAXASTEROIDS];
float savedangle[MAXASTEROIDS];
float savedthrust[MAXASTEROIDS];
float savedthrustdec[MAXASTEROIDS];
int savedradius[MAXASTEROIDS];
float savedrotation[MAXASTEROIDS];
float savedrotspeed[MAXASTEROIDS];

bool shipssavedactive[MAXSHIPS];
Vector2 shipssavedposition[MAXSHIPS];
float shipssavedangle[MAXSHIPS];
float shipssavedthrust[MAXSHIPS];
float shipssavedthrustdec[MAXSHIPS];
int shipssavedradius[MAXSHIPS];


// Regular variables.
typedef struct asteroid{
    bool active;
    Vector2 position;
    float angle;
    float thrust;
    float thrustdec;
    int radius;
    float rotation;
    float rotspeed;
}asteroid;

static struct asteroid ast[MAXASTEROIDS];

typedef struct ship{
    bool active;
    Vector2 position;
    float angle;
    float thrust;
    float thrustdec;
    int radius;
}ship;

static struct ship shp[MAXSHIPS];
    

Texture2D sprites;
Texture2D rocks;

const int screenWidth = 800;
const int screenHeight = 450;

void drawship(int x, int y, int ship,int rotation);
void drawrock(int x, int y, int rock, int rotation);
void updateasteroids();
void drawasteroids();
void iniasteroids();
void simulate();
void savestate();
void restorestate();
void updateships();
void drawships();
void updateships();
// Our rectsoverlap function. Returns true/false.
static bool rectsoverlap(int x1,int y1,int w1,int h1,int x2,int y2,int w2,int h2);


int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");
    sprites = LoadTexture("resources/spaceships.png");
    rocks = LoadTexture("resources/spacerocks.png");

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    shp[0].position.x = 100;
    shp[0].position.y = 100;
    shp[0].active = true;
    shp[0].radius = 16;
    shp[0].thrust = 0;
    shp[0].thrustdec = 0.01;

    iniasteroids();
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------
        if(IsKeyPressed(KEY_SPACE)){
            simulate();
            
        }
        
        updateasteroids();
        updateships();
        simulate();
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(BLACK);

        //drawship(100,100,146,0);
        drawships();
        drawasteroids();
        //drawrock(200,100,83,0);

        //DrawText(FormatText("%f",(float)GetRandomValue(0,10000)/10000*2-1), 190, 200, 20, LIGHTGRAY);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    UnloadTexture(sprites);
    UnloadTexture(rocks);

    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

void simulate(){
    
    if(shp[0].thrust>0)return;
    savestate();

    float rthrust = GetRandomValue(1,2);
    float angle = GetRandomValue(0,360)/DEG2RAD;

    shp[0].thrust=rthrust;
    shp[0].angle=angle;

    bool collided=false;
    
    for(int depth=0;depth<512;depth++){

        updateasteroids();
        updateships();
        //
        for(int i=0;i<MAXASTEROIDS;i++){
            if(rectsoverlap(shp[0].position.x,shp[0].position.y,32,32,ast[i].position.x,ast[i].position.y,32,32)==true){
                collided=true;                
            }
        }
        //
    }    
    
    
    restorestate();
    if(collided==false){
        shp[0].thrust=rthrust;
        shp[0].angle = angle;
    }
}


void iniasteroids(){
    for(int i=0;i<MAXASTEROIDS;i++){
        ast[i].active = true;
        ast[i].position.x = GetRandomValue(64,screenWidth-64);
        ast[i].position.y = GetRandomValue(64,screenHeight-64);
        ast[i].radius = 16;
        ast[i].thrust = 2;
        ast[i].angle = (float)GetRandomValue(0,PI*1000)/PI*1000.0f;
        ast[i].thrustdec = 0;
        ast[i].rotation = 0;
        ast[i].rotspeed = (float)GetRandomValue(0,1000)/10000.0f;
        if(GetRandomValue(0,10)<5)ast[i].rotspeed=-ast[i].rotspeed;
    }
}

void drawasteroids(){
    for(int i=0;i<MAXASTEROIDS;i++){
        if(ast[i].active==false)return;
        drawrock(ast[i].position.x,ast[i].position.y,83,ast[i].rotation*RAD2DEG);
    }
}

void updateasteroids(){
    for(int i=0;i<MAXASTEROIDS;i++){
        if(ast[i].active==false)return;
        ast[i].position.x += cos(ast[i].angle)*ast[i].thrust;
        ast[i].position.y += sin(ast[i].angle)*ast[i].thrust;
        if(ast[i].thrust>0){
            ast[i].thrust -= ast[i].thrustdec;
        }else{
            ast[i].thrust = 0;
        }
        ast[i].rotation+=ast[i].rotspeed;//
        if(ast[i].rotation>PI*2.0f)ast[i].rotation=0;
        if(ast[i].rotation<0.0f)ast[i].rotation=PI*2;
        //
        if(ast[i].position.y>screenHeight+32)ast[i].position.y=-32;
        if(ast[i].position.y<-32)ast[i].position.y=screenHeight+32;
        if(ast[i].position.x<-32)ast[i].position.x=screenWidth+32;
        if(ast[i].position.x>screenWidth+32)ast[i].position.x=-32;
    }
}

void drawships(){
    for(int i=0;i<MAXSHIPS;i++){
        if(shp[i].active==false)return;
        drawship(shp[i].position.x,shp[i].position.y,146,shp[i].angle*RAD2DEG+90);
    }
}

void updateships(){
    for(int i=0;i<MAXSHIPS;i++){
        if(shp[i].active==false)return;
        shp[i].position.x += cos(shp[i].angle)*shp[i].thrust;
        shp[i].position.y += sin(shp[i].angle)*shp[i].thrust;
        if(shp[i].thrust>0){
            shp[i].thrust -= shp[i].thrustdec;
        }else{
            shp[i].thrust = 0;
        }
        //
        if(shp[i].position.y>screenHeight+32)shp[i].position.y=-32;
        if(shp[i].position.y<-32)shp[i].position.y=screenHeight+32;
        if(shp[i].position.x<-32)shp[i].position.x=screenWidth+32;
        if(shp[i].position.x>screenWidth+32)shp[i].position.x=-32;
    }
    
}

void drawship(int x, int y, int ship, int rotation){
    // Get the tile x , y position.
    int ty = ship / 20;    
    int tx = ship-(ty*20);;
    DrawTexturePro(sprites,  (Rectangle){tx*16,ty*16,16,16},// the -96 (-)means mirror on x axis
                                    (Rectangle){x,y,32,32},
                                    (Vector2){16,16},rotation,WHITE);
    
}

void drawrock(int x, int y, int rock, int rotation){
    // Get the tile x , y position.
    int ty = rock / 20;    
    int tx = rock-(ty*20);;
    DrawTexturePro(rocks,  (Rectangle){tx*16,ty*16,16,16},// the -96 (-)means mirror on x axis
                                    (Rectangle){x,y,32,32},
                                    (Vector2){16,16},rotation,WHITE);
    
}


// save and restore state are to be used to save the screen and activity before the simulation starts.
// when the simulation ends the restorestate can be used to restore to the last saved state.
void savestate(){
    for(int i=0;i<MAXASTEROIDS;i++){
        savedactive[i] = ast[i].active;
        savedposition[i]= ast[i].position;
        savedangle[i]= ast[i].angle;
        savedthrust[i]= ast[i].thrust;
        savedthrustdec[i]= ast[i].thrustdec;
        savedradius[i]= ast[i].radius;
        savedrotation[i]= ast[i].rotation;
        savedrotspeed[i]= ast[i].rotspeed;
    }
    for(int i=0;i<MAXSHIPS;i++){
        shipssavedactive[i]=shp[i].active;
        shipssavedposition[i]=shp[i].position;
        shipssavedangle[i]=shp[i].angle;
        shipssavedthrust[i]=shp[i].thrust;
        shipssavedthrustdec[i]=shp[i].thrustdec;
        shipssavedradius[i]=shp[i].radius;
               
    }
}
void restorestate(){
    for(int i=0;i<MAXASTEROIDS;i++){
        ast[i].active=savedactive[i];
        ast[i].position=savedposition[i];
        ast[i].angle=savedangle[i];
        ast[i].thrust=savedthrust[i];
        ast[i].thrustdec=savedthrustdec[i];
        ast[i].radius=savedradius[i];
        ast[i].rotation=savedrotation[i];
        ast[i].rotspeed=savedrotspeed[i];
    }
    for(int i=0;i<MAXSHIPS;i++){
        shp[i].active=shipssavedactive[i];
        shp[i].position=shipssavedposition[i];
        shp[i].angle=shipssavedangle[i];
        shp[i].thrust=shipssavedthrust[i];
        shp[i].thrustdec=shipssavedthrustdec[i];
        shp[i].radius=shipssavedradius[i];
               
    }
}
// Rectangles overlap
bool rectsoverlap(int x1,int y1,int w1,int h1,int x2,int y2,int w2,int h2){
    if(x1 >= (x2 + w2) || (x1 + w1) <= x2) return false;
    if(y1 >= (y2 + h2) || (y1 + h1) <= y2) return false;
    return true;
}