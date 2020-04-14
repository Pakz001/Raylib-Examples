// Work in PRogress - todo put image map data  into a texture and draw to screen - test conversion.
//

#define MAPWIDTH 800
#define MAPHEIGHT 450


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
static void highsmoothrange(int low,int high,float perc);
static void darkenrange(int low,int high,int ranval);
static void smooth();

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib example. Please wait with Slow computer!!");

    RenderTexture2D target = LoadRenderTexture(MAPWIDTH, MAPHEIGHT);
    BeginTextureMode(target);    
    ClearBackground(BLANK); // Make the entire Sprite Transparent.
    EndTextureMode();     
 
    
    mapsetcolor(255,255,255);
    makelines();
    highsmoothrange(150,255,10);
    darkenrange(0,71,50);
    lightenrange(100,256);    
    smooth();
 
    BeginTextureMode(target);    
    for(int y=0;y<MAPHEIGHT;y++){
    for(int x=0;x<MAPWIDTH;x++){
        
        DrawRectangle(x,y,1,1,(Color){mapr[x][y],mapg[x][y],mapb[x][y],255});
        
    }
    }
    EndTextureMode();     
 
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
            DrawTextureEx(target.texture,(Vector2){0,0},0,1, WHITE);                

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadRenderTexture(target);    // Unload render texture   
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
            angle-=GetRandomValue(1,15-1);
            if(angle<0)angle = 360-angle;
            x+=cos(angle);
            y+=sin(angle);
        }
        else
        {
            angle+=GetRandomValue(1,15-1);
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
        for(int y=-1;y<2;y++){
        for(int x=-1;x<2;x++){
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

void highsmoothrange(int low,int high,float perc){
   		for(int i=0;i<(MAPWIDTH*MAPHEIGHT)*perc;i++){
			int x=GetRandomValue(1,MAPWIDTH-1);
			int y=GetRandomValue(1,MAPHEIGHT-1);
			int r1=mapr[x][y];
			int g1=mapg[x][y];
			int b1=mapb[x][y];			
			int r2=mapr[x+1][y];
			int g2=mapg[x+1][y];
			int b2=mapb[x+1][y];		
			int r3=mapr[x][y+1];
			int g3=mapg[x][y+1];
			int b3=mapb[x][y+1];		
			int r4=mapr[x+1][y+1];
			int g4=mapg[x+1][y+1];
			int b4=mapb[x+1][y+1];
			if(r1<high && g1<high && b1<high){
			if(r1>low && g1>low && b1>low){
                int valr=((r1+r2+r3+r4)/4)*1.2;
                int valg=((g1+g2+g3+g4)/4)*1.2;
                int valb=((b1+b2+b3+b4)/4)*1.2;					
                valr = Clamp(valr,0,255);
                valg = Clamp(valg,0,255);
                valb = Clamp(valb,0,255);
                mapr[x][y] = valr;
                mapg[x][y] = valg;
                mapb[x][y] = valb;			
			}}
		}
}

void darkenrange(int low,int high,int ranval){
    for(int y=0;y<MAPHEIGHT;y++){
    for(int x=0;x<MAPWIDTH;x++){
        if(mapr[x][y] > low){
            mapr[x][y] -= GetRandomValue(ranval/2,ranval-1);
            mapg[x][y] -= GetRandomValue(ranval/2,ranval-1);
            mapb[x][y] -= GetRandomValue(ranval/2,ranval-1);
        }
        if(mapg[x][y] > low){
            mapr[x][y] -= GetRandomValue(ranval/2,ranval-1);
            mapg[x][y] -= GetRandomValue(ranval/2,ranval-1);
            mapb[x][y] -= GetRandomValue(ranval/2,ranval-1);
        }
        if(mapb[x][y] > low){
            mapr[x][y] -= GetRandomValue(ranval/2,ranval-1);
            mapg[x][y] -= GetRandomValue(ranval/2,ranval-1);
            mapb[x][y] -= GetRandomValue(ranval/2,ranval-1);
        }
        mapr[x][y] = Clamp(mapr[x][y],0,255);
        mapg[x][y] = Clamp(mapg[x][y],0,255);
        mapb[x][y] = Clamp(mapb[x][y],0,255);						
    }}
}

void smooth(){
    for( int i=0;i< (MAPWIDTH*MAPHEIGHT);i++){
        int x=GetRandomValue(1,MAPWIDTH-1);
        int y=GetRandomValue(1,MAPHEIGHT-1);

        int col1r=mapr[x+1][y];
        int col1g=mapg[x+1][y];
        int col1b=mapb[x+1][y];   		    		
        
        int col2r=mapr[x+1][y+1];
        int col2g=mapg[x+1][y+1];   		
        int col2b=mapb[x+1][y+1];
        
        int col3r=mapr[x][y+1];    		    		
        int col3g=mapg[x][y+1];    		    		
        int col3b=mapb[x][y+1];    		    		

        int col4r=(col1r+col2r+col3r)/3;
        int col4g=(col1g+col2g+col3g)/3;
        int col4b=(col1b+col2b+col3b)/3;    		    		
        mapr[x][y] = col4r;
        mapg[x][y] = col4g;
        mapb[x][y] = col4b;
        mapr[x][y] = Clamp(mapr[x][y],0,255);
        mapg[x][y] = Clamp(mapg[x][y],0,255);
        mapb[x][y] = Clamp(mapb[x][y],0,255);
    }
}

// Clamp float value
float Clamp(float value, float min, float max)
{
    const float res = value < min ? min : value;
    return res > max ? max : res;
}    
