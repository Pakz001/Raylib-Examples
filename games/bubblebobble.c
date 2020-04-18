//
// ONE LEVEL.. Controls : Cursors Left,Right,Up,Down and Left Alt = shoot bubble.
//
//

enum flag2{PLAYER1,PLAYER2,LEFT,RIGHT};
enum bubblestates{SHOT,FLOATUP,FLOAT};
enum aistates{ROAMING,TRAPPED,DAMAGED,TRAJECTORY};
enum flag{SCROLLLEVELDOWN,INGAME};

#define MAX_JUMP 4.0f

#define MAX_PLAYERS 2 // second player not fully implemented yet.
#define MAX_FALL_SPEED 2.0f

#define MAX_BUBBLES 256
#define BUBBLE_SHOOTFORCE 10.0f //how fast does the bubble shoot away from the player
#define BUBBLE_LIFE 60*20 //how long does a empty bubble stay alive
#define CAPTURED_TIMEOUT 60*7 //how long does a ai stay inside the bubble
#define RESTOREFROMDAMAGED 200 //time a ai stays damaged
#define BUBBLE_PUSHTIME 4 //how long do we push the ai holding bubble before it pops

#define MAX_AI 32
#define AI_SPEED 3
#define AI_LAUNCHFORCE 4.5
#define AI_TRAJTIME 60*2

#define MAX_PICKUP 128
#define PICKUP_TIMEDISAPPEAR 60*6

#define MAX_PICKUPEFFECT 128
#define PICKUPEFFECT_MY 2
#define PICKUPEFFECT_DISAPPEAR 60*1.5
#define POINT1 200
#define POINT2 500
#define POINT3 1000
#define POINT4 10000
#define POINT5 20000

#include "raylib.h"
#include "math.h"

static const int screenWidth=800;
static const int screenHeight=450;
static int mapWidth;
static int mapHeight;
static float tileWidth;
static float tileHeight;
static int map[512][512];

static	Color c64color[16];  //' our colors

typedef struct player{
    bool active;
    float facing;
    float x;
    float y;
    int shotfiredtime;
    int w;
    int h;
    int score;
    bool canjump;    
    float jumpforce;
}player;

static struct player p[MAX_PLAYERS];

typedef struct bubble{
    bool active;
    bool contains;
    int timeout;
    int timeoutcaptured;
    float x;
    float y;
    int state;
    int r;
    float mx;
    float my;    
    int shakex;
    int shakey;
    int pushtime;    
}bubble;

static struct bubble arr_bubble[MAX_BUBBLES];

typedef struct ai{
    bool active;
    float rotation;
    int state;
    int animframetime;
    int facing;
    int timedamaged;
    float x;
    float y;
    int trajtime;
    float mx;
    float my;
    int w;
    int h;
    int lastjump;
    bool canjump;
    float jumpforce;
}ai;

static struct ai arr_ai[MAX_AI];

typedef struct pickup{
    bool active;
    int x;
    int y;
    int w;
    int h;
    int type;
    Color col;
    int timedisappear;
}pickup;

static struct pickup arr_pickup[MAX_PICKUP];

typedef struct pickupeffect{
    bool active;
    float x;
    float y;
    float my;
    int type;
    float alpha;
    float timedisappear;    
    int points;
}pickupeffect;

static struct pickupeffect arr_pueffect[MAX_PICKUPEFFECT];

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
static void bubbleaicollision(void);
static void playeraicollision(void);   
static void playerbubblecollision(void);
static void inigfx(void);
static void createaitrajectory(int x,int y,int facing);
static float getangle(float x1,float y1,float x2,float y2);
static void drawpickups(void);
static void updatepickups(void);
static void addpickup(int x,int y);
static void playerpickupcollision();
static void updatepickupeffects();
static void drawpickupeffects();
static void addpickupeffect(int type,int x, int y);
static int getscore(int type);
static void drawplayerbar(void);
static void inigame(void);
static void inic64colors(void);

// Here the gfx are defined.   
static RenderTexture2D tilepurple; 
static RenderTexture2D spritebobble1; 
static RenderTexture2D spritebobble2; 
static RenderTexture2D spriteai1; 
static RenderTexture2D spriteai2; 
   
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    InitWindow(screenWidth, screenHeight, "raylib example.");

     // Create a Image in memory
    tilepurple = LoadRenderTexture(32, 32);
    spritebobble1 = LoadRenderTexture(32, 32);
    spritebobble2 = LoadRenderTexture(32, 32);
    spriteai1 = LoadRenderTexture(32, 32);
    spriteai2 = LoadRenderTexture(32, 32);    

    inic64colors();
    inigfx();
 
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second

    inilevel();
    inigame();

    //--------------------------------------------------------------------------------------
    static int offsety;
    offsety = -(mapHeight*tileHeight);
    static int gamestate=SCROLLLEVELDOWN;
    //static int gamestate=INGAME;

    
    static int aiaddtime=10;
    static int aimax = 3;
    static int restarttime;
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
                bubbleaicollision();
                updateai();
                playeraicollision();
                playerbubblecollision();
                updatepickups();
                updatepickupeffects();
                playerpickupcollision();
                //see if there are no more ai then countdown and reset.
                restarttime+=1;
                for(int i=0;i<MAX_AI;i++){
                    if(arr_ai[i].active)restarttime=0;
                }
                if(restarttime>60*10){
                    inilevel();
                    inigame();
                    aiaddtime=10;
                    aimax = 3;
                    restarttime=0;
                    offsety = -(mapHeight*tileHeight);
                    gamestate = SCROLLLEVELDOWN;
                    
                }
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
                    drawplayerbar();
                    drawpickups();
                    drawpickupeffects();
                    drawplayers();
                    drawai();
                    drawbubbles();
                break;
            }
//            DrawTexturePro(spritebobble1.texture,       (Rectangle){0,0,spritebobble1.texture.width,
//                                                                        spritebobble1.texture.height},
//                                                        (Rectangle){50,
 //                                                                   50,
//                                                                    tileWidth,tileHeight},
//                                                        (Vector2){0,0},0,WHITE);            
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadRenderTexture(tilepurple);
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;


}

void drawmap(int offsetx,int offsety){
    for(int y=0;y<mapHeight;y++){
    for(int x=0;x<mapWidth;x++){
        if(map[y][x]==1 || map[y][x]==2){
            DrawTexturePro(tilepurple.texture,      (Rectangle){0,0,tilepurple.texture.width,tilepurple.texture.height},
                                                    (Rectangle){x*tileWidth+offsetx,y*tileHeight+offsety,
                                                    tileWidth,tileHeight},
                                                    (Vector2){0,0},0,WHITE);            
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
        //DrawRectangle(p[i].x,p[i].y,p[i].w,p[i].h,GREEN);
        if(p[i].shotfiredtime>-2)p[i].shotfiredtime-=1;
        
        if(p[i].facing==RIGHT){
            if(p[i].shotfiredtime>0){
                DrawTexturePro(spritebobble2.texture,       (Rectangle){0,0,spritebobble2.texture.width,
                                                                            spritebobble2.texture.height},
                                                            (Rectangle){p[i].x,
                                                                        p[i].y,
                                                                        p[i].w,p[i].h},
                                                            (Vector2){0,0},0,WHITE);            
            }else{
                DrawTexturePro(spritebobble1.texture,       (Rectangle){0,0,spritebobble1.texture.width,
                                                                            spritebobble1.texture.height},
                                                            (Rectangle){p[i].x,
                                                                        p[i].y,
                                                                        p[i].w,p[i].h},
                                                            (Vector2){0,0},0,WHITE);                            
            }
        }else{
            if(p[i].shotfiredtime>0){
                DrawTexturePro(spritebobble2.texture,       (Rectangle){0,0,-spritebobble2.texture.width,
                                                                            spritebobble2.texture.height},
                                                            (Rectangle){p[i].x,
                                                                        p[i].y,
                                                                        p[i].w,p[i].h},
                                                            (Vector2){0,0},0,WHITE);            
            }else{
                DrawTexturePro(spritebobble1.texture,       (Rectangle){0,0,-spritebobble1.texture.width,
                                                                            spritebobble1.texture.height},
                                                            (Rectangle){p[i].x,
                                                                        p[i].y,
                                                                        p[i].w,p[i].h},
                                                            (Vector2){0,0},0,WHITE);                        
            }
        }
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
    if(arr_bubble[num].active==false)return false;
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
        arr_bubble[i].contains = false;
        arr_bubble[i].timeout = BUBBLE_LIFE; //how long does the bubble stay alive
        arr_bubble[i].r = tileWidth/2;//radius of our bullet.
        if(direction==LEFT){
            arr_bubble[i].mx=-BUBBLE_SHOOTFORCE;
            arr_bubble[i].x = p[player].x;
        }
        if(direction==RIGHT){
            arr_bubble[i].mx=BUBBLE_SHOOTFORCE;        
            arr_bubble[i].x = p[player].x+p[player].w;            
        }
        
        arr_bubble[i].y = p[player].y+5;
        arr_bubble[i].state = SHOT;
        return;
    }    
}

void drawbubbles(){
    for(int i=0;i<MAX_BUBBLES;i++){
        if(arr_bubble[i].active==false)continue;
        int radius = arr_bubble[i].r;
        int x = arr_bubble[i].x+arr_bubble[i].shakex;
        int y = arr_bubble[i].y+arr_bubble[i].shakey;
        if(arr_bubble[i].contains){//contains a ai
            DrawRectangle(x-radius+6,y-radius+6,radius*2-12,radius*2-8,(Color){200,0,0,200});
        }
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
                    p[i].shotfiredtime=10;
                }else{
                    shootbubble(i,RIGHT);
                    p[i].shotfiredtime=10;
                }
            }
        }
    }
    // Bubbles movement and interaction
    for(int i=0;i<MAX_BUBBLES;i++){
        if(arr_bubble[i].active==false)continue;        
     
        // decrease the player push bubble effect/
        if(arr_bubble[i].pushtime>-1)arr_bubble[i].pushtime--;
     
        //timeout
        arr_bubble[i].timeout-=1;
        if(arr_bubble[i].timeout<0){
            arr_bubble[i].active=false;
            if(arr_bubble[i].contains==true){
                int newai;
                for(newai=0;newai<MAX_AI;newai++){
                    if(arr_ai[newai].active==true)continue;
                    arr_bubble[i].contains=false;
                    arr_ai[newai].active=true;
                    arr_ai[newai].x=arr_bubble[i].x;
                    arr_ai[newai].y=arr_bubble[i].y-4;
                    arr_ai[newai].w=tileWidth;
                    arr_ai[newai].h=tileHeight;
                    arr_ai[newai].canjump = false;
                    arr_ai[newai].state = DAMAGED;
                    arr_ai[newai].facing = LEFT;
                    arr_ai[newai].timedamaged=RESTOREFROMDAMAGED;
                    break;
                }
            }
        }
        
        //shake bubble
        if(arr_bubble[i].state==FLOAT){
            if(GetRandomValue(0,30)<2){
                arr_bubble[i].shakex = GetRandomValue(-3,3);
                arr_bubble[i].shakey = GetRandomValue(-3,3);
            }
        }
        
        //
        if(arr_bubble[i].state==FLOATUP){
            // if containing a ai then count down and release.
            if(arr_bubble[i].contains==true){
                if(arr_bubble[i].timeout>-2)arr_bubble[i].timeout-=1;
                // release the ai again but faster and killable.
                if(arr_bubble[i].timeout<0){
                    arr_bubble[i].active=false;
                    int newai;
                    for(newai=0;newai<MAX_AI;newai++){
                        if(arr_ai[newai].active==true)continue;
                        arr_ai[newai].active=true;
                        arr_ai[newai].x=arr_bubble[i].x;
                        arr_ai[newai].y=arr_bubble[i].y-4;
                        arr_ai[newai].w=tileWidth;
                        arr_ai[newai].h=tileHeight;
                        arr_ai[newai].canjump = false;
                        arr_ai[newai].state = DAMAGED;
                        arr_ai[newai].facing = LEFT;
                        arr_ai[newai].timedamaged=RESTOREFROMDAMAGED;
                        break;
                    }
                }
            }
            // float it up to center top screen and then shake it.
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
            for(int step=0;step<abs((int)arr_bubble[i].mx);step+=1){
                bool ex=false;      
                if(arr_bubble[i].mx<0.0f ){
                    arr_bubble[i].x -= 1;
                    if(bubbletilecollide(i,-1,0)){//if bubble collides then change state;
                        arr_bubble[i].mx=0;                
                        arr_bubble[i].state = FLOATUP;
                        ex=true;
                    }
                }
                if(arr_bubble[i].mx>0.0f){
                    arr_bubble[i].x += 1;
                    if(bubbletilecollide(i,1,0)){
                        arr_bubble[i].mx=0;                    
                        arr_bubble[i].state = FLOATUP;
                        ex=true;
                    }
                }
                if(ex)break;
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
        // animation
        arr_ai[i].animframetime-=1;
        if(arr_ai[i].animframetime<0)arr_ai[i].animframetime=30;
        //
        // if the ai is currently spinning then rotate him.
        if(arr_ai[i].state==TRAJECTORY){
            arr_ai[i].rotation+=20;
        }
        float rotation=arr_ai[i].rotation;
        // When the ai goes into a spin make sure the rotation is around the center.
        float centerx=tileWidth/2;
        float centery=tileHeight/2;
        // if the ai is currently not spinning then draw the sprites from left top offset.
        if(arr_ai[i].state!=TRAJECTORY){
            centerx = 0;
            centery = 0;
        }
        //
        if(arr_ai[i].state==DAMAGED || arr_ai[i].state==TRAJECTORY){
            //DrawRectangle(x,y,w,h,BLUE);
            if(arr_ai[i].animframetime<15){
                if(arr_ai[i].facing==RIGHT){
                    DrawTexturePro(spriteai1.texture,           (Rectangle){0,0,spriteai1.texture.width,
                                                                                spriteai1.texture.height},
                                                                (Rectangle){x,
                                                                            y,
                                                                            w,h},
                                                                (Vector2){centerx,centery},rotation,BLUE);            
                }else{
                    DrawTexturePro(spriteai1.texture,           (Rectangle){0,0,-spriteai1.texture.width,
                                                                                spriteai1.texture.height},
                                                                (Rectangle){x,
                                                                            y,
                                                                            w,h},
                                                                (Vector2){centerx,centery},rotation,BLUE);            
                    
                }    
            }else{
                if(arr_ai[i].facing==RIGHT){
                    DrawTexturePro(spriteai2.texture,           (Rectangle){0,0,spriteai2.texture.width,
                                                                                spriteai2.texture.height},
                                                                (Rectangle){x,
                                                                            y,
                                                                            w,h},
                                                                (Vector2){centerx,centery},rotation,BLUE);            
                }else{
                    DrawTexturePro(spriteai2.texture,           (Rectangle){0,0,-spriteai2.texture.width,
                                                                                spriteai2.texture.height},
                                                                (Rectangle){x,
                                                                            y,
                                                                            w,h},
                                                                (Vector2){centerx,centery},rotation,BLUE);            
                }
            }
        }else{
            //DrawRectangle(x,y,w,h,RED);
            if(arr_ai[i].animframetime<15){
                if(arr_ai[i].facing==RIGHT){
                    DrawTexturePro(spriteai1.texture,           (Rectangle){0,0,spriteai1.texture.width,
                                                                                spriteai1.texture.height},
                                                                (Rectangle){x,
                                                                            y,
                                                                            w,h},
                                                                (Vector2){centerx,centery},rotation,WHITE);            
                }else{
                    DrawTexturePro(spriteai1.texture,           (Rectangle){0,0,-spriteai1.texture.width,
                                                                                spriteai1.texture.height},
                                                                (Rectangle){x,
                                                                            y,
                                                                            w,h},
                                                                (Vector2){centerx,centery},rotation,WHITE);                                            
                }
            }else{
                if(arr_ai[i].facing==RIGHT){
                    DrawTexturePro(spriteai2.texture,           (Rectangle){0,0,spriteai2.texture.width,
                                                                                spriteai2.texture.height},
                                                                (Rectangle){x,
                                                                            y,
                                                                            w,h},
                                                                (Vector2){centerx,centery},rotation,WHITE);            
                }else{
                    DrawTexturePro(spriteai2.texture,           (Rectangle){0,0,-spriteai2.texture.width,
                                                                                spriteai2.texture.height},
                                                                (Rectangle){x,
                                                                            y,
                                                                            w,h},
                                                                (Vector2){centerx,centery},rotation,WHITE);                                            
                }
                
            }
        }
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
        
        // handle the trajectory of the ai. (if they got busted)
        if(arr_ai[num].state==TRAJECTORY){
            int speed=AI_SPEED;
            //If the ai is damaged than speed it it.            
            for(int z=0;z<speed;z++){
                if(arr_ai[num].facing==LEFT){
                    arr_ai[num].x-=1;
                    if(arr_ai[num].x<tileWidth){
                        arr_ai[num].facing=RIGHT;
                    }                        
                }
                if(arr_ai[num].facing==RIGHT){
                    arr_ai[num].x+=1;
                    if(arr_ai[num].x>screenWidth-(tileWidth*2)){
                        arr_ai[num].facing=LEFT;
                    }                        
                }                
            }
            // gravity on the trajectory 
            if(arr_ai[num].trajtime>-2)arr_ai[num].trajtime-=1;
            for(int z=0;z<abs((int)arr_ai[num].my);z++){
                if(arr_ai[num].my<0){
                    arr_ai[num].y+=1;//arr_ai[num].my;    
                }else{
                    arr_ai[num].y-=1;//arr_ai[num].my;    
                }
                                
                // If falling down long enough then end and drop loot                
                if(arr_ai[num].y>tileHeight){
                if(arr_ai[num].my<0 && arr_ai[num].y>screenHeight-(tileHeight*3))arr_ai[num].trajtime=-1;
                if(arr_ai[num].trajtime<0){
                    if( aitilecollide(num,99,0,1)==true &&
                        aitilecollide(num,99,0,0)==false &&
                        aitilecollide(num,99,-1,0)==false &&
                        aitilecollide(num,99,1,0)==false){
                        arr_ai[num].active=false;
                        addpickup(arr_ai[num].x,arr_ai[num].y);
                        break;
                    }
                }                
                }
            }
            arr_ai[num].my-=0.07f;
            //
            
        }
        
        // If ai is roaming or damaged (Moving around)
        if(arr_ai[num].state==ROAMING || arr_ai[num].state==DAMAGED){
            // restore from damages if time up.
            if(arr_ai[num].timedamaged>-2)arr_ai[num].timedamaged-=1;
            if(arr_ai[num].timedamaged<0 && bubbletilecollide(num,0,0)==false){
                arr_ai[num].state = ROAMING;
            }
            if(arr_ai[num].canjump==true){                
                int speed=AI_SPEED;
                //If the ai is damaged than speed it it.
                if(arr_ai[num].state==DAMAGED)speed+=1; 
                for(int z=0;z<speed;z++){
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
                        if(arr_ai[num].state==DAMAGED){
                            if(arr_ai[num].x<p[PLAYER1].x){
                                arr_ai[num].facing=LEFT;
                            }else{
                                arr_ai[num].facing=RIGHT;
                            }                            
                        }else{
                            if(arr_ai[num].x>p[PLAYER1].x){
                                arr_ai[num].facing=LEFT;
                            }else{
                                arr_ai[num].facing=RIGHT;
                            }
                            if(GetRandomValue(0,10)<5){
                                if(GetRandomValue(0,10)<5){
                                    arr_ai[num].facing=LEFT;
                                }else{
                                    arr_ai[num].facing=RIGHT;
                                }
                            }
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

void bubbleaicollision(){
    // If bubble touches ai
    for(int i=0;i<MAX_BUBBLES;i++){
        if(arr_bubble[i].active==false || arr_bubble[i].state!=SHOT || arr_bubble[i].contains)continue;
        // ai In a bubble than pop it
        for(int ii=0;ii<MAX_AI;ii++){
            if(arr_ai[ii].active==false || arr_ai[ii].state!=ROAMING)continue;
            int x1=arr_bubble[i].x;
            int y1=arr_bubble[i].y;
            int r=arr_bubble[i].r;
            int x2=arr_ai[ii].x;
            int y2=arr_ai[ii].y;
            int w=arr_ai[ii].w;
            int h=arr_ai[ii].h;
            if(circlerectcollide(x1,y1,r,x2,y2,w,h)){//if bubble colides with ai than float it.
                arr_ai[ii].active=false;
                arr_bubble[i].contains=true;
                arr_bubble[i].state=FLOATUP;
                arr_bubble[i].x = x2+tileWidth/2;
                arr_bubble[i].timeout=CAPTURED_TIMEOUT;
                
                break;
            }
        }
        // AI damaged than it im
        for(int ii=0;ii<MAX_AI;ii++){
            if(arr_ai[ii].active==false || arr_ai[ii].state!=DAMAGED)continue;
            int x1=arr_bubble[i].x;
            int y1=arr_bubble[i].y;
            int r=arr_bubble[i].r;
            int x2=arr_ai[ii].x;
            int y2=arr_ai[ii].y;
            int w=arr_ai[ii].w;
            int h=arr_ai[ii].h;
            if(circlerectcollide(x1,y1,r,x2,y2,w,h)){//if bubble colides with ai than float it.
                arr_ai[ii].active=false;
                arr_bubble[i].active=false;
                createaitrajectory(arr_ai[ii].x,arr_ai[ii].y,p[i].facing);
                break;
            }
        }        
    }
}

void playeraicollision(){
    
    for(int i=0;i<MAX_PLAYERS;i++){
        if(p[i].active==false)continue;
        //Collide with player and DAMAGED ai
        for(int ii=0;ii<MAX_AI;ii++){
            if(arr_ai[ii].active==false || arr_ai[ii].state!=DAMAGED)continue;
            if(rectsoverlap(    p[i].x,
                                p[i].y,
                                p[i].w,
                                p[i].h,
                                arr_ai[ii].x,
                                arr_ai[ii].y,
                                arr_ai[ii].w,
                                arr_ai[ii].h)){
            arr_ai[ii].active=false;
            createaitrajectory(arr_ai[ii].x,arr_ai[ii].y,p[i].facing);
            }
        }
    }
}


void playerbubblecollision(){
    for(int i=0;i<MAX_PLAYERS;i++){
        if(p[i].active==false)continue;
        for(int ii=0;ii<MAX_BUBBLES;ii++){
            if(arr_bubble[ii].active==false || arr_bubble[ii].contains==false)continue;
            int r=arr_bubble[ii].r;
            if(circlerectcollide(   arr_bubble[ii].x,
                                    arr_bubble[ii].y,
                                    r,
                                    p[i].x,
                                    p[i].y,
                                    p[i].w,
                                    p[i].h)){
                arr_bubble[ii].pushtime+=2;
                if(arr_bubble[ii].pushtime>BUBBLE_PUSHTIME){//frametime used
                    arr_bubble[ii].active=false;                    
                    createaitrajectory(arr_bubble[ii].x,arr_bubble[ii].y,p[i].facing);
                }else{//push bubble away from player
                    float an=getangle(  arr_bubble[ii].x,
                                        arr_bubble[ii].y,
                                        p[i].x,
                                        p[i].y);
                    int oldx = arr_bubble[ii].x;
                    int oldy = arr_bubble[ii].y;
                    arr_bubble[ii].x -= cos(an)*3;
                    arr_bubble[ii].y -= sin(an)*3;
                    if(bubbletilecollide(ii,0,0)){
                        arr_bubble[ii].x = oldx;
                        arr_bubble[ii].y = oldy;
                    }
                }           
            }
        }
    }
}

void createaitrajectory(int x,int y,int facing){
    for(int i=0;i<MAX_AI;i++){
        if(arr_ai[i].active)continue;
        arr_ai[i].active=true;
        arr_ai[i].x = x;
        arr_ai[i].y = y;
        arr_ai[i].state = TRAJECTORY;
        arr_ai[i].facing = facing;        
        arr_ai[i].w = tileWidth;
        arr_ai[i].h = tileHeight;
        if(arr_ai[i].facing==LEFT){
            arr_ai[i].mx=-3;            
        }else{
            arr_ai[i].mx=3;
        }
        arr_ai[i].my=AI_LAUNCHFORCE;
        arr_ai[i].trajtime = AI_TRAJTIME;
        return;
    }
}


static void inigfx(){
    int tile1[8][8] = {
    {4,4,10,10,4,4,10,10},
    {10,4,4,10,10,4,4,10},
    {10,10,4,4,10,10,4,4},
    {4,10,10,4,4,10,10,4},
    {4,4,10,10,4,4,10,10},
    {10,4,4,10,10,4,4,10},
    {10,10,4,4,10,10,4,4},
    {4,10,10,4,4,10,10,4}};
    // Clear our texture(image) before entering the game loop
    BeginTextureMode(tilepurple);    
    ClearBackground(BLANK); // Make the entire Sprite Transparent.
    EndTextureMode(); 
    // Draw something on it.
    for (int y=0;y<8;y++)
    {
        for (int x=0;x<8; x++)
        {            
            // Our sprite color 1
            if (tile1[y][x]==4)
            {
                BeginTextureMode(tilepurple);    
                DrawRectangle(x*4,y*4,4,4,(Color){200,100,0,255});
                EndTextureMode(); 
            }
            // Our sprite color 2
            if (tile1[y][x]==10)
            {
                BeginTextureMode(tilepurple);    
                DrawRectangle(x*4,y*4,4,8,(Color){100,50,0,255});
                EndTextureMode(); 
            }            
        }
    }
    
    //
    // bobble frame 1
int bframe1[8][8] = {
{5,5,5,2,2,9,2,2},
{0,5,5,5,2,1,2,0},
{0,2,2,5,5,5,5,13},
{0,1,5,5,0,1,0,0},
{1,1,5,5,1,5,1,13},
{0,5,5,1,6,5,6,1},
{1,1,5,5,1,5,1,0},
{0,0,0,5,5,13,0,0}};   
int bframe2[8][8] = {
{5,5,5,2,2,5,2,2},
{0,5,5,5,2,1,2,0},
{0,2,2,5,5,5,5,13},
{0,1,5,5,0,0,0,0},
{1,1,5,5,1,0,0,0},
{0,5,5,1,6,5,0,0},
{1,1,5,5,1,5,1,0},
{0,0,0,5,5,13,0,0}};
int aiframe1[8][8] = {
{0,0,0,2,2,0,0,0},
{12,12,2,2,2,2,12,0},
{2,12,12,12,12,12,12,12},
{2,11,2,11,11,0,0,0},
{1,2,2,1,15,1,15,1},
{0,1,1,15,6,15,6,1},
{0,1,1,1,1,1,1,1},
{0,0,0,1,1,1,1,0}};
int aiframe2[8][8] = {
{0,0,0,2,2,2,0,0},
{0,12,2,2,2,12,12,0},
{12,12,12,12,12,12,12,12},
{2,2,2,11,0,0,0,0},
{2,2,2,1,15,1,15,1},
{0,1,1,15,6,15,6,1},
{0,1,1,1,1,1,1,1},
{0,0,1,1,1,1,1,0}};
   // Clear our texture(image) before entering the game loop
    BeginTextureMode(spritebobble1);    
    ClearBackground(BLANK); // Make the entire Sprite Transparent.
    EndTextureMode(); 
    BeginTextureMode(spritebobble2);    
    ClearBackground(BLANK); // Make the entire Sprite Transparent.
    EndTextureMode(); 
    BeginTextureMode(spriteai1);    
    ClearBackground(BLANK); // Make the entire Sprite Transparent.
    EndTextureMode(); 
    BeginTextureMode(spriteai2);    
    ClearBackground(BLANK); // Make the entire Sprite Transparent.
    EndTextureMode(); 
    c64color[0] = (Color){0,0,0,0};
    // Draw something on it.
    for (int y=0;y<8;y++)
    {
        for (int x=0;x<8; x++)
        {            
                BeginTextureMode(spriteai1);    
                DrawRectangle(x*4,y*4,4,4,c64color[aiframe1[y][x]]);
                EndTextureMode(); 
                BeginTextureMode(spriteai2);    
                DrawRectangle(x*4,y*4,4,4,c64color[aiframe2[y][x]]);
                EndTextureMode(); 
                BeginTextureMode(spritebobble1);    
                DrawRectangle(x*4,y*4,4,4,c64color[bframe1[y][x]]);
                EndTextureMode(); 
                BeginTextureMode(spritebobble2);    
                DrawRectangle(x*4,y*4,4,4,c64color[bframe2[y][x]]);
                EndTextureMode(); 

        }
    }
 
}


// Return the angle from - to in float
float getangle(float x1,float y1,float x2,float y2){
    return (float)atan2(y2-y1, x2-x1);
}

void drawpickups(){
    for(int i=0;i<MAX_PICKUP;i++){
        if(arr_pickup[i].active==false)continue;
        Color col = arr_pickup[i].col;
        DrawRectangle(arr_pickup[i].x,arr_pickup[i].y,arr_pickup[i].w,arr_pickup[i].h,col);
    }
}
void updatepickups(){
    for(int i=0;i<MAX_PICKUP;i++){
        if(arr_pickup[i].active==false)continue;
        arr_pickup[i].timedisappear+=1;
        // If the pickup its time has come.
        if(arr_pickup[i].timedisappear>PICKUP_TIMEDISAPPEAR){
            arr_pickup[i].active=false;
        }
    }
}

void addpickup(int x, int y){
    for(int i=0;i<MAX_PICKUP;i++){
        if(arr_pickup[i].active)continue;        
        arr_pickup[i].active=true;
        arr_pickup[i].x = x;
        arr_pickup[i].y = y;
        arr_pickup[i].w = tileWidth;
        arr_pickup[i].h = tileHeight;  
        arr_pickup[i].timedisappear = 0;
        arr_pickup[i].type = GetRandomValue(0,4);
        switch (arr_pickup[i].type){
            case 0:
                arr_pickup[i].col = (Color){200,0,0,255};
            break;
            case 1:
                arr_pickup[i].col = (Color){0,200,0,255};
            break;
            case 2:
                arr_pickup[i].col = (Color){0,0,200,255};
            break;
            case 3:
                arr_pickup[i].col = (Color){200,200,0,255};
            break;
            case 4:
                arr_pickup[i].col = (Color){200,0,200,255};
            break;
            case 5:
                arr_pickup[i].col = (Color){200,200,200,255};
            break;
        }        
        return;
    }
}

void playerpickupcollision(){
    for(int i=0;i<MAX_PLAYERS;i++){
        if(p[i].active==false)continue;
        for(int ii=0;ii<MAX_PICKUP;ii++){   
            if(arr_pickup[ii].active==false)continue;
            if(rectsoverlap(    p[i].x,
                                p[i].y,
                                p[i].w,
                                p[i].h,
                                arr_pickup[ii].x,
                                arr_pickup[ii].y,
                                arr_pickup[ii].w,
                                arr_pickup[ii].h)){
            arr_pickup[ii].active=false;
            p[i].score+=getscore(arr_pickup[ii].type);
            addpickupeffect(arr_pickup[ii].type,arr_pickup[ii].x,arr_pickup[ii].y);
            // ADD effect here.
            }
        }
    }
}

void updatepickupeffects(){
    for(int i=0;i<MAX_PICKUPEFFECT;i++){
        if(arr_pueffect[i].active==false)continue;
        // If on screen to long than disappear.
        arr_pueffect[i].timedisappear-=1;
        if(arr_pueffect[i].timedisappear<0){
            arr_pueffect[i].active=false;
        }
        arr_pueffect[i].y-=arr_pueffect[i].my;
        if(arr_pueffect[i].alpha>-1)arr_pueffect[i].alpha -= 1;
    }
}
void drawpickupeffects(){
    for(int i=0;i<MAX_PICKUPEFFECT;i++){
        if(arr_pueffect[i].active==false)continue;
        int score=0;
        switch (arr_pueffect[i].type){
        case 0:
            score = POINT1;
        break;
        case 1:
            score = POINT2;
        break;
        case 2:
            score = POINT3;
        break;            
        case 3:
            score = POINT4;
        break;
        case 4:
            score = POINT5;
        break;
        }
        DrawText(   FormatText("%i",score),   
                    arr_pueffect[i].x,arr_pueffect[i].y,
                    tileWidth/2.5,(Color){255,255,255,arr_pueffect[i].alpha});
    }
}
void addpickupeffect(int type,int x, int y){
    for(int i=0;i<MAX_PICKUPEFFECT;i++){
        if(arr_pueffect[i].active)continue;    
        arr_pueffect[i].active = true;
        arr_pueffect[i].x = x;
        arr_pueffect[i].y = y;        
        arr_pueffect[i].alpha = 255;
        arr_pueffect[i].type = type;
        arr_pueffect[i].my = PICKUPEFFECT_MY;
        arr_pueffect[i].timedisappear = PICKUPEFFECT_DISAPPEAR;
        return;
    }
}

int getscore(int type){
    switch (type){
        case 0:
            return POINT1;
        break;
        case 1:
            return POINT2;
        break;
        case 2:
            return POINT3;
        break;
        case 3:
            return POINT4;
        break;
        case 4:
            return POINT5;
        break;
    }
    return 0;
}

void drawplayerbar(){
    DrawRectangle(0,0,150,20,BLACK);
    DrawText(FormatText("%i",p[0].score),20,1,20,WHITE);
    if(p[1].active==false)return;
    DrawRectangle(screenWidth/2,0,150,20,BLACK);
    DrawText(FormatText("%i",p[1].score),screenWidth/2+20,1,20,WHITE);
}

void inigame(){
    for(int i=0;i<MAX_PICKUP;i++){
        arr_pickup[i].active=false;
    }
    
    for(int i=0;i<MAX_AI;i++){
        arr_ai[i].active=false;
    }
    for(int i=0;i<MAX_BUBBLES;i++){
        arr_bubble[i].active=false;
        arr_bubble[i].contains=false;
    }
    for(int i=0;i<MAX_PICKUP;i++){
        arr_pickup[i].active=false;
    }
    for(int i=0;i<MAX_PICKUPEFFECT;i++){    
        arr_pueffect[i].active=false;
    }
    p[PLAYER1].active = true;
    p[PLAYER1].x = 2*tileWidth;
    p[PLAYER1].y = 12*tileHeight;
    p[PLAYER1].w = tileWidth;
    p[PLAYER1].h = tileHeight;
    p[PLAYER1].canjump = true;    
    

    p[PLAYER2].active=false;
    p[PLAYER2].score=0;
    p[PLAYER2].x = 5*tileWidth;
    p[PLAYER2].y = 12*tileHeight;
    p[PLAYER2].w = tileWidth;
    p[PLAYER2].h = tileHeight;
    p[PLAYER2].canjump = true;


}    

void inic64colors(void){
    c64color[0 ] = (Color){0  , 0 , 0 , 255 };//Black
    c64color[1 ] = (Color){255,255,255, 255 };//White
    c64color[2 ] = (Color){136,0  ,0  , 255 };//Red
    c64color[3 ] = (Color){170,255,238, 255 };//Cyan
    c64color[4 ] = (Color){204,68 ,204, 255 };//Violet / Purple
    c64color[5 ] = (Color){0  ,204,85 , 255 };//Green
    c64color[6 ] = (Color){0  ,0  ,170, 255 };//Blue
    c64color[7 ] = (Color){238,238,119, 255 };//Yellow
    c64color[8 ] = (Color){221,136,85 , 255 };//Orange
    c64color[9 ] = (Color){102,68 ,0  , 255 };//Brown
    c64color[10] = (Color){255,119,119, 255 };//Light red
    c64color[11] = (Color){51 ,51 ,51 , 255 };//Dark grey / Grey 1
    c64color[12] = (Color){119,119,119, 255 };//Grey 2
    c64color[13] = (Color){170,255,102, 255 };//Light green
    c64color[14] = (Color){0  ,136,255, 255 };//Light blue
    c64color[15] = (Color){187,187,187, 255 };//Light grey / grey 3    
}
