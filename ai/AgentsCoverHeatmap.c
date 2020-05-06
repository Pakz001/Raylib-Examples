// Agents take Cover.

// Todo :   
//          add agents and agents follow path.
//              select path not going through heatmap.


#include "raylib.h"
#include <math.h>

enum tiles{FLOOR,WALL};

#define MAP_WIDTH 40
#define MAP_HEIGHT 20
#define MAX_TURRETS 1
#define TURRET_TURNSPEED 0.01
#define MAX_BULLETS 128
#define BULLET_SPEED 3
#define TURRET_TARGET_TIME 600
#define MAX_AGENTS 15
#define MAX_PATH 100024 

static int screenWidth,screenHeight;

// This is our tile map. Note that [y][x]!!
// 1 = wall
int tempmap[10][20] =            {  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
                                    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                                    {1,0,0,1,1,0,0,0,1,1,1,1,0,0,0,1,1,0,0,1},
                                    {1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1},
                                    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                                    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                                    {1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1},
                                    {1,0,0,1,1,0,0,0,1,1,1,1,0,0,0,1,1,0,0,1},
                                    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                                    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}};  
int map[MAP_HEIGHT][MAP_WIDTH]={0};
// heat map..
// The heat map is used here to let the agents know the map area heated is dangerous and not
// to be traveled on. (influence mapping.)
float hmap[MAP_HEIGHT][MAP_WIDTH]={0};
// cover position map..
// The cover map is a map where positions are where bullets from the enemy do not have effect. (behind walls)
// I find them by simulating the turret shooting in every angle.
bool cmap[MAP_HEIGHT][MAP_WIDTH]={0};
// coverislands 
// coverspots connected to each other have a unique number.
// These are used to let the agents run from one island to another
int cimap[MAP_HEIGHT][MAP_WIDTH]={0};
static int numislands;

//path map
int pathmap[MAP_HEIGHT][MAP_WIDTH]={0};


float tileWidth,tileHeight;


typedef struct turret{
    float x,y;
    float angle;    
    float direction;
    int burst;
    bool targetactive;
    int targetx,targety;
    int targettime;
}turret;

static struct turret arr_turret[MAX_TURRETS];

typedef struct bullet{
    bool active;
    float x,y;
    float angle;
    int speed;
}bullet;

static struct bullet arr_bullet[MAX_BULLETS];

typedef struct agent{
    bool active;
    float x,y;
    int speed;
    int pathx[1024];
    int pathy[1024];
    int pathloc;
    int pathlen;
    int myisland;
}agent;

static struct agent arr_agent[MAX_AGENTS];

typedef struct pathnode{
    int x;
    int y;
    
}pathnode;

static pathnode arr_path[MAX_PATH];
static int arr_path_len;



static void drawmap();
static void drawturrets();
static void updateturrets();
static void drawbullets();
static void updatebullets();
static void shootbullet(int x, int y, float angle);
static bool recttilecollide(int x,int y,int w, int h, int offsetx,int offsety);
static bool rectsoverlap(int x1,int y1,int w1,int h1,int x2,int y2,int w2,int h2);
static void drawheatmap();
static void updateheatmap();
static int orientation(int ax,int ay,int bx, int by, int cx, int cy);
static void createcovermap();
static void drawcovermap();
static void drawcoverislands();    
static void createcoverislands();
static void drawagents();    
static void updateagents();
static bool agentfindpath(int agent, int island);
        
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    screenWidth = 800;
    screenHeight = 450;
    tileWidth = ceil((float)screenWidth/(float)MAP_WIDTH);
    tileHeight = ceil((float)screenHeight/(float)MAP_HEIGHT);
    
    // first let us copy the temp map into the main map(double size)
    for (int y=0;y<MAP_HEIGHT/2;y++){
    for(int x=0;x<MAP_WIDTH/2;x++){
        map[y*2][x*2]=tempmap[y][x];
        map[y*2+1][x*2]=tempmap[y][x];
        map[y*2][x*2+1]=tempmap[y][x];
        map[y*2+1][x*2+1]=tempmap[y][x];

    }
    }
    
    InitWindow(screenWidth, screenHeight, "raylib example.");
 
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Our turret is placed in the center..
    arr_turret[0].x = 20;

    arr_turret[0].y = 10;
    arr_turret[0].angle = 0;
    arr_turret[0].direction = TURRET_TURNSPEED;
    arr_turret[0].burst=0;
    arr_turret[0].targetactive = true;
    arr_turret[0].targetx = 12;
    arr_turret[0].targety = 3;
    arr_turret[0].targettime=TURRET_TARGET_TIME;
    // create our covermap
    createcovermap();
    createcoverislands();
    
  
for (int i=0;i<MAX_AGENTS;i++){
    arr_agent[i].active = true;
    arr_agent[i].x = 3*tileWidth;
    arr_agent[i].y = 3*tileHeight;
    arr_agent[i].speed = 2;
    arr_agent[i].myisland=1;
    arr_agent[i].pathloc=-1;
    arr_agent[i].pathlen=-1;
}    
/*    arr_agent[0].active = true;
    arr_agent[0].x = 3*tileWidth;
    arr_agent[0].y = 3*tileHeight;
    arr_agent[0].speed = 2;
    arr_agent[0].myisland=1;
    arr_agent[0].pathloc=-1;
    arr_agent[0].pathlen=-1;
*/

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        updateturrets();
        updatebullets();
        updateheatmap();
        updateagents();
        
        // If the user presses the mouse then place new target on map at mpos;
        if(IsMouseButtonDown(0)){
            arr_turret[0].targetactive=true;
            arr_turret[0].targetx = GetMouseX()/tileWidth;
            arr_turret[0].targety = GetMouseY()/tileHeight;
            arr_turret[0].targettime=TURRET_TARGET_TIME;
        }



        //----------------------------------------------------------------------------------
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);
            drawheatmap();
            drawcovermap();
            drawcoverislands();
            drawmap();
            drawturrets();
            drawbullets();
            //draw the path..
            for(int i=0;i<arr_path_len-1;i++){
                DrawRectangle(arr_path[i].x*tileWidth,arr_path[i].y*tileHeight,tileWidth,tileHeight,YELLOW);
            }
            drawagents();
                            
            DrawText("Press mouse to place target.. Space to path..",0,0,20,GRAY);
            
            DrawRectangle(0,screenHeight-44,screenWidth,28,DARKGRAY);
            DrawText("Heatmap",130,screenHeight-40,20,WHITE);
            DrawRectangle(100,screenHeight-40,20,20,RED);
            DrawRectangle(screenWidth/2,screenHeight-40,20,20,GREEN);
            DrawText("Covermap",screenWidth/2+30,screenHeight-40,20,WHITE);


        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;


}

void drawmap(){
    for(int y=0;y<MAP_HEIGHT;y++){
    for(int x=0;x<MAP_WIDTH;x++){
        if(map[y][x]==1){
            DrawRectangle(x*tileWidth,y*tileHeight,tileWidth,tileHeight,BLACK);
        }
    }
    }
}

void drawturrets(){
    for(int i=0;i<MAX_TURRETS;i++){
        int x = arr_turret[i].x*tileWidth;
        int y = arr_turret[i].y*tileHeight;
        float angle = arr_turret[i].angle;
        int x1 = x+tileWidth/2;
        int y1 = y+tileHeight/2;
        int x2 = x1+cos(angle)*(tileWidth);
        int y2 = y1+sin(angle)*(tileHeight);
        DrawRectangle(x,y,tileWidth,tileHeight,BLUE);
        DrawLineEx((Vector2){x1,y1},(Vector2){x2,y2},6,RED);
        // draw target if active
        if(arr_turret[i].targetactive){
            x = arr_turret[i].targetx*tileWidth;
            y = arr_turret[i].targety*tileHeight;
            DrawRectangle(x,y,tileWidth,tileHeight,BLUE);
        }
    }
}

void updateturrets(){
    for(int i=0;i<MAX_TURRETS;i++){
        
        // If there is no target the turn randomly
        if(arr_turret[i].targetactive==false && GetRandomValue(0,500)<2){
            arr_turret[i].direction = -arr_turret[i].direction;
        }
        // If there is a target then turn towards taget.
        if(arr_turret[i].targetactive){
            int x=(arr_turret[i].x*tileWidth);
            int y=(arr_turret[i].y*tileHeight);
            int oriented=orientation(       x,
                                            y,
                                            x+cos(arr_turret[i].angle)*tileWidth*12,
                                            y+sin(arr_turret[i].angle)*tileHeight*12,
                                            arr_turret[i].targetx*tileWidth,
                                            arr_turret[i].targety*tileHeight);
            if(oriented==-1){                        
                        arr_turret[i].direction=-TURRET_TURNSPEED;
                        }else if(oriented==1){
                            arr_turret[i].direction=TURRET_TURNSPEED;
                        }
        }
        // rotate the turret
        arr_turret[i].angle+=arr_turret[i].direction;        
        if(arr_turret[i].angle>PI*2.0f){
            arr_turret[i].angle=0;
        }

        if(arr_turret[i].angle<0){
            arr_turret[i].angle=PI*2.0f;
        }
        // shoot bullets
        if(GetRandomValue(0,60)<2 || arr_turret[i].burst>0){            
            if(arr_turret[i].burst>0){
                arr_turret[i].burst-=1;
            }else{
                if(GetRandomValue(0,20)<2)arr_turret[i].burst=5;
            }
            int bx = arr_turret[i].x*tileWidth;
            int by = arr_turret[i].y*tileHeight;
            shootbullet(    (bx+tileWidth/2)+(cos(arr_turret[i].angle)*tileWidth),
                            (by+tileHeight/2)+(sin(arr_turret[i].angle)*tileHeight),
                            arr_turret[i].angle);
        }
        // if target is active then decrease that time
        if(arr_turret[i].targetactive){
            arr_turret[i].targettime-=1;
            if(arr_turret[i].targettime<=0){
                arr_turret[i].targetactive = false;
            }
        }

        
    }
}

void drawbullets(){
    for(int i=0;i<MAX_BULLETS;i++){
        if(arr_bullet[i].active==false)continue;
        DrawRectangle(arr_bullet[i].x,arr_bullet[i].y,tileWidth/4,tileHeight/4,BLACK);
        DrawRectangle(arr_bullet[i].x+1,arr_bullet[i].y+1,tileWidth/4-2,tileHeight/4-2,YELLOW);
    }
}

void updatebullets(){
    for(int i=0;i<MAX_BULLETS;i++){
        if(arr_bullet[i].active==false)continue;
 
        for(int speed=0;speed<arr_bullet[i].speed;speed++){
            arr_bullet[i].x += cos(arr_bullet[i].angle)*1;
            arr_bullet[i].y += sin(arr_bullet[i].angle)*1;
            // if outside screen then disable bullet.
            if(     arr_bullet[i].x<0 || 
                    arr_bullet[i].x>screenWidth || 
                    arr_bullet[i].y<0 || 
                    arr_bullet[i].y>screenHeight){
                arr_bullet[i].active = false;
            }
            if(recttilecollide( arr_bullet[i].x,
                                arr_bullet[i].y,
                                tileWidth/4,
                                tileHeight/4,
                                0,0)){
                arr_bullet[i].active=false;
            }
        }
    }    
}

void shootbullet(int x,int y,float angle){
    for(int i=0;i<MAX_BULLETS;i++){
        if(arr_bullet[i].active==false){
            arr_bullet[i].active = true;
            arr_bullet[i].x = x;
            arr_bullet[i].y = y;
            arr_bullet[i].angle = angle;
            arr_bullet[i].speed = BULLET_SPEED;
            return;
        }
    }
}

//Unit collide with solid blocks true/false
bool recttilecollide(int x,int y,int w, int h, int offsetx,int offsety){
    int cx = (x+offsetx)/tileWidth;
    int cy = (y+offsety)/tileHeight;
    for(int y2=cy-1; y2<cy+5;y2++){
    for(int x2=cx-1; x2<cx+5;x2++){
        if(x2>=0 && x2<MAP_WIDTH && y2>=0 && y2<MAP_HEIGHT){
            if(map[y2][x2] == 1){
                int x3 = (x2)*tileWidth;
                int y3 = (y2)*tileHeight;
                if(rectsoverlap(x+offsetx,y+offsety,w,h,x3,y3,tileWidth,tileHeight)){
                    return true;
                }
            }
        }
    }}
    return false;
}

// Rectangles overlap
bool rectsoverlap(int x1,int y1,int w1,int h1,int x2,int y2,int w2,int h2){
    if(x1 >= (x2 + w2) || (x1 + w1) <= x2) return false;
    if(y1 >= (y2 + h2) || (y1 + h1) <= y2) return false;
    return true;
}

void updateheatmap(){
    // increase heat below bullets
    for(int i=0;i<MAX_BULLETS;i++){
        if(arr_bullet[i].active==false)continue;
        int x=arr_bullet[i].x/tileWidth;
        int y=arr_bullet[i].y/tileHeight;
        if(hmap[y][x]<255)hmap[y][x]+=5;        
        if(hmap[y-1][x]<255)hmap[y-1][x]+=1.5;        
        if(hmap[y][x+1]<255)hmap[y][x+1]+=1.5;        
        if(hmap[y+1][x]<255)hmap[y+1][x]+=1.5;        
        if(hmap[y][x-1]<255)hmap[y][x-1]+=1.5;        
        if(hmap[y-2][x]<255)hmap[y-2][x]+=1.0;        
        if(hmap[y][x+2]<255)hmap[y][x+2]+=1.0;        
        if(hmap[y+2][x]<255)hmap[y+2][x]+=1.0;        
        if(hmap[y][x-2]<255)hmap[y][x-2]+=1.0;        

    }
    // decrease the heat..
    for(int y=0;y<MAP_HEIGHT;y++){
    for(int x=0;x<MAP_WIDTH;x++){
        if(hmap[y][x]==0)continue;
        hmap[y][x]-=0.5;
        if(hmap[y][x]<0.1)hmap[y][x]=0;
    }}    
}
void drawheatmap(){
    for(int y=0;y<MAP_HEIGHT;y++){
    for(int x=0;x<MAP_WIDTH;x++){
        if(hmap[y][x]==0)continue;
        Color col = {hmap[y][x],0,0,25};
        DrawRectangle(x*tileWidth,y*tileHeight,tileWidth,tileHeight,col);
    }
    }
}

//
// This is the orientation function. It returns -1 if the point is left of the inputted line.
// 0 if on the same and 1 if on the right of the line.
// aa,bb,point
int orientation(int ax,int ay,int bx, int by, int cx, int cy){
	if(((bx-ax)*(cy-ay)-(by-ay)*(cx-ax))<0)return -1;
    if(((bx-ax)*(cy-ay)-(by-ay)*(cx-ax))>0)return 1;
    return 0;
}


void createcovermap(){
    for(int i=0;i<MAX_TURRETS;i++){
        
        // Find all cover positions
        for(float angle=0;angle<PI*2.0f;angle+=0.03f){
            float x=(float)arr_turret[i].x*tileWidth;
            float y=(float)arr_turret[i].y*tileHeight;
            for(int z=0;z<300;z++){
                x+=cos(angle);
                y+=sin(angle);
                int x2 = (float)x/(float)tileWidth;
                int y2 = (float)y/(float)tileHeight;
                if(x2<1 || y2<1 || x2>=MAP_WIDTH || y2>=MAP_HEIGHT){
                    goto LABEL;
                }
                if(recttilecollide(x,y,1,1,0,0)){                    
                    while(recttilecollide(x,y,1,1,0,0)){
                        x+=cos(angle);
                        y+=sin(angle);
                        x2 = (float)x/(float)tileWidth;
                        y2 = (float)y/(float)tileHeight;
                        if(x2<1 || y2<1 || x2>=MAP_WIDTH || y2>=MAP_HEIGHT){
                            goto LABEL;
                        }                    
                    }
                    cmap[y2][x2] = true;
                    goto LABEL;
                }
            }
            LABEL:{}
        }
        
        // Remove edge cases.
        for(float angle=0;angle<PI*2.0f;angle+=0.03f){
            float x=(float)arr_turret[i].x*tileWidth;
            float y=(float)arr_turret[i].y*tileHeight;
            for(int z=0;z<300;z++){
                x+=cos(angle);
                y+=sin(angle);
                int x2 = (float)x/(float)tileWidth;
                int y2 = (float)y/(float)tileHeight;
                if(x2<0 || y2<0 || x2>=MAP_WIDTH || y2>=MAP_HEIGHT){
                    goto LABEL2;
                }
                cmap[y2][x2]=false;
                if(recttilecollide(x,y,1,1,0,0)){                    
                    goto LABEL2;
                }
            }
            LABEL2:{}
        }

    }
}

void drawcovermap(){
    for(int y=0;y<MAP_HEIGHT;y++){
    for(int x=0;x<MAP_WIDTH;x++){
        if(cmap[y][x]==false)continue;        
        DrawRectangle(x*tileWidth,y*tileHeight,tileWidth,tileHeight,GREEN);
    }
    }
}

void drawcoverislands(){
    for(int y=0;y<MAP_HEIGHT;y++){
    for(int x=0;x<MAP_WIDTH;x++){
        if(cimap[y][x]==0)continue;
        DrawText(FormatText("%i",cimap[y][x]),x*tileWidth,y*tileHeight,15,BLACK);
    }}
}

void createcoverislands(){
    int island=1;
    for(int y=0;y<MAP_HEIGHT;y++){
    for(int x=0;x<MAP_WIDTH;x++){
        if(cmap[y][x]==true && cimap[y][x]==0){
            //flood
            cimap[y][x]=island;
            int mx[200];
            int my[200];
            int mc=0;
            mx[mc]=x;
            my[mc]=y;
            mc++;
            while(mc>0){
                mc--;
                int fx=mx[mc];
                int fy=my[mc];
                
                for(int zy=fy-1;zy<fy+2;zy++){
                for(int zx=fx-1;zx<fx+2;zx++){
                    if(zx<0 || zy<0 || zx>=MAP_WIDTH || zy>MAP_HEIGHT)continue;
                    if(cmap[zy][zx]==true && cimap[zy][zx]==0){                                                
                        mx[mc]=zx;
                        my[mc]=zy;
                        cimap[zy][zx]=island;
                        mc++;
                    }
                }
                }
            }
            island++;
            numislands=island;
        }
        
    }}    
}

void drawagents(){
    for(int i=0;i<MAX_AGENTS;i++){
        if(arr_agent[i].active==false)continue;        
        DrawRectangle(arr_agent[i].x,arr_agent[i].y,tileWidth,tileHeight,RED);
    }
}

void updateagents(){
    for(int a=0;a<MAX_AGENTS;a++){
        if(arr_agent[a].active==false)continue;
        // Move agent from path location to next path location(smooth)
        if(arr_agent[a].pathloc>=0){
            for(int spd=0;spd<arr_agent[a].speed;spd++){
                if(arr_agent[a].x<arr_agent[a].pathx[arr_agent[a].pathloc]*tileWidth)arr_agent[a].x+=1;
                if(arr_agent[a].x>arr_agent[a].pathx[arr_agent[a].pathloc]*tileWidth)arr_agent[a].x-=1;
                if(arr_agent[a].y<arr_agent[a].pathy[arr_agent[a].pathloc]*tileHeight)arr_agent[a].y+=1;
                if(arr_agent[a].y>arr_agent[a].pathy[arr_agent[a].pathloc]*tileHeight)arr_agent[a].y-=1;
                if(     arr_agent[a].x==arr_agent[a].pathx[arr_agent[a].pathloc]*tileWidth && 
                        arr_agent[a].y==arr_agent[a].pathy[arr_agent[a].pathloc]*tileHeight){
                    arr_agent[a].pathloc--;                    
                    break;
                }
            }
        }
        // If the agent is at the end of its path.
        // move to next cover position.
        bool planpath = false;
        if(arr_agent[a].pathloc == -1 && GetRandomValue(0,1000)<10 )planpath=true;
        if(planpath==true){
            
            for(int shuffle=0;shuffle<5;shuffle++){
                int i=GetRandomValue(1,numislands);
                if(arr_agent[a].myisland!=i && agentfindpath(a,i)==true){
                    arr_agent[a].myisland = i;
                    arr_agent[a].pathlen = arr_path_len;
                    arr_agent[a].pathloc = arr_path_len-1;
                    for(int p=0;p<arr_path_len;p++){
                        arr_agent[a].pathx[p] = arr_path[p].x;
                        arr_agent[a].pathy[p] = arr_path[p].y;
                    }                    
                    return;
                }    
            }
        }
    }
}
bool agentfindpath(int agent, int island){
    // 4 way search! left/up/down/right
//    int dx[4]={ 0,1,0,-1};
//    int dy[4]={-1,0,1,0};    
    // 8 way search! left/up/down/right
    int dx[8]={-1,0,1,-1,1,-1,0,1};
    int dy[8]={-1,-1,-1,0,0,1,1,1};    

    //find closest cover position that we are currently not ontop of.
    //
    // find a start and end location.
    //
    for(int y=0;y<MAP_HEIGHT;y++){
        for(int x=0;x<MAP_WIDTH;x++){
            pathmap[y][x]=0;
    }}

    int startx,starty,endx,endy;


    //flood to find closest cover position
    startx = arr_agent[agent].x/tileWidth;    
    starty = arr_agent[agent].y/tileHeight;    
    // find the destination coordinates.
    bool exitloop=false;
    int failloop=0; 
    while(exitloop==false){
        int x = GetRandomValue(0,MAP_WIDTH);
        int y = GetRandomValue(0,MAP_HEIGHT);
        if(cimap[y][x]==island){
            endx = x;
            endy = y;
            exitloop=true;
        }
        failloop++;
        if(failloop>1000000)return false;
    }
    
        
    //
    // Flood the map with distances from the start.
    //
    
    struct pathnode list[MAX_PATH];
    //
    // We store the distance on each map cell if there is no wall there.
    //
    pathmap[starty][startx]=1;
    int listlen=0;    
    list[listlen].x=startx;
    list[listlen].y=starty;
    listlen+=1;        
    int failed=0;
    // While we have a list to work with
    while(listlen>0){
        // Take the first value from the array.
        int x1=list[0].x;
        int y1=list[0].y;
        
        // shift all up.
        for(int i=0;i<listlen;i++){
            list[i].x = list[i+1].x;
            list[i].y = list[i+1].y;
        }
        if(x1==endx && y1==endy){                    
            break;
        }

        // Decrease list length
        listlen-=1;
        //
        // Here we check around our current position.
        for(int i=0;i<8;i++){
            int nx = x1+dx[i];
            int ny = y1+dy[i];
            if(nx<0 || ny<0 || nx>= MAP_WIDTH || ny>= MAP_HEIGHT)continue;            
            // If we can get there then put the new distance there and add this position
            // to the list.
            if(pathmap[ny][nx]==0 && map[ny][nx]==0){
                pathmap[ny][nx]=pathmap[y1][x1]+1;
                // add to last
                //
                list[listlen].x = nx;
                list[listlen].y = ny;
                listlen++;
                //                
            }
        }
        // Error?
        failed+=1;
        if(failed>160000)return;
        
    }
    
    //
    // Here we create the actual path.
    //
    arr_path_len = 0;
    int x1=endx;
    int y1=endy;
    arr_path[0].x = x1;
    arr_path[0].y = y1;
    arr_path_len+=1;
    failed=0;
    // While distance is greater than 1
    while(pathmap[y1][x1]>1){
        
        int nx=0;
        int ny=0;
        // Get the current distance
        int lowest = pathmap[y1][x1];
        for(int i=0;i<8;i++){
            int x2=x1+dx[i];
            int y2=y1+dy[i];
            if(x2<0 || y2 <0 || x2>=MAP_WIDTH || y2>= MAP_HEIGHT )continue; // stay in bounds of array
            if(pathmap[y2][x2]>0 && pathmap[y2][x2]<lowest){ //if there is a map distance and if it is lower than the lowest variable
                lowest = pathmap[y2][x2];
                nx = x2;
                ny = y2;
            }                
        }

        // store our new location
        x1 = nx;
        y1 = ny;
        if(hmap[y1][x1]>0){
            arr_path_len=0;
            return false;
        }
        // add to the path struct
        arr_path[arr_path_len].x = nx;
        arr_path[arr_path_len].y = ny;
        // add to length
        arr_path_len+=1;
        // error?
        failed+=1;
        if(failed>15000)return;
    }
    arr_agent[agent].x = startx*tileWidth;
    arr_agent[agent].y = starty*tileHeight;
    
    
    return true;
}
 

