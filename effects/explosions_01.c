//
// Here some random eplosion/fireworks effects.

// I actually discovered here that the GetRandomValue does not return floats!
//
//
//


#include "raylib.h"
#include <math.h>

#define MAX_EFFECT 1000

int myMap[10][11] =  {  {1,1,1,1,1,1,1,1,1,1,1},
                        {1,1,1,1,1,1,1,1,1,1,1},
                        {1,1,1,1,1,1,1,1,1,1,1},
                        {1,1,1,1,1,1,1,1,1,1,1},
                        {1,1,1,1,1,1,1,1,1,1,1},
                        {1,1,1,1,1,1,1,1,1,1,1},
                        {1,1,1,1,1,1,1,1,1,1,1},
                        {1,1,1,1,1,1,1,1,1,1,1},
                        {1,1,1,1,1,1,1,1,1,1,1},
                        {1,1,1,1,1,1,1,1,1,1,1}
                    };  
int mapWidth = 11;
int mapHeight = 10;
float tileWidth;
float tileHeight;

static struct effect{
    bool active;
    Vector2 position;
    Vector2 inc;
    Vector2 incmod;
    int w;
    int h;
    int countdown;
}effect;

static struct effect arr_effect[MAX_EFFECT];


int screenWidth;
int screenHeight;

void createeffect();
//Unit collide with solid blocks true/false
bool recttilecollide(int x, int y, int w, int h, int offsetx,int offsety);
// Our rectsoverlap function. Returns true/false.
static bool rectsoverlap(int x1,int y1,int w1,int h1,int x2,int y2,int w2,int h2);

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    screenWidth = 800;
    screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib example.");
    tileWidth = ceil((float)(float)screenWidth/(float)mapWidth);
    tileHeight = ceil((float)screenHeight/(float)mapHeight);

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    createeffect();
    int effectdelay=0;

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        
        effectdelay--;
        if(effectdelay<0){
            effectdelay=GetRandomValue(0,100);
            createeffect();
        }
        
        // update the effect
        for(int i=0;i<MAX_EFFECT;i++){
            if(arr_effect[i].active==true){
                // When exit the effect and disable
                arr_effect[i].countdown--;
                if(arr_effect[i].countdown<0)arr_effect[i].active=false;
                
                arr_effect[i].inc.y += arr_effect[i].incmod.y;
                
                if(arr_effect[i].inc.x<0){
                    arr_effect[i].inc.x-=arr_effect[i].incmod.x;
                }else{
                    arr_effect[i].inc.x+=arr_effect[i].incmod.x;;
                }
                
                arr_effect[i].position.x += arr_effect[i].inc.x;
                arr_effect[i].position.y += arr_effect[i].inc.y;
            }
        }
        
        
        //----------------------------------------------------------------------------------
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);
                
            // draw the effect
            for(int i=0;i<MAX_EFFECT;i++){
                if(arr_effect[i].active==false)continue;
                DrawRectangle(arr_effect[i].position.x,arr_effect[i].position.y,arr_effect[i].w,arr_effect[i].h,RED);
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

void createeffect(){
    int posx = GetRandomValue(0,screenWidth);
    int posy = GetRandomValue(0,screenHeight);
    int i=0;
    int cnt=0;
    while(cnt<50){
        i++;
        if(i>MAX_EFFECT){
            continue;
            cnt=51;
        }
        if(arr_effect[i].active==true){        
            continue;
        }
        arr_effect[i].active = true;
        arr_effect[i].position.x = posx;
        arr_effect[i].position.y = posy;
        arr_effect[i].w = 16;
        arr_effect[i].h = 16;
        arr_effect[i].inc.x = GetRandomValue(-1,1);
        arr_effect[i].inc.y = GetRandomValue(-10,-5);
        arr_effect[i].incmod.x = (float)(GetRandomValue(0,100)/2500.0f);
        arr_effect[i].incmod.y = (float)(GetRandomValue(0,100)/1000.0f)+0.2f;
        if(GetRandomValue(0,8)==1){
            arr_effect[i].incmod.x*=5;
            arr_effect[i].inc.y*=1.5;
        }
        arr_effect[i].countdown = GetRandomValue(50,120);
        cnt++;
    }
}


//Unit collide with solid blocks true/false
bool recttilecollide(int x, int y,int w, int h, int offsetx,int offsety){
    int cx = (x+offsetx)/tileWidth;
    int cy = (y+offsety)/tileHeight;
    for(int y2=cy-1; y2<cy+2;y2++){//Note that the - and + are to be set differently with differently sized players
    for(int x2=cx-1; x2<cx+2;x2++){
        if(x2>=0 && x2<mapWidth && y2>=0 && y2<mapHeight){
            if(myMap[y2][x2] == 1){
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
