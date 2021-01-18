//
// Morrowind Player Inventory Remake Project...
//
// Work in progress


#include "raylib.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define MAPWIDTH 100
#define MAPHEIGHT 100

int screenWidth = 800;
int screenHeight = 450;



static Color texmap[MAPWIDTH+1][MAPHEIGHT+1];


void makebasetexture();
void texmap_linestroke(float x1,float y1,float angle,int len,Color col);
float float_rand( float min, float max );
void texmap_holdblur();
void texmap_mossy(Color col);
void texmap_maketilable();
    
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    screenWidth = 800;
    screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib example.");
 
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------


    RenderTexture2D target = LoadRenderTexture(800, 450);
    BeginTextureMode(target);    
    ClearBackground(BLANK); // Make the entire Sprite Transparent.
    EndTextureMode();   
    RenderTexture2D tex = LoadRenderTexture(MAPWIDTH, MAPHEIGHT);
    BeginTextureMode(tex);    
    ClearBackground(BLANK); // Make the entire Sprite Transparent.
    EndTextureMode();  
    
    RenderTexture2D horline = LoadRenderTexture(MAPWIDTH, 4);
    BeginTextureMode(horline);    
    ClearBackground(BLANK); // Make the entire Sprite Transparent.
    EndTextureMode();   
    RenderTexture2D verline = LoadRenderTexture(4, MAPHEIGHT);
    BeginTextureMode(verline);    
    ClearBackground(BLANK); // Make the entire Sprite Transparent.
    EndTextureMode();   


    for(int y=0;y<MAPHEIGHT;y++){
    for(int x=0;x<MAPWIDTH;x++){
        texmap[x][y] = (Color){255,255,255,255};
    }}
    makebasetexture();
    
    // horizontal line texture
    BeginTextureMode(horline);    
    for(int y=0;y<4;y++){
    for(int x=0;x<MAPWIDTH;x++){
        Color l = texmap[x][y+32];
        if(y==3 || y==0){
            l.r/=2.0f;
            l.g/=2.0f;
            l.b/=2.0f;
        }
        DrawRectangle(x,y,1,1,l);
        
    }}
    EndTextureMode();  
    // vertical line texture
    BeginTextureMode(verline);    
    for(int y=0;y<MAPHEIGHT;y++){
    for(int x=0;x<4;x++){
        Color l = texmap[x+32][y];
        if(x==3 || x==0){
            l.r/=2.0f;
            l.g/=2.0f;
            l.b/=2.0f;
        }
        DrawRectangle(x,y,1,1,l);
        
    }}
    EndTextureMode();  


    //main texture
    BeginTextureMode(tex);    
    for(int y=0;y<MAPHEIGHT;y++){
    for(int x=0;x<MAPWIDTH;x++){
        DrawRectangle(x,y,1,1,texmap[x][y]);       
    }}
    EndTextureMode();  

    BeginTextureMode(target);           
//    DrawRectangle(0,0,screenWidth,screenHeight,(Color){0,0,0,255});
    DrawRectangle(32,32,screenWidth-64,screenHeight-100,(Color){0,0,0,255});
    for(int y1=50;y1<screenHeight-100;y1+=64){
    for(int x1=50;x1<screenWidth-50;x1+=64){
        DrawRectangle(x1+8,y1+8,64-16,64-16,(Color){0,0,0,255});
        for (int i=0; i<64;i++){
            int x = x1+32;
            int y = y1+32;
            Vector2 point[3];
            point[0] = (Vector2){x+GetRandomValue(-16,16),y+GetRandomValue(-16,16)};
            point[1] = (Vector2){x+GetRandomValue(-16,16),y+GetRandomValue(-16,16)};
            point[2] = (Vector2){x+GetRandomValue(-16,16),y+GetRandomValue(-16,16)};    
            int cr = GetRandomValue(0,55);
            int cg = cr;
            int cb = GetRandomValue(0,55);
            if(GetRandomValue(0,10)<5){
                cr=155;
                cb=50;
            }
            
            if(GetRandomValue(0,10)<5){
                if(GetRandomValue(0,10)<5){
                    cg=155;
                }else{
                    cg=0;
                }
            }
            
            if(GetRandomValue(0,50)<5){
                cr=GetRandomValue(200,255);
                cg=GetRandomValue(200,255);
                cb=GetRandomValue(200,255);
                
            }
            DrawTriangle(point[0],point[1],point[2], (Color){cr,cg,cb,255-GetRandomValue(10,255)});
        }
    }
    }
    EndTextureMode();


    
    
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
            


            for(int y=0;y<screenHeight;y+=50){
            for(int x=0;x<screenWidth;x+=50){
            DrawTexturePro(         tex.texture,
                                    (Rectangle){0,0,tex.texture.width,-tex.texture.height},
                                    (Rectangle){x,y,50,50},
                                    (Vector2){0,0},0,WHITE);




            }}    
            DrawTexturePro(         target.texture,
                                    (Rectangle){0,0,target.texture.width,-target.texture.height},
                                    (Rectangle){0,0,screenWidth,screenHeight},
                                    (Vector2){0,0},0,WHITE);


            // LINES
            DrawTexturePro(         horline.texture,
                                    //(Rectangle){0,0,horline.texture.width,-horline.texture.height},
                                    (Rectangle){0,0,200,-horline.texture.height},
                                    (Rectangle){50,50,200,4},
                                    (Vector2){0,0},0,WHITE);
            DrawTexturePro(         verline.texture,
                                    //(Rectangle){0,0,horline.texture.width,-horline.texture.height},
                                    (Rectangle){0,0,verline.texture.width,-200,},
                                    (Rectangle){50,54,4,200},
                                    (Vector2){0,0},0,WHITE);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadRenderTexture(target);    // Unload render texture  
    UnloadRenderTexture(tex);    // Unload render texture  
    UnloadRenderTexture(horline);    // Unload render texture  
    UnloadRenderTexture(verline);    // Unload render texture  
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;


}

void makebasetexture(){
    for(int y=0;y<MAPHEIGHT;y++){
    for(int x=0;x<MAPWIDTH;x++){
        texmap[y][x] = (Color){136,109,68,255};
    }}

    texmap_mossy((Color){50,20,10,255});

    //
    // Lines
    for(int i=0;i<80;i++){
        int x=GetRandomValue(-100,100);
        int y=GetRandomValue(-100,100);
        int len=GetRandomValue(40,80);
        texmap_linestroke(x,y,PI*2.0f/8,len,(Color){136/2,109/2,68/2,255});        
    }


    texmap_maketilable();
    texmap_holdblur();
    texmap_holdblur();

}


void texmap_maketilable(){
    Color tempmap[MAPWIDTH][MAPHEIGHT];
    for(int y=0;y<MAPHEIGHT;y++){
    for(int x=0;x<MAPWIDTH;x++){
        tempmap[x][y] = texmap[x][y];
    }}

    // left side
    for(int x1=0;x1<5;x1++){
    for(int y1=0;y1<MAPHEIGHT;y1++){
        int x2 = x1+MAPWIDTH-5;
        int y2 = y1;
        Color col3 = (Color){   texmap[x1][y1].r/(2.0)+texmap[x2][y2].r/(2.0),
                                texmap[x1][y1].g/(2.0)+texmap[x2][y2].g/(2.0),
                                texmap[x1][y1].b/(2.0)+texmap[x2][y2].b/(2.0),
                                255
                                };
        tempmap[x1][y1] = col3;
    }
    }
    // right side
    
    for(int x1=0;x1<5;x1++){
    for(int y1=0;y1<MAPHEIGHT;y1++){
        int x2 = (MAPWIDTH-1)-x1;        
        int y2 = y1;
        Color col3 = (Color){   texmap[x1][y1].r/2.0f+texmap[x2][y2].r/2.0f,
                                texmap[x1][y1].g/2.0f+texmap[x2][y2].g/2.0f,
                                texmap[x1][y1].b/2.0f+texmap[x2][y2].b/2.0f,
                                255
                                };
        tempmap[x2][y2] = col3;
    }}
    // top side
    for(int y1=0;y1<5;y1++){
    for(int x1=0;x1<MAPWIDTH;x1++){
        int y2 = y1+MAPHEIGHT-5;
        int x2 = x1;
        Color col3 = (Color){   texmap[x1][y1].r/2.0f+texmap[x2][y2].r/2.0f,
                                texmap[x1][y1].g/2.0f+texmap[x2][y2].g/2.0f,
                                texmap[x1][y1].b/2.0f+texmap[x2][y2].b/2.0f,
                                255
                                };
        tempmap[x1][y1] = col3;
    }}
    // bottom
    
    for(int y1=0;y1<5;y1++){
    for(int x1=0;x1<MAPWIDTH;x1++){
        int y2 = (MAPHEIGHT-1)-y1;        
        int x2 = x1;
        Color col3 = (Color){   texmap[x1][y1].r/2.0f+texmap[x2][y2].r/2.0f,
                                texmap[x1][y1].g/2.0f+texmap[x2][y2].g/2.0f,
                                texmap[x1][y1].b/2.0f+texmap[x2][y2].b/2.0f,
                                255
                                };
        tempmap[x2][y2] = col3;
    }}


    for(int y=0;y<MAPHEIGHT;y++){
    for(int x=0;x<MAPWIDTH;x++){
        texmap[x][y] = tempmap[x][y];
    }}
}

void texmap_linestroke(float x1,float y1,float angle,int len,Color col){
    bool diverse=false;
    int diversecnt=0;
    float diverseinc;
    float angle2 = angle;

    for(int i=0;i<len;i++){
        x1+=cos(angle2)*1;
        y1+=sin(angle2)*1;
        if(GetRandomValue(0,20)<2){
            diverse=true;
            diversecnt=10;
            diverseinc = float_rand(0,.1);            
        }
        if(diversecnt>5){
            diversecnt--;
            angle2 += diverseinc;
            if(angle2>PI*2.0f)angle=0;
            if(angle2<0)angle=PI*2.0f;            
        }
        if(diversecnt<6 && diversecnt>0){
            diversecnt--;
            angle2 += diverseinc;
            if(angle2>PI*2.0f)angle=0;
            if(angle2<0)angle=PI*2.0f;            
        }
        if(diversecnt==0)angle2=angle;


        for(int y2=-1;y2<0;y2++){
        for(int x2=-1;x2<0;x2++){
            if(GetRandomValue(0,10)<2)continue;
            int x3 = x1+x2-3;
            int y3 = y1+y2-2;
            if(x3<0 || x3>=MAPWIDTH || y3<0 || y3>=MAPHEIGHT)continue;
            Color col2 = col;
            col2.r = col.r*2.5;
            col2.g = col.g*2.5;
            col2.b = col.b*2.5;
            texmap[(int)x3][(int)y3] = col2; 
        }}

        for(int y2=-1;y2<2;y2++){
        for(int x2=-1;x2<2;x2++){
            if(GetRandomValue(0,10)<2)continue;
            int x3 = x1+x2;
            int y3 = y1+y2;
            if(x3<0 || x3>=MAPWIDTH || y3<0 || y3>=MAPHEIGHT)continue;
            texmap[(int)x3][(int)y3] = col; 
        }}
    }
}

void texmap_holdblur(){
    Color tempmap[MAPWIDTH][MAPHEIGHT] = {0};
    for(int y=0;y<MAPHEIGHT;y++){
    for(int x=0;x<MAPWIDTH;x++){
        tempmap[x][y]=texmap[x][y];
    }}    
    for(int i=0;i<MAPWIDTH*MAPHEIGHT*2;i++){
        int x=GetRandomValue(0,MAPWIDTH);
        int y=GetRandomValue(0,MAPHEIGHT);
        float div=0;
        Vector3 ac = (Vector3){0,0,0};
        for(int y1=y-1;y1<y+1;y1++){
        for(int x1=x-1;x1<x+1;x1++){
            if(x1<0 || y1>=MAPHEIGHT || x1>=MAPWIDTH || y1<0){
            continue;
            }
            ac.x += texmap[x1][y1].r;
            ac.y += texmap[x1][y1].g;
            ac.z += texmap[x1][y1].b;
            div+=1;            
        }}
        ac.x /= div;
        ac.y /= div;
        ac.z /= div;
        texmap[x][y].r = ac.x;
        texmap[x][y].g = ac.y;
        texmap[x][y].b = ac.z;
        
    }
    for(int y=0;y<MAPHEIGHT;y++){
    for(int x=0;x<MAPWIDTH;x++){
        if(texmap[x][y].r<80 && texmap[x][y].g<80 && texmap[x][y].b<80){
            texmap[x][y]=tempmap[x][y];
        }
    }}    

}

void texmap_mossy(Color col){
    
    int tempmap[MAPWIDTH+1][MAPHEIGHT+1];
    for(int y=0;y<MAPHEIGHT;y++){
    for(int x=0;x<MAPWIDTH;x++){
        tempmap[x][y]=0;
    }}
    
    for(int i=0;i<MAPWIDTH*6;i++){
        int x=GetRandomValue(0,MAPWIDTH-1);
        int y=GetRandomValue(0,MAPHEIGHT-1);
        tempmap[x][y]=1;
    }

    for(int i=0;i<(MAPWIDTH*MAPHEIGHT)*2;i++){
        int x=GetRandomValue(0,MAPWIDTH-1);
        int y=GetRandomValue(0,MAPHEIGHT-1);
        if(tempmap[x][y]==1){
            for(int y1=y-1;y1<y+1;y1++){
            for(int x1=x-1;x1<x+1;x1++){
                if(x1<0 || y1<0 || x1>=MAPWIDTH || y1>=MAPHEIGHT)continue;
                if(GetRandomValue(0,10)<2)continue;
                tempmap[x1][y1]=1;
            }}                
        }
    }
    for(int y=0;y<MAPHEIGHT;y++){
    for(int x=0;x<MAPWIDTH;x++){
        if(tempmap[x][y]==1){
            //texmap[x][y] = (Color){Clamp(texmap[x][y].r*2,0,255),Clamp(texmap[x][y].r*2,0,255),Clamp(texmap[x][y].r*2,0,255)};
            Color col2 = texmap[x][y];
            col2.r = col.r*3.1f;
            col2.g = col.g*3.1f;
            col2.b = col.b*3.1f;
            
            texmap[x][y] = col2;
        }
       
    }}
 
}

float float_rand( float min, float max ){
    float scale = rand() / (float) RAND_MAX; /* [0, 1.0] */
    return min + scale * ( max - min );      /* [min, max] */
}
