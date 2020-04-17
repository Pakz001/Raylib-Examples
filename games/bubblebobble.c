enum flag{SCROLLLEVELDOWN,INGAME};

#define MAX_PLAYERS 1
#define MAX_FALL_SPEED 2.0f

#include "raylib.h"
#include <math.h>

static const int screenWidth=800;
static const int screenHeight=450;
static int mapWidth;
static int mapHeight;
static float tileWidth;
static float tileHeight;
static int map[512][512];

typedef struct player{
    bool active;
    int x;
    int y;
    int w;
    int h;
    bool canjump;    
    float jumpforce;
}player;

static struct player p[MAX_PLAYERS];


static void inilevel(void);
static void drawmap(int offsetx,int offsety);
static void drawplayers(void);
static void updateplayers(void);
static bool rectsoverlap(int x1,int y1,int w1,int h1,int x2,int y2,int w2,int h2);    
static bool playertilecollide(int num,int tile, int offsetx,int offsety);
static void updateplayergravity();
    
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
    static int gamestate=SCROLLLEVELDOWN;
    
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
            }
            if(p[num].canjump==false && 
                                        p[num].jumpforce<0 && 
                                        IsKeyDown(KEY_RIGHT) &&
                                        playertilecollide(num,2,1,0)==false){
                p[num].x+=1;
            }
            //move left and right while falling down
            if(p[num].canjump==false && 
                                        p[num].jumpforce>0 && 
                                        IsKeyDown(KEY_LEFT) &&
                                        playertilecollide(num,99,-1,0)==false){
                p[num].x-=1;
            }
            if(p[num].canjump==false && 
                                        p[num].jumpforce>0 && 
                                        IsKeyDown(KEY_RIGHT) &&
                                        playertilecollide(num,99,1,0)==false){
                p[num].x+=1;
            }
            //move left and right normal
            if(p[num].canjump && 
                                        IsKeyDown(KEY_LEFT) &&
                                        playertilecollide(num,99,-1,0)==false){
                p[num].x-=1;
            }
            if(p[num].canjump && 
                                        IsKeyDown(KEY_RIGHT) &&
                                        playertilecollide(num,99,1,0)==false){
                p[num].x+=1;
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
                // If we are still jumping then update the position.
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
