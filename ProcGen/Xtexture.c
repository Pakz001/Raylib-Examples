#include "raylib.h"
#include <math.h>

float direction(float x, float y);
// Return the angle from - to in float
float getangledeg(float x1,float y1,float x2,float y2);
void createXtexture();

RenderTexture2D target;

int tw = 640;
int th = 480;

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 600;
    //tw=screenWidth;
    //th=screenHeight;
    InitWindow(screenWidth, screenHeight, "raylib example.");
    //ToggleFullscreen();
    createXtexture();
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------
    int x1=200;
    int y1=200;
    int x2=0;
    int y2=0;
    float an=0;
    int val=0;
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        //an+=.01;
        //x2 = x1+cos(an)*64;
        //y2 = y1+sin(an)*64;
        //if(an>PI*2.0f)an=0;
        
        val++;
        if(val>200){
            val=0;
            createXtexture();
        }
        
        //----------------------------------------------------------------------------------
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            // NOTE: Render texture must be y-flipped due to default OpenGL coordinates (left-bottom)
            //void DrawTexturePro(Texture2D texture, Rectangle source, Rectangle dest, Vector2 origin, float rotation, Color tint);  
            DrawTexturePro(target.texture,  (Rectangle){ 0, 0,tw,-th },
                                            (Rectangle){ 0,0,screenWidth,screenHeight}, (Vector2){ 0,0 },0, WHITE);

            //float n = direction(cos(an),sin(an));//getangledeg(x1,y1,x2,y2); //*(180/PI));//direction(0.0f,0.0f); 
            //float n = direction(0,-1);
            //DrawText(FormatText("%f",n),10,10,20,RED);
            //DrawLine(x1,y1,x2,y2,RED);

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


void createXtexture(){
    // A map array for getting the tiny spots on the canvas
    int map[tw][th];
    for(int y=0;y<th;y++){
    for(int x=0;x<tw;x++){
        map[x][y]=0;
    }}
    
    // Create some small line parts on the map
    for(int i=0;i<tw*2;i++){
        float x1 = GetRandomValue(0,tw);
        float y1 = GetRandomValue(0,th);
        float x = x1;
        float y = y1;
        float an=PI*2.0f/2;//GetRandomValue(0,360)*DEG2RAD;
        int len=GetRandomValue(1,3);
        if(GetRandomValue(0,10)<2)len*=4;
        for(int j=0;j<len;j++){
            x+=cos(an)*2;
            y+=sin(an)*2;
            //an=GetRandomValue(0,360)*DEG2RAD;
            an+=.5;
            if(x>=0 && y>=0 && x<tw && y<th)
                map[(int)x][(int)y]=1;
        }
        // here we add some lighter line parts nearby
        if(GetRandomValue(0,10)<8){
        x = x1+1;
        y = y1+1;
        an=PI*2.0f/2;
        len/=2;
        for(int j=0;j<len;j++){
            x+=cos(an)*1;
            y+=sin(an)*1;
            //an=GetRandomValue(0,360)*DEG2RAD;
            an+=.5;
            if(x>=0 && y>=0 && x<tw && y<th)
                map[(int)x][(int)y]=2;
        }
        }


    }
    // grow the spots on the map
    for(int i=0;i<tw*th;i++){
        int x=GetRandomValue(1,tw-2);
        int y=GetRandomValue(1,th-2);
        if(map[x][y]>0){
            int nx=GetRandomValue(-1,1);
            int ny=GetRandomValue(-1,1);
            map[x+nx][y+ny]=map[x][y];
        }
    }
 
    // add some light dots with shadow
    for(int i=0;i<tw/2;i++){
        int x = GetRandomValue(2,tw-2);
        int y = GetRandomValue(2,th-2);
        map[x][y]=3;
        map[x+1][y+1]=2;
    }
 
    // our rendertexture
    
    target = LoadRenderTexture(tw,th);
    // Clear our texture(image) before entering the game loop
    BeginTextureMode(target);    


    //ClearBackground(DARKPURPLE);
    //DrawCircle(0,0, 100, YELLOW);
    
    // These are the colors we use
    Color back=(Color){75,50,80,255};
    Color high=(Color){80,75,99,255};
    Color low=(Color){30,50,70,255};
    ClearBackground(back);

    // Here we draw the data we created in the mapp[][]
    for(int y=0;y<th;y++){
    for(int x=0;x<tw;x++){
        // This is the noise
        if(GetRandomValue(0,10)<9){
            Color tc;
            int val = GetRandomValue(-50,50);
            if(GetRandomValue(0,10)<5)val/=2;
            if(GetRandomValue(0,10)<9)val=GetRandomValue(-5,5);
            tc.r = back.r+val;
            tc.g = back.g+val;
            tc.b = back.b+val;
            tc.a = 255;
            if(tc.r<0)tc.r=0;
            if(tc.g<0)tc.g=0;
            if(tc.b<0)tc.b=0;
            if(tc.r>255)tc.r=255;
            if(tc.g>255)tc.g=255;
            if(tc.b>255)tc.b=255;

            DrawPixel(x,y,tc);
        }
        if(map[x][y]==1)
            DrawPixel(x,y,low);
        if(map[x][y]==2)
            DrawPixel(x,y,high);
        if(map[x][y]==3)
            DrawPixel(x,y,BLACK);    
        

    }}

    // here we create the cracks!!
    for(int j=0;j<tw/3;j++){
        int len=GetRandomValue(5,tw/2);
        if(GetRandomValue(0,20)<10)len=GetRandomValue(5,10);
        float angle=(float)(GetRandomValue(0,360)*DEG2RAD);
        float x=GetRandomValue(0,tw);
        float y=GetRandomValue(0,th);    
        for(int i=0 ;i<len;i++){
            int val=GetRandomValue(-20,20);
            Color high2 = (Color){high.r-val,high.g-val,high.b-val,255};
            int val2=GetRandomValue(0,20);
            Color low2 = (Color){low.r+val2,low.g+val2,low.b+val2,255};
            int val3=GetRandomValue(0,20);
            Color BLACK2 = (Color){val3,val3,val3,255};
            
            DrawPixel(x,y,BLACK2);
            if(GetRandomValue(0,10)<5)DrawPixel(x,y,low2);
            int d=direction(cos(angle),sin(angle));
            if(d==0)// || d==3 || d==4 || d==5 || d==7)
                if(GetRandomValue(0,10)<2)DrawPixel(x,y+1,high2);
            if(d==1)
                if(GetRandomValue(0,10)<2)DrawPixel(x+1,y,high2);
            if(d==2)
                DrawPixel(x+1,y,high2);
            if(d==3)
                DrawPixel(x+1,y,high2);
                
            if(d==4)
                DrawPixel(x,y+1,high2);
                if(GetRandomValue(0,10)<2)DrawPixel(x,y-1,BLACK2);

            //if(d==5)
            //    DrawPixel(x,y,YELLOW);
            if(d==6)
                DrawPixel(x+1,y,high2);
            if(d==7)
                DrawPixel(x,y+1,high2);
            x+=cos(angle)*1;
            y+=sin(angle)*1;
            if(GetRandomValue(0,20)<2)
                angle+=(float)(GetRandomValue(-90,90)*DEG2RAD);
            //angle+=.05;
        }
    }
    
    
    // Draw something on it.
    

    EndTextureMode();               // This needs to be called after every different draw command used. Do not forget to use begintexture also..
}
//
// Takes two float numbers x and y -,+
// Returns right=0;rightdown1;down=2....
float direction(float x, float y){
    
    float angle = (float)atan2(y-0.0f, x-0.0f);
    
    angle*=RAD2DEG;
    if (angle < 0)
    {
        angle+=360;
    }
    int i = (angle + 22.50)/45;
    
    return i % 8;        
}
// Return the angle from - to in float
float getangledeg(float x1,float y1,float x2,float y2){
    float angle = (float)atan2(y2-y1, x2-x1);
    angle*=RAD2DEG;
    if (angle < 0)
    {
        angle+=360;
    }
    return angle;    
}
