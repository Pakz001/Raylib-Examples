/*
    Particle Swarm (UNOPTIMIZED!)
    
    Note : I'm not sure, but there is something wrong with the random. I have to do a -1.7 .. 1 for the
    swarm to stay in one spot. Not sure what I did wrong to cause that. ????
    Update: I added a float_rand function from the internet and this seemed to have fixed the problem.
    
    
    
    Optimalization : create a array[y][x][MAX_PARTICLES] of struct which contain
    the particles on that location.
*/

#include "raylib.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_PARTICLES 128//500/10 is slow here on my laptop(2021)
#define MAX_MOVESPEED 25

// This is out collision map. It is a array for each spot a particle can be on.
// It is true if there is a particle there or false if not.
// Before moving a particle we can check if there is a particle there already so
// not to move there.
//
// We need to check this map every time a particle gets moved (uncheck move check)
//
// Functions : setcollisionmap and checkcollisionmap
bool collisionmap[1024][1024]={false};

typedef struct particle{
    Vector2 position;
}particle;

struct particle arr_particle[MAX_PARTICLES];

Vector2 targetposition;


static float edistance(float x1,float y1,float x2,float y2);
void setcollisionmap(Vector2 position,bool value);
bool checkcollisionmap(Vector2 position);
float float_rand( float min, float max );
void moveparticles();
void movetoneigbours();
void movetotarget();    

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib example.");
 
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    //create a set of particles
    Vector2 p = (Vector2){200,200};
    for(int i=0;i<MAX_PARTICLES;i++){
        Vector2 pos=(Vector2){GetRandomValue(p.x-35,p.x+35),GetRandomValue(p.y-35,p.y+35)};
        
        while(checkcollisionmap(pos)==true){
            pos=(Vector2){GetRandomValue(p.x-35,p.x+35),GetRandomValue(p.y-35,p.y+35)};
        }
        arr_particle[i].position = pos;
        setcollisionmap(arr_particle[i].position,true);
        
        
    }

    int renew=0;
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        if(renew<=0){
            targetposition.x = GetRandomValue(50,screenWidth-50);
            targetposition.y = GetRandomValue(50,screenHeight-50);
            renew=500;
        }
        renew--;

        for(int i=0;i<MAX_MOVESPEED;i++){
            moveparticles();
            movetoneigbours();
            movetotarget();
        }
        //----------------------------------------------------------------------------------
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);
                
            // Draw particles
            for(int i=0;i<MAX_PARTICLES;i++){
                Vector2 p = arr_particle[i].position;
                DrawCircle(p.x,p.y,2,RED);
            }
            
            // Draw target position
            DrawCircle(targetposition.x,targetposition.y,20,(Color){255,0,0,100});

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;


}

// Euclidean distance (more precise)
float edistance(float x1,float y1,float x2,float y2){
    return sqrt( (x1-x2)*(x1-x2)+(y1-y2)*(y1-y2) );
}
void setcollisionmap(Vector2 position,bool value){
        if(position.x<0)return;
        if(position.x>1023)return;
        if(position.y<0)return;
        if(position.y>1023)return;

        collisionmap[(int)position.x][(int)position.y]=value;;
}
bool checkcollisionmap(Vector2 position){
    if(position.x<0)return true;
    if(position.x>1023)return true;
    if(position.y<0)return true;
    if(position.y>1023)return true;

    return (collisionmap[(int)position.x][(int)position.y]);
}

float float_rand( float min, float max ){
    float scale = rand() / (float) RAND_MAX; /* [0, 1.0] */
    return min + scale * ( max - min );      /* [min, max] */
}

void moveparticles(){
    //update particles
    //move random
    for(int i=0;i<MAX_PARTICLES;i++){            
        Vector2 np = arr_particle[i].position;                                       
        //np.x+=floatuniform(-1.72,1);
        //np.y+=floatuniform(-1.72,1);
        np.x += float_rand(-1,1);
        np.y += float_rand(-1,1);
        if(checkcollisionmap(np)==false){
            setcollisionmap(arr_particle[i].position,false);
            arr_particle[i].position=np;
            setcollisionmap(arr_particle[i].position,true);
        }
    }
    //move to midpoint nearest
}

void movetoneigbours(){    
    for(int i=0;i<MAX_PARTICLES;i++){
        // find neigbours (3 of them)
        Vector2 p1 = arr_particle[i].position;
        int d1=0;
        int cnt=0;
        int timeout=0;
        Vector2 midpoint = (Vector2){0,0};
        while(cnt<5 && timeout<1000){
            for(int j=0;j<MAX_PARTICLES;j++){
                if(j==i || cnt>4)continue;                   
                Vector2 p2 = arr_particle[j].position;
                int d2 = (int)edistance(p1.x,p1.y,p2.x,p2.y);
                if(d2==d1 && cnt<5){                        
                    if(cnt==0){                            
                        midpoint.x = p2.x;
                        midpoint.y = p2.y;
                    }else{
                        midpoint.x += p2.x;
                        midpoint.y += p2.y;
                    }
                    cnt++;                        
                }
            }
            d1++;
            timeout++;
            
        }
        midpoint.x /= 5.0f;
        midpoint.y /= 5.0f;
        float angle=atan2(midpoint.y-p1.y,midpoint.x-p1.x);            
        
        Vector2 newposition = (Vector2){    arr_particle[i].position.x + cos(angle)*.3,
                                            arr_particle[i].position.y + sin(angle)*.3};
        if( checkcollisionmap(newposition)==false ){                
            setcollisionmap(arr_particle[i].position,false);
            arr_particle[i].position.x += cos(angle)*.3;
            arr_particle[i].position.y += sin(angle)*.3;
            setcollisionmap(newposition,true);
        }
    }    
}

void movetotarget(){

    for(int i=0;i<MAX_PARTICLES;i++){        
        //
        // Move to target            
        //
        Vector2 p1 = arr_particle[i].position;
        float targetangle=atan2(targetposition.y-p1.y,targetposition.x-p1.x);            
        Vector2 newposition = (Vector2){    arr_particle[i].position.x + cos(targetangle)*.4,
                                            arr_particle[i].position.y + sin(targetangle)*.4};
        if( checkcollisionmap(newposition)==false ){                
            setcollisionmap(arr_particle[i].position,false);
            arr_particle[i].position.x += cos(targetangle)*.4;
            arr_particle[i].position.y += sin(targetangle)*.4;
            setcollisionmap(newposition,true);
        }
    }
}
