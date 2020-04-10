/*
Pattern Movement Example.

Move a computer entity around using a set of instructions.  (pattern)

*/


#include "raylib.h"

// This is for our map.
static int mapwidth=20;
static int mapheight=10;
static int map[10][20] =    {   {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
                                {1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,1,1,1,1},
                                {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1},
                                {1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,1,1,1,1},
                                {1,1,1,1,1,1,1,1,0,1,1,1,1,0,0,1,1,1,1,1},
                                {1,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,1},
                                {1,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,1},
                                {1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,1},
                                {1,0,0,0,0,0,0,1,0,0,0,1,1,1,1,0,0,0,0,1},
                                {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}}; 
                       
// The enum are for the instructions inside the agent command array.                       
enum flag{up,down,left,right,wait1,wait2,stop};

typedef struct agent{
    bool active;
    int x;
    int y;
    double time;
    int commandposition; // Where in the command array are we at.
    int command[1024];   // The array containing the movement instructions. 
}agent;


static void moveagent(void);

// Create our agent.
static agent myagent;


int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib example.");

    float tilewidth = (float)screenWidth/(float)mapwidth;
    float tileheight = (float)screenHeight/(float)mapheight;
 
    // initialize agent.
    myagent.active = true;
    myagent.x = 1;
    myagent.y = 1;
    myagent.commandposition = 0; // we start at the first slot in the command array.
    // This is the set of instructions we give the agent to let him move around the map. (script!)
    int c[1024] = { right,wait1,right,wait1,right,wait1,right,wait1,right,wait1,
                    right,wait1,right,wait1,down,wait1,down,wait2,down,wait1,
                    down,wait1,down,wait1,down,wait1,right,wait2,right,wait1,
                    left,wait1,left,wait1,left,wait1,left,wait1,left,wait1,stop};
    // Put the instructions inside the myagent command array.
    for(int i=0;i<1024;i++){
        myagent.command[i] = c[i];
        if(c[i]==stop)break;
    }
 
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        
        moveagent();
        
        //----------------------------------------------------------------------------------
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            // Draw our tilemap.
            for(int y=0;y<10;y++){
                for(int x=0;x<20;x++){
                    if(map[y][x]==1){
                        DrawRectangle(x*tilewidth,y*tileheight,tilewidth,tileheight,BLACK);
                    }
                }
            } 
            // Draw our agent.
            DrawRectangle(myagent.x*tilewidth,myagent.y*tileheight,tilewidth,tileheight,RED);    
                

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;


}

// With this function we move the agent around. His instructions are inside the command array.
// 
void moveagent(){
    if (myagent.active==false)return;
    if (myagent.command[myagent.commandposition]==stop)return; // He has finished his current set of instructions. 'stop' 
    //
    switch (myagent.command[myagent.commandposition]){
        case up:// up is a enum
        myagent.y-=1;
        myagent.commandposition+=1;
        break;
        case down:
        myagent.y+=1;
        myagent.commandposition+=1;
        break;
        case left:
        myagent.x-=1;
        myagent.commandposition+=1;
        break;
        case right:
        myagent.x+=1;
        myagent.commandposition+=1;
        break;
        case wait1://wait for a certain amount of time.
        if(myagent.time<1)myagent.time+=0.05f;
        if(myagent.time>=1){
            myagent.time=0;
            myagent.commandposition+=1;
        }
        break;
        case wait2: //wait longer.
        if(myagent.time<2)myagent.time+=0.05f;
        if(myagent.time>=2){
            myagent.time=0;
            myagent.commandposition+=1;
        }
        break;
        case stop://do nothing
        break;
    }
    
}
