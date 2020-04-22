

// WORK IN PROGRESS CONVERSION....
// WORK IN PROGRESS CONVERSION....
//
// Added - Press p to paste 8x8 sprite back into the editor.

// Conversion from the Monkey2 version that I wrote.
//
// Todo : Add isometric/hexagon map edit/view, Fix update preview and spritelib), Floodfill(more testing),  ovals!(find solution for layout), testing. 

#include "raylib.h"
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// This one is for getting the sprite from the clipboard


static int tempmap[32][32] = {0};

	// start setup
static int weasel=0;
static	int startsetuppalettemode;  // '0=c64 1=db32
static	bool startsetupdone;
static	int startsetupx;
static	int startsetupy;
static	int startsetupwidth;
static	int startsetupheight;
static	RenderTexture2D startsetupim[5];    //5=numstartsetup
//Field startsetupcan:Canvas[]
static	int startsetupselected;
static	int startsetup8x8id;
static	int startsetup16x16id = 1;
static	int startsetup32x32id = 2;
static	int startsetupc64id = 3;
static	int startsetupdb32id = 4;
static	int numstartsetup = 5;
	//'
	//'tool view
static	int toolx;
static  int tooly;
static	int toolwidth;
static  int toolheight;
static	RenderTexture2D toolim[16]; //16 = numtools
	//Field toolcan:Canvas[]
static	bool toolgridtoggle = true;
static	int toolselected = 0;
static	int toolpencilid = 0;
static	int tooleraserid = 1;
static	int toolfillid = 2;
static	int toollineid = 3;
static	int toolselectionid = 4;
static	int toolcopyid = 5;
static	int toolpasteid = 6;
static	int toolcutid = 7;
static	int toolflipverticalid = 8;
static	int toolfliphorizontalid = 9;
static	int toolcolorpickerid = 10;
static	int toolgridid = 11;
static	int toolfilledrectid = 12;
static	int tooloutlinerectid = 13;
static	int toolfilledcircleid = 14;
static	int tooloutlinecircleid = 15;
static	int numtools = 16;
static	int delay; 
static	int delaydefault = 20;
	//'
	//' sprite library
static	int spritelibx;
static    int spriteliby;
static	int spritelibwidth;
static    int spritelibheight;
static	int numspritelib;
static RenderTexture2D spritelibim[80*4]; // 80*4 = numspritelib
	//Field spritelibcan:Canvas[]
static	int spritelibmap[80*4][32][32]; // numspritelib,
static	int spritelibselected = 0;
static	float spritelibscale;
	
	//'
	//'preview
static	RenderTexture2D previewim;
	//Field previewcan:Canvas
static	int previewx;
static  int previewy;
static	int previewwidth;
static  int previewheight;
static	int previewcellwidth;
static  int previewcellheight;
	// tile map view
static	int tilemapx;
static  int tilemapy;
static	int tilemapwidth;
static  int tilemapheight;
static	int tilemaptileshorizontal;
static  int tilemaptilesvertical;
static	int tilemaptilesscreenhorizontal=100;
static	int tilemaptilesscreenvertical=100;
static	int tilemap[100][100]={0};
	//'
	//'sprite view
static int map[32][32]; // 8 = edit canvas width
static	int canvasx;
static  int canvasy; //'canvas x and y position on the scrern
static	float canvaswidth=256;
static  float canvasheight=256; //'width and height of our canvas
static	float gridwidth;
static  float gridheight;//	 ' grids width and height
static	int spritewidth;
static  int spriteheight;// ' our main sprite width and height
	//' line tool fields
static	bool linepressed=false;
static	bool lineactive=false;
static	int linestartx;
static  int linestarty;
static	int lineendx;
static  int lineendy;	
	//' Selection fields
static	bool selectionpressed = false ;
static	bool selectionactive = false;
static	int selectionstartx;
static  int selectionstarty;
static	int selectionendx;
static  int selectionendy;
static	int selectionbuffer[32][32]; //'our copy paste buffer
static	int selectionbufferstartx;
static  int selectionbufferstarty;
static	int selectionbufferendx;
static  int selectionbufferendy;
static	bool selectionnegativeswitchx = false;// ' switch if negative selection
static	bool selectionnegativeswitchy = false;
	//' filled/outlined rectsangles and circles fields
static	bool bcselectionpressed = false; 
static	bool bcselectionactive = false;
static	int bcselectionstartx;
static  int bcselectionstarty;
static	int bcselectionendx;
static  int bcselectionendy;
static	int bcselectionbuffer[32][32];// 'our copy paste buffer
static	int bcselectionbufferstartx;
static  int bcselectionbufferstarty;
static	int bcselectionbufferendx;
static  int bcselectionbufferendy;
static	bool bcselectionnegativeswitchx = false; // ' switch if negative selection
static	bool bcselectionnegativeswitchy = false;
	//'
	//' palette	
static	Color c64color[16];  //' our colors
static	Color db32color[32];// ' our colors	
static	int paletteselected;// ' our selected color from palette
static  int paletteundermouse; // for floodfill
static	int paletteeraser;
static	int palettex;
static  int palettey; //'screen x and y
static	float palettewidth;
static  float paletteheight;// ' our palette screen w and h
static	float palettecellwidth;
static  float palettecellheight; //'cell width and height of color
static	int numpalette;// 'number of colors (holds the amount of the selected palette.)
	
	//'
	//' Top Bar
static	int topbarx;
static  int topbary;
static	int topbarwidth;
static  int topbarheight;
static	Color topbarcolor;
static	RenderTexture2D topbarim[3];
//Field topbarcan:Canvas[]
static	int topbarspriteeditid = 0;	
static	int topbarmapeditid = 1;
static	int numtopbaricons = 2;
static	int topbarcurrentid = 0;

	//'
	//' Middle bar
static	int middlebarx;
static  int middlebary;
static	int middlebarwidth;
static  int middlebarheight;
static	Color middlebarcolor;
static	RenderTexture2D middlebarim[4]; //nummiddlebaricons
	//Field middlebarcan:Canvas[]
static	int middlebar0id = 0;
static	int middlebar1id = 1;
static	int middlebar2id = 2;
static	int middlebar3id = 3;
static	int nummiddlebaricons = 4;
static	int middlebarcurrentid = 0;
	//'
	//' Bottom Bar
static	int bottombarx;
static  int bottombary;
static	int bottombarwidth;
static  int bottombarheight;
static	Color bottombarcolor;

static void inic64colors(void);
static void inidb32colors(void);
static bool rectsoverlap(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2);
static bool circlerectcollide(int cx,int cy,int cr,int rx,int ry,int rw,int rh);
static float Clamp(float value, float min, float max);
static void startsetupsetup(void);
static void startsetupview(void);
static void bottombarview(void);
static void middlebarview();
static void setupmiddlebar(void);
static void setuptopbar(void);
static void topbarview(void);
static void setuptoolview(void);
static void paletteview(void);
static void spritegrid(void);
static void toolview(void);
static void spritelibview(void);
static void spriteview(void);
static void updatespritelib(void);
static void updatepreview(void);
static void previewview(void);
static void spritelibcopytocanvas(void);
static void previewline(bool drawit);
static void previewselection(bool drawit);
static void tilemapview(void);
static void fillatposition(int x, int y,int oldcolor,int newcolor);
static void midptellipse(int rx, int ry, int xc, int yc);
// for the copy from clipboard
static int countcommas(char *in);
static int countnumbers(char *in);
static void copyfromclipboard(void);
static void copytoclipboard(void);
static bool importspriteis8x8(int in);
static bool importspriteis16x16(int in);
static bool importspriteis32x32(int in);
static void readtempsprite(int w,int h,char *in);


    
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 640;
    const int screenHeight = 480;

    InitWindow(screenWidth, screenHeight, "raylib example.");
    //ToggleFullscreen();

    //'start setup setup
    //' be sure to start the editor with the selection
    startsetupdone=false;
    startsetupx = 320-80;
    startsetupy = 150;
    startsetupwidth = 150;
    startsetupheight = 200-35;
    //startsetupim = New Image[numstartsetup];
    //startsetupcan = New Canvas[numstartsetup]
    for(int i=0;i<numstartsetup;i++){
        startsetupim[i] = LoadRenderTexture(32, 32);
        BeginTextureMode(startsetupim[i]);    
        ClearBackground(BLANK); // Make the entire Sprite Transparent.
        EndTextureMode();         
        //startsetupcan[i] = New Canvas(startsetupim[i])
    }
    inic64colors();
    inidb32colors();
    startsetupsetup();
    
    paletteeraser = 0;
    palettex = 640-150;
    palettey = 32;
    palettewidth = 32*4;
    paletteheight = 32*4;
    numpalette = 16;
    palettecellwidth = 32;
    palettecellheight = 32;		


    //sprite canvas setup
    canvasx = 32;
    canvasy = 28;
    //spritewidth = 16;
    //spriteheight = 16;
    //map = New Int[spritewidth,spriteheight]	
    //selectionbuffer = New Int[spritewidth,spriteheight]	
    
    canvaswidth=256;
    canvasheight=256;
    //  gridwidth = canvaswidth/spritewidth;		
    //  gridheight = canvasheight/spriteheight;





    //'Bottom bar (global)
    bottombarx = 0;
    bottombary = 480-24;
    bottombarwidth = 640;
    bottombarheight = 24;	
    bottombarcolor = (Color){ 78, 22, 9,255	};//Color.Puce


    //'
    //' Middle bar
    middlebarx = 0;
    middlebary = canvasheight+32;
    middlebarwidth = 640;
    middlebarheight = 32;
    middlebarcolor = GRAY;
    nummiddlebaricons = 4;
    //middlebarim = New Image[nummiddlebaricons]
    //middlebarcan = New Canvas[nummiddlebaricons]
    for(int i=0;i<nummiddlebaricons;i++){
        middlebarim[i] = LoadRenderTexture(32, 32);
        BeginTextureMode(middlebarim[i]);
        ClearBackground(BLANK);
        EndTextureMode();
        //middlebarcan[i] = New Canvas(middlebarim[i])
    }

    setupmiddlebar();

    //Top bar (global)
    
    topbarx = 0;
    topbary = 0;
    topbarwidth = 640;
    topbarheight = 24;
    topbarcolor = (Color){ 78, 22, 9,255};//Color.Puce
    //topbarim = New Image[3]
    //topbarcan = New Canvas[3]
    for(int i=0;i<2;i++){
        topbarim[i] = LoadRenderTexture(32, 32);
        BeginTextureMode(topbarim[i]);
        ClearBackground(BLANK);
        EndTextureMode();
        
    }
    setuptopbar();


    //'	
    //'spritelib setup
    spritelibx = 0;
    spriteliby = canvasheight+32+32;
    spritelibwidth = 640;
    spritelibheight = 128;
    numspritelib = 80*4;
    spritelibselected = 0;
    spritelibscale = 2;
    //spritelibim = New Image[numspritelib]
    //spritelibcan = New Canvas[numspritelib]		
 //   For Local i:Int=0 Until numspritelib
 //       spritelibim[i] = New Image(spritewidth*spritelibscale,spriteheight*spritelibscale)
 //       spritelibcan[i] = New Canvas(spritelibim[i])
 //       spritelibcan[i].Clear(Color.Black)
  //      spritelibcan[i].Flush()
//    Next	
    for(int i=0;i<numspritelib;i++){
        spritelibim[i] = LoadRenderTexture(32, 32);
        BeginTextureMode(spritelibim[i]);
        ClearBackground(BLANK);
        EndTextureMode();
        
    }
    


    //' tool view
    toolx = 340;
    tooly = 186-32;
    toolwidth = 32*4;
    toolheight = 32*4;
    //'numtools = 12
    //toolim = New Image[numtools]
    //toolcan = New Canvas[numtools]
    for(int i=0;i<numtools;i++){
        toolim[i] = LoadRenderTexture(32,32);
        BeginTextureMode(toolim[i]);
        ClearBackground(BLANK);
        EndTextureMode();
        //toolcan[i] = New Canvas(toolim[i])
        //toolcan[i].Clear(Color.Black)
        //toolcan[i].Flush()
    }

    setuptoolview();


    //' previewview setup
    previewx = 640-100;
    previewy = 200;
    previewcellwidth = 2;
    previewcellheight = 2;
    previewwidth = 64;
    previewheight = 64;
    previewim = LoadRenderTexture(previewwidth,previewheight);//New Image(previewwidth,previewheight)
    BeginTextureMode(previewim);
    ClearBackground(BLANK);
    EndTextureMode();

    //previewcan = New Canvas(previewim)
    updatepreview();

    //tilemap setup
    tilemapx = 0;
    tilemapy = 32;
    tilemaptileshorizontal = 100;
    tilemaptilesvertical = 100;
    tilemapwidth = 640;
    tilemapheight= 256;
    //tilemaptilesscreenhorizontal = tilemapwidth/(spritewidth*spritelibscale);
    //tilemaptilesscreenvertical = tilemapheight/(spriteheight*spritelibscale);
    //tilemap = New Int[tilemaptileshorizontal,tilemaptilesvertical]


    
    
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    int timeupd=0; // update what is being drawn into preview window and lib
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        
        
        //----------------------------------------------------------------------------------
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(DARKGRAY);

            //' We start the sprite editor with the sprite dimension
            //' resolution selection.
            if(startsetupdone==false){
                startsetupview();
            }else{
                //' Here is the map and sprite editor section
                if(topbarcurrentid == topbarspriteeditid){
                    bottombarview();
                    middlebarview();                    
                    topbarview();
                    spriteview();
                    previewline(false);
                    spritegrid();
                    previewselection(false);		
                    paletteview();
                    previewview();
                    spritelibview();
                    toolview();
                    
                    // The editing got a little slow with the update so here
                    // it now updates the preview and lib every 100 frames.
                    timeupd++;
                    if(timeupd>100){
                        updatepreview();
                        updatespritelib();
                        timeupd=0;
                    }

                }else if(topbarcurrentid == topbarmapeditid){
                    tilemapview();
                    bottombarview();
                    middlebarview();
                    topbarview();
                    spritelibview();

                }
            }
            DrawText(FormatText("%i",weasel),0,0,20,BLACK);
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    for(int i=0;i<numstartsetup;i++){
        UnloadRenderTexture(startsetupim[i]);
    }
    for(int i=0;i<numtools;i++){
        UnloadRenderTexture(toolim[i]);
    }
    for(int i=0;i<numspritelib;i++){
        UnloadRenderTexture(spritelibim[i]);
    }
    UnloadRenderTexture(previewim);
    for(int i=0;i<2;i++){
        UnloadRenderTexture(topbarim[i]);
    }
    for(int i=0;i<4;i++){
        UnloadRenderTexture(middlebarim[i]);
    }



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

void startsetupsetup(){
    // the 8x8 selection button
    int eightxeight[16][16] = {
    {12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12},
    {12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12},
    {12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12},
    {12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12},
    {12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12},
    {12,12,1,1,1,12,12,12,12,12,1,1,1,12,12,12},
    {12,12,1,12,1,12,1,12,1,12,1,12,1,12,12,12},
    {12,12,1,1,1,12,12,1,12,12,1,1,1,12,12,12},
    {12,12,1,12,1,12,1,12,1,12,1,12,1,12,12,12},
    {12,12,1,1,1,12,12,12,12,12,1,1,1,12,12,12},
    {12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12},
    {12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12},
    {12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12},
    {12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12},
    {12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12},
    {12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12}};		
    for(int y=0;y<16;y++){
    for(int x=0;x<16;x++){
        BeginTextureMode(startsetupim[startsetup8x8id]);
        //startsetupcan[startsetup8x8id].Color = c64color[eightxeight[y][x]]
        //If eightxeight[y][x] = 0 Then startsetupcan[startsetup8x8id].Alpha=0 Else startsetupcan[startsetup8x8id].Alpha=1
        //startsetupcan[startsetup8x8id].DrawRect(x*2,y*2,2,2)
        if(eightxeight[y][x]>0)DrawRectangle(x*2,y*2,2,2,c64color[eightxeight[y][x]]);
        EndTextureMode();
    }}
    //startsetupcan[startsetup8x8id].Flush()
    
    //the 16x16 selection button
    int sixteenxsixteen[16][16] = {
    {12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12},
    {12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12},
    {12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12},
    {12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12},
    {12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12},
    {1,12,1,1,1,12,12,12,12,12,1,12,1,1,1,12},
    {1,12,1,12,12,12,1,12,1,12,1,12,1,12,12,12},
    {1,12,1,1,1,12,12,1,12,12,1,12,1,1,1,12},
    {1,12,1,12,1,12,1,12,1,12,1,12,1,12,1,12},
    {1,12,1,1,1,12,12,12,12,12,1,12,1,1,1,12},
    {12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12},
    {12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12},
    {12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12},
    {12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12},
    {12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12},
    {12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12}};
    for(int y=0;y<16;y++){
    for(int x=0;x<16;x++){
        BeginTextureMode(startsetupim[startsetup16x16id]);
        //startsetupcan[startsetup16x16id].Color = c64color[sixteenxsixteen[y][x]]
        //If sixteenxsixteen[y][x] = 0 Then startsetupcan[startsetup16x16id].Alpha=0 Else startsetupcan[startsetup16x16id].Alpha=1
        if(sixteenxsixteen[15-y][x]>0)DrawRectangle(x*2,y*2,2,2,c64color[sixteenxsixteen[15-y][x]]);
        //startsetupcan[startsetup16x16id].DrawRect(x*2,y*2,2,2)
        EndTextureMode();
    }}    	
    //startsetupcan[startsetup16x16id].Flush()

    // the 32x32 selection button
    int thirtytwoxthirtytwo[16][16] = {
    {12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12},
    {12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12},
    {12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12},
    {12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12},
    {12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12},
    {1,1,12,1,1,12,12,12,12,12,1,1,12,1,1,12},
    {12,1,12,12,1,12,1,12,1,12,12,1,12,12,1,12},
    {1,1,12,1,1,12,12,1,12,12,1,1,12,1,1,12},
    {12,1,12,1,12,12,1,12,1,12,12,1,12,1,12,12},
    {1,1,12,1,1,12,12,12,12,12,1,1,12,1,1,12},
    {12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12},
    {12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12},
    {12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12},
    {12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12},
    {12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12},
    {12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12}};
    for(int y=0;y<16;y++){
    for(int x=0;x<16;x++){
        BeginTextureMode(startsetupim[startsetup32x32id]);
        //startsetupcan[startsetup32x32id].Color = c64color[thirtytwoxthirtytwo[y][x]]
        //If thirtytwoxthirtytwo[y][x] = 0 Then startsetupcan[startsetup32x32id].Alpha=0 Else startsetupcan[startsetup32x32id].Alpha=1
        //startsetupcan[startsetup32x32id].DrawRect(x*2,y*2,2,2)
        if(thirtytwoxthirtytwo[15-y][x]>0)DrawRectangle(x*2,y*2,2,2,c64color[thirtytwoxthirtytwo[15-y][x]]);

        EndTextureMode();
    }}
    //startsetupcan[startsetup32x32id].Flush()

    // the c64 palette selection button
    int c641616[16][16] = {
    {12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12},
    {12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12},
    {12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12},
    {12,12,1,1,12,12,12,1,1,12,12,1,12,12,12,12},
    {12,1,12,12,12,12,1,12,12,12,12,1,12,12,12,12},
    {12,1,12,12,12,12,1,12,12,12,12,1,12,12,1,12},
    {12,1,12,12,12,12,1,1,1,12,12,1,1,1,1,12},
    {12,1,12,12,12,12,1,12,12,1,12,12,12,12,1,12},
    {12,1,12,12,12,12,1,12,12,1,12,12,12,12,1,12},
    {12,12,1,1,12,12,12,1,1,12,12,12,12,12,1,12},
    {12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12},
    {12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12},
    {12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12},
    {12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12},
    {12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12},
    {12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12}};
    for(int y=0;y<16;y++){
    for(int x=0;x<16;x++){
        BeginTextureMode(startsetupim[startsetupc64id]);
        //startsetupcan[startsetupc64id].Color = c64color[c641616[y][x]]
        //If c641616[y][x] = 0 Then startsetupcan[startsetupc64id].Alpha=0 Else startsetupcan[startsetupc64id].Alpha=1
        //startsetupcan[startsetupc64id].DrawRect(x*2,y*2,2,2)
        if(c641616[15-y][x]>0)DrawRectangle(x*2,y*2,2,2,c64color[c641616[15-y][x]]);

        EndTextureMode();
    }}
    //startsetupcan[startsetupc64id].Flush()

    // the db32 palette selection button
    int db321616[16][16] = {
    {12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12},
    {12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12},
    {12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12},
    {12,12,1,12,1,12,12,12,1,1,12,12,1,1,1,12},
    {12,12,1,12,1,12,12,12,12,12,1,12,12,12,1,12},
    {12,12,1,12,1,12,12,12,12,12,1,12,12,12,1,12},
    {12,1,1,12,1,1,12,12,1,1,12,12,1,1,1,12},
    {1,12,1,12,1,12,1,12,12,12,1,12,1,12,12,12},
    {1,12,1,12,1,12,1,12,12,12,1,12,1,12,12,12},
    {1,1,1,12,1,1,1,12,1,1,12,12,1,1,1,12},
    {12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12},
    {12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12},
    {12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12},
    {12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12},
    {12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12},
    {12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12}};
    for(int y=0;y<16;y++){
    for(int x=0;x<16;x++){
        BeginTextureMode(startsetupim[startsetupdb32id]);
        //startsetupcan[startsetupdb32id].Color = c64color[db321616[y][x]]
        //If db321616[y][x] = 0 Then startsetupcan[startsetupdb32id].Alpha=0 Else startsetupcan[startsetupdb32id].Alpha=1
        //startsetupcan[startsetupdb32id].DrawRect(x*2,y*2,2,2)
        if(db321616[15-y][x]>0)DrawRectangle(x*2,y*2,2,2,c64color[db321616[15-y][x]]);        
        EndTextureMode();
    }}
    //startsetupcan[startsetupdb32id].Flush()


}

void startsetupview(){
    
    //canvas.Color = Color.Black
    DrawRectangle(startsetupx-1,startsetupy-1,startsetupwidth+2,startsetupheight+2,BLACK);

    //canvas.Color = c64color[12]
    DrawRectangle(startsetupx,startsetupy,startsetupwidth,startsetupheight,c64color[12]);

    //Local selected:Bool=False		
    bool selected = false;
    for(int i=0;i<3;i++){
        //canvas.Color = Color.Grey.Blend(Color.Black,.5)
        //DrawRectangle(startsetupx+8+20,startsetupy+8+i*48,32,32,GRAY);
        //canvas.Color = Color.White
        DrawTexture(startsetupim[i].texture,startsetupx+9+20,startsetupy+9+i*48,WHITE);
        //' draw the palette buttons
        //canvas.Color = Color.Black
        //DrawRectangle(startsetupx+8+50,startsetupy+8+i*48,32,32,BLACK);
        //canvas.Color = Color.White
        //DrawTexture(startsetupim[3],startsetupx+9+50,startsetupy+9+i*48,0,.9,.9)
        DrawTexture(startsetupim[3].texture,startsetupx+9+60,startsetupy+9+i*48,WHITE);
        //canvas.Color = Color.Black
        //DrawRectangle(startsetupx+8+80,startsetupy+8+i*48,32,32,BLACK);
        //canvas.Color = Color.White
        DrawTexture(startsetupim[4].texture,startsetupx+9+100,startsetupy+9+i*48,WHITE);
                    
        if(IsMouseButtonDown(0)){
            if(rectsoverlap(GetMouseX(),GetMouseY(),1,1,startsetupx+9+60,startsetupy+9+i*48,32,32)){
                startsetupselected = i;
                startsetuppalettemode = 0;
                selected = true;                                
            }
            if(rectsoverlap(GetMouseX(),GetMouseY(),1,1,startsetupx+9+100,startsetupy+9+i*48,32,32)){
                startsetupselected = i;
                startsetuppalettemode = 1;
                selected = true;                                
            }
        }
    }
    
    if(selected == true){
        if(startsetuppalettemode == 1){// ' if the db32 palette was selected
            
            paletteeraser = 0;
            palettex = 640-150;
            palettey = 32;
            palettewidth = 32*4;
            paletteheight = 32*4;
            numpalette = 32;
            palettecellwidth = 32*4/4;
            palettecellheight = 32*4/8;
        }
        
        switch (startsetupselected){
            case 0:
                spritelibscale = 4;
                spritewidth = 8;
                spriteheight = 8;
                //map = New Int[spritewidth,spriteheight]	
                //selectionbuffer = New Int[spritewidth,spriteheight];	
                gridwidth = canvaswidth/spritewidth;		
                gridheight = canvasheight/spriteheight;
                //weasel = tilemaptilesscreenhorizontal;
                //spritelibmap = New Int[numspritelib,spritewidth,spriteheight];					
                break;
            case 1:
                spritelibscale = 2;
                spritewidth = 16;
                spriteheight = 16;
                //map = New Int[spritewidth,spriteheight];	
                //selectionbuffer = New Int[spritewidth,spriteheight];	
                gridwidth = canvaswidth/spritewidth;		
                gridheight = canvasheight/spriteheight;					
                //spritelibmap = New Int[numspritelib,spritewidth,spriteheight];	
                break;
            case 2:
                spritelibscale = 1;
                spritewidth = 32;
                spriteheight = 32;
                //map = New Int[spritewidth,spriteheight];	
                //selectionbuffer = New Int[spritewidth,spriteheight];	
                gridwidth = canvaswidth/spritewidth;		
                gridheight = canvasheight/spriteheight;
                //spritelibmap = New Int[numspritelib,spritewidth,spriteheight];											
                break;
        }
        startsetupdone = true;
    }
    
}

void bottombarview(){
    //canvas.Color = bottombarcolor
    DrawRectangle(bottombarx,bottombary,bottombarwidth,bottombarheight,bottombarcolor);
    //canvas.Color = bottombarcolor/2
    DrawRectangle(bottombarx,bottombary+bottombarheight/2,bottombarwidth,bottombarheight/2,(Color){50,5,2,255});
}

void middlebarview(){
    //canvas.Color = middlebarcolor
    DrawRectangle(middlebarx,middlebary,middlebarwidth,middlebarheight,middlebarcolor);
    //canvas.Color = middlebarcolor/2
    DrawRectangle(middlebarx,middlebary+middlebarheight/2,middlebarwidth,middlebarheight/2,DARKGRAY);
    //' Draw the icons ..
    int num = 0;
    for(int x = 640-128;x<640;x+=32){
        if(num>=nummiddlebaricons)continue;
        int pointx=x+middlebarx;
        int pointy=middlebary;
        if(middlebarcurrentid == num){
            //canvas.Color=Color.White
            DrawTexture(middlebarim[num].texture,pointx,pointy,WHITE);            
            }else{
            DrawTexture(middlebarim[num].texture,pointx,pointy,DARKGRAY);            
            //canvas.Color=Color.DarkGrey						
        }
        //canvas.DrawImage(middlebarim[num],pointx,pointy)
        //'
        //' Selection of the sprite editor or the map editor
        if(IsMouseButtonDown(0)){
            if(rectsoverlap(GetMouseX(),GetMouseY(),1,1,pointx,pointy,32,32)){
                middlebarcurrentid = num;
            }
        }
        
        num+=1;
    }
    
}


void setupmiddlebar(){
    int zero[8][8]={
    {0,12,12,12,12,12,12,0},
    {12,12,12,1,1,1,12,12},
    {12,12,12,1,12,1,12,12},
    {12,12,12,1,12,1,12,12},
    {12,12,12,1,12,1,12,12},
    {12,12,12,1,1,1,12,12},
    {12,12,12,12,12,12,12,12},
    {12,12,12,12,12,12,12,12}};
    for(int y=0;y<8;y++){
    for(int x=0;x<8;x++){
        //middlebarcan[middlebar0id].Color = c64color[zero[y][x]]
        //If zero[y][x] = 0 Then middlebarcan[middlebar0id].Alpha=0 Else middlebarcan[middlebar0id].Alpha=1
        BeginTextureMode(middlebarim[middlebar0id]);
        DrawRectangle(x*4,y*4,4,4,c64color[zero[7-y][x]]);
        EndTextureMode();
    }}
    //middlebarcan[middlebar0id].Flush()

    int one[8][8]={
    {0,12,12,12,12,12,12,0},
    {12,12,12,1,1,12,12,12},
    {12,12,12,12,1,12,12,12},
    {12,12,12,12,1,12,12,12},
    {12,12,12,12,1,12,12,12},
    {12,12,12,1,1,1,12,12},
    {12,12,12,12,12,12,12,12},
    {12,12,12,12,12,12,12,12}};
    for(int y=0;y<8;y++){
    for(int x=0;x<8;x++){
        //middlebarcan[middlebar1id].Color = c64color[one[y][x]]
        //If one[y][x] = 0 Then middlebarcan[middlebar1id].Alpha=0 Else middlebarcan[middlebar1id].Alpha=1
        BeginTextureMode(middlebarim[middlebar1id]);
        DrawRectangle(x*4,y*4,4,4,c64color[one[7-y][x]]);
        EndTextureMode();
    }}
    //middlebarcan[middlebar1id].Flush()
    
    int two[8][8]={
    {0,12,12,12,12,12,12,0},
    {12,12,12,1,1,1,12,12},
    {12,12,12,12,12,1,12,12},
    {12,12,12,12,1,12,12,12},
    {12,12,12,1,12,12,12,12},
    {12,12,12,1,1,1,12,12},
    {12,12,12,12,12,12,12,12},
    {12,12,12,12,12,12,12,12}};		
    for(int y=0;y<8;y++){
    for(int x=0;x<8;x++){
        //middlebarcan[middlebar2id].Color = c64color[two[y][x]]
        //If two[y][x] = 0 Then middlebarcan[middlebar2id].Alpha=0 Else middlebarcan[middlebar2id].Alpha=1
        BeginTextureMode(middlebarim[middlebar2id]);
        DrawRectangle(x*4,y*4,4,4,c64color[two[7-y][x]]);
        EndTextureMode();
    }}    
    //middlebarcan[middlebar2id].Flush()

    int three[8][8]={
    {0,12,12,12,12,12,12,0},
    {12,12,12,1,1,1,12,12},
    {12,12,12,12,12,1,12,12},
    {12,12,12,12,1,1,12,12},
    {12,12,12,12,12,1,12,12},
    {12,12,12,1,1,1,12,12},
    {12,12,12,12,12,12,12,12},
    {12,12,12,12,12,12,12,12}};		
    for(int y=0;y<8;y++){
    for(int x=0;x<8;x++){
        //middlebarcan[middlebar3id].Color = c64color[three[y][x]]
        //If three[y][x] = 0 Then middlebarcan[middlebar3id].Alpha=0 Else middlebarcan[middlebar3id].Alpha=1        
        BeginTextureMode(middlebarim[middlebar3id]);
        DrawRectangle(x*4,y*4,4,4,c64color[three[7-y][x]]);
        EndTextureMode();
    }}    
    //middlebarcan[middlebar3id].Flush()

}


void setuptopbar(){
		int mapedit[8][8] = {
		{1,1,1,0,0,1,1,0},
		{1,1,1,0,0,1,1,0},
		{1,1,1,0,0,0,0,0},
		{0,0,0,1,1,1,1,1},
		{1,1,0,1,1,1,1,1},
		{1,1,0,1,1,1,1,1},
		{0,0,0,1,1,1,1,1},
		{0,0,0,1,1,1,1,1}};
		for(int y=0;y<8;y++){
        for(int x=0;x<8;x++){
			//topbarcan[topbarmapeditid].Color = c64color[mapedit[y][x]]
			//If mapedit[y][x] = 0 Then topbarcan[topbarmapeditid].Alpha=0 Else topbarcan[topbarmapeditid].Alpha=1
            BeginTextureMode(topbarim[topbarmapeditid]);
			if(mapedit[7-y][x]>0)DrawRectangle(x*4,y*4,4,4,c64color[mapedit[7-y][x]]);
            EndTextureMode();
        }}
		//topbarcan[topbarmapeditid].Flush()

		int spriteedit[8][8] = {
		{1,0,0,0,0,0,0,1},
		{0,1,0,0,0,0,1,0},
		{0,1,0,1,1,0,1,0},
		{0,0,1,1,1,1,0,0},
		{0,1,1,0,0,1,1,0},
		{0,1,1,1,1,1,1,0},
		{0,0,1,1,1,1,0,0},
		{0,1,0,1,1,0,1,0}};
		for(int y=0;y<8;y++){
        for(int x=0;x<8;x++){
			//topbarcan[topbarspriteeditid].Color = c64color[spriteedit[y][x]]
			//If spriteedit[y][x] = 0 Then topbarcan[topbarspriteeditid].Alpha=0 Else topbarcan[topbarspriteeditid].Alpha=1
            BeginTextureMode(topbarim[topbarspriteeditid]);
			if(spriteedit[7-y][x]>0)DrawRectangle(x*4,y*4,4,4,c64color[spriteedit[7-y][x]]);
            EndTextureMode();
        }}
		//topbarcan[topbarspriteeditid].Flush()
		
}

void topbarview(){
    //canvas.Color = topbarcolor
    DrawRectangle(topbarx,topbary,topbarwidth,topbarheight,topbarcolor);
    //canvas.Color = topbarcolor/2
    DrawRectangle(topbarx,topbary+topbarheight/2,topbarwidth,topbarheight/2,(Color){50,5,2,255});
    //'
    //' Draw the icons ..
    int num = 0;
    for(int x=640-200;x<640;x+=32){
        if(num>=numtopbaricons) continue;
        int pointx=x+topbarx;
        int pointy=topbary+3;
        if(topbarcurrentid == num){
            //canvas.Color=Color.White
            DrawTextureEx(topbarim[num].texture,(Vector2){pointx,pointy},0,0.6f,WHITE);            
            }else{
            DrawTextureEx(topbarim[num].texture,(Vector2){pointx,pointy},0,0.6f,DARKGRAY);            
            //canvas.Color=Color.DarkGrey						
        }
        //canvas.DrawImage(topbarim[num],pointx,pointy,0,.6,.6)
        //'
        //' Selection of the sprite editor or the map editor
        if(IsMouseButtonDown(0)){
            if(rectsoverlap(GetMouseX(),GetMouseY(),1,1,pointx,pointy,32,32)){
                topbarcurrentid = num;
            }
        }			
        
        num+=1;
    }   
}

void paletteview(){
    //canvas.Color = Color.Black
    DrawRectangle(palettex,palettey,palettewidth,paletteheight,BLACK);
    //canvas.Scissor = New Recti(palettex+2,palettey+2,palettex+2+palettewidth-4,palettey+2+paletteheight-4)
    //canvas.Color = Color.White
    int cc=0;
    for(int y=0;y<paletteheight;y+=palettecellheight){
    for(int x=0;x<palettewidth;x+=palettecellwidth){
        if(cc>=numpalette)break;
        float pointx=x+palettex;
        float pointy=y+palettey;
        //'
        //' Draw our color
        if(startsetuppalettemode == 0){
            //canvas.Color = c64color[cc]
            DrawRectangle(pointx,pointy,palettecellwidth,palettecellheight,c64color[cc]);
            }else{
            //canvas.Color = db32color[cc]
            DrawRectangle(pointx,pointy,palettecellwidth,palettecellheight,db32color[cc]);
        }
        //canvas.DrawRect(pointx,pointy,palettecellwidth,palettecellheight)
        //'
        //' Draw a white bar around the currently selected color
        if(paletteselected == cc){
            //canvas.OutlineMode = OutlineMode.Solid
            //canvas.OutlineWidth = 3
            //canvas.OutlineColor = Color.Black
            //canvas.DrawRect(pointx+2,pointy+2,palettecellwidth-4,palettecellheight-4)
            DrawRectangleLinesEx((Rectangle){pointx+2,pointy+2,palettecellwidth-4,palettecellheight-4}, 3, BLACK);
            //canvas.OutlineMode = OutlineMode.Solid
            //canvas.OutlineWidth = 1
            //canvas.OutlineColor = Color.Yellow
            //canvas.DrawRect(pointx+2,pointy+2,palettecellwidth-4,palettecellheight-4)	
            DrawRectangleLinesEx((Rectangle){pointx+2,pointy+2,palettecellwidth-4,palettecellheight-4}, 1, YELLOW);
            //canvas.OutlineMode = OutlineMode.None
        }
        //'
        //' Select our color
        if(IsMouseButtonDown(0)){				
            if(rectsoverlap(GetMouseX(),GetMouseY(),1,1,pointx,pointy,palettecellwidth,palettecellheight)){
                paletteselected = cc;
            }
        }
        //'
        cc+=1;
    }}
    //canvas.Scissor = New Recti(0,0,640,480)
    //'canvas.Color = c64color[2]
}


void spritegrid(){
    //' If grid noview then exit this method		
    if(toolgridtoggle == false)return;
    
    //' draw our grid	
    //canvas.Color = Color.Grey
    
    for(int y=0;y<spriteheight;y++){
    for(int x=0;x<spritewidth;x++){
        int pointx=(x*gridwidth)+canvasx;
        int pointy=(y*gridheight)+canvasy;
        DrawLine(pointx,pointy,pointx+gridwidth,pointy,GRAY);
        DrawLine(pointx,pointy,pointx,pointy+gridheight,GRAY);
    }}
}

void setuptoolview(){
    //'read icons
    int pencil[8][8] = {
    {12,12,12,12,12,12,12,12},
    {12,1,1,1,1,12,12,12},
    {12,1,1,1,12,1,12,12},
    {12,1,1,12,1,1,1,12},
    {12,1,12,1,1,1,1,1},
    {12,12,1,1,1,1,1,1},
    {12,12,12,1,1,1,1,1},
    {12,12,12,12,1,1,1,1}};		
    for(int y=0;y<8;y++){
    for(int x=0;x<8;x++){
        
        //toolcan[toolpencilid].Color = c64color[pencil[y][x]]
        BeginTextureMode(toolim[toolpencilid]);
        DrawRectangle(x*4,y*4,4,4,c64color[pencil[7-y][x]]);                
        EndTextureMode();

    }
    }
    //toolcan[toolpencilid].Flush()
    
    int eraser[8][8] = {
    {12,12,12,12,12,12,12,12},
    {12,12,12,1,12,12,12,12},
    {12,12,1,1,1,12,12,12},
    {12,1,1,12,1,1,12,12},
    {12,12,1,1,12,1,1,12},
    {12,12,12,1,1,1,12,12},
    {12,12,12,12,1,12,12,12},
    {12,12,12,12,12,12,12,12}};
    for(int y=0;y<8;y++){
    for(int x=0;x<8;x++){
        //toolcan[tooleraserid].Color = c64color[eraser[y][x]]
        BeginTextureMode(toolim[tooleraserid]);
        DrawRectangle(x*4,y*4,4,4,c64color[eraser[7-y][x]]);
        EndTextureMode();
    }
    }
    //toolcan[tooleraserid].Flush()

    int fill[8][8] = {
    {12,12,12,1,12,12,12,12},
    {12,12,12,12,1,12,12,12},
    {12,12,12,12,12,1,12,12},
    {1,1,1,1,1,1,1,12},
    {12,1,1,1,1,1,1,1},
    {1,12,1,1,1,1,1,12},
    {12,12,12,1,1,1,12,12},
    {1,12,12,12,1,12,12,12}};		
    for(int y=0;y<8;y++){
    for(int x=0;x<8;x++){
        //toolcan[toolfillid].Color = c64color[fill[y][x]]
        BeginTextureMode(toolim[toolfillid]);
        DrawRectangle(x*4,y*4,4,4,c64color[fill[7-y][x]]);
        EndTextureMode();
    }
    }
    //toolcan[toolfillid].Flush()

    int line[8][8] = {
    {12,12,12,12,12,12,12,12},
    {12,1,12,12,12,12,12,12},
    {12,12,1,12,12,12,12,12},
    {12,12,12,1,12,12,12,12},
    {12,12,12,12,1,12,12,12},
    {12,12,12,12,12,1,12,12},
    {12,12,12,12,12,12,1,12},
    {12,12,12,12,12,12,12,12}};
    for(int y=0;y<8;y++){
    for(int x=0;x<8;x++){
        //toolcan[toollineid].Color = c64color[line[y][x]]
        BeginTextureMode(toolim[toollineid]);
        DrawRectangle(x*4,y*4,4,4,c64color[line[7-y][x]]);
        EndTextureMode();
    }
    }
    //toolcan[toollineid].Flush()

    int selection[8][8] = {
    {12,12,12,12,12,12,12,12},
    {12,1,12,1,1,12,1,12},
    {12,12,12,12,12,12,12,12},
    {12,1,12,12,12,12,1,12},
    {12,1,12,12,12,12,1,12},
    {12,12,12,12,12,12,12,12},
    {12,1,12,1,1,12,1,12},
    {12,12,12,12,12,12,12,12}};
    for(int y=0;y<8;y++){
    for(int x=0;x<8;x++){
        //toolcan[toolselectionid].Color = c64color[selection[y][x]]
        BeginTextureMode(toolim[toolselectionid]);
        DrawRectangle(x*4,y*4,4,4,c64color[selection[7-y][x]]);
        EndTextureMode();
    }
    }
    //toolcan[toolselectionid].Flush()

    int copy[8][8] = {
    {1,12,12,12,12,12,12,12},
    {1,1,12,1,1,12,1,12},
    {1,1,1,12,12,12,12,12},
    {1,1,1,1,12,12,1,12},
    {1,1,1,12,12,12,1,12},
    {1,1,12,12,12,12,12,12},
    {1,12,12,1,1,12,1,12},
    {12,12,12,12,12,12,12,12}};
    for(int y=0;y<8;y++){
    for(int x=0;x<8;x++){
        //toolcan[toolcopyid].Color = c64color[copy[y][x]]
        BeginTextureMode(toolim[toolcopyid]);
        DrawRectangle(x*4,y*4,4,4,c64color[copy[7-y][x]]);
        EndTextureMode();
    }
    }
    //toolcan[toolcopyid].Flush()

    int paste[8][8] = {
    {12,12,12,1,12,12,12,12},
    {12,12,1,1,1,12,1,12},
    {12,1,1,1,12,12,12,12},
    {1,1,1,1,12,12,1,12},
    {12,1,1,1,12,12,1,12},
    {12,12,1,1,12,12,12,12},
    {12,12,12,1,1,12,1,12},
    {12,12,12,12,12,12,12,12}};
    for(int y=0;y<8;y++){
    for(int x=0;x<8;x++){
        //toolcan[toolpasteid].Color = c64color[paste[y][x]]
        BeginTextureMode(toolim[toolpasteid]);
        DrawRectangle(x*4,y*4,4,4,c64color[paste[7-y][x]]);
        EndTextureMode();
    }
    }
    //toolcan[toolpasteid].Flush()

    int cut[8][8] = {
    {12,12,12,12,12,12,12,12},
    {12,12,12,1,12,12,12,12},
    {12,12,12,12,1,12,12,12},
    {12,1,12,12,1,12,12,12},
    {12,12,1,1,12,1,1,12},
    {12,12,12,12,1,12,1,1},
    {12,12,12,12,1,1,12,12},
    {12,12,12,12,12,1,1,12}};
    for(int y=0;y<8;y++){
    for(int x=0;x<8;x++){
        //toolcan[toolcutid].Color = c64color[cut[y][x]]
        BeginTextureMode(toolim[toolcutid]);
        DrawRectangle(x*4,y*4,4,4,c64color[cut[7-y][x]]);
        EndTextureMode();
    }
    }
    //toolcan[toolcutid].Flush()


    int flipvertical[8][8] = {
    {12,12,12,1,1,12,12,12},
    {12,12,1,1,1,1,12,12},
    {12,1,1,1,1,1,1,12},
    {12,12,12,1,1,12,12,12},
    {12,12,12,1,1,12,12,12},
    {12,1,1,1,1,1,1,12},
    {12,12,1,1,1,1,12,12},
    {12,12,12,1,1,12,12,12}};
    for(int y=0;y<8;y++){
    for(int x=0;x<8;x++){
        //toolcan[toolflipverticalid].Color = c64color[flipvertical[y][x]]
        BeginTextureMode(toolim[toolflipverticalid]);
        DrawRectangle(x*4,y*4,4,4,c64color[flipvertical[7-y][x]]);
        EndTextureMode();
    }
    }
    //toolcan[toolflipverticalid].Flush()
    
    int fliphorizontal[8][8] = {
    {12,12,12,12,12,12,12,12},
    {12,12,1,12,12,1,12,12},
    {12,1,1,12,12,1,1,12},
    {1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1},
    {12,1,1,12,12,1,1,12},
    {12,12,1,12,12,1,12,12},
    {12,12,12,12,12,12,12,12}};		
    for(int y=0;y<8;y++){
    for(int x=0;x<8;x++){
        //toolcan[toolfliphorizontalid].Color = c64color[fliphorizontal[y][x]]
        BeginTextureMode(toolim[toolfliphorizontalid]);
        DrawRectangle(x*4,y*4,4,4,c64color[fliphorizontal[7-y][x]]);
        EndTextureMode();
    }
    }
    //toolcan[toolfliphorizontalid].Flush()

    int colorpicker[8][8] = {
    {12,1,1,12,12,12,12,12},
    {1,1,12,12,1,12,12,12},
    {1,12,12,1,12,12,12,12},
    {12,12,1,12,1,12,12,12},
    {12,1,12,1,12,1,12,12},
    {12,12,12,12,1,12,1,12},
    {12,12,12,12,12,1,1,12},
    {12,12,12,12,12,12,12,1}};
    for(int y=0;y<8;y++){
    for(int x=0;x<8;x++){
        //toolcan[toolcolorpickerid].Color = c64color[colorpicker[y][x]]
        BeginTextureMode(toolim[toolcolorpickerid]);
        DrawRectangle(x*4,y*4,4,4,c64color[colorpicker[7-y][x]]);
        EndTextureMode();
    }
    }
    //toolcan[toolcolorpickerid].Flush()

    int grid[8][8] = {
    {12,12,12,12,12,12,12,12},
    {12,12,1,12,12,1,12,12},
    {12,1,1,1,1,1,1,12},
    {12,12,1,12,12,1,12,12},
    {12,12,1,12,12,1,12,12},
    {12,1,1,1,1,1,1,12},
    {12,12,1,12,12,1,12,12},
    {12,12,12,12,12,12,12,12}};		
    for(int y=0;y<8;y++){
    for(int x=0;x<8;x++){
        //toolcan[toolgridid].Color = c64color[grid[y][x]]
        BeginTextureMode(toolim[toolgridid]);
        DrawRectangle(x*4,y*4,4,4,c64color[grid[7-y][x]]);
        EndTextureMode();
    }
    }
    //toolcan[toolgridid].Flush()
    
            
    int filledrect[8][8] = {
    {12,12,12,12,12,12,12,12},
    {12,1,1,1,1,1,1,12},
    {12,1,1,1,1,1,1,12},
    {12,1,1,1,1,1,1,12},
    {12,1,1,1,1,1,1,12},
    {12,1,1,1,1,1,1,12},
    {12,1,1,1,1,1,1,12},
    {12,12,12,12,12,12,12,12}};		
    for(int y=0;y<8;y++){
    for(int x=0;x<8;x++){
        //toolcan[toolfilledrectid].Color = c64color[filledrect[y][x]]
        BeginTextureMode(toolim[toolfilledrectid]);
        DrawRectangle(x*4,y*4,4,4,c64color[filledrect[7-y][x]]);
        EndTextureMode();
    }
    }
    //toolcan[toolfilledrectid].Flush()

    int outlinerect[8][8] = {
    {12,12,12,12,12,12,12,12},
    {12,1,1,1,1,1,1,12},
    {12,1,12,12,12,12,1,12},
    {12,1,12,12,12,12,1,12},
    {12,1,12,12,12,12,1,12},
    {12,1,12,12,12,12,1,12},
    {12,1,1,1,1,1,1,12},
    {12,12,12,12,12,12,12,12}};
    for(int y=0;y<8;y++){
    for(int x=0;x<8;x++){
        //toolcan[tooloutlinerectid].Color = c64color[outlinerect[y][x]]
        BeginTextureMode(toolim[tooloutlinerectid]);
        DrawRectangle(x*4,y*4,4,4,c64color[outlinerect[7-y][x]]);
        EndTextureMode();
    }
    }
    //toolcan[tooloutlinerectid].Flush()

    int filledcircle[8][8] = {
    {12,12,12,1,1,12,12,12},
    {12,12,1,1,1,1,12,12},
    {12,1,1,1,1,1,1,12},
    {1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1},
    {12,1,1,1,1,1,1,12},
    {12,12,1,1,1,1,12,12},
    {12,12,12,1,1,12,12,12}};
    for(int y=0;y<8;y++){
    for(int x=0;x<8;x++){
        int p=filledcircle[7-y][x];
        //toolcan[toolfilledcircleid].Color = c64color[p]
        BeginTextureMode(toolim[toolfilledcircleid]);
        DrawRectangle(x*4,y*4,4,4,c64color[p]);
        EndTextureMode();
    }
    }
    //toolcan[toolfilledcircleid].Flush()

    int outlinecircle[8][8] = {
    {12,12,12,1,1,12,12,12},
    {12,12,1,12,12,1,12,12},
    {12,1,12,12,12,12,1,12},
    {1,12,12,12,12,12,12,1},
    {1,12,12,12,12,12,12,1},
    {12,1,12,12,12,12,1,12},
    {12,12,1,12,12,1,12,12},
    {12,12,12,1,1,12,12,12}};
    for(int y=0;y<8;y++){
    for(int x=0;x<8;x++){
        int p=outlinecircle[7-y][x];
        //toolcan[tooloutlinecircleid].Color = c64color[p]
        BeginTextureMode(toolim[tooloutlinecircleid]);
        DrawRectangle(x*4,y*4,4,4,c64color[p]);
        EndTextureMode();
    }
    }
    //toolcan[tooloutlinecircleid].Flush()

}


void toolview(){
    //canvas.Color=Color.Black
    DrawRectangle(toolx,tooly,toolwidth,toolheight,BLACK);
    //canvas.Color=Color.White
    if(delay>0)delay-=1;
    int num=0;
    for(int y=tooly;y<tooly+toolheight;y+=32){
    for(int x=toolx;x<toolx+toolwidth;x+=32){
        if(num>=numtools)continue;        
        int pointx=x;
        int pointy=y;
        if(toolselected == num){
            //canvas.Color = Color.Yellow
            DrawRectangle(pointx,pointy,32,32,YELLOW);
            //canvas.Color = Color.White
            //canvas.Scissor = New Recti(pointx+2,pointy+2,pointx+30,pointy+30)
            DrawTexture(toolim[num].texture,pointx,pointy,WHITE);
            //canvas.Scissor = New Recti(0,0,640,480)
        }else{
            DrawTexture(toolim[num].texture,pointx,pointy,WHITE);
        }
        //'
        //'/ Interaction with the tool area
        //'
        if(IsMouseButtonDown(0)){
            if(rectsoverlap(GetMouseX(),GetMouseY(),1,1,pointx,pointy,32,32)){
                toolselected = num;


                //' Pastethe selected area
                if(toolselected == toolpasteid){
                    if(selectionstartx == selectionendx && selectionstarty == selectionendy){
                    }else{
                        for(int y1=selectionbufferstarty; y1<selectionbufferendy;y1++){
                        for(int x1=selectionbufferstartx; x1<selectionbufferendx;x1++){
                            int destx=selectionstartx+x1-selectionbufferstartx;
                            int desty=selectionstarty+y1-selectionbufferstarty;
                            if(x1<0 || y1<0 || x1>=32 || y1>=32) continue;
                            if(destx<0 || desty<0 || destx>=32 || desty>=32)continue;
                            if(destx>selectionendx || desty>selectionendy)continue;
                            map[destx][desty] = selectionbuffer[x1][y1];
                        }
                        }
                        toolselected = toolpencilid;
                    }
                }
            
                
                //' Copy the selected area
                if(toolselected == toolcopyid){
                    if(selectionstartx == selectionendx && selectionstarty == selectionendy){
                        
                    }else{
                        selectionbufferstartx = selectionstartx;
                        selectionbufferstarty = selectionstarty;
                        selectionbufferendx = selectionendx;
                        selectionbufferendy = selectionendy;
                        for(int y1=selectionstarty;y1<selectionendy;y1++){
                        for(int x1=selectionstartx;x1<selectionendx;x1++){
                            selectionbuffer[x1][y1] = map[x1][y1];
                        }
                        }
                        toolselected = toolpencilid;
                    }						
                }
                // Cut the selected area		
                if(toolselected == toolcutid){
                    if(selectionstartx == selectionendx && selectionstarty == selectionendy){
                    
                    }else{
                        for(int y1=selectionstarty;y1<selectionendy;y1++){
                        for(int x1=selectionstartx;x1<selectionendx;x1++){
                            map[x1][y1] = paletteeraser;
                        }
                        }
                        toolselected = toolpencilid;
                    }
                }
                
                //' Mirror vertically
                if(toolselected == toolflipverticalid && delay <= 0)
                {
                    //int tempmap[32][32] = {0};
                    for(int y1=0;y1<spriteheight;y1++){
                    for(int x1=0;x1<spritewidth;x1++){
                        tempmap[x1][y1] = map[x1][y1];
                    }
                    }
                    for(int y1=0;y1<spriteheight;y1++){
                    for(int x1=0;x1<spritewidth;x1++){
                        map[x1][y1] = tempmap[x1][spriteheight-1-y1];
                    }
                    }
                    delay = delaydefault;
                    toolselected = toolpencilid;
                }
                //Mirror Horizontal
                if(toolselected == toolfliphorizontalid && delay <= 0){
                    //int tempmap[spritewidth][spriteheight];
                    for(int y1=0;y1<spriteheight;y1++){
                    for(int x1=0;x1<spritewidth;x1++){
                        tempmap[x1][y1] = map[x1][y1];
                    }
                    }
                    for(int y1=0;y1<spriteheight;y1++){
                    for(int x1=0;x1<spritewidth;x1++){							
                        map[x1][y1] = tempmap[spritewidth-1-x1][y1];
                    }
                    }
                    delay = delaydefault;
                    toolselected = toolpencilid;
                }
                //' Grid toggle
                if(toolselected == toolgridid && delay <= 0){
                    if(toolgridtoggle == true){
                        toolgridtoggle = false;
                    }else{
                        toolgridtoggle = true;
                    }
                    delay = delaydefault;
                    toolselected = toolpencilid;
                }
            }
        }
        num+=1;
        
    }
    }
}

void spritelibview(){
    //canvas.Color = Color.Black
    DrawRectangle(spritelibx,spriteliby,spritelibwidth,spritelibheight,BLACK);
    //canvas.Color = Color.White
    int num=(middlebarcurrentid*80);
    for(int y=spriteliby;y<spriteliby+spritelibheight;y+=spriteheight*spritelibscale){
    for(int x=spritelibx;x<spritelibx+spritelibwidth;x+=spritewidth*spritelibscale){
        int pointx=x;
        int pointy=y;

        if(num == spritelibselected){
            
            //canvas.Color = Color.White
            //DrawRectangle(pointx,pointy,spritewidth*spritelibscale,spriteheight*spritelibscale,WHITE);
            //canvas.Scissor = New Recti(pointx+2,pointy+2,pointx+spritewidth*spritelibscale-3,pointy+spriteheight*spritelibscale-3)
            //'canvas.Scissor(z1)
            //DrawTextureRec(spritelibim[num].texture,(Rectangle){0,0,spritewidth,spriteheight},pointx,pointy,WHITE);
            //DrawTextureEx(spritelibim[num].texture,(Vector2){pointx,pointy},0,spritelibscale,WHITE);
            //DrawTexture(spritelibim[num].texture,pointx,pointy,WHITE);
            // NOTE: Render texture must be y-flipped due to default OpenGL coordinates (left-bottom)
            DrawTextureRec(spritelibim[num].texture, (Rectangle){ 0, 0, spritelibim[num].texture.width, -spritelibim[num].texture.height }, (Vector2){pointx,pointy}, WHITE);
            DrawRectangle(pointx,pointy,spritewidth*spritelibscale,spriteheight*spritelibscale,(Color){100,100,100,100});
            //'Local z2:=New Recti(0,0,640,480)
            //canvas.Scissor = New Recti(0,0,640,480)
        }else{
            //canvas.Color = Color.White
            //canvas.DrawImage(spritelibim[num],pointx,pointy)	
            //DrawTexture(spritelibim[num].texture,pointx,pointy,WHITE);	
            //DrawTextureEx(spritelibim[num].texture,(Vector2){pointx,pointy},0,spritelibscale,WHITE);
            //DrawTexture(spritelibim[num].texture,pointx,pointy,WHITE);
            //DrawTextureRec(spritelibim[num].texture, (Rectangle){ 0, 0, spritewidth, -spriteheight }, (Vector2){ 0, 0 }, WHITE);            
            DrawTextureRec(spritelibim[num].texture, (Rectangle){ 0, 0, spritelibim[num].texture.width, -spritelibim[num].texture.height }, (Vector2){pointx,pointy}, WHITE);
        }

        


        
        if(IsMouseButtonDown(0)){
            if(rectsoverlap(GetMouseX(),GetMouseY(),1,1,pointx,pointy,spritewidth*spritelibscale,spriteheight*spritelibscale)){
                spritelibselected = num;
                spritelibcopytocanvas();
            }
        }
                
        num+=1;
        if(num>=numspritelib)break;
    }
        if(num>=numspritelib)break;
    }

}	


void spriteview(){
    
    
    for(int y=0;y<spriteheight;y++){
    for(int x=0;x<spritewidth;x++){

        int pointx=(x*gridwidth)+canvasx;
        int pointy=(y*gridheight)+canvasy;
        //'canvas.DrawRect()
        if(startsetuppalettemode == 0){
            //canvas.Color = c64color[map[x,y]]
            DrawRectangle(pointx,pointy,gridwidth,gridheight,c64color[map[x][y]]);			
        }else{
            //canvas.Color = db32color[map[x,y]]			
            DrawRectangle(pointx,pointy,gridwidth,gridheight,db32color[map[x][y]]);
        }
        //canvas.DrawRect(pointx,pointy,gridwidth,gridheight)
        
        //'
        //' Mouse down (LEFT)
        if(IsMouseButtonDown(0)){
            if(rectsoverlap(GetMouseX(),GetMouseY(),1,1,pointx,pointy,gridwidth,gridheight)){								
                if(toolselected == toolpencilid){
                    map[x][y] = paletteselected;
                }
                if(toolselected == tooleraserid){
                    map[x][y] = paletteeraser;
                }

            }
        }
        if(IsMouseButtonPressed(0)){
            if(rectsoverlap(GetMouseX(),GetMouseY(),1,1,pointx,pointy,gridwidth,gridheight)){								
                if(toolselected == toolfillid){
                    paletteundermouse = map[x][y];
                    if(paletteundermouse!=paletteselected)fillatposition(x,y,map[x][y],paletteselected);
                }
            }
        }
        // Line tool
        if(IsMouseButtonDown(0)){
            if(rectsoverlap(GetMouseX(),GetMouseY(),1,1,pointx,pointy,gridwidth,gridheight)){
                if(toolselected == toollineid){
                    if(linepressed == false && lineactive == false){
                        lineactive = true;
                        linepressed = true;
                        linestartx = x;
                        linestarty = y;						
                    }
                    if(lineactive == true){
                        lineendx = x;
                        lineendy = y;                      
                    }
                }
            }
        }
        if(IsMouseButtonDown(0) == false){
            if(toolselected == toollineid){
                if(lineactive == true){						
                    previewline(true);
                    lineactive = false;
                    linepressed = false;						
                }
            }
        }
        
        //' Selection Tool
        if(IsMouseButtonDown(0)){
            if(rectsoverlap(GetMouseX(),GetMouseY(),1,1,pointx,pointy,gridwidth,gridheight)){
                if(toolselected == toolselectionid){
                    if(selectionpressed == false && selectionactive == false){
                        selectionactive = true;
                        selectionpressed = true;
                        selectionstartx = x;
                        selectionstarty = y;
                        selectionnegativeswitchx = true;
                        selectionnegativeswitchy = true;
                    }
                    if(selectionactive == true){
                        selectionendx = x;
                        selectionendy = y;
                        if(selectionendx < selectionstartx){
                            selectionendx-=1;
                            if(selectionnegativeswitchx){
                                selectionnegativeswitchx = false;
                                selectionstartx+=1;
                            }
                        }
                        if(selectionendy < selectionstarty){
                            selectionendy-=1;
                            if(selectionnegativeswitchy){
                                selectionnegativeswitchy = false;
                                selectionstarty+=1;
                            }
                        }
                        
                    }						
                }
            }
        }
        if(IsMouseButtonDown(0) == false){
            if(toolselected == toolselectionid){
                if(selectionactive == true){
                    //'previewselection(canvas,True)
                    selectionactive = false;
                    selectionpressed = false;	

                    //' if the end is smaller then then start then switch them
                    if(selectionendx<selectionstartx){ 
                        int a=selectionstartx;
                        int b=selectionendx;
                        selectionstartx = b+1;
                        selectionendx = a -1;
                    }
                    if(selectionendy<selectionstarty){
                        int a=selectionstarty;
                        int b=selectionendy;
                        selectionstarty = b +1;
                        selectionendy = a -1 ;
                    }
                }
            }
        }
        
        //' Remove the selection with rmb
        if(IsMouseButtonDown(1) == true){
            selectionstartx=0;
            selectionstarty=0;
            selectionendx=0;
            selectionendy=0;
        }
        
        
        //'		
        //' Mouse down (MIDDLE) Color Picker
        if(IsMouseButtonDown(2)){
            if(rectsoverlap(GetMouseX(),GetMouseY(),1,1,pointx,pointy,gridwidth,gridheight)){
                paletteselected = map[x][y];
            }
        }
        
        if(IsMouseButtonDown(0)){
            if(rectsoverlap(GetMouseX(),GetMouseY(),1,1,pointx,pointy,gridwidth,gridheight)){
                if(toolselected == toolcolorpickerid){
                    paletteselected = map[x][y];
                }
            }
        }

         //'
        //' Mouse Down(LEFT) Filled rect / outlined rect / filled circle / outlined circle
        if(IsMouseButtonDown(0)){
            if(rectsoverlap(GetMouseX(),GetMouseY(),1,1,pointx,pointy,gridwidth,gridheight)){
                
                if(toolselected == toolfilledrectid || toolselected == tooloutlinerectid || toolselected == toolfilledcircleid || toolselected == tooloutlinecircleid){
                    
                    if(bcselectionpressed == false && bcselectionactive == false){
                        
                        bcselectionactive = true;
                        bcselectionpressed = true;
                        bcselectionstartx = x;
                        bcselectionstarty = y;
                        bcselectionnegativeswitchx = true;
                        bcselectionnegativeswitchy = true;
                    }
                    if(bcselectionactive == true){
                        bcselectionendx = x;
                        bcselectionendy = y;
                        if(bcselectionendx < bcselectionstartx){
                            bcselectionendx-=1;
                            if(bcselectionnegativeswitchx){
                                bcselectionnegativeswitchx = false;
                                bcselectionstartx+=1;
                            }
                        }
                        if(bcselectionendy < bcselectionstarty){ 
                            bcselectionendy-=1;
                            if(bcselectionnegativeswitchy){
                                bcselectionnegativeswitchy = false;
                                bcselectionstarty+=1;
                            }
                        }
                        
                    }					
                }
            }
        }
        
        if(IsMouseButtonDown(0) == false){           
            
            if(toolselected == toolfilledrectid || toolselected == tooloutlinerectid || toolselected == toolfilledcircleid || toolselected == tooloutlinecircleid){
                        
                if(bcselectionactive == true){

                    //'previewselection(canvas,True)
                    bcselectionactive = false;
                    bcselectionpressed = false;	

                    //' if the end is smaller then then start then switch them
                    if(bcselectionendx<bcselectionstartx){
                        int a=bcselectionstartx;
                        int b=bcselectionendx;
                        bcselectionstartx = b+1;
                        bcselectionendx = a -1;
                    }
                    if(bcselectionendy<bcselectionstarty){
                        int a=bcselectionstarty;
                        int b=bcselectionendy;

                        bcselectionstarty = b +1;
                        bcselectionendy = a -1; 
                    }
                    //'
                    //' Do the filling
                    if(toolselected == toolfilledrectid || toolselected == tooloutlinerectid){
                        
                        for(int y=bcselectionstarty;y<=bcselectionendy;y++){
                        for(int x=bcselectionstartx;x<=bcselectionendx;x++){
                            if(toolselected == toolfilledrectid)map[x][y] = paletteselected;
                            if(toolselected == tooloutlinerectid){
                                if(x == bcselectionstartx || x == bcselectionendx || y == bcselectionendy || y==bcselectionstarty){
                                    map[x][y] = paletteselected;
                                }
                            }
                        }
                        }
                    }else if(toolselected == toolfilledcircleid || toolselected == tooloutlinecircleid){
                        //' add circle code here	
                        
                        int w=abs(bcselectionstartx-bcselectionendx)+1;
                        int h=abs(bcselectionstarty-bcselectionendy)+1;

                        if(toolselected == tooloutlinecircleid){
                            //Local ti:Image = New Image(spritewidth,spriteheight)
                            //Local tc:Canvas = New Canvas(ti)
                            
                            //tc.Clear(Color.Black)
                            //tc.Color = Color.White
                            
                            //BeginTextureMode(ti);
                            //ClearBackground(BLANK);
                            //EndTextureMode();

                            //tc.OutlineMode=OutlineMode.Smooth
                            //tc.OutlineColor = Color.Green
                            //tc.OutlineWidth = 0
                            //tc.DrawOval(bcselectionstartx,bcselectionstarty,w-1,h-1)
                            //tc.DrawOval(bcselectionstartx,bcselectionstarty,w-1,h-1)
                            midptellipse((w/2)-1,(h/2)-1,bcselectionstartx+w/2,bcselectionstarty+h/2);                            
                            //tc.Flush()
                            for(int y=0;y<spriteheight;y++){
                            for(int x=0;x<spritewidth;x++){
                                if(tempmap[x][y] == 1){
                                    map[x][y] = paletteselected;
                                }
                            }
                            }
                        }else{
                            /*
                            Local ti:Image = New Image(spritewidth,spriteheight)
                            Local tc:Canvas = New Canvas(ti)
                            tc.Clear(Color.Black)
                            tc.Color = Color.White
                            tc.DrawOval(bcselectionstartx-1,bcselectionstarty-1,w+1,h+1)
                            tc.Flush()
                            For Local y:Int=0 Until spriteheight
                            For Local x:Int=0 Until spritewidth
                                If ti.GetPixel(x,y) <> Color.Black
                                    map[x,y] = paletteselected
                                End If
                            Next
                            Next
                            */
                        }
                    
                    }
                    
                    bcselectionendy=0;
                    bcselectionendx=0;
                    bcselectionstarty=0;
                    bcselectionstartx=0;
                }
            }
        }
       
        //' Copy to clipboard
        if(IsKeyReleased(KEY_C)){
            copytoclipboard();
        }
        // Copy FROM clipboard.
        if(IsKeyReleased(KEY_P)){
            copyfromclipboard();
            updatepreview();
            updatespritelib();

        }

    }
    }
 //   updatepreview()
 //   updatespritelib()
}


void updatepreview(){			
    if(spritewidth==8)previewcellwidth=8;
    if(spritewidth==16)previewcellwidth=4;
    if(spritewidth==32)previewcellwidth=2;
    //previewcan.Clear(Color.Black)	
    for(int y=0;y<spriteheight;y++){
    for(int x=0;x<spritewidth;x++){
        int pointx=x*previewcellwidth;
        int pointy=y*previewcellwidth;
        BeginTextureMode(previewim);
        if(startsetuppalettemode == 0){
            DrawRectangle(pointx,pointy,previewcellwidth,previewcellwidth,c64color[map[x][y]]);
            //previewcan.Color = c64color[map[x,y]]
        }else{
            //previewcan.Color = db32color[map[x,y]]
            DrawRectangle(pointx,pointy,previewcellwidth,previewcellwidth,db32color[map[x][y]]);
        }
        EndTextureMode();
        //previewcan.DrawRect(pointx,pointy,previewcellwidth,previewcellheight)
    }
    }
    //previewcan.Flush()

}

void updatespritelib(){

    for(int y=0;y<spriteheight;y++){
    for(int x=0;x<spritewidth;x++){
        float pointx=x*spritelibscale;
        float pointy=y*spritelibscale;
        BeginTextureMode(spritelibim[spritelibselected]);
        if(startsetuppalettemode == 0){
            DrawRectangle(pointx,pointy,spritelibscale,spritelibscale,c64color[map[x][y]]);
            //spritelibcan[spritelibselected].Color = c64color[map[x,y]]
        }else{
            DrawRectangle(pointx,pointy,spritelibscale,spritelibscale,db32color[map[x][y]]);
            //spritelibcan[spritelibselected].Color = db32color[map[x,y]]
        }
        //spritelibcan[spritelibselected].DrawRect(pointx,pointy,spritelibscale,spritelibscale)			
        spritelibmap[spritelibselected][x][y] = map[x][y];
        EndTextureMode();
    }
    }
    //spritelibcan[spritelibselected].Flush()

}

    /*
void fillatposition(int x,int y){
    Local ol:List<pathnode> = New List<pathnode>
    ' Add the start position on the list
    ol.AddLast(New pathnode(x,y))
    ' set the cloes map at the start position to distance 1
    Local colorundermouse:Int=map[x,y]
    map[x,y] = paletteselected
    
    ' some helper arrays. We can determine the top,right,and bottom
    ' and left position cells with these numbers.
    Local dx:Int[] = New Int[](0,1,0,-1)
    Local dy:Int[] = New Int[](-1,0,1,0)
    ' While there is contents in the list
    While ol.Count() <> 0
        
        ' Get the current location
        Local x1:Int=ol.First.x
        Local y1:Int=ol.First.y
        ' Remove the current location from the list
        ol.RemoveFirst()
        ' Get 4 new positions around the current positions
        For Local i:=0 Until 4
            ' Set new x and y
            Local nx:Int=x1+dx[i]
            Local ny:Int=y1+dy[i]
            ' If the coordinates are inside the map
            If nx>=0 And ny>=0 And nx<spritewidth And ny<spriteheight
                ' If the closedmap is not written to yet
                If map[nx,ny] = colorundermouse And map[nx,ny] <> paletteselected
                    ' Set the new distance based on the current distance
                    map[nx,ny] = paletteselected
                    ' Add new position to the list
                    ol.AddLast(New pathnode(nx,ny))
                End If
            End If
        Next
    Wend
    
}
*/    

void previewview(){
    //canvas.Color = Color.White
    DrawRectangle(previewx,previewy,previewwidth,previewheight,WHITE);
    //DrawTexture(previewim.texture,previewx+1,previewy+1,WHITE);		
    DrawTextureRec(previewim.texture, (Rectangle){ 0,0, 64, -64 }, (Vector2){previewx,previewy}, WHITE);       
}


void spritelibcopytocanvas(){
    for(int y=0;y<spriteheight;y++){
    for(int x=0;x<spritewidth;x++){
        map[x][y] = spritelibmap[spritelibselected][x][y];
    }
    }
}

void previewline(bool drawit){
    if(lineactive == false)return;
   
    int x1=linestartx;
    int y1=linestarty;
    int x2=lineendx;
    int y2=lineendy;		
    int dx;
    int dy;
    int sx;
    int sy;
    int e;

    dx = abs(x2 - x1);
    sx = -1;
    if(x1 < x2)sx = 1;      
    dy = abs(y2 - y1);
    sy = -1;
    if(y1 < y2)sy = 1;
    if(dx < dy){
        e = dx / 2;
    }else{
        e = dy / 2;        
    }
    bool exitloop=false;
    while(exitloop == false){
        int pointx=canvasx+(x1*gridwidth);
        int pointy=canvasy+(y1*gridheight);
        if(startsetuppalettemode == 0){
            //canvas.Color = c64color[paletteselected]
            DrawRectangle(pointx,pointy,gridwidth,gridheight,c64color[paletteselected]);  
        }else{
            //canvas.Color = db32color[paletteselected]
            DrawRectangle(pointx,pointy,gridwidth,gridheight,db32color[paletteselected]);  
        }
        //canvas.DrawRect(pointx,pointy,gridwidth,gridheight)  
        if(drawit==true){
            if(x1<0 || y1<0 || x1>=spritewidth || y1>=spriteheight)break;
            map[x1][y1] = paletteselected;
        }
        if(x1 == x2){
          if(y1 == y2){
              exitloop = true;
          }
        }
        if(dx > dy){
            x1 += sx;
            e -= dy;
            if(e < 0){
              e += dx;
              y1 += sy;
            }
        }else{
          y1 += sy;
          e -= dx; 
          if(e < 0){
            e += dy;
            x1 += sx;
          }
        }
      
    }
        
}


//' Selection for the selection tool or the filled outlined tools
//'
void previewselection(bool drawit){
    //' the selection rectangle
    if(selectionstartx == selectionendx && selectionstarty == selectionendy){
    }else{
        int x1=canvasx+(selectionstartx*gridwidth);
        int y1=canvasy+(selectionstarty*gridheight);
        int x2=canvasx+((selectionendx+1)*gridwidth);
        int y2=canvasy+((selectionendy+1)*gridheight);		
                
        
        //canvas.Color = Color.White
        DrawLine(x1,y1,x2,y1,WHITE);
        DrawLine(x1,y1,x1,y2,WHITE);
        DrawLine(x1,y2,x2,y2,WHITE);
        DrawLine(x2,y1,x2,y2,WHITE);
        //Color = Color.Black
        x1+=1;y1+=1;x2-=1;y2-=1;
        DrawLine(x1,y1,x2,y1,BLACK);
        DrawLine(x1,y1,x1,y2,BLACK);
        DrawLine(x1,y2,x2,y2,BLACK);
        DrawLine(x2,y1,x2,y2,BLACK);
        x1+=2;y1+=2;x2-=2;y2-=2;
        DrawLine(x1,y1,x2,y1,BLACK);
        DrawLine(x1,y1,x1,y2,BLACK);
        DrawLine(x1,y2,x2,y2,BLACK);
        DrawLine(x2,y1,x2,y2,BLACK);
    }
    //' the filled outlined tool
    if(bcselectionstartx == bcselectionendx && bcselectionstarty == bcselectionendy){
    }else{
        int x1=canvasx+(bcselectionstartx*gridwidth);
        int y1=canvasy+(bcselectionstarty*gridheight);
        int x2=canvasx+((bcselectionendx+1)*gridwidth);
        int y2=canvasy+((bcselectionendy+1)*gridheight);		
        
        //canvas.Color = Color.White
        DrawLine(x1,y1,x2,y1,WHITE);
        DrawLine(x1,y1,x1,y2,WHITE);
        DrawLine(x1,y2,x2,y2,WHITE);
        DrawLine(x2,y1,x2,y2,WHITE);
        //canvas.Color = Color.Black
        x1+=1;y1+=1;x2-=1;y2-=1;
        DrawLine(x1,y1,x2,y1,BLACK);
        DrawLine(x1,y1,x1,y2,BLACK);
        DrawLine(x1,y2,x2,y2,BLACK);
        DrawLine(x2,y1,x2,y2,BLACK);
        x1+=2;y1+=2;x2-=2;y2-=2;
        DrawLine(x1,y1,x2,y1,BLACK);
        DrawLine(x1,y1,x1,y2,BLACK);
        DrawLine(x1,y2,x2,y2,BLACK);
        DrawLine(x2,y1,x2,y2,BLACK);
    }
}

void copytoclipboard(){
/* 
    //This is the original code from the monkey2 language..
    Local out:String="int sprite["+spriteheight+"]["+spritewidth+"] = {"+String.FromChar(10)
    For Local y:Int=spriteheight-1 to 0 Step -1
        Local a:String
    For Local x:Int=0 Until spritewidth
        a+=map[x,y]+","
    Next
    Local l:Int=a.Length
    a=a.Slice(0,a.Length-1)
    out += "{"+a+"},"+String.FromChar(10)
    Next
    out=out.Slice(0,out.Length-2)
    out+="};"
    instance.ClipboardText = out
*/
    // Here we create a copy and paste ready
    // text that can be pasted into a code editor.
    // It shows how to convert a array into c language code.
    char output1[1000] = "int sprite[";
    char output2[32];
    sprintf(output2, "%d", spritewidth);    
    strcat(output1,output2);
    strcat(output1,"][");
    sprintf(output2, "%d", spriteheight);    
    strcat(output1,output2);
    strcat(output1,"] = {\n");
    
    int x=0;int y=0;
    for (y=0;y<spriteheight;y++){

        for(x=0;x<spritewidth-1;x++){
            if(x==0)strcat(output1,"{");
            char num[16];
            sprintf(num, "%d", map[y][x]);    
            strcat(output1,num);
            strcat(output1,",");
        }
        char num2[16];
        sprintf(num2, "%d", map[y][spritewidth-1]);    
        strcat(output1,num2);
        if(y<spriteheight-1){
            strcat(output1,"},\n");
        }else{
        strcat(output1,"}};\n");
        }
    }

    SetClipboardText(output1);

}

void tilemapview(){
//    canvas.Color = Color.Black
//    canvas.DrawRect(tilemapx,tilemapy,tilemapwidth,tilemapheight)
    DrawRectangle(tilemapx,tilemapy,tilemapwidth,tilemapheight,BLACK);
//    canvas.Color = Color.White
    
    for(int y=0; y<tilemaptilesscreenvertical;y++){
    for(int x=0; x<tilemaptilesscreenhorizontal;x++){
        int pointx=(x*spritewidth*spritelibscale)+tilemapx;
        int pointy=(y*spriteheight*spritelibscale)+tilemapy;
        //canvas.DrawImage(spritelibim[tilemap[x,y]],pointx,pointy)			
        //DrawTexture(spritelibim[tilemap[x][y]].texture,pointx,pointy,WHITE);
        DrawTextureRec( spritelibim[tilemap[x][y]].texture,
                        (Rectangle){ 0, 0, 
                        spritelibim[tilemap[x][y]].texture.width, 
                        -spritelibim[tilemap[x][y]].texture.height }, 
                        (Vector2){pointx,pointy}, WHITE);
    }}
    
    if(IsMouseButtonDown(0)){
        if(rectsoverlap(GetMouseX(),GetMouseY(),1,1,tilemapx,tilemapy,tilemapwidth,tilemapheight)){
            int x=(GetMouseX()-tilemapx) / (spritewidth*spritelibscale);
            int y=(GetMouseY()-tilemapy) / (spriteheight*spritelibscale);
            if(x<0 || y<0 || x>=tilemaptileshorizontal || y>=tilemaptilesvertical){
            }else{
            tilemap[x][y] = spritelibselected;            
            }
        }
    }
}


void fillatposition(int x, int y,int oldcolor,int newcolor)
{   
    if(x<0 || x>=10 || y<0 || y>=10)return;
    if(map[x][y] == oldcolor){
        map[x][y] = newcolor;
        fillatposition(x-1,y,oldcolor,newcolor);
        fillatposition(x+1,y,oldcolor,newcolor);
        fillatposition(x,y-1,oldcolor,newcolor);
        fillatposition(x,y+1,oldcolor,newcolor);
        return;
    }

}
/*
	Method fillatposition(x:Int,y:Int)
		Local ol:List<pathnode> = New List<pathnode>
	 	' Add the start position on the list
	 	ol.AddLast(New pathnode(x,y))
	 	' set the cloes map at the start position to distance 1
	 	Local colorundermouse:Int=map[x,y]
	 	map[x,y] = paletteselected
	 	
	 	' some helper arrays. We can determine the top,right,and bottom
	 	' and left position cells with these numbers.
	 	Local dx:Int[] = New Int[](0,1,0,-1)
	 	Local dy:Int[] = New Int[](-1,0,1,0)
	 	' While there is contents in the list
	 	While ol.Count() <> 0
		 	
	 		' Get the current location
	 		Local x1:Int=ol.First.x
	 		Local y1:Int=ol.First.y
	 		' Remove the current location from the list
	 		ol.RemoveFirst()
	 		' Get 4 new positions around the current positions
			For Local i:=0 Until 4
				' Set new x and y
				Local nx:Int=x1+dx[i]
				Local ny:Int=y1+dy[i]
				' If the coordinates are inside the map
				If nx>=0 And ny>=0 And nx<spritewidth And ny<spriteheight
					' If the closedmap is not written to yet
		 			If map[nx,ny] = colorundermouse And map[nx,ny] <> paletteselected
		 				' Set the new distance based on the current distance
		 				map[nx,ny] = paletteselected
		 				' Add new position to the list
		 				ol.AddLast(New pathnode(nx,ny))
		 			End If
	 			End If
			Next
	 	Wend
 	 		
	End Method
*/

// For creating ovals(circles)
//
void midptellipse(int rx, int ry,  
                  int xc, int yc){ 

    // Erase the tempmap array.
    for(int y=0;y<32;y++){
        for(int x=0;x<32;x++){
            tempmap[x][y] = 0;
        }
    }

    float dx, dy, d1, d2, x, y; 
    x = 0; 
    y = ry; 
  
    // Initial decision parameter of region 1 
    d1 = (ry * ry) - (rx * rx * ry) +  
                     (0.25 * rx * rx); 
    dx = 2 * ry * ry * x; 
    dy = 2 * rx * rx * y; 
  
    // For region 1 
    while (dx < dy)  
    { 
  
        // Print points based on 4-way symmetry 
        int tx[4] = {x+xc,-x+xc,x+xc,-x+xc};
        int ty[4] = {y+yc,y+yc,-y+yc,-y+yc};
        for(int i=0;i<4;i++){
            if(tx[i]>=0 && tx[i]<32 && ty[i]>=0 && ty[i]<32)tempmap[tx[i]][ty[i]] = 1;
        }
        // Checking and updating value of 
        // decision parameter based on algorithm 
        if (d1 < 0) 
        { 
            x++; 
            dx = dx + (2 * ry * ry); 
            d1 = d1 + dx + (ry * ry); 
        } 
        else 
        { 
            x++; 
            y--; 
            dx = dx + (2 * ry * ry); 
            dy = dy - (2 * rx * rx); 
            d1 = d1 + dx - dy + (ry * ry); 
        } 
    } 
  
    // Decision parameter of region 2 
    d2 = ((ry * ry) * ((x + 0.5) * (x + 0.5))) +  
         ((rx * rx) * ((y - 1) * (y - 1))) - 
          (rx * rx * ry * ry); 
  
    // Plotting points of region 2 
    while (y >= 0) 
    { 
  
        int tx[4] = {x+xc,-x+xc,x+xc,-x+xc};
        int ty[4] = {y+yc,y+yc,-y+yc,-y+yc};
        for(int i=0;i<4;i++){
            if(tx[i]>=0 && tx[i]<32 && ty[i]>=0 && ty[i]<32)tempmap[tx[i]][ty[i]] = 1;
        }

        // Checking and updating parameter 
        // value based on algorithm 
        if (d2 > 0)  
        { 
            y--; 
            dy = dy - (2 * rx * rx); 
            d2 = d2 + (rx * rx) - dy; 
        } 
        else 
        { 
            y--; 
            x++; 
            dx = dx + (2 * ry * ry); 
            dy = dy - (2 * rx * rx); 
            d2 = d2 + dx - dy + (rx * rx); 
        } 
    } 
} 

bool rectsoverlap(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2){
    if((x1 >= (x2 + w2) || (x1 + w1) <= x2))return false;
    if((y1 >= (y2 + h2) || (y1 + h1) <= y2))return false;
    return true;
}
bool circlerectcollide(int cx,int cy,int cr,int rx,int ry,int rw,int rh){
    
    float closestx = Clamp(cx, rx, rx+rw);
    float closesty = Clamp(cy, ry, ry+rh);
    float distancex = cx - closestx;
    float distancey = cy - closesty;
    float distancesquared = (distancex * distancex) + (distancey * distancey);
    return distancesquared < (cr * cr);
}

// Clamp float value
float Clamp(float value, float min, float max)
{
    const float res = value < min ? min : value;
    return res > max ? max : res;
}

void copyfromclipboard(){
    for(int y=0;y<32;y++){
    for(int x=0;x<32;x++){
        tempmap[x][y]=0;
    }
    }

    // Create a char to read the clipboard with.
    char clipBoardTxt[10024];
    // Create a pointer to the clipboardtxt char
    const char *clipPointer = clipBoardTxt;
    // Read the contents of the clipboard into the clipboardtxt pointer
    clipPointer = GetClipboardText();
    // create a char 
    char work[1024];
    
    if(strlen(clipPointer)<1000){
    // copy the contents of the char that has a pointer containing the clipboard txt into this work char
    strcpy(work,clipPointer);
    }
    // count the comma's and the numbers.
//    int numCommas = countcommas(work);
    int numNumbers = countnumbers(work);

    if(importspriteis8x8(numNumbers)){
        readtempsprite(8,8,work);
        
        //create8x8sprite();
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
            char charnum[14]="";
            
            //while(in[i]!=','){                
            do{    
                charnum[z]=in[i];
                i++;
                z++;
                if(i+1>=strlen(in))break;//no longer numbers than 3
            }while(in[i]!=',');
            
            num = atoi(charnum);
             
            cnt++;
            if(cnt>1){
                y++;
                if(y>=h){
                    y=0;
                    x++;
                }                
            }
            
            if(num>=numpalette)num=0;
            //tempsprite[x][y] = num;//in[i]-'0';
            map[x][y] = num;//in[i]-'0';
            //if(num>15)map[x][y]=0;
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
        if(in[i] >= '0' && in[i] <= '9'){
            while(in[i] >= '0' && in[i] <= '9' && i<strlen(in)){
                i++;
            }
            out++;        
        }
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
