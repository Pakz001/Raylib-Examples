
#include "raylib.h"

static	Color c64color[16];  //' our colors
static	Color db32color[32];// ' our colors	

static void inic64colors(void);
static void inidb32colors(void);


int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib example.");

    inic64colors();
    inidb32colors();
 
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
            
            int num=0;
            DrawText("C64 Palette..",200,180,20,GRAY);
            for( int x=0 ; x<screenWidth ; x+=(screenWidth/16) )
            {
                DrawRectangle(x,200,screenWidth/16,32,c64color[num]);
                num++;
            }
            num=0;
            DrawText("Db32 Palette..",200,244,20,GRAY);
            for( int x=0 ; x<screenWidth ; x+=(screenWidth/32) )
            {
                DrawRectangle(x,264,screenWidth/32,32,db32color[num]);
                num++;
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
