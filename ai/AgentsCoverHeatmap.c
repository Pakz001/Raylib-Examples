// Agents take Cover.

// Todo :   add heatmap below bullets path.
//          add agents and agents follow path.


#include "raylib.h"
#include <math.h>

enum tiles{FLOOR,WALL};

#define MAP_WIDTH 40
#define MAP_HEIGHT 20
#define MAX_TURRETS 1
#define MAX_BULLETS 128
#define BULLET_SPEED 3
#define TURRET_TARGET_TIME 600

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
    arr_turret[0].direction = 0.02;
    arr_turret[0].burst=0;
    arr_turret[0].targetactive = true;
    arr_turret[0].targetx = 12;
    arr_turret[0].targety = 3;
    arr_turret[0].targettime=TURRET_TARGET_TIME;
    // create our covermap
    createcovermap();

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        updateturrets();
        updatebullets();
        updateheatmap();
        
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
            drawmap();
            drawturrets();
            drawbullets();
            
            DrawText("Press mouse to place target..",0,0,20,GRAY);

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
                        arr_turret[i].direction=-0.02;
                        }else if(oriented==1){
                            arr_turret[i].direction=0.02;
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
        
        for(float angle=0;angle<PI*2.0f;angle+=0.03f){
            float x=(float)arr_turret[i].x*tileWidth;
            float y=(float)arr_turret[i].y*tileHeight;
            for(int z=0;z<300;z++){
                x+=cos(angle);
                y+=sin(angle);
                int x2 = (float)x/(float)tileWidth;
                int y2 = (float)y/(float)tileHeight;
                if(x2<0 || y2<0 || x2>=MAP_WIDTH || y2>=MAP_HEIGHT){
                    goto LABEL;
                }
                if(recttilecollide(x,y,1,1,0,0)){                    
                    while(recttilecollide(x,y,1,1,0,0)){
                        x+=cos(angle);
                        y+=sin(angle);
                        x2 = (float)x/(float)tileWidth;
                        y2 = (float)y/(float)tileHeight;
                        if(x2<0 || y2<0 || x2>=MAP_WIDTH || y2>=MAP_HEIGHT){
                            goto LABEL;
                        }                    
                    }
                    cmap[y2][x2] = true;
                    goto LABEL;
                }
            }
            LABEL:{}
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
