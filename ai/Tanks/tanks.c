// Scoring <max bullets , no collision between tanks / 


#include "raylib.h"
#include <math.h>

#define MAXCOMMANDS 10
#define MAXENTITIES 10
#define MAXBULLETS 256

Texture2D sprites;
Texture2D tiles;

typedef struct bullet{
    bool active;
    Vector2 position;
    Vector2 inc;
    int countdown;
}bullet;

static struct bullet bul[MAXBULLETS];


typedef struct entity{
    bool active;
    int collisionskip;
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


void getrandomcommands(int entity);
void updateentities();
void advancedcollision();
void entitiescollision();
void drawentities();
void inientity(int entity,int x, int y);
void updatebullets();
void drawbullets();
void shootbullet(Vector2 position,float angle);
float distance(float x1,float y1,float x2,float y2);
float getangle(float x1,float y1,float x2,float y2);
float angledifference(float angle1, float angle2);
void drawmap();
void drawtile(int tile, int x, int y);
void createmap();

int debug;

const int screenWidth = 800;
const int screenHeight = 600;

int map[150][150] = {0};

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");
    sprites = LoadTexture("resources/sprites.png");
    tiles = LoadTexture("resources/tiles.png");

    createmap();

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    for(int i=0;i<MAXBULLETS;i++){
        bul[i].active=false;
        bul[i].position=(Vector2){320,200};
        
    }
    for(int i=0;i<MAXENTITIES;i++){
        ent[i].active=false;
    }


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

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------
        
        updateentities();
        //entitiescollision();
        advancedcollision();
        updatebullets();
        //if(GetRandomValue(0,20)==1)shootbullet((Vector2){320,200},GetRandomValue(0,360));
        

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);



        drawmap();
        drawentities();
        drawbullets();

        DrawText(FormatText("%i",debug), 0, 0, 20, BLACK);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    UnloadTexture(sprites);
    UnloadTexture(tiles);
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

void createmap(){
    // Generate a kind of basic map
    for(int y=0;y<screenHeight/32;y++){
    for(int x=0;x<screenWidth/32;x++){
        map[x][y]=9;

    }}
    for(int y=0;y<10;y++){
    for(int x=0;x<10;x++){
        map[x][y]=11;
    }}
    for(int y=0;y<50;y++){
    for(int x=0;x<5;x++){
        map[x][y]=11;
    }}
    for(int y=0;y<5;y++){
    for(int x=0;x<50;x++){
        map[x][y]=11;
    }}
    
    // Auto tile it!
    for(int y=1;y<50;y++){
    for(int x=1;x<50;x++){
        if(map[x][y]==11){
            if(map[x+1][y]==11 && map[x][y+1]==11 && map[x+1][y+1]==9){
                map[x+1][y+1]=4;
            }
        }
        if(map[x][y]==11){
            if(map[x+1][y]==9 && map[x][y+1]==9 && map[x+1][y+1]==9){
                map[x+1][y+1]=21;//31;
            }
        }
        
    }}
    for(int y=1;y<50;y++){
    for(int x=1;x<50;x++){
        if(map[x][y]==11){
            if(map[x][y+1]==9){
                map[x][y+1]=5;
            }
            if(map[x+1][y]==9){
                map[x+1][y]=12;
            }
        }
    }}

}

void drawmap(){
    for(int y=0;y<screenHeight/32;y++){
    for(int x=0;x<screenWidth/32;x++){
        drawtile(map[x][y],x*32,y*32);
    }
    }
}
void drawtile(int tile, int x, int y){
    // Get the tile x , y position.
    int ty = tile / 9;    
    int tx = tile-(ty*9);;
    debug = tx;
    
    DrawTexturePro(tiles,  (Rectangle){tx*16,ty*16,16,16},// the -96 (-)means mirror on x axis
                                    (Rectangle){x,y,32,32},
                                    (Vector2){0,0},0,WHITE);

}

void shootbullet(Vector2 position, float angle){
    for(int i=0;i<MAXBULLETS;i++){
        if(bul[i].active==false){
            bul[i].active = true;
            bul[i].position = position;
            bul[i].inc.x = (cos(angle*DEG2RAD));
            bul[i].inc.y = (sin(angle*DEG2RAD));
            bul[i].inc.x *=3;
            bul[i].inc.y *=3;

            bul[i].position.x += cos(angle*DEG2RAD)*16;
            bul[i].position.y += sin(angle*DEG2RAD)*16;

            bul[i].countdown = 350;
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

//
// This function checks collision with each of the entities. It works by taking the current command and running it to its end.
// WIth forward and Backwards movement. If in this command we wil hit a other entity we scrap the entire command chain and
// build a new.
//
void advancedcollision(){
    for(int i=0;i<MAXENTITIES;i++){
    for(int j=0;j<MAXENTITIES;j++){
        if(i==j)continue;
        if(ent[i].command[ent[i].pos]==1){
            Vector2 pos = ent[i].position;
            for(int z=0;z<ent[i].value[ent[i].pos]-ent[i].valuecount;z++){
                pos.x+=cos(ent[i].angle*DEG2RAD)*1;
                pos.y+=sin(ent[i].angle*DEG2RAD)*1;
                if(distance(pos.x,pos.y,ent[j].position.x,ent[j].position.y)<36){
                    getrandomcommands(i);
                    continue;
                }
            }
        }
        if(ent[i].command[ent[i].pos]==4){
            Vector2 pos = ent[i].position;
            for(int z=0;z<ent[i].value[ent[i].pos]-ent[i].valuecount;z++){
                pos.x-=cos(ent[i].angle*DEG2RAD)*1;
                pos.y-=sin(ent[i].angle*DEG2RAD)*1;
                if(distance(pos.x,pos.y,ent[j].position.x,ent[j].position.y)<36){
                    getrandomcommands(i);
                    
                    continue;
                }
            }
        }
        
    }}

}

void entitiescollision(){
    for(int i=0;i<MAXENTITIES;i++){
    for(int j=0;j<MAXENTITIES;j++){
        if(i==j)continue;
            if(ent[i].collisionskip>0)ent[i].collisionskip--;
            if(ent[i].collisionskip==0 && distance(ent[i].position.x,ent[i].position.y,ent[j].position.x,ent[j].position.y)<36){
                
                //ent[i].active=false;
                float an=getangle(ent[i].position.x,ent[i].position.y,ent[j].position.x,ent[j].position.y);
                ent[i].angle=an*RAD2DEG;
                ent[i].command[0]=4;
                ent[i].value[0]=40;
                ent[i].pos=0;
                ent[i].valuecount=-1;

                }                 
            }
    }
    
}

void updateentities(){
 
    for(int i=0;i<MAXENTITIES;i++){
        if(ent[i].active==false)continue;






        if(ent[i].pos>=ent[i].maxcommand){
            ent[i].pos=0;
            for(int ii=0;ii<ent[i].maxcommand;ii++){
                ent[i].command[ii]=GetRandomValue(1,4);
                ent[i].value[ii]=GetRandomValue(0,10);
            }
            
            ent[i].command[0]=1;
            ent[i].value[0]=GetRandomValue(10,60);
            
        }
        if(ent[i].pos2>=ent[i].maxcommand2){
            
            ent[i].pos2=0;
            for(int ii=0;ii<ent[i].maxcommand2;ii++){
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

//
// This function gives the entity a new set of commands. This for the body and the turret.
//
void getrandomcommands(int entity){
        ent[entity].pos=0;
        for(int ii=0;ii<ent[entity].maxcommand;ii++){
            ent[entity].command[ii]=GetRandomValue(1,4);
            ent[entity].value[ii]=GetRandomValue(0,10);
        }
                
        
        ent[entity].pos2=0;
        for(int ii=0;ii<ent[entity].maxcommand2;ii++){
            ent[entity].command2[ii]=GetRandomValue(2,3);
            ent[entity].value2[ii]=GetRandomValue(10,100);
            if(GetRandomValue(0,10)==1){
                ent[entity].command2[ii]=4;
            }
        }
            

}

// Manhattan Distance (less precise)
float distance(float x1,float y1,float x2,float y2){
    return (float)abs(x2-x1)+abs(y2-y1);
}
// Return the angle from - to in float
float getangle(float x1,float y1,float x2,float y2){
    return (float)atan2(y2-y1, x2-x1);
}
// takes radian iput! <0 is left is shorter else right turn is shorter.
// When it outputs >3 you can asume it aligns with the target(2) angle.
float angledifference(float angle1, float angle2){
    float difference = angle1 - angle2;
    while (difference < -PI){
        difference += (PI*2);
    }
    while (difference > PI){ 
        difference -= (PI*2);
    }
    return difference;

}