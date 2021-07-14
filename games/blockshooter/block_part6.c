
#include "raylib.h"
#include <math.h>

int focus=0;
int map[100][100] = {0};
int tileHeight = 32;
int tileWidth = 32;
int mapWidth = 100;
int mapHeight = 100;


// Get our angle between two points.
static float getangle(float x1,float y1,float x2,float y2);
// Our rectsoverlap function. Returns true/false.
static bool rectsoverlap(int x1,int y1,int w1,int h1,int x2,int y2,int w2,int h2);
//Unit collide with solid blocks true/false
bool recttilecollide(int x, int y, int w, int h, int offsetx,int offsety);
void score();



int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib example.");
 
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    for(int i=0;i<25;i++){
        map[i][0]=GetRandomValue(1,3);
    }
    map[5][1]=1;

    float posx = 320;
    float posy = 400;
    
    float animcolor = 1;
    float animx=posx;
    float animy=posy;
    float animtargetx;
    float animtargety;
    bool startanim = false;

    int slidedowntimer = 0;
    int slidedowntimermax = 20;
    bool actualgameover=false;

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        
        if(IsMouseButtonDown(1)){
            for(int x=0;x<25;x++){
                tileWidth=16;
                
            }
        }
        
        slidedowntimer++;
        if(slidedowntimer>slidedowntimermax && startanim==false){
            slidedowntimer=0;
            
            actualgameover=false;
            for(int x=0;x<25;x++){
                if(map[x][7]>0){
                    actualgameover=true;
                }
            }
            if(actualgameover==false){
                for(int y=20;y>0;y--){
                    for(int x=0;x<25;x++){
                        map[x][y]=map[x][y-1];
                }}
                for(int i=0;i<25;i++){
                    map[i][0]=GetRandomValue(1,3);                
                }
            }
            
        }

        score();
        
        if(startanim==false){
            if(IsMouseButtonDown(0)){
                
                float x = GetMouseX()/32;
                float y = GetMouseY()/32;
                
                    if(map[(int)x][(int)y]==0){
                    if( map[(int)x][(int)y-1]>0 || map[(int)x-1][(int)y]>0 || map[(int)x+1][(int)y]>0){
                        //DrawRectangle(x*32,y*32,32,32,DARKGRAY);                        
                        startanim=true;
                        animtargetx = x*32;
                        animtargety = y*32;
                        animx = posx;
                        animy = posy;
                        
                        bool banana=true;
                        while(banana){
                            float an = getangle(animx,animy,animtargetx,animtargety);
                            animx += cos(an)*1;
                            animy += sin(an)*1;
                            if(recttilecollide(animx,animy,32,32,0,0)==true){
                                banana=false;
                                startanim=false;
                                animx = posx;
                                animy = posy;
                                focus=1;
                            }
                            if(rectsoverlap(animx-1,animy-1,32+2,32+2,animtargetx-1,animtargety-1,1,1)){
                                banana=false;
                                animtargetx = x*32;
                                animtargety = y*32;
                                animx = posx;
                                animy = posy;
                                focus=0;
                            }
                        }

                        
                    }}

                    
                    
                }
        }
        if(startanim){
            float an = getangle(animx,animy,animtargetx,animtargety);
            animx += cos(an)*8;
            animy += sin(an)*8;
            if(rectsoverlap(animx-1,animy-1,32+2,32+2,animtargetx-1,animtargety-1,32+2,32+2)){
                startanim=false;
                animx = posx;
                animy = posy;                
                map[(int)animtargetx/32][(int)animtargety/32]=animcolor;
                animcolor = GetRandomValue(1,3);
            }
            
        }
        
        //----------------------------------------------------------------------------------
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);
 
            for(int y=0;y<10;y++){
            for(int x=0;x<25;x++){
                if(map[x][y]==1){
                    DrawRectangle(x*tileWidth,y*32,tileWidth,32,RED);
                    DrawRectangle(x*tileWidth+1,y*32+1,tileWidth-2,32-2,RED);
                }
                if(map[x][y]==2){
                    DrawRectangle(x*tileWidth,y*32,tileWidth,32,RED);
                    DrawRectangle(x*tileWidth+1,y*32+1,tileWidth-2,32-2,GREEN);
                }
                if(map[x][y]==3){
                    DrawRectangle(x*tileWidth,y*32,tileWidth,32,RED);
                    DrawRectangle(x*tileWidth+1,y*32+1,tileWidth-2,32-2,BLUE);
                }

            }}
 
            
            
            if(animcolor==1){
                DrawRectangle(animx,animy,32,32,RED);
                DrawRectangle(animx+1,animy+1,32-2,32-2,RED);

            }
            if(animcolor==2){
                DrawRectangle(animx,animy,32,32,RED);
                DrawRectangle(animx+1,animy+1,32-2,32-2,GREEN);
            }
            if(animcolor==3){
                DrawRectangle(animx,animy,32,32,RED);
                DrawRectangle(animx+1,animy+1,32-2,32-2,BLUE);
            }
            
 
            float x = GetMouseX()/32;
            float y = GetMouseY()/32;
            
            if(map[(int)x][(int)y]==0){
            if( map[(int)x][(int)y-1]>0 || map[(int)x-1][(int)y]>0 || map[(int)x+1][(int)y]>0){
                DrawRectangle(x*32,y*32,32,32,DARKGRAY);
            }}
            
            float an = getangle(posx,posy,x*32,y*32);
 
            DrawLine(posx,posy,posx+cos(an)*32,posy+sin(an)*32,DARKGRAY);
            

            

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;


}
// Return the angle from - to in float
float getangle(float x1,float y1,float x2,float y2){
    return (float)atan2(y2-y1, x2-x1);
}
// Rectangles overlap
bool rectsoverlap(int x1,int y1,int w1,int h1,int x2,int y2,int w2,int h2){
    if(x1 >= (x2 + w2) || (x1 + w1) <= x2) return false;
    if(y1 >= (y2 + h2) || (y1 + h1) <= y2) return false;
    return true;
}

//Unit collide with solid blocks true/false
bool recttilecollide(int x, int y,int w, int h, int offsetx,int offsety){
    int cx = (x+offsetx)/tileWidth;
    int cy = (y+offsety)/tileHeight;
    for(int y2=cy-1; y2<cy+2;y2++){//Note that the - and + are to be set differently with differently sized players
    for(int x2=cx-1; x2<cx+2;x2++){
        if(x2>=0 && x2<mapWidth && y2>=0 && y2<mapHeight){
            if(map[x2][y2] > 0){
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

void score(){        
    // Here we check if any of the same block types are in a certain shape.
    for(int y=1;y<20;y++){
        for(int x=0;x<100;x++){
            if(map[x][y]>0){
                // 2 width / 2 height
                if(map[x][y]==map[x][y] && map[x][y+1]==map[x][y]&& map[x+1][y]==map[x][y] && map[x+1][y+1]==map[x][y]){
                    map[x][y]=0;
                    map[x+1][y]=0;
                    map[x][y+1]=0;
                    map[x+1][y+1]=0;
                }
                // 3 width
                if(map[x][y]==map[x][y] && map[x+1][y]==map[x][y] && map[x+2][y]==map[x][y]){                        
                    map[x][y]=0;
                    map[x+1][y]=0;
                    map[x+2][y]=0;
                    
                }
                // 
                // 3 height
                if(map[x][y]==map[x][y] && map[x][y+1]==map[x][y] && map[x][y+2]==map[x][y]){                        
                    map[x][y]=0;
                    map[x][y+1]=0;
                    map[x][y+2]=0;
                    
                }

            }
    }}
}
