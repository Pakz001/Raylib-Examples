/*******************************************************************************************
*

* Trying to figure out how to do a racing game using lanes...
*
* I made a array of vector2d's, this in a circle as the racetrack.
* there are 5 lanes. 5 bots that should be able to race against each other.
*
* work in progress or figure out yourself.
*
********************************************************************************************/

#include "raylib.h"
#include <math.h>

#define MAXLANES 5
#define MAXBOTS 5

typedef struct thebot{
    Vector2 position;
    float speed;
    int pos;
    int lane;
}thebot;

static struct thebot bot[MAXBOTS];

Vector2 track[5][360];

void maketrack();
void drawtrack();
void updatebots();
void drawbots();
static float edistance(float x1,float y1,float x2,float y2);

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");
    maketrack();
    for(int i=0;i<MAXBOTS;i++){
        bot[i].position = track[i][0];
        bot[i].lane=i;
        bot[i].pos = 0;
        bot[i].speed = GetRandomValue(500,1000)/1000.0f;
    }

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------


    //track[0][0] = (Vector2){0,0};

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------
        updatebots();
        updatebots();
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);


        drawtrack();
        drawbots();
        //DrawText(FormatText("%f",(float)GetRandomValue(0,10000)/10000*2-1), 190, 200, 20, LIGHTGRAY);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

void drawtrack(){
    for(int i=1;i<360;i++){
    for(int t=0;t<5;t++){
        DrawLineV(track[t][i-1],track[t][i],BLACK);
    }
    }
}

void maketrack(){
    float x = 0.0f;
    float y = 0.0f;
    for(int i=0;i<360;i++){
    for(int t=0;t<5;t++){
        track[t][i] = (Vector2){x+x/100*t*10.0f,y+y/100*t*10.0f};
        track[t][i].x += 320;
        track[t][i].y += 100;
        track[t][i].y -= t*10;
    }
        x+=cos(i*DEG2RAD)*2;
        y+=sin(i*DEG2RAD)*2;

    }
}

void updatebots(){
    for(int i=0;i<MAXBOTS;i++){
        float l=edistance(      bot[i].position.x,
                                bot[i].position.y,
                                track[bot[i].lane][bot[i].pos].x,
                                track[bot[i].lane][bot[i].pos].y);
        if(l<5){
            bot[i].pos+=1;
            if(bot[i].pos>359)bot[i].pos=0;            
        }
        float an = atan2(bot[i].position.y-track[bot[i].lane][bot[i].pos].y,bot[i].position.x-track[bot[i].lane][bot[i].pos].x);
        bot[i].position.x-=cos(an)*bot[i].speed;
        bot[i].position.y-=sin(an)*bot[i].speed;
        // Switch lane logic
        bool close=false;
        for(int j=0;j<MAXBOTS;j++){
            if(j==i)continue;
            float l=edistance(      bot[i].position.x,
                                    bot[i].position.y,
                                    bot[j].position.x,
                                    bot[j].position.y);
            if(l<20)close=true;
        }
        if(close==false){
            if(bot[i].lane>0)bot[i].lane--;
        }
        // if overtake and only 2 close by
        int cnt=0;
        for(int j=0;j<MAXBOTS;j++){
            if(j==i)continue;
            float l=edistance(      bot[i].position.x,
                                    bot[i].position.y,
                                    bot[j].position.x,
                                    bot[j].position.y);
            if(l<20 && bot[j].speed<bot[i].speed)cnt++;
        }
        if(cnt==1){
            if(bot[i].lane==0){
                bot[i].lane++;
                bot[i].pos++;
            }
        }
    }    
}

void drawbots(){
    for(int i=0;i<MAXBOTS;i++){
        DrawCircleV(bot[i].position,5,RED);
    }
}

// Euclidean distance (more precise)
float edistance(float x1,float y1,float x2,float y2){
    return sqrt( (x1-x2)*(x1-x2)+(y1-y2)*(y1-y2) );
}
