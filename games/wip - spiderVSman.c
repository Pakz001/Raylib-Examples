//
// STILL A WORK IN PROGRESS
//
//


#include "raylib.h"

typedef struct spider{
    Vector2 position;
    float width;
    float height;    
}spider;


typedef struct player{
    Vector2 position;
    float width;
    float height;    
}player;

// This is out collision map.
static bool colmap[10][20] = {false};
// This is our tile map.
static int map[10][20] = { {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
                    {1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,1,1,1,1},
                    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1},
                    {1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,1,1,1,1},
                    {1,1,1,1,1,1,1,1,0,1,1,1,1,0,0,1,1,1,1,1},
                    {1,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,1},
                    {1,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,1},
                    {1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,1},
                    {1,0,0,0,0,0,0,1,0,0,0,1,1,1,1,0,0,0,0,1},
                    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}};   

static float tileWidth;
static float tileHeight;
static int mapWidth = 20;//width and height of map dimensions
static int mapHeight = 10;

static player myplayer = {0};
static player myspider = {0};


static	Color db32color[32];// ' our colors	

static RenderTexture2D spritespider1; 
static RenderTexture2D spritespider2; 
static RenderTexture2D spriteplayer; 


static void inidb32colors(void);		
static void inisprites(void);
static bool playertilecollide(int offsetx,int offsety);
static bool rectsoverlap(int x1,int y1,int w1,int h1,int x2,int y2,int w2,int h2);


int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib example.");
    mapWidth = 20;
    mapHeight = 10;
    tileWidth = (float)screenWidth/mapWidth;
    tileHeight = (float)screenHeight/mapHeight;
    spritespider1=LoadRenderTexture(tileWidth,tileHeight); 
    spritespider2=LoadRenderTexture(tileWidth,tileHeight); 
    spriteplayer=LoadRenderTexture(tileWidth,tileHeight); 
    inidb32colors(); 
    inisprites();
    
    // Our player setup
    myplayer.position = (Vector2){352,240};
    myplayer.width = tileWidth/1.5;
    myplayer.height = tileHeight/1.5;
    myspider.width = tileWidth/1.5;
    myspider.height = tileHeight/1.5;
    myspider.position = (Vector2){320+tileWidth/2,240};
    
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------
    
    int sa=0;
    int frame=1;
    int time=0;
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        
        sa+=1;
        time++;
        if(time>20){
            frame++;
            time=0;
        }
        if(frame>2)frame=1;
        if(sa>360)sa=0;
        
        if(IsKeyDown(KEY_RIGHT)&& myplayer.position.x+1<(mapWidth-1)*tileWidth){
            if(playertilecollide(1,0)==false){
                myplayer.position.x+=1;
            }
        }
        if(IsKeyDown(KEY_LEFT) && myplayer.position.x-1>-1){
            if(playertilecollide(-1,0)==false){
                myplayer.position.x-=1;
            }
        }
        if(IsKeyDown(KEY_UP)&& myplayer.position.y-1>-1){
            if(playertilecollide(0,-1)==false){
                myplayer.position.y-=1;
            }
        }
        if(IsKeyDown(KEY_DOWN) && myplayer.position.y+1<(mapHeight-1)*tileHeight){
            if(playertilecollide(0,1)==false){
                myplayer.position.y+=1;
            }
        }        
        //----------------------------------------------------------------------------------
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground((Color){60,120,60,255});

            // Draw our tilemap.
            for(int y=0;y<10;y++){
                for(int x=0;x<20;x++){
                    if(map[y][x]==1){
                        DrawRectangle(x*tileWidth,y*tileHeight,tileWidth,tileHeight,LIGHTGRAY);
                    }
                }
            }            
            if(frame==1){
            DrawTexturePro(spritespider1.texture,    (Rectangle){0,0,spritespider1.texture.width,spritespider1.texture.height},
                                                    (Rectangle){myspider.position.x,myspider.position.y,
                                                    myspider.width,myspider.height},
                                                    (Vector2){myspider.width/2,myspider.height/1.5},sa,WHITE);                  
            }
            if(frame==2){
            DrawTexturePro(spritespider2.texture,    (Rectangle){0,0,spritespider2.texture.width,spritespider2.texture.height},
                                                    (Rectangle){myspider.position.x,myspider.position.y,
                                                    myspider.width,myspider.height},
                                                    (Vector2){myspider.width/2,myspider.height/1.5},sa,WHITE);                  
            }

            DrawTexturePro(spriteplayer.texture,    (Rectangle){0,0,spriteplayer.texture.width,spriteplayer.texture.height},
                                                    (Rectangle){myplayer.position.x,myplayer.position.y,
                                                    myplayer.width,myplayer.height},
                                                    (Vector2){0,0},0,WHITE);                  

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadRenderTexture(spritespider1); 
    UnloadRenderTexture(spritespider2); 
    UnloadRenderTexture(spriteplayer); 
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;


}


void inisprites(){
    //
    // The sprite data goes here.
int sprite_spider1[8][8] = {
{0,21,21,0,21,21,0,21},
{0,21,0,21,21,0,21,21},
{21,0,0,0,0,21,0,21},
{21,0,0,0,8,0,21,0},
{21,0,0,0,8,0,21,0},
{21,0,0,0,0,21,0,21},
{0,21,0,21,21,0,21,21},
{0,21,21,0,21,21,0,21}};

int sprite_spider2[8][8] = {
{21,0,21,21,0,21,21,21},
{21,21,0,21,0,21,21,21},
{0,0,0,0,0,21,0,0},
{21,0,0,0,8,0,21,21},
{21,0,0,0,8,0,21,21},
{0,0,0,0,0,21,0,0},
{21,21,0,21,0,21,21,21},
{21,0,21,21,0,21,21,21}};


int sprite_player[8][8] = {
{21,21,21,21,6,21,21,21},
{21,21,21,18,21,21,21,21},
{21,21,20,21,21,2,3,0},
{0,6,19,18,3,21,21,21},
{0,6,19,18,3,21,21,21},
{21,21,20,21,21,2,3,0},
{21,21,21,18,21,21,21,21},
{21,21,21,21,6,21,21,21}};

    
    BeginTextureMode(spritespider1);    
    ClearBackground(BLANK); // Make the entire Sprite Transparent.
    EndTextureMode();
    BeginTextureMode(spritespider2);    
    ClearBackground(BLANK); // Make the entire Sprite Transparent.
    EndTextureMode();
    BeginTextureMode(spriteplayer);    
    ClearBackground(BLANK); // Make the entire Sprite Transparent.
    EndTextureMode();
    
    //db32color[0] = (Color){0,0,0,0};
    // Draw something on it.
    for (int y=0;y<8;y++)
    {
        for (int x=0;x<8; x++)
        {            
                BeginTextureMode(spritespider1);    
                if(sprite_spider1[x][7-y]!=21)DrawRectangle(x*4,y*4,4,4,db32color[sprite_spider1[x][7-y]]);
                EndTextureMode(); 
                BeginTextureMode(spritespider2);    
                if(sprite_spider2[x][7-y]!=21)DrawRectangle(x*4,y*4,4,4,db32color[sprite_spider2[x][7-y]]);
                EndTextureMode(); 

                BeginTextureMode(spriteplayer);    
                if(sprite_player[x][7-y]!=21)DrawRectangle(x*4,y*4,4,4,db32color[sprite_player[x][7-y]]);
                EndTextureMode(); 


        }
    }

}

void inidb32colors(){		
    db32color[0 ] =  (Color){0      ,0      ,0      ,255};
    db32color[1 ] =  (Color){34     ,32     ,52     ,255};
    db32color[2 ] =  (Color){69     ,40     ,60     ,255};
    db32color[3 ] =  (Color){102    ,57     ,49     ,255};
    db32color[4 ] =  (Color){143    ,86     ,59     ,255};
    db32color[5 ] =  (Color){223    ,113    ,38     ,255};
    db32color[6 ] =  (Color){217    ,160    ,102    ,255};
    db32color[7 ] =  (Color){238    ,195    ,154    ,255};
    db32color[8 ] =  (Color){251    ,242    ,54     ,255};
    db32color[9 ] =  (Color){153    ,229    ,80     ,255};
    db32color[10] =  (Color){106    ,190    ,48     ,255};
    db32color[11] =  (Color){55     ,148    ,110    ,255};
    db32color[12] =  (Color){75     ,105    ,47     ,255};
    db32color[13] =  (Color){82     ,75     ,36     ,255};
    db32color[14] =  (Color){50     ,60     ,57     ,255};
    db32color[15] =  (Color){63     ,63     ,116    ,255};
    db32color[16] =  (Color){48     ,96     ,130    ,255};
    db32color[17] =  (Color){91     ,110    ,225    ,255};
    db32color[18] =  (Color){99     ,155    ,225    ,255};
    db32color[19] =  (Color){95     ,205    ,228    ,255};
    db32color[20] =  (Color){203    ,219    ,252    ,255};
    db32color[21] =  (Color){255    ,255    ,255    ,255};
    db32color[22] =  (Color){155    ,173    ,183    ,255};
    db32color[23] =  (Color){132    ,126    ,135    ,255};
    db32color[24] =  (Color){105    ,106    ,106    ,255};
    db32color[25] =  (Color){89     ,86     ,82     ,255};
    db32color[26] =  (Color){118    ,66     ,138    ,255};
    db32color[27] =  (Color){172    ,50     ,50     ,255};
    db32color[28] =  (Color){217    ,87     ,99     ,255};
    db32color[29] =  (Color){215    ,123    ,186    ,255};
    db32color[30] =  (Color){143    ,151    ,74     ,255};
    db32color[31] =  (Color){138    ,111    ,48     ,255};
}

//Unit collide with solid blocks true/false
bool playertilecollide(int offsetx,int offsety){
    int cx = (myplayer.position.x+offsetx)/tileWidth;
    int cy = (myplayer.position.y+offsety)/tileHeight;
    for(int y2=cy-2; y2<cy+3;y2++){//Note that the - and + are to be set differently with differently sized players
    for(int x2=cx-2; x2<cx+3;x2++){
        if(x2>=0 && x2<mapWidth && y2>=0 && y2<mapHeight){
            if(map[y2][x2] == 1){
                int x3 = (x2)*tileWidth;
                int y3 = (y2)*tileHeight;
                if(rectsoverlap(myplayer.position.x+offsetx,myplayer.position.y+8+offsety,myplayer.width-5,myplayer.height-8,x3,y3,tileWidth,tileHeight)){
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
