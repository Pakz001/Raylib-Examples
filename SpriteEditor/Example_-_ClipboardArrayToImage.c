//
//
//
//

#include "raylib.h"
#include <string.h>
#include <stdlib.h>

static	Color c64color[16];  //' our colors

static int countcommas(char *in);
static int countnumbers(char *in);
static bool importspriteis8x8(int in);
static bool importspriteis16x16(int in);
static bool importspriteis32x32(int in);
static void readtempsprite(int w,int h,char *in);
static void create8x8sprite();
static void inic64colors();

static RenderTexture2D target;

static int tempsprite[8][8] = {0};    
    
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib example.");
    inic64colors();
    target = LoadRenderTexture(32,32);
    BeginTextureMode(target);    
    ClearBackground(BLACK); // Make the entire Sprite Transparent.
    EndTextureMode();  
    
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------
  
    // Create a char to read the clipboard with.
    char clipBoardTxt[1024];
    // Create a pointer to the clipboardtxt char
    const char *clipPointer = clipBoardTxt;
    // Read the contents of the clipboard into the clipboardtxt pointer
    clipPointer = GetClipboardText();
    // create a char 
    char work[1024];
    // copy the contents of the char that has a pointer containing the clipboard txt into this work char
    strcpy(work,clipPointer);
    
    // count the comma's and the numbers.
    int numCommas = countcommas(work);
    int numNumbers = countnumbers(work);

    static bool banana=false;
    if(importspriteis8x8(numNumbers)){
        readtempsprite(8,8,work);
        create8x8sprite();
        banana=true;
    }
    

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
  
//            if(banana){
//                DrawText("~!!!!!!!!!!!!!",0,0,20,BLACK);
//            }
            
            DrawText(work,100,100,20,BLACK);
            DrawText(FormatText("Comma Count : %i",numCommas),100,80,20,BLACK);
            DrawText(FormatText("Number Count : %i",numNumbers),100,60,20,BLACK);

            if(importspriteis8x8(numNumbers)){
                DrawText("Sprite is 8x8",100,40,20,BLACK);
            }

            for(int y=0;y<8;y++){
                for(int x=0;x<8;x++){
                    DrawText(FormatText("%i",tempsprite[y][x]),x*30,y*30,20,BLACK);
                }
            }

            char banana[20]="{1,20,3,4}";
            char zet[10]="";
            zet[0]=banana[3];
            zet[1]=banana[4];
            int zet2=0;
            zet2 = atoi(zet);
            zet2++;
            DrawText(FormatText("%i",zet2),400,0,20,BLACK);

          
            DrawTexture(target.texture,100,100, WHITE);
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

void create8x8sprite(){
    
    BeginTextureMode(target);    
    ClearBackground(BLUE); // Make the entire Sprite Transparent.
    EndTextureMode();     
    // Draw something on it.
    for (int y=0;y<8;y++)
    {
        for (int x=0;x<8; x++)
        {            
            BeginTextureMode(target);    
            DrawRectangle(x*4,y*4,4,4,c64color[tempsprite[y][x]]);
            EndTextureMode(); 
        }
    }

}

void readtempsprite(int w,int h,char *in){
    int cnt=0;
    int x=0;
    int y=0;
    bool insidearray=false;
    int cnt2=0;
    for(int i=0;in[i];i++){
        
        if(cnt2<2 && in[i]=='{')cnt2++;
        if(cnt2>=1)insidearray=true;
            
        if(insidearray && in[i] >= '0' && in[i] <='9'){
        
            int z=0;
            int num=0;
            char charnum[14];
            while(in[i+z]!=','){                
                charnum[z]=in[i+z];
                z++;
                if(z>=3)break;//no longer numbers than 3
            }
            i+=z;
            num = atoi(charnum);
            
            cnt++;
            if(cnt>1){
                y++;
                if(y>=h){
                    y=0;
                    x++;
                }                
            }
            
            tempsprite[x][y] = num;//in[i]-'0';
        }
    }
}

int countcommas(char *in){
    int out=0;
    for(int i=0;in[i];i++){
        if(in[i] == ',')out++;
    }
    return out;
}

int countnumbers(char *in){
    int out=0;
    for(int i=0;in[i];i++){
        if(in[i] >= '0' && in[i] <= '9')out++;        
    }
    return out;
}

bool importspriteis8x8(int in){
    if(in-2 == 8*8)return true;
    return false;
}
bool importspriteis16x16(int in){
    if(in-2 == 16*16)return true;
    return false;
}
bool importspriteis32x32(int in){
    if(in-2 == 32*32)return true;
    return false;
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
