//
// Genetic Algorithm Example 

// Strategy board game war. Units(2-blue) try to capture the target(4-white/yellow) objective
// and eliminate the enemy units(3-red)
// 
// A random movement phase is executed. Here a random spot is selected on the map
// if there is a movable unit than this is moved in any direction. If the new position
// is a enemy than this enemy is taken out. The objective can also be captured.
//
// There are MAX_CODE of moves per set. Then a score is calculated based on how
// close the units are to the GOAL(4) and how many enemies are destroyed(3) and if
// the GOAL(4) has been captured.

// There are MAX_ARC amount of moves per set stored. The top 3 scoring runs get
// copied and these copies get mutated so they might score better. Also a number
// of random new runs are added.
// 
// The process of simulating the moves and scoring and mutating is done MAX_RUNS amount of times.

// The final top scoring set of moves is played on the canvas.
//

// Better scoring could be done by storing the amount of move taken to take out x enemy units and capturing target.
//
// Building on this you might change the amount of units for each side. maybe mod the amount of goals to be captured.
// In a real game you might use this to simulate if a war for a ai player would be doable in a certain tested setting like this.
//
// Create unit types with different att and def, more scoring with 1+units attacking at the same time.
// add scoring in the execute script stage : units stay close to each other, direct path to target etc.
//
// Crossbreeding - taking two from higher scorers and combining these into new.
// Seeding the list at start with for instance steps into the right direction. 
// IMPLEMENTED>>>If move on enemy position (chance of being destroyed itself(.5) <x if more units of own side are nearby)

#include "raylib.h"
#include <math.h>

#define MAX_RUNS 200
#define MAX_CODE 1200
#define MAX_ARC 64

enum terrain{TREE=1,GROUND=0,AIPLAYER1=2,AIPLAYER2=3,GOAL=4,GOALREACHED=5};
enum flag{UP,DOWN,LEFT,RIGHT,WAIT};

//
// You can set any number of 3 and 2 on the map. more 4's untested. keep the borders(1) intact(memory errors)
//
//1=border,2=ai unit,3=defensiveunit,4=objective
int map1[10][10] = {    {1,1,1,1,1,1,1,1,1,1},
                        {1,0,0,0,0,0,0,0,0,1},
                        {1,0,0,0,3,0,0,4,0,1},
                        {1,0,0,3,0,0,3,0,0,1},
                        {1,0,0,0,0,0,0,0,0,1},
                        {1,2,0,0,0,0,0,3,0,1},
                        {1,0,2,0,0,0,0,0,0,1},
                        {1,0,0,0,0,0,0,2,0,1},
                        {1,0,0,0,0,0,0,2,0,1},
                        {1,1,1,1,1,1,1,1,1,1}
                        };  
int map[10][10] = {0};


typedef struct code{
    Vector2 position;
    Vector2 move;
}code;
static struct code arr_code[MAX_CODE];

typedef struct codearc{
    Vector2 position[MAX_CODE];
    Vector2 move[MAX_CODE];
    int score;
}codearc;
static struct codearc arr_codearc[MAX_ARC];
static struct codearc arr_temparc[MAX_ARC];


int getscore();
void executescript();
void storescript(int z,int score);
void sortarc();//copy top 3 to max-5
void mutateandnew();
void geneticalgorithm();
static float distance(float x1,float y1,float x2,float y2);
static float edistance(float x1,float y1,float x2,float y2);

int screenWidth;
int screenHeight;
int tileWidth,tileHeight,mapWidth,mapHeight;



int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    screenWidth = 800;
    screenHeight = 600;
    mapWidth = 10;
    mapHeight = 10;
    tileWidth = ceil((float)screenWidth/(float)mapWidth);
    tileHeight = ceil((float)screenHeight/(float)mapHeight);
    
    InitWindow(screenWidth, screenHeight, "raylib example.");
 
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    geneticalgorithm();
   
    
    int playposition=0;
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        
        if(IsKeyPressed(KEY_SPACE)|| playposition>MAX_CODE-2){
            geneticalgorithm();
            playposition=0;
        }
        
        if(playposition==0){
            // restore map
            for(int y=0;y<10;y++){
            for(int x=0;x<10;x++){
                map[y][x]=map1[y][x];
            }}                
        }
        // execute script
        if(map[ (int)arr_code[playposition].position.y ][ (int)arr_code[playposition].position.x ]==AIPLAYER1){
            Vector2 p=arr_code[playposition].position;
            Vector2 m=arr_code[playposition].move;
            Vector2 np=(Vector2){p.x+m.x,p.y+m.y};
            if(map[ (int)np.y ][ (int)np.x ]==GOAL){
                map[ (int)np.y ][ (int)np.x ] = GOALREACHED;
            }
            if(map[ (int)np.y ][ (int)np.x ]==GROUND){
                map[ (int)np.y ][ (int)np.x ] = AIPLAYER1;
                map[ (int)p.y ][ (int)p.x ] = GROUND;
            }
            if(map[ (int)np.y ][ (int)np.x ]==AIPLAYER2){
                map[ (int)np.y ][ (int)np.x ] = AIPLAYER1;
                map[ (int)p.y ][ (int)p.x ] = GROUND;
                
            }                
        }
        
        playposition+=1;
        if(playposition>MAX_CODE-1){
            playposition=0;
        }
        
        //----------------------------------------------------------------------------------
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);
            //drawmap
            for(int y=0;y<10;y++){
                for(int x =0;x<10;x++){
                    int m=map[y][x];
                    if(m==TREE){
                        
                        DrawRectangle(x*tileWidth,y*tileHeight,tileWidth,tileHeight,(Color){40,90,20,255});
                        DrawCircle(x*tileWidth+tileWidth/1.5,y*tileHeight+tileHeight/1.5,tileWidth/5,(Color){20,50,20,255});
                        DrawRectangle(x*tileWidth+tileWidth/2.2,y*tileHeight+tileHeight/2,tileWidth/8,tileHeight/3,BROWN);
                        
                        DrawCircle(x*tileWidth+tileWidth/2,y*tileHeight+tileHeight/3,tileWidth/4,(Color){120,250,20,255});
                        DrawCircle(x*tileWidth+tileWidth/2.2,y*tileHeight+tileHeight/4,tileWidth/9,(Color){220,255,220,155});
                    }
                    if(m==GROUND){
                        DrawRectangle(x*tileWidth,y*tileHeight,tileWidth,tileHeight,DARKGREEN);                        
                        DrawRectangle(x*tileWidth+5,y*tileHeight+10,2,1,GREEN);                        
                        DrawRectangle(x*tileWidth+tileWidth/6,y*tileHeight+tileHeight/6,2,1,GREEN);                        
                        DrawRectangle(x*tileWidth+tileWidth/1.5,y*tileHeight+tileHeight/1.5,2,1,GREEN);                        
                        DrawRectangle(x*tileWidth+tileWidth/2,y*tileHeight+tileHeight/2,2,1,GREEN);                        
                        
                    }
                    if(m==AIPLAYER1){
                        DrawRectangle(x*tileWidth,y*tileHeight,tileWidth,tileHeight,BLUE);                        
                        DrawText("AI_Player",x*tileWidth,y*tileHeight+tileHeight/4,16,BLACK);                        
                    }
                    if(m==AIPLAYER2){
                        DrawRectangle(x*tileWidth,y*tileHeight,tileWidth,tileHeight,RED); 
                        DrawText("F",x*tileWidth+tileWidth/3,y*tileHeight+tileHeight/4,40,WHITE);
                    }
                    if(m==GOAL){
                        DrawRectangle(x*tileWidth,y*tileHeight,tileWidth,tileHeight,WHITE);      
                       DrawText("City",x*tileWidth+tileWidth/4,y*tileHeight+tileHeight/4,26,BLACK);                        
                    }
                    if(m==GOALREACHED){
                        DrawRectangle(x*tileWidth,y*tileHeight,tileWidth,tileHeight,YELLOW);                        
                        DrawText("Captured",x*tileWidth+4,y*tileHeight+tileHeight/4,16,BLACK);                        
                    }


            }
            }
            DrawText("Press space for new simulation.(Autorun=on)",10,10,26,BLACK);
            DrawText("Press space for new simulation.(Autorun=on)",9,9,26,WHITE);
            
            
            /*
            int c=0;
            for(int y=0;y<40;y++){
            for(int x=0;x<5;x++){
                if(c<MAX_ARC){
                    DrawText(FormatText("%i",arr_codearc[c].score),x*200,y*20,20,BLACK);
                }
                c++;
            }}
            */

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;


}

void geneticalgorithm(){

    // First run
    for(int z=0;z<MAX_ARC;z++){
        //create random script
        for(int i=0;i<MAX_CODE;i++){
            arr_code[i].position=(Vector2){GetRandomValue(0,10),GetRandomValue(0,10)};
            arr_code[i].move=(Vector2){GetRandomValue(-1,1),GetRandomValue(-1,1)};
        }
        
        // restore map
        for(int y=0;y<10;y++){
        for(int x=0;x<10;x++){
            map[y][x]=map1[y][x];
        }}
        
        executescript();
        // SCORING!
        int score = getscore();
        // store script
        storescript(z,score);


    }
    
    //keep top 3
    // copy top 3 to max-5
    //mutate 3..max-5
    //new random max-5 max
    //
    // Run it and mutate picking winners
    for(int t=0;t<MAX_RUNS;t++){
        sortarc();
        mutateandnew();

        for(int z=0;z<MAX_ARC;z++){
            // simulate new
            // restore map
            for(int y=0;y<10;y++){
            for(int x=0;x<10;x++){
                map[y][x]=map1[y][x];
            }}
            getscript(z);
            executescript();
            
            
            // SCORING!
            int score = getscore();
            arr_codearc[z].score = score;
            // store script
            //storescript(z,score);
        }
    }
    //return;
    // restore map and get the victor into the arr_code (our script)
    // restore map
    for(int y=0;y<10;y++){
    for(int x=0;x<10;x++){
        map[y][x]=map1[y][x];
    }}    
    getscript(0);
    //executescript();
}

void mutateandnew(){    
    //mutate 3 to max-5
    // This is from a starting point from %10 to %50 to end
    for(int z=3;z<MAX_ARC-5;z++){
        int start=GetRandomValue(MAX_CODE/4,MAX_CODE/1.5);
        for(int i=start;i<MAX_CODE;i++){
            arr_codearc[z].position[i]=(Vector2){GetRandomValue(0,10),GetRandomValue(0,10)};
            arr_codearc[z].move[i]=(Vector2){GetRandomValue(-1,1),GetRandomValue(-1,1)};
        }
    }
    // max-5 to max is new ones
    for(int z=MAX_ARC-5;z<MAX_ARC;z++){
        for(int i=0;i<MAX_CODE;i++){
            arr_codearc[z].position[i]=(Vector2){GetRandomValue(0,10),GetRandomValue(0,10)};
            arr_codearc[z].move[i]=(Vector2){GetRandomValue(-1,1),GetRandomValue(-1,1)};
        }
    }
}
 

void sortarc(){
    int top3[3] = {0};
    int startval=800;
    int currentpos=0;
    // get top 3 in list top3
    while(startval>0 && currentpos<3){
        for(int i=0;i<MAX_ARC;i++){
            if(arr_codearc[i].score==startval){
                if(currentpos<3){
                    top3[currentpos]=i;
                }
                currentpos++;
            }
        }
        startval--;
    }
    
    // copy the 3 tops into temp arc
    for(int i=0;i<MAX_CODE;i++){
        for(int j=0;j<3;j++){
            arr_temparc[j].position[i] = arr_codearc[top3[j]].position[i];
            arr_temparc[j].move[i] = arr_codearc[top3[j]].move[i];
            arr_temparc[j].score = arr_codearc[top3[j]].score;
        }
    }
    
    // copy top up to max-5
    for(int i=0;i<MAX_ARC-8;i+=3){            
        for(int k=0;k<3;k++){
            for(int j=0;j<MAX_CODE;j++){
                arr_codearc[i+k].position[j] = arr_temparc[k].position[j];
                arr_codearc[i+k].move[j] = arr_temparc[k].move[j];
                arr_codearc[i+k].score = arr_temparc[k].score;
            }
        }
    }
}

void getscript(int z){
    // store script
    //arr_codearc[z].score = score;
    //arr_code.score = 
    for(int i=0;i<MAX_CODE;i++){
        //arr_codearc[z].move[i] = arr_code[i].move;
        //arr_codearc[z].position[i] = arr_code[i].position;
        arr_code[i].move = arr_codearc[z].move[i];
        arr_code[i].position = arr_codearc[z].position[i];
    }    
}        


void storescript(int z,int score){
    // store script
    arr_codearc[z].score = score;

    for(int i=0;i<MAX_CODE;i++){
        arr_codearc[z].move[i] = arr_code[i].move;
        arr_codearc[z].position[i] = arr_code[i].position;
    }    
}        

int getscore(){
    int score=0;
    //count enemies left
    int numenemiesleft=0;
    int playersleft=0;
    for(int y=0;y<10;y++){
    for(int x=0;x<10;x++){
        if(map[y][x]==AIPLAYER2)numenemiesleft++;
        if(map[y][x]==AIPLAYER1)playersleft++;
    }
    }
    //was the goal taken
    bool objectivesuccess = false;
    Vector2 goalposition;
    for(int y=0;y<10;y++){
    for(int x=0;x<10;x++){
        if(map[y][x]==GOALREACHED)objectivesuccess=true;
        if(map[y][x]==GOAL || map[y][x]==GOALREACHED){
            goalposition.x = x;
            goalposition.y = y;
        }
    }
    }
 
    //count aiplayer1 and distance to target
    float avdist=0;
    int nums=0;
    int distanceunit[114]={0};
    for(int y=0;y<10;y++){
    for(int x=0;x<10;x++){
        if(map[y][x]==AIPLAYER1){
            int d = 0;
            d=edistance((float)goalposition.x,(float)goalposition.y,(float)x,(float)y)*2;
            distanceunit[nums]=(int)d;
            avdist+=d;
            nums++;
        }
    }}
    avdist=avdist/(float)playersleft;
    

    score = (100-(numenemiesleft*25))*2;
    
    score+=(100-(int)avdist*5);
    if(objectivesuccess==true)score+=75;
    // if every unit is close then extra score
    bool allclose=true;
    for(int i=0;i<playersleft;i++){
        if(distanceunit[i]>4)allclose=false;
    }
    if(allclose==true)score+=100;

    

    //score=avdist;
    //if(score<0)score=9999;
    
    return score;
}


void executescript(){
    // execute script
    for(int i=0;i<MAX_CODE;i++){
        if(map[ (int)arr_code[i].position.y ][ (int)arr_code[i].position.x ]==AIPLAYER1){
            Vector2 p=arr_code[i].position;
            Vector2 m=arr_code[i].move;
            Vector2 np=(Vector2){p.x+m.x,p.y+m.y};
            if(map[ (int)np.y ][ (int)np.x ]==GOAL){
                map[ (int)np.y ][ (int)np.x ] = GOALREACHED;
            }
            if(map[ (int)np.y ][ (int)np.x ]==GROUND){
                map[ (int)np.y ][ (int)np.x ] = AIPLAYER1;
                map[ (int)p.y ][ (int)p.x ] = GROUND;
            }
            if(map[ (int)np.y ][ (int)np.x ]==AIPLAYER2){
                int neigh=0;
                //for(int y=p.y-1;p.y+1;y++){
                //for(int x1=(int)p.x-1;(int)p.x+1;x1++){
                //
                //
                
                if(map[(int)np.y][(int)np.x-1]==AIPLAYER1)neigh+=1;
                if(map[(int)np.y][(int)np.x+1]==AIPLAYER1)neigh+=1;
                if(map[(int)np.y-1][(int)np.x]==AIPLAYER1)neigh+=1;
                if(map[(int)np.y+1][(int)np.x]==AIPLAYER1)neigh+=1;
                if(map[(int)np.y+1][(int)np.x-1]==AIPLAYER1)neigh+=1;
                if(map[(int)np.y-1][(int)np.x+1]==AIPLAYER1)neigh+=1;
                if(map[(int)np.y-1][(int)np.x+1]==AIPLAYER1)neigh+=1;
                if(map[(int)np.y+1][(int)np.x-1]==AIPLAYER1)neigh+=1;


                //}//}                
                if(GetRandomValue(0,10-(neigh*2))<4){
                    map[ (int)np.y ][ (int)np.x ] = AIPLAYER1;
                    map[ (int)p.y ][ (int)p.x ] = GROUND;
                }else{
                //map[ (int)np.y ][ (int)np.x ] = AIPLAYER2;
                    map[ (int)p.y ][ (int)p.x ] = GROUND;                    

                }
                
                
            }                
        }
    }
}


// Manhattan Distance (less precise)
float distance(float x1,float y1,float x2,float y2){
    return (float)abs(x2-x1)+abs(y2-y1);
}

// Euclidean distance (more precise)
float edistance(float x1,float y1,float x2,float y2){
    return sqrt( (x1-x2)*(x1-x2)+(y1-y2)*(y1-y2) );
}
