/*******************************************************************************************
* I started this racing game experiment on the iphone and with appgamekit mobile during todays f1's forumula 1 race.
* 
*


*
* Situation 1 - Here I am trying to figure out when to make the bots switch lanes. Trying to make the bots think ahead
* in a lane that is free and take that lane. (*)
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
    int lanelockcnt;
}thebot;

static struct thebot bot[MAXBOTS];

Vector2 track[5][360];

void maketrack();
void drawtrack();
void updatebots();
void drawbots();
static float edistance(float x1,float y1,float x2,float y2);
void crowdedgoinside(int b);
void overtake(int b);    


int debug=-1;

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
    bot[0].speed=2;

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------


    //track[0][0] = (Vector2){0,0};
    int countdown=150;
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------
        if(countdown>0)countdown--;
        if(countdown==0){
        updatebots();
        updatebots();
        }
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);


        drawtrack();
        drawbots();
        DrawText(FormatText("%i",debug), 0, 0, 20, BLACK);

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
//
// We have a array of [lanes][nodes] = vector2d
// We create it by just making a circle in 360 steps. The cars wil go from node to node and loop this.
//
//
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

//
// Here the magic should happen(i think) Move towards the next node/. 
// If close to the target node then next node.
//
// Lane magic code should be added here.
//
void updatebots(){
    for(int i=0;i<MAXBOTS;i++){
        if(bot[i].lanelockcnt>0)bot[i].lanelockcnt--;
        
        
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
        if(bot[i].lanelockcnt==0 && close==false){
            if(bot[i].lane>0){
                bot[i].lane--;
                bot[i].pos+=2;
                bot[i].lanelockcnt=20;
            }
        }
        if(bot[i].lanelockcnt==0)crowdedgoinside(i);
        if(bot[i].lanelockcnt==0)overtake(i);
        // if overtake and only 2 close by
        //int cnt=0;
        //for(int j=0;j<MAXBOTS;j++){
        //    if(j==i)continue;
        //    float l=edistance(      bot[i].position.x,
        //                            bot[i].position.y,
        //                            bot[j].position.x,
        //                            bot[j].position.y);
        //    if(l<20 && bot[j].speed<bot[i].speed)cnt++;
       // }
        //if(cnt==1){
        //    if(bot[i].lane==0){
        //        bot[i].lane++;
        //        bot[i].pos+=2;
        //    }
       // }
    }    
}

void overtake(int b){
    Vector2 temp = bot[b].position;
    int lane= bot[b].lane;
    int pos = bot[b].pos;
    // go ahead a bit
    for(int z=0;z<10;z++){
        float l=edistance(      temp.x,
                                temp.y,
                                track[lane][pos].x,
                                track[lane][pos].y);
        if(l<5){
            pos+=1;
            if(pos>359)pos=0;            
        }
        float an = atan2(temp.y-track[lane][pos].y,temp.x-track[lane][pos].x);
        temp.x-=cos(an)*bot[b].speed;
        temp.y-=sin(an)*bot[b].speed;
        
    }
    // is there anything slower there.
    bool con=false;
    for(int i=0;i<MAXBOTS;i++){        
        if(i==b)continue;
        if(edistance(bot[i].position.x,bot[i].position.y,temp.x,temp.y)<20){            
            if(bot[i].speed<bot[b].speed)con=true;
            
            break;            
        }       
    }
    // simulate ahead from each lane from inside to outside for free passage.
    if(con==false)return;
    
    //debug = GetRandomValue(0,100);
    for(lane=0;lane<MAXLANES;lane++){
    temp = bot[b].position;
    pos = bot[b].pos;
    bool col=false;
    // go ahead a bit
    for(int z=0;z<10;z++){
        float l=edistance(      temp.x,
                                temp.y,
                                track[lane][pos].x,
                                track[lane][pos].y);
        if(l<5){
            pos+=1;
            if(pos>359)pos=0;            
        }
        float an = atan2(temp.y-track[lane][pos].y,temp.x-track[lane][pos].x);
        temp.x-=cos(an)*bot[b].speed;
        temp.y-=sin(an)*bot[b].speed;
        for(int i=0;i<MAXBOTS;i++){
            if(i==b)continue;
            if(edistance(bot[i].position.x,bot[i].position.y,temp.x,temp.y)<5){
                col=true;                
                
            }
        }
    }
    if(col==false){
        //debug=GetRandomValue(0,100);
        bot[b].lane=lane;
        
        bot[b].lanelockcnt=20;
        return;
    }
    }
    
    
}

// If there are a couple of bots clumped together(collision area) and the bot tested here is in the most
// inside position and if it can go futher inside then switch to a more inside lane.
//
void crowdedgoinside(int b){
    if(bot[b].lane==0)return;
    int lanepos = bot[b].lane;
    int cnt=0;
    for(int i=0;i<MAXBOTS;i++){
        if(i==b)continue;
        if(edistance(bot[i].position.x,bot[i].position.y,bot[b].position.x,bot[b].position.y)<32){
            if(bot[i].lane<bot[b].lane)return;
            cnt++;
        }
        
    }
    if(cnt>0 && bot[b].lane>0){
        bot[b].lane--;
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