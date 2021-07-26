// Scoring <max bullets , no collision between tanks / 


#include "raylib.h"
#include <math.h>

#define MAXCOMMANDS 10
#define MAXENTITIES 10
#define MAXBULLETS 256

Texture2D sprites;

typedef struct bullet{
    bool active;
    Vector2 position;
    Vector2 inc;
    int countdown;
}bullet;

static struct bullet bul[MAXBULLETS];


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
void entitiescollision();
void drawentities();
void inientity(int entity,int x, int y);
void updatebullets();
void drawbullets();
void shootbullet(Vector2 position,int angle);
float distance(float x1,float y1,float x2,float y2);

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");
    sprites = LoadTexture("resources/sprites.png");
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------


    int x=200;
    int y=200;
    for(int i=0;i<MAXENTITIES;i++){
        inientity(i,x,y);
        x+=64;
        if(x>500){
            y+=64;
            x=200;
        }
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
        entitiescollision();
        updatebullets();
        

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);




        drawentities();
        drawbullets();

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

void shootbullet(Vector2 position, int angle){
    for(int i=0;i<MAXBULLETS;i++){
        if(bul[i].active==false){
            bul[i].active = true;
            bul[i].position = position;
            bul[i].inc.x = cos(angle*DEG2RAD)*3;
            bul[i].inc.y = sin(angle*DEG2RAD)*3;
            bul[i].position.x += cos(angle*DEG2RAD)*16;
            bul[i].position.y += sin(angle*DEG2RAD)*16;

            bul[i].countdown = 256;
            return;
        }
    }
}

void updatebullets(){
    for(int i=0;i<MAXBULLETS;i++){
        if(bul[i].active==false)continue;        
        bul[i].position.x += bul[i].inc.x;
        bul[i].position.y += bul[i].inc.y;
        bul[i].countdown--;
        if(bul[i].countdown<0)bul[i].active=false;

    }
}

void drawbullets(){
    for(int i=0;i<MAXBULLETS;i++){
        if(bul[i].active==false)continue;
        DrawTexturePro(sprites,  (Rectangle){32,0,16,16},// the -96 (-)means mirror on x axis
                                        (Rectangle){bul[i].position.x-16,bul[i].position.y-16,32,32},
                                        (Vector2){0,0},0,WHITE);

    }
    
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
        ent[entity].command[i]=GetRandomValue(1,4);
        ent[entity].value[i]=GetRandomValue(0,10);
    }
    for(int i=0;i<ent[entity].maxcommand2;i++){
        ent[entity].command2[i]=GetRandomValue(2,3);
        ent[entity].value2[i]=GetRandomValue(10,100);
        if(GetRandomValue(0,10)==1){
            ent[entity].command2[i]=4;
        }

        
    }

}

void drawentities(){
    for(int i=0;i<MAXENTITIES;i++){
        if(ent[i].active==false)continue;
        DrawTexturePro(sprites,  (Rectangle){0,0,16,16},// the -96 (-)means mirror on x axis
                                        (Rectangle){ent[i].position.x,ent[i].position.y,32,32},
                                        (Vector2){16,16},ent[i].angle,WHITE);
        DrawTexturePro(sprites,  (Rectangle){16,0,16,16},// the -96 (-)means mirror on x axis
                                        (Rectangle){ent[i].position.x,ent[i].position.y,32,32},
                                        (Vector2){16,16},ent[i].angle2,WHITE);
    }
}

void entitiescollision(){
    for(int i=0;i<MAXENTITIES;i++){
    for(int j=0;j<MAXENTITIES;j++){
        if(i==j)continue;
            if(distance(ent[i].position.x,ent[i].position.y,ent[j].position.x,ent[j].position.y)<36){
                //ent[i].active=false;
                int cnt=999;
                float angle=ent[i].angle*DEG2RAD;
                int x=ent[i].position.x;
                int y=ent[i].position.y;
                while(distance(x,y,ent[j].position.x,ent[j].position.y)<46){
                    x+=cos(angle)*2;
                    y+=sin(angle)*2;
                    cnt--;
                    if(cnt<0)break;
                }
                if(cnt>0){
                    ent[i].command[0]=1;
                    ent[i].value[0]=50;
                    ent[i].command[1]=2;
                    ent[i].value[1]=30;
                    ent[i].pos=0;
                    ent[i].valuecount=-1;

                }
                if(cnt<0){
                    x = ent[i].position.x;
                    y = ent[i].position.y;
                    angle=ent[i].angle*DEG2RAD;
                    cnt=999;
                    while(distance(x,y,ent[j].position.x,ent[j].position.y)<46){
                        x-=cos(angle)*2;
                        y-=sin(angle)*2;
                        cnt--;
                        if(cnt<0)break;
                    }
                    if(cnt>0){
                        ent[i].command[0]=4;
                        ent[i].value[0]=50;                   
                        ent[i].command[1]=3;
                        ent[i].value[1]=50;
                        ent[i].pos=0;
                        ent[i].valuecount=-1;
                        
                    }   
                }
                
                    
                
            }
    }
    }
}

void updateentities(){
 
    for(int i=0;i<MAXENTITIES;i++){
        if(ent[i].active==false)continue;






        if(ent[i].pos==ent[i].maxcommand){
            ent[i].pos=0;
            for(int ii=0;i<ent[i].maxcommand;i++){
                ent[i].command[ii]=GetRandomValue(1,4);
                ent[i].value[ii]=GetRandomValue(0,10);
            }
        }
        if(ent[i].pos2==ent[i].maxcommand2){
            
            ent[i].pos2=0;
            for(int ii=0;i<ent[i].maxcommand2;i++){
                ent[i].command2[ii]=GetRandomValue(2,3);
                ent[i].value2[ii]=GetRandomValue(10,100);
                if(GetRandomValue(0,10)==1){
                    ent[i].command2[ii]=4;
                }
                
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
                case 4://Backwards
                    if(ent[i].valuecount==-1){
                        ent[i].valuecount=ent[i].value[ent[i].pos];
                    }
                    if(ent[i].valuecount>0){
                        ent[i].valuecount--;
                        ent[i].position.x -= cos(ent[i].angle*DEG2RAD)*1;
                        ent[i].position.y -= sin(ent[i].angle*DEG2RAD)*1;                        
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
                case 4://shoot
                    shootbullet(ent[i].position,ent[i].angle2);
                    ent[i].pos2++;
                    ent[i].valuecount2=-1;
            }
        }






    }


        

    
 
}

// Manhattan Distance (less precise)
float distance(float x1,float y1,float x2,float y2){
    return (float)abs(x2-x1)+abs(y2-y1);
}