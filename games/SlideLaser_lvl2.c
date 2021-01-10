//
// Sliding laser.

// The idea here is to have a player slide a laser below a enemy or enemy weapon. When the laser
// stops sliding a laser is shot upwards and it should destroy the threat.
//
// Going on a break.. Maybe I will expand and make it more playable later.
//
//

#include "raylib.h"
#include <math.h>


#define MAX_SLIDELASERS 10
#define MAX_CEILTURRETS 10
#define MAX_BULLETS 64   
#define MAX_EFFECT 1000
   
int myMap[20][11] =  {  
                        {1,1,1,1,1,1,1,1,1,1,1},
                        {1,1,1,1,0,0,0,2,0,0,1},
                        {1,1,1,1,0,0,0,0,0,0,1},
                        {1,1,1,1,0,0,0,0,0,0,1},
                        {1,0,0,0,0,0,0,0,0,0,1},
                        {1,0,1,1,1,1,1,1,1,1,1},
                        {1,0,1,1,1,1,1,1,1,1,1},
                        {1,0,1,0,2,0,1,1,1,1,1},
                        {1,0,1,0,0,0,1,1,1,1,1},
                        {1,0,1,0,0,0,1,1,1,1,1},
                        {1,0,0,0,0,0,1,1,1,1,1},
                        {1,0,0,0,0,0,0,0,0,0,1},
                        {1,1,1,1,1,1,1,1,0,0,1},
                        {1,1,1,1,1,1,1,1,0,0,1},
                        {1,1,1,0,2,0,0,1,0,0,1},
                        {1,1,1,0,0,0,0,1,0,0,1},
                        {1,1,1,0,0,0,0,1,0,0,1},
                        {1,1,1,0,0,0,0,1,0,0,1},
                        {1,0,0,0,0,0,0,0,0,0,1},
                        {1,1,1,1,1,1,1,1,1,1,1}
                    };  
int mapy;
int mapx;
int mapWidth = 11;
int mapHeight = 20;
float tileWidth;
float tileHeight;

typedef struct player{
    bool active;
    Vector2 position;    
    int direction; // -1 left, 2 - right
    int w;
    int h;
    int numslidelasers;
}player;

static player myplayer = {0};

static struct effect2{
    bool active;
    Vector2 position;
    int startdelay;
    int radius;
    int countdown;
}effect2;
static struct effect2 arr_effect2[MAX_EFFECT];

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


// design.
// shoot x amount of rays into any direction to see if player is there.
// if player is there than the bullet is shot.
static struct bullet{
    bool active;
    Vector2 position;
    Vector2 inc;
    int radius;
}bullet;

static struct bullet arr_bullet[MAX_BULLETS];

static struct ceilturret{
    bool active;
    Vector2 position;

    int w;
    int h;
    int shootdelay;
}aiceilturret;

static struct ceilturret arr_ceilturret[MAX_CEILTURRETS];

static struct slidelaser{
    bool active;
    int state;
    Vector2 position;
    Vector2 inc;
    Vector2 incdec;
    int ceilingloc;
    int w;
    int h;
}slidelaser;

static struct slidelaser arr_slidelaser[MAX_SLIDELASERS];


//Unit collide with solid blocks true/false
bool recttilecollide(int x, int y, int w, int h, int offsetx,int offsety);
// Our rectsoverlap function. Returns true/false.
static bool rectsoverlap(int x1,int y1,int w1,int h1,int x2,int y2,int w2,int h2);
void createeffect(int x, int y);                   
void updateentities(int x,int y);

int screenWidth;
int screenHeight;

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    screenWidth = 800;
    screenHeight = 600;
    tileWidth = ceil((float)(float)screenWidth/(float)mapWidth);
    tileHeight = ceil((float)screenHeight/(float)(mapHeight/2));
    mapy=-tileHeight*10;
    InitWindow(screenWidth, screenHeight, "raylib example.");
 
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    
    arr_slidelaser[0].active = false;
    arr_slidelaser[0].state = 0;
    arr_slidelaser[0].position = (Vector2){160,530};
    arr_slidelaser[0].w = 16;
    arr_slidelaser[0].h = 10;
    arr_slidelaser[0].inc.x = 3;
    arr_slidelaser[0].incdec.x = 0.02;
    
    myplayer.active = true;
    myplayer.w = 16;
    myplayer.h = 24;
    myplayer.position = (Vector2){100,516};
    myplayer.direction = 2;
    
    int ceilcount=0;
    for(int y=0;y<mapHeight;y++){
    for(int x=0;x<mapWidth;x++){
        if(myMap[y][x]==2){
            myMap[y][x]=0;
            arr_ceilturret[ceilcount].active = true;
            arr_ceilturret[ceilcount].position.x=x*tileWidth;
            arr_ceilturret[ceilcount].position.y=y*tileHeight+mapy;
            arr_ceilturret[ceilcount].w = tileWidth/2;
            arr_ceilturret[ceilcount].h = tileHeight/2;
            ceilcount++;
        }
    }}
    
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        
        // update the effect 2
        for(int i=0;i<MAX_EFFECT;i++){
            if(arr_effect2[i].active==false)continue;
            arr_effect2[i].countdown--;
            if(arr_effect2[i].countdown<0){
                arr_effect2[i].active=false;                
            }
        }
        
        // update the effect
        for(int i=0;i<MAX_EFFECT;i++){
            if(arr_effect[i].active==true){
                // When exit the effect and disable
                Vector2 oldpos = arr_effect[i].position;
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
                
                if(recttilecollide(arr_effect[i].position.x,arr_effect[i].position.y,1,32,0,-1)){
                    arr_effect[i].inc.y=-arr_effect[i].inc.y/4;
                }
                if(recttilecollide(arr_effect[i].position.x,arr_effect[i].position.y,1,6,0,0)){
                    arr_effect[i].inc.x/=2;
                    arr_effect[i].position = oldpos;
                }
                

            }
        }
        
        //Update the bullets
        for(int i=0;i<MAX_BULLETS;i++){
            if(arr_bullet[i].active==false)continue;
                for(int speed=0;speed<8;speed++){
                    arr_bullet[i].position.x+=arr_bullet[i].inc.x;
                    arr_bullet[i].position.y+=arr_bullet[i].inc.y;
                    if(recttilecollide(arr_bullet[i].position.x,arr_bullet[i].position.y,6,6,0,0)){
                        arr_bullet[i].active=false;
                    }
                }
        }

        // update the turrets
        for(int i=0;i<MAX_CEILTURRETS;i++){
            if(arr_ceilturret[i].active==false)continue;
            arr_ceilturret[i].shootdelay-=1;
            if(arr_ceilturret[i].shootdelay>0)continue;            
            int touch;//if collided with map count
            for(int a=0;a<10;a++){
                float angle=(float)GetRandomValue(0,6200)/100;
                Vector2 position = arr_ceilturret[i].position;
                for(int b=0;b<screenHeight;b++){
                    position.x += cos(angle)*2;
                    position.y += sin(angle)*2;
                    if(recttilecollide(position.x,position.y,6,6,0,0))b=999999;
                    if(rectsoverlap(position.x,position.y,6,6,myplayer.position.x,myplayer.position.y,myplayer.w,myplayer.h)){                        
                        for(int free=0;free<MAX_BULLETS;free++){
                            if(arr_ceilturret[i].shootdelay<0 && arr_bullet[free].active==false){
                                arr_bullet[free].active=true;
                                arr_bullet[free].radius = 6;                                               
                                arr_bullet[free].position = arr_ceilturret[i].position;
                                arr_bullet[free].position.x+=GetRandomValue(0,10);
                                arr_bullet[free].inc.x = cos(angle)*1;
                                arr_bullet[free].inc.y = sin(angle)*1;
                                arr_ceilturret[i].shootdelay = 20;                                
                                
                            }
                        }
                    }
                }
            }
            
        }

        // Update the slidelasers
        for(int i=0;i<MAX_SLIDELASERS;i++){
            if(arr_slidelaser[i].active==false)continue;
            if(arr_slidelaser[i].state==0){
                if(arr_slidelaser[i].inc.x>0){
                    arr_slidelaser[i].inc.x-=arr_slidelaser[i].incdec.x;
                }else{
                    arr_slidelaser[i].inc.x+=arr_slidelaser[i].incdec.x;
                }
                if((arr_slidelaser[i].inc.x>-0.2 && arr_slidelaser[i].inc.x<0.2) || recttilecollide(arr_slidelaser[i].position.x,arr_slidelaser[i].position.y,arr_slidelaser[i].w,arr_slidelaser[i].h,0,0)){
                    arr_slidelaser[i].state = 1;
                    arr_slidelaser[i].inc.x = 0;                    
                    // find ceiling(top of the laser beam;)
                    int ceilingloc;
                    for(int y=arr_slidelaser[i].position.y;y>0;y--){
                        if(recttilecollide(arr_slidelaser[i].position.x+4,y,1,1,0,0)){
                            ceilingloc=y+1;
                            break;
                        }
                    }
                    arr_slidelaser[i].ceilingloc = ceilingloc;

                }
                arr_slidelaser[i].position.x+=arr_slidelaser[i].inc.x;
            }
            // If the laser cuts into the first! ceiling turret then remove it.
            if(arr_slidelaser[i].state==1){
                if(arr_slidelaser[i].active==true){
                    for(int j=0;j<MAX_CEILTURRETS;j++){
                        if(rectsoverlap(arr_slidelaser[i].position.x,0,arr_slidelaser[i].w,10,arr_ceilturret[j].position.x-arr_ceilturret[j].w,0,arr_ceilturret[j].w*2,10)){
                            if(arr_ceilturret[j].active)createeffect(arr_ceilturret[j].position.x,arr_ceilturret[j].position.y+tileHeight/4);
                            arr_ceilturret[j].active=false;
                        }
                    }
                }
            }
            
        }


        // Update the player..
        if(myplayer.active==true){
            Vector2 oldpos = myplayer.position;
            int fast=1;
            if(IsKeyDown(KEY_LEFT_SHIFT)){
                fast++;
            }
            if(IsKeyDown(KEY_DOWN)){
                for(int i=0;i<fast;i++){
                    if(mapy>-tileHeight*10){
                        mapy--;
                        updateentities(0,-1);
                    }
                };
                //if(mapy<-tileHeight*10)mapy=-tileHeight*10;
                //myplayer.position.y += fast;
                //myplayer.direction = 2;
            }
            if(IsKeyDown(KEY_UP)){
                for(int i=0;i<fast;i++){
                    if(mapy<0){
                        mapy++;
                        updateentities(0,1);
                    }
                }
                //if(mapy>0)mapy=0;
                //myplayer.position.y += fast;
                //myplayer.direction = 2;
            }

            if(IsKeyDown(KEY_RIGHT)){
                myplayer.position.x += fast;
                myplayer.direction = 2;
            }
            if(IsKeyDown(KEY_LEFT)){
                myplayer.position.x -= fast;
                myplayer.direction = 1;
            }
            if(recttilecollide(myplayer.position.x,myplayer.position.y,myplayer.w,myplayer.h,0,0)){
                myplayer.position = oldpos;
            }
            if(IsKeyPressed(KEY_Z)){ // Slide the laser weapon
                if(myplayer.numslidelasers<3){
                    int cl=myplayer.numslidelasers; // current sliding laser number
                    arr_slidelaser[cl].active = true;
                    arr_slidelaser[cl].w = 16;
                    arr_slidelaser[cl].h = 10;    
                    arr_slidelaser[cl].incdec.x = 0.02; 
                    arr_slidelaser[cl].state = 0;
                    arr_slidelaser[cl].position.x = myplayer.position.x;
                    arr_slidelaser[cl].position.y = myplayer.position.y+arr_slidelaser[0].h+4;
                    if(myplayer.direction==1){
                        arr_slidelaser[cl].inc.x = -1.5*fast;
                    }else{
                        arr_slidelaser[cl].inc.x = 1.5*fast;
                    }
                    myplayer.numslidelasers++;
                }
            }
        }
        
        //----------------------------------------------------------------------------------
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(LIGHTGRAY);
 
            // Draw the turrets
            for(int i=0;i<MAX_CEILTURRETS;i++){
                if(arr_ceilturret[i].active==false)continue;
                DrawEllipse(arr_ceilturret[i].position.x,arr_ceilturret[i].position.y,arr_ceilturret[i].w,arr_ceilturret[i].h,WHITE);
                DrawEllipse(arr_ceilturret[i].position.x+4,arr_ceilturret[i].position.y,arr_ceilturret[i].w-4,arr_ceilturret[i].h,BLACK);
                DrawEllipse(arr_ceilturret[i].position.x+2,arr_ceilturret[i].position.y+2,arr_ceilturret[i].w-5,arr_ceilturret[i].h-4,YELLOW);
                
            }
 
            // Draw map
            for (int y = 0; y< mapHeight ; y++)
            {
                for (int x = 0; x< mapWidth ; x++)
                {
                    if (myMap[y][x] == 1)
                    {
                        DrawRectangle(x*tileWidth,y*tileHeight+mapy,tileWidth,tileHeight,(Color){60,50,250,255});
                    }
                }
            }
            
          

            // draw the player
            if(myplayer.active==true){
                DrawRectangle(myplayer.position.x,myplayer.position.y,myplayer.w,myplayer.h,GREEN);
            }
          
            // Draw the slidelasers
            for(int i=0;i<MAX_SLIDELASERS;i++){
                if(arr_slidelaser[i].active==false)continue;                
                DrawRectangle(arr_slidelaser[i].position.x,arr_slidelaser[i].position.y,arr_slidelaser[i].w,arr_slidelaser[i].h,GRAY);
                if(arr_slidelaser[i].state==1){
                    DrawRectangle(arr_slidelaser[i].position.x+3,arr_slidelaser[i].ceilingloc,arr_slidelaser[i].w-6,arr_slidelaser[i].position.y-arr_slidelaser[i].ceilingloc,YELLOW);
                }
            }
            
            // Draw the bullets
            for(int i=0;i<MAX_BULLETS;i++){
                if(arr_bullet[i].active==false)continue;
                DrawCircle(arr_bullet[i].position.x,arr_bullet[i].position.y,6,RED);
            }
            
            // draw the effect
            for(int i=0;i<MAX_EFFECT;i++){
                if(arr_effect[i].active==false)continue;
                DrawRectangle(arr_effect[i].position.x,arr_effect[i].position.y,arr_effect[i].w,arr_effect[i].h,RED);
            }            
           // draw the effect2
            for(int i=0;i<MAX_EFFECT;i++){
                if(arr_effect2[i].active==false)continue;
                arr_effect2[i].startdelay--;
                if(arr_effect2[i].startdelay>0)continue;
                DrawCircle(arr_effect2[i].position.x,arr_effect2[i].position.y,arr_effect2[i].radius,WHITE);
            }          

  
 // some screen info
            DrawText("Cursor Left and Right. Left Shift = Run. Z key is slide laser weapon.",2,2,22,WHITE);
            DrawText(FormatText("SlideLasers : %02i",3-myplayer.numslidelasers),2,screenHeight-32,26,WHITE);

//DrawText(FormatText("SlideLasers : %f",PI*2.0f),312,screenHeight-32,26,WHITE);

        EndDrawing();
        //------------------------------- --------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;


}

void updateentities(int x, int y){
    for(int i=0;i<MAX_CEILTURRETS;i++){
        if(arr_ceilturret[i].active==false)continue;
        arr_ceilturret[i].position.y+=y;
    }
    for(int i=0;i<MAX_BULLETS;i++){
        if(arr_bullet[i].active==false)continue;
        arr_bullet[i].position.y+=y;
    }
    for(int i=0;i<MAX_EFFECT;i++){
        if(arr_effect[i].active==false)continue;
        arr_effect[i].position.y+=y;
    }
    for(int i=0;i<MAX_EFFECT;i++){
        if(arr_effect2[i].active==false)continue;
        arr_effect2[i].position.y+=y;
    }
    for(int i=0;i<MAX_SLIDELASERS;i++){
        if(arr_slidelaser[i].active==false)continue;
        arr_slidelaser[i].position.y+=y;
    }

}

void createeffect(int posx, int posy){
    //int posx = GetRandomValue(0,screenWidth);
    //int posy = GetRandomValue(0,screenHeight);
    int i=0;
    int cnt=0;
    
    while(cnt<5){
        i++;
        if(i>MAX_EFFECT){
            continue;
        }
        if(arr_effect2[i].active==true){        
            continue;
        }
        arr_effect2[i].active = true;
        arr_effect2[i].position.x = posx+GetRandomValue(-32,32);
        arr_effect2[i].position.y = posy+GetRandomValue(-32,32);        
        arr_effect2[i].startdelay = GetRandomValue(0,60);
        arr_effect2[i].countdown = 10+arr_effect2[i].startdelay;
        arr_effect2[i].radius = GetRandomValue(tileWidth/4,tileWidth);
        cnt++;
    }
    
    i=0;
    cnt=0;
    while(cnt<32){
        i++;
        if(i>MAX_EFFECT){
            continue;            
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
        arr_effect[i].inc.y = GetRandomValue(-5,-2);
        arr_effect[i].incmod.x = (float)(GetRandomValue(0,100)/3500.0f);
        arr_effect[i].incmod.y = (float)(GetRandomValue(0,100)/1000.0f);//+0.1f;
        if(GetRandomValue(0,8)==1){
            arr_effect[i].incmod.x*=5;
            arr_effect[i].inc.y*=1.5;
        }
        if(GetRandomValue(0,8)==1){
            arr_effect[i].incmod.x*=5;
            arr_effect[i].inc.y/=2;
        }
        arr_effect[i].countdown = GetRandomValue(30,70);
        cnt++;
    }
}

//Unit collide with solid blocks true/false
bool recttilecollide(int x, int y,int w, int h, int offsetx,int offsety){
    y+=abs(mapy);
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
