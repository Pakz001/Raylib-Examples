//
// STILL A WORK IN PROGRESS
//
//


#include "raylib.h"

float tileWidth = 32;
float tileHeight = 32;

static	Color db32color[32];// ' our colors	

static RenderTexture2D spritespider; 
static RenderTexture2D spriteplayer; 


static void inidb32colors(void);		
static void inisprites(void);

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib example.");
    spritespider=LoadRenderTexture(32,32); 
    spriteplayer=LoadRenderTexture(32,32); 
    inidb32colors(); 
    inisprites();
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
            DrawTexturePro(spritespider.texture,    (Rectangle){0,0,spritespider.texture.width,spritespider.texture.height},
                                                    (Rectangle){320,240,
                                                    tileWidth,tileHeight},
                                                    (Vector2){0,0},0,WHITE);                  
            DrawTexturePro(spriteplayer.texture,    (Rectangle){0,0,spriteplayer.texture.width,spriteplayer.texture.height},
                                                    (Rectangle){352,240,
                                                    tileWidth,tileHeight},
                                                    (Vector2){0,0},0,WHITE);                  

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadRenderTexture(spritespider); 
    UnloadRenderTexture(spriteplayer); 
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;


}


void inisprites(){
    //
    // The sprite data goes here.
int sprite_spider[8][8] = {
{0,21,21,0,21,21,0,21},
{0,21,0,21,21,0,21,21},
{21,0,0,0,0,21,0,21},
{21,0,0,0,8,0,21,0},
{21,0,0,0,8,0,21,0},
{21,0,0,0,0,21,0,21},
{0,21,0,21,21,0,21,21},
{0,21,21,0,21,21,0,21}};

int sprite_player[8][8] = {
{21,21,21,21,6,21,21,21},
{21,21,21,18,21,21,21,21},
{21,21,20,21,21,2,3,0},
{0,6,19,18,3,21,21,21},
{0,6,19,18,3,21,21,21},
{21,21,20,21,21,2,3,0},
{21,21,21,18,21,21,21,21},
{21,21,21,21,6,21,21,21}};

    
    BeginTextureMode(spritespider);    
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
                BeginTextureMode(spritespider);    
                if(sprite_spider[x][7-y]!=21)DrawRectangle(x*4,y*4,4,4,db32color[sprite_spider[x][7-y]]);
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
