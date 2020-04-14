// Work in PRogress - todo put image map data  into a texture and draw to screen - test conversion.
//

#define MAPWIDTH 128
#define MAPHEIGHT 128


#include "raylib.h"
#include <math.h>

static int mapr[MAPWIDTH][MAPHEIGHT];
static int mapg[MAPWIDTH][MAPHEIGHT];
static int mapb[MAPWIDTH][MAPHEIGHT];

void mapsetcolor(int r,int g,int b);
static float Clamp(float value, float min, float max);
static void makelines(void);
static void makelinesolid(float x,float y,float dist,int r,int g,int b);
static void makelineshade(float x,float y,float dist);
static void lightenrange(int low,int high);

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib example.");
 
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        
        
        //----------------------------------------------------------------------------------
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);
                

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;


}

void mapsetcolor(int r,int g,int b){
    for(int y=0;y<MAPHEIGHT;y++){
    for(int x=0;x<MAPWIDTH;x++){
        mapr[x][y] = r;
        mapg[x][y] = g;
        mapb[x][y] = b ;
    }}
}

void makelines(){
    for(int i=0;i<(MAPWIDTH*MAPHEIGHT/10);i++){
        int x=GetRandomValue(0,MAPWIDTH-1);
        int y=GetRandomValue(0,MAPHEIGHT-1);
        int dist=GetRandomValue(2,12-1);
        if(GetRandomValue(0,10-1)<2){
            makelinesolid(x,y,dist,GetRandomValue(0,55),GetRandomValue(0,55),GetRandomValue(0,55));
        }
        makelineshade(x,y,dist);
    }
}

void makelinesolid(float x,float y,float dist,int r,int g,int b){
    int angle=GetRandomValue(0,360);
    for(float i = 0;i<dist;i+=0.2){
        int beh=GetRandomValue(0,9);
        if (beh<5)
        {
            x+=cos(angle);
            y+=sin(angle);
        }
        else if (beh>4 && beh<8)
        {
            angle-=GetRandomValue(1,15);
            if(angle<0)angle = 360-angle;
            x+=cos(angle);
            y+=sin(angle);
        }
        else
        {
            angle+=GetRandomValue(1,15);
            if(angle>359)angle = 0+angle;
            x+=cos(angle);
            y+=sin(angle);    			
        }        
        if(x>-1 && y>-1 && x<MAPWIDTH && y<MAPHEIGHT){
            mapr[(int)x][(int)y] = r;
            mapg[(int)x][(int)y] = g;
            mapb[(int)x][(int)y] = b ;   		    		
        }
    }
}
    
void makelineshade(float x,float y,float dist){
    int angle=GetRandomValue(0,360);
    for(float i = 0;i<dist;i+=0.2){
        int beh=GetRandomValue(0,10-1);
        if(beh<5){
            x+=cos(angle);
            y+=sin(angle);
        }
        else if(beh>4 && beh<8){
            angle-=GetRandomValue(1,15-1);
            if(angle<0)angle= 360-angle;
            x+=cos(angle);
            y+=sin(angle);
        }
        else
        {
            angle+=GetRandomValue(1,15-1);
            if(angle>359) angle = 0+angle;
            x+=cos(angle);
            y+=sin(angle);   			
        }
        if(x>-1 && y>-1 && x<MAPWIDTH && y<MAPHEIGHT){
            int valr=mapr[(int)x][(int)y] / 1.2;
            int valg=mapg[(int)x][(int)y] / 1.2;
            int valb=mapb[(int)x][(int)y] / 1.2;   			
            valr = Clamp(valr,0,255);
            valg = Clamp(valg,0,255);
            valb = Clamp(valb,0,255);
            mapr[(int)x][(int)y] = valr;
            mapg[(int)x][(int)y] = valg;
            mapb[(int)x][(int)y] = valb;
        }
    }
}

void lightenrange(int low,int high){
    for(int i=0;i<(MAPWIDTH*MAPHEIGHT)*4;i++){
        int x1=GetRandomValue(1,MAPWIDTH-2);
        int y1=GetRandomValue(1,MAPHEIGHT-2);
        int cnt=0;
        for(int y=-1;y<1;y++){
        for(int x=-1;x<1;x++){
            if(mapr[x1+x][y1+y] > low && mapr[x1+x][y1+y] < high){
            if(mapg[x1+x][y1+y] > low && mapg[x1+x][y1+y] < high){
            if(mapb[x1+x][y1+y] > low && mapb[x1+x][y1+y] < high) {  		    		
                cnt+=1;           
            }
            }
            }
        }}
        if(cnt>7){
            mapr[x1][y1] = Clamp(mapr[x1][y1] + 20,0,255);
            mapg[x1][y1] = Clamp(mapg[x1][y1] + 20,0,255);    	
            mapb[x1][y1] = Clamp(mapb[x1][y1] + 20,0,255);   	
        }
    }
}

// Clamp float value
float Clamp(float value, float min, float max)
{
    const float res = value < min ? min : value;
    return res > max ? max : res;
}    
