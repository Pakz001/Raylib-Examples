
#include "raylib.h"
#include <math.h>

#define MAXCOMMANDS 10
#define MAXENTITIES 10

Texture2D sprites;

typedef struct entity{
    bool active;
    int command[MAXCOMMANDS];
    int value[MAXCOMMANDS];
    int command2[MAXCOMMANDS];
    int value2[MAXCOMMANDS];
    Vector2 position;
    int maxcommand;
    int maxcommand2;
    float angle;
    float angle2;
    int valuecount;
    int valuecount2;
    int pos;
    int pos2;
    int time;
    int timemax;
    int time2;
    int timemax2;
    
}entity;

static struct entity ent[MAXENTITIES];


void updateentities();
void drawentities();
void inientity(int entity,int x, int y);

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");
    sprites = LoadTexture("resources/sprites.png");
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------


    for(int i=0;i<MAXENTITIES;i++){
        inientity(i,GetRandomValue(200,400),GetRandomValue(200,400));
    }

    int debug=0;
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------
        
        updateentities();
        
        

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);




        drawentities();

        DrawText(FormatText("%i",debug), 100, 20, 20, BLACK);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    UnloadTexture(sprites);
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

void inientity(int entity,int x, int y){
    ent[entity].active = true;
    ent[entity].position.x = x;
    ent[entity].position.y = y;
    ent[entity].timemax = 1;
    ent[entity].timemax2 = 1;
    ent[entity].angle = 0;
    ent[entity].angle2 = 0;
    ent[entity].pos = 0;
    ent[entity].pos2 = 0;
    ent[entity].time = 0;
    ent[entity].time2 = 0;
    ent[entity].valuecount = -1;
    ent[entity].valuecount2 = -1;
    ent[entity].maxcommand = MAXCOMMANDS;
    ent[entity].maxcommand2 = MAXCOMMANDS;
    for(int i=0;i<ent[entity].maxcommand;i++){
        ent[entity].command[i]=GetRandomValue(1,3);
        ent[entity].value[i]=GetRandomValue(10,100);
    }
    for(int i=0;i<ent[entity].maxcommand2;i++){
        ent[entity].command2[i]=GetRandomValue(2,3);
        ent[entity].value2[i]=GetRandomValue(10,100);
        
    }

}

void drawentities(){
    for(int i=0;i<MAXENTITIES;i++){
        DrawTexturePro(sprites,  (Rectangle){0,0,16,16},// the -96 (-)means mirror on x axis
                                        (Rectangle){ent[i].position.x,ent[i].position.y,64,64},
                                        (Vector2){32,32},ent[i].angle,WHITE);
        DrawTexturePro(sprites,  (Rectangle){16,0,16,16},// the -96 (-)means mirror on x axis
                                        (Rectangle){ent[i].position.x,ent[i].position.y,64,64},
                                        (Vector2){32,32},ent[i].angle2,WHITE);
    }
}

void updateentities(){
 
    for(int i=0;i<MAXENTITIES;i++){







        if(ent[i].pos==ent[i].maxcommand){
            ent[i].pos=0;
            for(int ii=0;i<ent[i].maxcommand;i++){
                ent[i].command[ii]=GetRandomValue(1,3);
                ent[i].value[ii]=GetRandomValue(10,100);
            }
        }
        if(ent[i].pos2==ent[i].maxcommand2){
            
            ent[i].pos2=0;
            for(int ii=0;i<ent[i].maxcommand2;i++){
                ent[i].command2[ii]=GetRandomValue(2,3);
                ent[i].value2[ii]=GetRandomValue(10,100);
                
            }
        }

        ent[i].time++;
        if(ent[i].time>ent[i].timemax && ent[i].pos<ent[i].maxcommand){
            ent[i].time=0;
            //
            switch (ent[i].command[ent[i].pos]){
                case 1://forward
                    if(ent[i].valuecount==-1){
                        ent[i].valuecount=ent[i].value[ent[i].pos];
                    }
                    if(ent[i].valuecount>0){
                        ent[i].valuecount--;
                        ent[i].position.x += cos(ent[i].angle*DEG2RAD)*1;
                        ent[i].position.y += sin(ent[i].angle*DEG2RAD)*1;                        
                    }
                    if(ent[i].valuecount==0){
                        ent[i].pos++;
                        ent[i].valuecount=-1;
                    }
                break;
                case 2://left
                    if(ent[i].valuecount==-1){
                        ent[i].valuecount=ent[i].value[ent[i].pos];
                    }
                    if(ent[i].valuecount>0){
                        ent[i].valuecount--;
                        ent[i].angle--;                        
                    }
                    if(ent[i].valuecount==0){
                        ent[i].pos++;
                        ent[i].valuecount=-1;
                    }
                break;
                case 3://right
                    if(ent[i].valuecount==-1){
                        ent[i].valuecount=ent[i].value[ent[i].pos];
                    }
                    if(ent[i].valuecount>0){
                        ent[i].valuecount--;
                        ent[i].angle++;                        
                    }
                    if(ent[i].valuecount==0){
                        ent[i].pos++;
                        ent[i].valuecount=-1;
                    }
                break;

            }
        }
    

        

        ent[i].time2++;
        if(ent[i].time2>ent[i].timemax2 && ent[i].pos2<ent[i].maxcommand2){
            ent[i].time2=0;
            //
            switch (ent[i].command2[ent[i].pos2]){
                case 2://left
                    if(ent[i].valuecount2==-1){
                        ent[i].valuecount2=ent[i].value2[ent[i].pos2];
                    }
                    if(ent[i].valuecount2>0){
                        ent[i].valuecount2--;
                        ent[i].angle2--;                        
                    }
                    if(ent[i].valuecount2==0){
                        ent[i].pos2++;
                        ent[i].valuecount2=-1;
                    }
                break;
                case 3://right
                    if(ent[i].valuecount2==-1){
                        ent[i].valuecount2=ent[i].value2[ent[i].pos2];
                    }
                    if(ent[i].valuecount2>0){
                        ent[i].valuecount2--;
                        ent[i].angle2++;                        
                    }
                    if(ent[i].valuecount2==0){
                        ent[i].pos2++;
                        ent[i].valuecount2=-1;
                    }
                break;
            }
        }






    }


        

    
 
}