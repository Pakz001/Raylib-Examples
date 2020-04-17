enum flag2{PLAYER1,PLAYER2,LEFT,RIGHT};
enum flag{SCROLLLEVELDOWN,INGAME};

#define MAX_PLAYERS 1
#define MAX_FALL_SPEED 2.0f

#define MAX_BUBBLES 128
#define BUBBLE_SHOOTFORCE 10.0f

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
    float x;
    float y;
    int state;
    int r;
    float mx;
    float my;    
}bubble;

static struct bubble arr_bubble[MAX_BUBBLES];

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
    
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------

    InitWindow(screenWidth, screenHeight, "raylib example.");
 
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second

    inilevel();
    p[0].active = true;
    p[0].x = 2*tileWidth;
    p[0].y = 12*tileHeight;
    p[0].w = tileWidth;
    p[0].h = tileHeight;
    p[0].canjump = true;
    //--------------------------------------------------------------------------------------
    static int offsety;
    offsety = -(mapHeight*tileHeight);
    //static int gamestate=SCROLLLEVELDOWN;
    static int gamestate=INGAME;

    arr_bubble[0].active=true;
    arr_bubble[0].r = tileWidth/2;
    
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        
        switch (gamestate){
            case INGAME:
                updateplayers();
                updateplayers();
                updateplayers();
                updateplayergravity();
                updatebubbles();
            break;
        }
        
        arr_bubble[0].x = GetMouseX();
        arr_bubble[0].y = GetMouseY();
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
                    drawbubbles();
                break;
            }
            if(bubbletilecollide(0,0,0)){
                DrawText(FormatText("%f",arr_bubble[0].mx),0,0,20,WHITE);
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
                p[num].jumpforce = -4.0f; // How high we jump.
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
        arr_bubble[i].r = tileWidth/2;
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
        int x = arr_bubble[i].x;
        int y = arr_bubble[i].y;
        DrawCircle(x,y,radius,(Color){0,50,0,20});
        DrawCircleLines(x,y,radius,(Color){0,100,0,100});
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
        
        for(int step=0;step<abs(arr_bubble[i].mx);step+=1){
                  
            if(arr_bubble[i].mx<0.0f ){
                arr_bubble[i].x -= 1;
                if(bubbletilecollide(i,-1,0)){
                    arr_bubble[i].mx=0;                    
                }
            }
            if(arr_bubble[i].mx>0.0f){
                arr_bubble[i].x += 1;
                if(bubbletilecollide(i,1,0)){
                    arr_bubble[i].mx=0;                    
                }
            }
        }
        
        if(arr_bubble[i].mx<0){
            arr_bubble[i].mx+=0.2f;
        }else{
            arr_bubble[i].mx-=0.2f;
        }
        if(arr_bubble[i].mx>-0.3f && arr_bubble[i].mx < 0.3f){
            arr_bubble[i].mx = 0;
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
