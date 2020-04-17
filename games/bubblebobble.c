//
// ONE LEVEL.. Controls : Cursors Left,Right,Up,Down and Left Alt = shoot bubble.
//
//

enum flag2{PLAYER1,PLAYER2,LEFT,RIGHT};
enum bubblestates{SHOT,FLOATUP,FLOAT};
enum aistates{ROAMING,TRAPPED,DAMAGED};
enum flag{SCROLLLEVELDOWN,INGAME};

#define MAX_JUMP 4.0f

#define MAX_PLAYERS 2
#define MAX_FALL_SPEED 2.0f

#define MAX_BUBBLES 128
#define BUBBLE_SHOOTFORCE 10.0f
#define BUBBLE_LIFE 60*20;

#define MAX_AI 32
#define AI_SPEED 4

#include "raylib.h"
#include "math.h"

static const int screenWidth=800;
static const int screenHeight=450;
static int mapWidth;
static int mapHeight;
static float tileWidth;
static float tileHeight;
static int map[512][512];

typedef struct player{
    bool active;
    float facing;
    float x;
    float y;
    int w;
    int h;
    bool canjump;    
    float jumpforce;
}player;

static struct player p[MAX_PLAYERS];

typedef struct bubble{
    bool active;
    int timeout;
    float x;
    float y;
    int state;
    int r;
    float mx;
    float my;    
    int shakex;
    int shakey;
}bubble;

static struct bubble arr_bubble[MAX_BUBBLES];

typedef struct ai{
    bool active;
    int state;
    int facing;
    float x;
    float y;
    int w;
    int h;
    int lastjump;
    bool canjump;
    float jumpforce;
}ai;

static struct ai arr_ai[MAX_AI];

static void inilevel(void);
static void drawmap(int offsetx,int offsety);
static void drawplayers(void);
static void updateplayers(void);
static bool rectsoverlap(int x1,int y1,int w1,int h1,int x2,int y2,int w2,int h2);    
static bool playertilecollide(int num,int tile, int offsetx,int offsety);
static void updateplayergravity();
static void shootbubble(int player, int direction);
static void drawbubbles(void);
static void updatebubbles(void);
static bool circlerectcollide(int cx,int cy,int cr, int rx, int ry,int rw,int rh);
static bool bubbletilecollide(int num,int offsetx,int offsety);
static float Clamp(float value, float min, float max);
static void drawai(void);
static void addai(int x,int y);
static void updateai(void);
static bool aitilecollide(int num,int tile,int offsetx,int offsety);    
    
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------

    for(int i=0;i<MAX_AI;i++){
        arr_ai[i].active=false;
    }
    for(int i=0;i<MAX_BUBBLES;i++){
        arr_bubble[i].active=false;
    }
    p[PLAYER1].active=false;
    p[PLAYER2].active=false;
    
    InitWindow(screenWidth, screenHeight, "raylib example.");
 
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second

    inilevel();
    p[PLAYER1].active = true;
    p[PLAYER1].x = 2*tileWidth;
    p[PLAYER1].y = 12*tileHeight;
    p[PLAYER1].w = tileWidth;
    p[PLAYER1].h = tileHeight;
    p[PLAYER1].canjump = true;
    //--------------------------------------------------------------------------------------
    static int offsety;
    offsety = -(mapHeight*tileHeight);
    //static int gamestate=SCROLLLEVELDOWN;
    static int gamestate=INGAME;

    arr_bubble[0].active=true;
    arr_bubble[0].r = tileWidth/2;
    
    static int aiaddtime=10;
    static int aimax = 3;
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        
        switch (gamestate){
            case INGAME:
                if(aimax>0)aiaddtime-=1;
                if(aiaddtime==0 && aimax>0){
                    addai(10*tileWidth,tileHeight);
                    aiaddtime=20;
                    aimax-=1;
                }
                updateplayers();
                updateplayers();
                updateplayers();
                updateplayergravity();
                updatebubbles();
                updateai();
            break;
        }
        
        //----------------------------------------------------------------------------------
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(BLACK);
            switch (gamestate){
                case SCROLLLEVELDOWN:
                    offsety+=4;
                    drawmap(0,offsety);
                    if(offsety>=0)gamestate=INGAME;
                break;
                    case INGAME:
                    drawmap(0,0);
                    drawplayers();
                    drawai();
                    drawbubbles();
                break;
            }
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;


}

void drawmap(int offsetx,int offsety){
    for(int y=0;y<mapHeight;y++){
    for(int x=0;x<mapWidth;x++){
        if(map[y][x]==1){
            DrawRectangle(x*tileWidth+offsetx,y*tileHeight+offsety,tileWidth,tileHeight,PURPLE);
        }
        if(map[y][x]==2){
            DrawRectangle(x*tileWidth+offsetx,y*tileHeight+offsety,tileWidth,tileHeight,PURPLE);
        }

    }}
}

// Read the level.
void inilevel(void){
    // 3 = map tile and start of enemies.
    mapWidth = 20;
    mapHeight = 15;
    tileWidth = abs((float)screenWidth/(float)mapWidth);
    tileHeight = abs((float)screenHeight/(float)mapHeight);
    //
    int temp[15][20] =                  {   {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},
                                            {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
                                            {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
                                            {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
                                            {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
                                            {2,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,2},
                                            {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
                                            {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
                                            {2,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,2},
                                            {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
                                            {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
                                            {2,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,2},
                                            {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
                                            {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
                                            {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2}};
    // copy the temp into the main map array.
    for(int y=0;y<mapHeight;y++){
    for(int x=0;x<mapWidth;x++){
        map[y][x] = temp[y][x];
    }}
}

void drawplayers(void){
    for(int i=0;i<MAX_PLAYERS;i++){
        if(p[i].active==false)continue;
        DrawRectangle(p[i].x,p[i].y,p[i].w,p[i].h,GREEN);
    }
}

//Unit collide with solid blocks true/false
// num = player
bool playertilecollide(int num,int tile,int offsetx,int offsety){
    if(p[num].active==false)return false;
    int cx = (p[num].x+offsetx)/tileWidth;
    int cy = (p[num].y+offsety)/tileHeight;
    for(int y2=cy-1; y2<cy+2;y2++){//Note that the - and + are to be set differently with differently sized players
    for(int x2=cx-1; x2<cx+2;x2++){
        if(x2>=0 && x2<mapWidth && y2>=0 && y2<mapHeight){
            if(map[y2][x2] == tile){
                int x3 = (x2)*tileWidth;
                int y3 = (y2)*tileHeight;
                if(rectsoverlap(p[num].x+offsetx,p[num].y+offsety,p[num].w,p[num].h,x3,y3,tileWidth,tileHeight)){
                    return true;
                }
            }
            if(tile==99){//collide with any tile mode
            if(map[y2][x2] > 0){
                int x3 = (x2)*tileWidth;
                int y3 = (y2)*tileHeight;
                if(rectsoverlap(p[num].x+offsetx,p[num].y+offsety,p[num].w,p[num].h,x3,y3,tileWidth,tileHeight)){
                    return true;
                }
            }
            }

        }
    }}
    return false;
}

//Unit collide with solid blocks true/false
// num = player
bool bubbletilecollide(int num,int offsetx,int offsety){
    if(p[num].active==false)return false;
    int cx = (arr_bubble[num].x+offsetx)/tileWidth;
    int cy = (arr_bubble[num].y+offsety)/tileHeight;
    for(int y2=cy-1; y2<cy+2;y2++){//Note that the - and + are to be set differently with differently sized players
    for(int x2=cx-1; x2<cx+2;x2++){
        if(x2>=0 && x2<mapWidth && y2>=0 && y2<mapHeight){
            if(map[y2][x2] > 0 ){
                int x3 = (x2)*tileWidth;
                int y3 = (y2)*tileHeight;
                if(circlerectcollide(   arr_bubble[num].x+offsetx,
                                        arr_bubble[num].y+offsety,
                                        arr_bubble[num].r,
                                        x3,y3,tileWidth,tileHeight)){
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

void updateplayers(void){
    for(int num=0;num<MAX_PLAYERS;num++){
        if(num==0){ //player 1
            // move left and right while jumping up.
            if(p[num].canjump==false && 
                                        p[num].jumpforce<0 && 
                                        IsKeyDown(KEY_LEFT) &&
                                        playertilecollide(num,2,-1,0)==false){
                p[num].x-=1;
                p[num].facing=LEFT;
            }
            if(p[num].canjump==false && 
                                        p[num].jumpforce<0 && 
                                        IsKeyDown(KEY_RIGHT) &&
                                        playertilecollide(num,2,1,0)==false){
                p[num].x+=1;
                p[num].facing=RIGHT;
            }
            //move left and right while falling down
            if(p[num].canjump==false && 
                                        p[num].jumpforce>0 && 
                                        IsKeyDown(KEY_LEFT) &&
                                        playertilecollide(num,99,-1,0)==false){
                p[num].x-=1;
                p[num].facing=LEFT;
                p[num].facing=RIGHT;
            }
            if(p[num].canjump==false && 
                                        p[num].jumpforce>0 && 
                                        IsKeyDown(KEY_RIGHT) &&
                                        playertilecollide(num,99,1,0)==false){
                p[num].x+=1;
                p[num].facing=RIGHT;
            }
            //move left and right normal
            if(p[num].canjump && 
                                        IsKeyDown(KEY_LEFT) &&
                                        playertilecollide(num,99,-1,0)==false){
                p[num].x-=1;
                p[num].facing=LEFT;
            }
            if(p[num].canjump && 
                                        IsKeyDown(KEY_RIGHT) &&
                                        playertilecollide(num,99,1,0)==false){
                p[num].x+=1;
                p[num].facing=RIGHT;
            }
            


            if(IsKeyDown(KEY_SPACE) && p[num].canjump){
                p[num].canjump = false; // We are now jumping.
                p[num].jumpforce = -MAX_JUMP; // How high we jump.
            }

        }
    }
}

void updateplayergravity(){
    for(int num=0;num<MAX_PLAYERS;num++){
        // JUMPING
 
 
        // if hanging in the air then fall down
        if(playertilecollide(num,99,0,1)==false)p[num].canjump=false;
        

        // If we are currently not jumping and if the player presses space.
        // If we are currently in a jump.
        if(p[num].canjump==false){
            // Here we update the player y position.
            // We want to move 1 pixel at a time. This for collision detection(Not going through tiles.)
            for(int i=0;i<abs((int)p[num].jumpforce);i++){
                // If we are still jumping then update the position.☺
                if(p[num].canjump==false){
                    
                    p[num].y += p[num].jumpforce;
                }
                if(p[num].jumpforce<0 && playertilecollide(num,2,0,-1)==true){    
                    p[num].jumpforce = 0.1;                    
                    break;
                }                
                // if we touch the ground then reset out position and set canjump to true.
                if(p[num].jumpforce>0 && playertilecollide(num,99,0,1)==true){    
                    p[num].y = p[num].y-2;
                    p[num].canjump = true;
                    break;
                }                
            }
            // Here we add to the jumpforce. It will go from - to +.
            p[num].jumpforce += 0.2f;            
            if(p[num].jumpforce>MAX_FALL_SPEED)p[num].jumpforce=MAX_FALL_SPEED;
        }    
    }    
}

void shootbubble(int player, int direction){
    for(int i=0;i<MAX_BUBBLES;i++){
        if(arr_bubble[i].active==true)continue;
        arr_bubble[i].active = true;
        arr_bubble[i].timeout = BUBBLE_LIFE; //how long does the bubble stay alive
        arr_bubble[i].r = tileWidth/2;//radius of our bullet.
        if(direction==LEFT){
            arr_bubble[i].mx=-BUBBLE_SHOOTFORCE;
            arr_bubble[i].x = p[player].x;
        }
        if(direction==RIGHT){
            arr_bubble[i].mx=BUBBLE_SHOOTFORCE;        
            arr_bubble[i].x = p[player].x+p[player].w*2;            
        }
        
        arr_bubble[i].y = p[player].y+5;
        arr_bubble[i].state = 0;
        return;
    }    
}

void drawbubbles(){
    for(int i=0;i<MAX_BUBBLES;i++){
        if(arr_bubble[i].active==false)continue;
        int radius = arr_bubble[i].r;
        int x = arr_bubble[i].x+arr_bubble[i].shakex;
        int y = arr_bubble[i].y+arr_bubble[i].shakey;
        DrawCircle(x,y,radius,(Color){0,50,0,40});
        DrawCircleLines(x,y,radius,(Color){0,200,0,100});
        DrawCircle(x-radius+(radius/1.5),y-radius+(radius/1.5),6,(Color){200,255,200,200});
        DrawCircle(x+(radius/2.1),y+(radius/2.1),2,(Color){200,255,200,200});
        
    }
}

void updatebubbles(){
    //player(s) shoot bubbles
    for(int i=0;i<MAX_PLAYERS;i++){
        if(p[i].active==false)continue;
        if(i==0){
            if(IsKeyPressed(KEY_LEFT_ALT)){
                if(p[i].facing==LEFT){
                    shootbubble(i,LEFT);
                }else{
                    shootbubble(i,RIGHT);
                }
            }
        }
    }
    // Bubbles movement and interaction
    for(int i=0;i<MAX_BUBBLES;i++){
        if(arr_bubble[i].active==false)continue;        
        
        //timeout
        arr_bubble[i].timeout-=1;
        if(arr_bubble[i].timeout<0)arr_bubble[i].active=false;
        
        
        if(arr_bubble[i].state==FLOAT){
            if(GetRandomValue(0,30)<2){
                arr_bubble[i].shakex = GetRandomValue(-3,3);
                arr_bubble[i].shakey = GetRandomValue(-3,3);
            }
        }
        
        if(arr_bubble[i].state==FLOATUP){
            if(arr_bubble[i].y>2.5*tileHeight){
                arr_bubble[i].y-=1;
            }
            if(arr_bubble[i].y<3*tileHeight){
                if(arr_bubble[i].x<screenWidth/2){
                    arr_bubble[i].x+=1;
                }
                if(arr_bubble[i].x>screenWidth/2){
                    arr_bubble[i].x-=1;
                }                
                if(rectsoverlap(    arr_bubble[i].x,arr_bubble[i].y,arr_bubble[i].r,arr_bubble[i].r,
                                    screenWidth/2-5,0,10,100)){
                    for(int z=0;z<MAX_BUBBLES;z++){
                        if(z!=i && arr_bubble[z].active==false)continue;
                        if(rectsoverlap(arr_bubble[z].x,
                                        arr_bubble[z].y,
                                        arr_bubble[z].r,
                                        arr_bubble[z].r,
                                        arr_bubble[i].x,
                                        arr_bubble[z].y,
                                        arr_bubble[z].r,
                                        arr_bubble[z].r)){
                        arr_bubble[i].state = FLOAT;
                        }
                    }
                    
                }
            }
        }
        // If the bubble was just shot!
        if(arr_bubble[i].state==SHOT){
            for(int step=0;step<abs(arr_bubble[i].mx);step+=1){
                      
                if(arr_bubble[i].mx<0.0f ){
                    arr_bubble[i].x -= 1;
                    if(bubbletilecollide(i,-1,0)){//if bubble collides then change state;
                        arr_bubble[i].mx=0;                
                        arr_bubble[i].state = FLOATUP;
                    }
                }
                if(arr_bubble[i].mx>0.0f){
                    arr_bubble[i].x += 1;
                    if(bubbletilecollide(i,1,0)){
                        arr_bubble[i].mx=0;                    
                        arr_bubble[i].state = FLOATUP;
                    }
                }
            }
            
            if(arr_bubble[i].mx<0){
                arr_bubble[i].mx+=0.2f;
            }else{
                arr_bubble[i].mx-=0.2f;
            }
            // If the bubble slows down enough then change state to FLOAT;
            if(arr_bubble[i].mx>-0.3f && arr_bubble[i].mx < 0.3f){
                arr_bubble[i].mx = 0;
                arr_bubble[i].state = FLOATUP;
            }
        }
    }
    
}

bool circlerectcollide(int cx,int cy,int cr, int rx, int ry,int rw,int rh){
    float closestx = Clamp(cx, rx, rx+rw);
    float closesty = Clamp(cy, ry, ry+rh);
    float distancex = cx - closestx;
    float distancey = cy - closesty;
    float distancesquared = (distancex * distancex) + (distancey * distancey);
    return distancesquared < (cr * cr);
}

// Clamp float value
float Clamp(float value, float min, float max)
{
    const float res = value < min ? min : value;
    return res > max ? max : res;
}

void drawai(){
    for(int i=0;i<MAX_AI;i++){
        if(arr_ai[i].active==false)continue;
        int x = arr_ai[i].x;
        int y = arr_ai[i].y;
        int w = arr_ai[i].w;
        int h = arr_ai[i].h;
        DrawRectangle(x,y,w,h,RED);
    }
}

void addai(int x,int y){
    for(int i=0;i<MAX_AI;i++){
        if(arr_ai[i].active==true)continue;
        arr_ai[i].active = true;
        arr_ai[i].x = x;
        arr_ai[i].y = y;
        arr_ai[i].w = tileWidth;
        arr_ai[i].h = tileHeight;
        arr_ai[i].canjump = false;
        arr_ai[i].state = ROAMING;
        arr_ai[i].facing = LEFT;
        return;
    }
}

void updateai(){
    for(int num=0;num<MAX_AI;num++){
        if(arr_ai[num].active==false)continue;
 
        if(arr_ai[num].state==ROAMING){
            
            if(arr_ai[num].canjump==true){
                for(int z=0;z<AI_SPEED;z++){
                    if(arr_ai[num].facing==LEFT){
                        arr_ai[num].x-=1;
                        if(aitilecollide(num,99,-1,0)){
                            arr_ai[num].facing=RIGHT;
                        }                        
                    }
                    if(arr_ai[num].facing==RIGHT){
                        arr_ai[num].x+=1;
                        if(aitilecollide(num,99,1,0)){
                            arr_ai[num].facing=LEFT;
                        }                        
                    }
                    
                }
                // Jump the ai
                // jump up randomly if player is above us.
                if(GetRandomValue(0,400)<2){
                    if(aitilecollide(num,99,0,-(tileHeight*1.1)) && arr_ai[num].y > p[PLAYER1].y){
                        arr_ai[num].jumpforce = -MAX_JUMP;
                        arr_ai[num].canjump = false;
                        arr_ai[num].lastjump=0;
                    }
                }

                // Jump randomly if a platform is reachable.
                if(GetRandomValue(0,400)<2){
                    if(aitilecollide(num,99,0,-(tileHeight*1.1))){
                        arr_ai[num].jumpforce = -MAX_JUMP;
                        arr_ai[num].canjump = false;
                        arr_ai[num].lastjump=0;
                    }
                }
                // This is a jump fast after the first jump
                if(arr_ai[num].lastjump<100)arr_ai[num].lastjump+=1;
                if(arr_ai[num].lastjump<100 && GetRandomValue(0,100)<2){
                    if(aitilecollide(num,99,0,-(tileHeight*1.1))){
                        arr_ai[num].jumpforce = -MAX_JUMP;
                        arr_ai[num].canjump = false;
                        arr_ai[num].lastjump=101;
                    }                    
                }
            }
            
            // if hanging in the air then fall down
            if(aitilecollide(num,99,0,1)==false)arr_ai[num].canjump=false;


            // If we are currently not jumping and if the player presses space.
            // If we are currently in a jump.
            if(arr_ai[num].canjump==false){
                // Here we update the player y position.
                // We want to move 1 pixel at a time. This for collision detection(Not going through tiles.)
                for(int i=0;i<abs((int)arr_ai[num].jumpforce);i++){
                    bool exit=false;
                    // If we are still jumping then update the position.
                    if(arr_ai[num].canjump==false){
                        
                        arr_ai[num].y += arr_ai[num].jumpforce;
                    }
                    if(arr_ai[num].jumpforce<0 && aitilecollide(num,2,0,-1)==true){    
                        arr_ai[num].jumpforce = 0.1;                    
                        break;
                    }                
                    // if we touch the ground then reset out position and set canjump to true.
                    if(arr_ai[num].jumpforce>0 && aitilecollide(num,99,0,1)==true){    
                        arr_ai[num].y = arr_ai[num].y-1;
                        arr_ai[num].canjump = true;
                        // look if the player is left or right and adjust direction.
                        if(arr_ai[num].x>p[PLAYER1].x){
                            arr_ai[num].facing=LEFT;
                        }else{
                            arr_ai[num].facing=RIGHT;
                        }
                        exit=true;
                        break;
                    }                
                    if(exit)break;
                }
                // Here we add to the jumpforce. It will go from - to +.
                arr_ai[num].jumpforce += 0.2f;            
                if(arr_ai[num].jumpforce>MAX_FALL_SPEED)arr_ai[num].jumpforce=MAX_FALL_SPEED;             
            }
        }
 
    }
}

//Unit collide with solid blocks true/false
// num = player
bool aitilecollide(int num,int tile,int offsetx,int offsety){
    if(arr_ai[num].active==false)return false;
    int cx = (arr_ai[num].x+offsetx)/tileWidth;
    int cy = (arr_ai[num].y+offsety)/tileHeight;
    for(int y2=cy-1; y2<cy+2;y2++){//Note that the - and + are to be set differently with differently sized players
    for(int x2=cx-1; x2<cx+2;x2++){
        if(x2>=0 && x2<mapWidth && y2>=0 && y2<mapHeight){
            if(map[y2][x2] == tile){
                int x3 = (x2)*tileWidth;
                int y3 = (y2)*tileHeight;
                if(rectsoverlap(arr_ai[num].x+offsetx,arr_ai[num].y+offsety,arr_ai[num].w,arr_ai[num].h,x3,y3,tileWidth,tileHeight)){
                    return true;
                }
            }
            if(tile==99){//collide with any tile mode
            if(map[y2][x2] > 0){
                int x3 = (x2)*tileWidth;
                int y3 = (y2)*tileHeight;
                if(rectsoverlap(arr_ai[num].x+offsetx,arr_ai[num].y+offsety,arr_ai[num].w,arr_ai[num].h,x3,y3,tileWidth,tileHeight)){
                    return true;
                }
            }
            }

        }
    }}
    return false;
}
