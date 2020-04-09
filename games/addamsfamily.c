//
// This is based on a level from a Commodore Amiga game from the 90's called Addams Family. This 
// was a platformer game.
//
// This remake was originally written around the 00's in Blitz Basic. It was ported to
// Raylib and C in April 2020.
//


#include "raylib.h"
#include <math.h>

#define MAX_AI 100
#define MAX_BULLETS 100
#define MAX_MONEY 100
#define MAX_ROT 100

static int map[300][300];

static const int screenWidth = 640;
static const int screenHeight = 480;

//static int weasel; //debug

typedef struct game{
    RenderTexture2D back;
    int offx;
    int offy;
    int cx;
    int cy;
    //demo
    int sdir;
    int editor;
}game;

typedef struct player{
	float x;
    float y;
    int w;
    int h;
    int ow;
    int oh;
    int duckwidth;
    int duckheight;
	int jump;
    float fallspeed;
    int fall;
	int direction;
    int slide;
    float slidespeed;
	int state; //; stand,duck
	int money;
    int cheat;
    int lives;
    int hits;
	int blinking;
    double blinkingdelay;
    int blink;
    double blinktimer;
}player;

typedef struct ai{
	bool active;
    float x;
    float y;
    float velx;
    float vely;
    int w;
    int h;
	int kind;
	double firedelay;
    double jumpdelay;
	int jump;
    float fallspeed;
}ai;

typedef struct bullet{
    bool active;
	float x;
    float y;
    int w;
    int h;
	int state;
	int timer1;
	float velx;
    float vely;
	int timeout;
}bullet;

typedef struct money{
    bool active;
	float x;
    float y;
    float fallspeed;
}monkey;

typedef struct rot{
    bool active;
	float ang;
	float sine;
	int angdir;
    double timedelay;
    int cnt;
	float chainx[3];
    float chainy[3];
	float ballx;
    float bally;
}rot;


static struct bullet arr_bullet[MAX_BULLETS]={0};
static struct ai arr_ai[MAX_AI]={0};
static struct money arr_money[MAX_MONEY]={0};
static struct rot arr_rot[MAX_ROT] = {0};
static game g = {0};
static player p = {0};

static bool circlerectcollide(int cx,int cy,int cr, int rx, int ry,int rw,int rh);
static float Clamp(float value, float min, float max);
static bool RectsOverlap(int x1,int y1,int w1,int h1,int x2,int y2,int w2,int h2);
static void readlevel(int level);
static void drawlevel(void);
static void drawplayer(void);
static int maprealx(void);
static int maprealy(void); 
static void inigame(void);
static void gravity(void);   
static bool rectmapcollision(int x, int y, int w, int h, int offx, int offy);
static bool pmcollision(int offx,int offy);
static void playercontrols(void);
static bool playerright(void);
static bool playerleft(void);
static bool playerduck(void);
static void playerslide(void);
static void mapscroll(void);
static void scrolllevel(int num);
static void iniai(int x, int y, int kind);
static void drawai(void);
static void jumpai(int num);
static void updateai(void);
static void drawbullets(void);
static void updatebullets(void);
static void inibullet(int x,int y,int dir);
static void drawmoney(void);
static void drawmoneyimage(int x,int y);
static void inimoney(int x,int y);
static void updatemoney(void);static void drawrot(void);
static void updaterot(void);
static void inirot(int x,int y);
static void playerblinkingeffect(void);
static void drawhud(void);
static void drawlifeimage(int x,int y);
static void playermoneycollision(void);    
static void playeraikill(void);
static void playerdecreasehit(void);
static void rotplayerkill(void);
static void spikesplayerkill(void);
static void bulletsplayerkill(void);
static void aiplayerkill(void);

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------

    InitWindow(screenWidth, screenHeight, "raylib example.");
    //ToggleFullscreen();
    
    g.back = LoadRenderTexture(640,480);    

    inigame();

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        updateai();    
        updatebullets();
        updaterot();
        gravity();        
        playeraikill();
        aiplayerkill();
        rotplayerkill();
        spikesplayerkill();
        bulletsplayerkill();
        playerblinkingeffect();
        updatemoney();
        playermoneycollision();
        playercontrols();
        playerslide();
        mapscroll();
        //----------------------------------------------------------------------------------
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(BLACK);
                
            BeginTextureMode(g.back);
            ClearBackground(BLACK);
            drawbullets();
            drawmoney();
            drawrot();
            drawlevel();
            drawai();
            drawplayer();
            EndTextureMode();
            //DrawTextureEx(g.back.texture,-32,0,WHITE);
            DrawTextureRec(g.back.texture, (Rectangle){ 0, 0, g.back.texture.width, -g.back.texture.height }, (Vector2){ -32, 0 }, WHITE);
            drawhud();
            // Some debug things...
            //DrawText(FormatText("%i",weasel),0,0,20,RED);
            //if(rectmapcollision(GetMouseX(),GetMouseY(),32,32,0,0)){
            //DrawText(FormatText("%i",g.cx),100,0,20,RED);
            //if(pmcollision(0,0)){
            //    DrawText("ewkfh",0,0,20,RED);
            //    
           // }
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadRenderTexture(g.back);
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;


}

void readlevel(int level){

    int level1suba[15][70] = {
    {0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
    {0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
    {0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
    {0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
    {0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
    {0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
    {0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,6,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
    {0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,6,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
    {0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
    {0,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0 },
    {0,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0 },
    {0,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0 },
    {0,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0 },
    {0,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,3,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,5,5,5,5,1,1,1,1,1,1,1,1,1,1,1,5,5,5,5,1,1,1,1,1,1,1,0,0,0,0 },
    {0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 }};
   int level1subb[15][70] = {
    {0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1},
    {0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1},
    {0,1,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,0,0,0,1,1,1,1,1,0,0,0,0,1},
    {0,1,0,0,0,0,0,0,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,0,0,0,0,1,1,1,0,0,0,0,0,1},
    {0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,1,0,0,0,0,0,0,4,0,0,0,0,0,0,1},
    {0,1,0,6,6,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,0,0,0,0,0,0,0,0,0,4,0,0,0,0,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {0,1,0,6,6,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,6,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,6,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,1},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,5,5,5,5,1,1,1,1,1,5,5,5,5,1,1,1,1,1,1,1,1,1,1,1,1,1,6,6,6,1},
    {0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}};

	//Restore level1suba
	for(int y=0; y<15 ; y++){
	for(int x=0; x<70 ; x++){
		//Read a
		int a = level1suba[y][x];
        switch (a)
            {
            case 1:
            map[x][y] = a;
            break;
            case 2: //; ai - shooter
            iniai(x*32,y*32-(48-32),0);
            break;
            case 3: //; ai - walker
            iniai(x*32,y*32-32,1);
            break;
            case 4: //; rotating thing
            inirot(x*32,y*32-32);
            break;
            case 5: //; spikes
            map[x][y] = 2;
            break;       
            case 6: //; money
            inimoney(x*32,y*32);
            break;
		}
	}}

 

	//Restore level1subb
	for(int y=0; y<15 ; y++){
	for(int x=0; x<70 ; x++){
		int x1 = x + 69;
		int y1 = y; 
		int a = level1subb[y][x];
		switch (a){
            case 1:
            
            map[x1][y1] = a;
            break;
            case 2: //; ai - shooter
            iniai(x1*32,y1*32-(48-32),0);
            break;
            case 3: //; ai - walker
            iniai(x1*32,y1*32-32,1);
            break;
            case 4: //; rotating thing
            inirot(x1*32+16,y1*32-32);
            break;
            case 5: //; spikes
            map[x1][y1] = 2;
            break;
            case 6: //; money
            inimoney(x1*32,y1*32);
            break;
        }
    }}

}

void drawlevel(){
    int x1;
    int y1;
	for (int y=0;y<15;y++){
	for (int x=0;x<20;x++){
		if(RectsOverlap((g.cx)+x,(g.cy)+y,1,1,0,0,160,15)){
            switch (map[x+g.cx][y+g.cy]){
                case 1:                
                DrawRectangle((x*32)+g.offx,(y*32)+g.offy,32,32,(Color){255,255,255,255});
                break;
                case 2:
                x1 = (x*32)+g.offx;
                y1 = (y*32)+g.offy;                
                DrawLine(x1,y1+32,x1+16,y1,(Color){255,255,255,255});
                DrawLine(x1+16,y1,x1+32,y1+32,(Color){255,255,255,255});
                break;
            }
		}
	}}
}

void drawplayer(){
	if(p.blink==false)return;
	switch (p.state){
		case 0: //; stand
			//Color 0,0,255
            DrawRectangle(p.x-maprealx(),p.y-maprealy(),p.ow,p.oh,(Color){0,0,255,255});
            break;
		case 1: //; duck
			//Color 0,0,255
			DrawRectangle(p.x-maprealx(),p.y+48-maprealy(),p.duckwidth,p.duckheight,(Color){0,0,255,255});
            break;
	}
}


int maprealx(){
	int a = g.cx * 32;
	int b = g.offx;
	return a-b;
}
int maprealy(){
	int a = g.cy * 32;
	int b = g.offy;
	return a-b;
}

void inigame(){
	//;p\x = 3180
    for(int i=0;i<MAX_AI;i++){
        arr_ai[i].active=false;
    }
    for(int i=0;i<MAX_BULLETS;i++){
        arr_bullet[i].active=false;
    }
    for(int i=0;i<MAX_MONEY;i++){
        arr_money[i].active=false;
    }
   for(int i=0;i<MAX_ROT;i++){
        arr_rot[i].active=false;
    }


	//freegame
	p.x = 190;
	p.y = 58+48;
	p.w = 48 ;
    p.ow = 48;
    p.duckwidth = 48;
	p.h = 80;
    p.oh = 80;
    p.duckheight = 32;
	p.hits = 3;
	p.lives = 1;    
	p.blinkingdelay = GetTime() + 5;
	p.blinktimer = 0.05f;
	p.blink = true;
    p.fallspeed = 0;
	g.cx = 0;//;33//;;88
	g.offx = 0;
	g.offy = 0;
	g.editor = false;
	readlevel(1);
}

void gravity(){
	if(p.jump == true && pmcollision(0,p.fallspeed) == true){
		p.fall = true;
		p.jump = false;
		p.fallspeed = 2;
    }
	if(pmcollision(0,2) == false && p.fall == false && p.jump == false){
		p.fall = true;
		p.jump = true;
		p.fallspeed = 2;
	}
	if(p.jump == true || p.fall == true){
        
		if(p.fallspeed > 0) p.fall = true;
		p.fallspeed = p.fallspeed + 0.4f;
		p.y += p.fallspeed;
   		for(int i=1;i<(int)p.fallspeed;i++){
            if(pmcollision(0,i)){               
                while (pmcollision(0,i)){
                    p.y=p.y - 1;
                }
                p.fall = false;
                p.jump = false;
                break;
                
            }
		}
       
        
	}
}

bool pmcollision(int offx,int offy){
	switch (p.state){
		case 0:
        return rectmapcollision(p.x,p.y,48,80,offx,offy);
		break;
        case 1:
        return rectmapcollision(p.x,p.y,48,80,offx,offy);
        break;
    }
    return false;
}

bool rectmapcollision(int x, int y,int w, int h, int offx, int offy){
	int px = (x) / 32;
	int py = (y) / 32;
	for(int y1 = -3;y1<4;y1++){
	for(int x1 = -3;x1<3;x1++){
		if(RectsOverlap(px+x1,py+y1,1,1,0,0,70*2,15)){
			if(map[px+x1][py+y1] == 1){
				if(RectsOverlap(x+offx,y+offy,w,h,(px+x1)*32,(py+y1)*32,32,32)){
					return true;
				}
			}
		}
	}}
    return false;
}

bool RectsOverlap(int x1,int y1,int w1,int h1,int x2,int y2,int w2,int h2){
    if(x1 >= (x2 + w2) || (x1 + w1) <= x2) return false;
    if(y1 >= (y2 + h2) || (y1 + h1) <= y2) return false;
    return true;
}



void playercontrols(){
    //;	p\state=0
	p.w = p.ow;
	p.h = p.oh;
	if(playerright()){
        if(pmcollision(4,0) == false){
            p.direction  = 0;
            p.slide = true;
            p.slidespeed = 2;
            p.x=p.x+4;
        }
    }
	if(playerleft()){
        if(pmcollision(-4,0) == false){
            p.direction = 1;
            p.slide = true;
            p.slidespeed = 2;            
            p.x=p.x-4;
        }
    }
	//if KeyDown(200) = True Or KeyDown(57) = True
    if(IsKeyDown(KEY_SPACE)){
        if(p.fall == false && p.jump == false){
            p.jump = true;
            p.fallspeed = -11.5;
        }
    }
	
    if(playerduck()==true){
    if(p.jump == false && p.fall == false){
        p.state = 1;
        p.w = p.duckwidth;
        p.h = p.duckheight;
    }
    }else{
        p.state = 0;
	}
}

bool playerright(){
	if(IsKeyDown(KEY_RIGHT))return true;
    return false;
}
bool playerleft(){
	if(IsKeyDown(KEY_LEFT))return true;
    return false;
}
bool playerduck(){
	if(IsKeyDown(KEY_DOWN))return true;
    return false;
}

void playerslide(){
	if(p.slide){
        //;	p\state = 0
        if(p.jump){
            p.slide = false; 
            return;
        }
        if(p.fall){
            p.slide = false;
            return;
        }
        switch (p.direction){
            case 0:
                if(playerright()==false){
                    if(pmcollision(p.slidespeed,0)== false){
                        p.x = p.x + p.slidespeed;
                        p.slidespeed = p.slidespeed - .1;
                    }else{
                        p.x = p.x/32*32;
                        p.slide = false;
                        p.slidespeed = 0;
                    }
                }
            break;
            case 1:
                if(playerleft() == false){
                    if(pmcollision(-p.slidespeed,0) == false){
                        p.x = p.x - p.slidespeed; 
                        p.slidespeed = p.slidespeed - .1;
                        }else{
                        p.x = p.x/32*32;
                        p.slide = false;
                        p.slidespeed = 0;
                    }
                }
            break;
        }
    }   
	if(p.slidespeed < 0)p.slide = false;
}

void mapscroll(){
	int x = p.x - maprealx();
	//;
	if(playerright() == false && playerleft() == false){
        if(p.slide){
            switch (p.direction){
                case 0:
                for(int i=0;i<p.slidespeed-1;i++){
                    scrolllevel(0);
                }
                break;
                case 1:
                for(int i=0;i<p.slidespeed-1;i++){
                    scrolllevel(1);
                }
                break;
            }
        }
    }	;
	if(x+40> screenWidth / 2 && playerright()){
		if(g.cx < 70*2-20){
		scrolllevel(0);
        scrolllevel(0);
        scrolllevel(0);
        scrolllevel(0);
        }
    }
	if(x-40< screenWidth / 2 && playerleft()){
		if(g.cx > 0){
			scrolllevel(1);
            scrolllevel(1);
            scrolllevel(1);
            scrolllevel(1);
        }
    }
}


void scrolllevel(int num){
	switch (num){
		case 0:// ; right
		g.offx = g.offx - 1;
		if(g.offx < 0){
            g.offx = 31;
            g.cx = g.cx + 1;
        }
        break;
		case 1:	
		g.offx = g.offx + 1;
		if(g.offx > 31){
            g.offx = 0;
            g.cx = g.cx - 1;
        }
        break;
    }
}

void iniai(int x, int y, int kind){
    for(int i=0;i<MAX_AI;i++){
        if(arr_ai[i].active==false){
            arr_ai[i].active = true;
            arr_ai[i].x = x;
            arr_ai[i].y = y;
            switch (kind){
                case 0:
                    arr_ai[i].w = 48;
                    arr_ai[i].h = 48;
                    arr_ai[i].kind = 0;
                    arr_ai[i].firedelay = GetTime() + (200/1000);
                    arr_ai[i].jumpdelay = (int)GetTime() + (4800/1000);
                break;
                case 1:
                arr_ai[i].w = 48;
                arr_ai[i].h = 64;
                arr_ai[i].kind = 1;
                arr_ai[i].velx = 1.7;
                break;
            }
            return;
        }
    }
}

void drawai(){
	for(int i=0;i<MAX_AI;i++){
        if (arr_ai[i].active){
            switch (arr_ai[i].kind){
                case 0:		
                DrawRectangle(arr_ai[i].x-maprealx(),arr_ai[i].y-maprealy(),48,48,(Color){255,0,0,255});
                break;
                case 1:
                DrawRectangle(arr_ai[i].x-maprealx(),arr_ai[i].y-maprealy(),48,64,(Color){255,0,0,255});	
                break;
            }
        }
	}
}

void jumpai(int num){
	if(arr_ai[num].jump == false) return;
	arr_ai[num].y += arr_ai[num].fallspeed;
	arr_ai[num].fallspeed += 0.1f;
	for(int i=0;i<arr_ai[num].fallspeed;i++){
        if(rectmapcollision(arr_ai[num].x,arr_ai[num].y,48,48,0,i)){
            arr_ai[num].y = arr_ai[num].y/32*32-4;
            arr_ai[num].jump = false;
            break;
        }
    }
}

void updateai(){
	for(int i=0;i<MAX_AI;i++){ 
        if(arr_ai[i].active){            
		switch (arr_ai[i].kind){
            case 0:
            
            if(arr_ai[i].jumpdelay < GetTime()){                
                arr_ai[i].jump = true;
                arr_ai[i].fallspeed = -4;
                arr_ai[i].jumpdelay = GetTime() + 4800/1000;
            }
            
            if(arr_ai[i].firedelay < GetTime()){
                inibullet(arr_ai[i].x,arr_ai[i].y,1);                
                arr_ai[i].firedelay = GetTime() + 4800/1000;
            }
            jumpai(i);
            break;
            case 1:
            arr_ai[i].x += arr_ai[i].velx;
            if(rectmapcollision(arr_ai[i].x,arr_ai[i].y,48,64,arr_ai[i].velx,0)){
                arr_ai[i].velx = -arr_ai[i].velx;
            }
            break;
        }
		}
	}
}

void drawbullets(){
	for(int i=0;i<MAX_BULLETS;i++){
        if(arr_bullet[i].active){
            
		//Color 255,255,0
		//Oval this\x-maprealx(),this\y-maprealy(),48,48,True
        DrawCircle(arr_bullet[i].x-maprealx(),arr_bullet[i].y-maprealy(),48/2,(Color){255,255,0,255});
        }
    }
}

void updatebullets(){
	for(int i=0;i<MAX_BULLETS;i++){
        if(arr_bullet[i].active){
            switch (arr_bullet[i].state){
                case 0:                 
                if(arr_bullet[i].timer1 + (1500/1000) < GetTime() )arr_bullet[i].state = 1;
                break;
                case 1:                
                arr_bullet[i].velx += .2;
                if(arr_bullet[i].velx > 5)arr_bullet[i].state = 2;
                break;
                case 2:                
                break;
            }
            arr_bullet[i].x += arr_bullet[i].velx;
            arr_bullet[i].y += arr_bullet[i].vely;
            if(arr_bullet[i].timeout < GetTime())arr_bullet[i].active=false;
        }
	}
}

void inibullet(int x,int y,int dir){
	for(int i=0;i<MAX_BULLETS;i++){
        if(arr_bullet[i].active==false){
            
            arr_bullet[i].active=true;
            arr_bullet[i].x = x;
            arr_bullet[i].y = y;
            arr_bullet[i].w = 48;
            arr_bullet[i].h = 48;
            arr_bullet[i].state=0;
            arr_bullet[i].timer1 = GetTime();
            arr_bullet[i].timeout = GetTime() + 3800/1000;
            if(dir == 0)arr_bullet[i].velx = 6;
            if(dir == 1)arr_bullet[i].velx = -6;
            return;
        }
    }
}

void drawmoney(){
	for(int i=0;i<MAX_MONEY;i++){
        if(arr_money[i].active){
            drawmoneyimage(arr_money[i].x-maprealx(),arr_money[i].y-maprealy());
        }
	}
}

void drawmoneyimage(int x,int y){
	//Color 0,0,0
	//Oval x-1,y-1,34,34,True
    DrawCircle(x-1+10,y-1+15,34/2,(Color){0,0,0,255});
	//Color 255,255,0
	DrawCircle(x+10,y+15,32/2,(Color){255,255,0,255});
	//Color 0,0,0
	//SetFont font\money
	DrawText("$",x+6,y,32,BLACK);
	//SetFont font\normal
}

void inimoney(int x,int y){
    for(int i=0;i<MAX_MONEY;i++){
        if(arr_money[i].active==false){
            arr_money[i].active=true;
            arr_money[i].x = x;
            arr_money[i].y = y;
            arr_money[i].fallspeed = GetRandomValue(-1,0);
            return;
        }
    }
}

void updatemoney(){
	for(int i=0;i<MAX_MONEY;i++){
        if(arr_money[i].active){
            arr_money[i].y += arr_money[i].fallspeed;
            arr_money[i].fallspeed += .09;
            if(arr_money[i].fallspeed > 1){
                arr_money[i].fallspeed = -1;
                arr_money[i].y = arr_money[i].y/32*32;
            }
        }
	}
}

void updaterot(){
    
	for(int i=0;i<MAX_ROT;i++){
        if(arr_rot[i].active){
            int add=0;
            arr_rot[i].sine += 0.03f;
            float mth=1.90f*sin(arr_rot[i].sine);
            for(int a=0;a<3;a++){
                float angle=(mth);
                arr_rot[i].chainx[a] = arr_rot[i].ballx+((add)*sin(angle));
                arr_rot[i].chainy[a] = arr_rot[i].bally+((add)*cos(angle));
                add=add+48;
                if(a==2)add+=32;
           }

        }
			
	}
}

void drawrot(){
	for(int i=0;i<MAX_ROT;i++){
        if(arr_rot[i].active){
            //Color 0,0,255
            //MidHandle gfx\smallball
            for(int ii=0;ii<2;ii++){
                DrawCircle(arr_rot[i].chainx[ii]-maprealx(),arr_rot[i].chainy[ii]-maprealy(),16,(Color){0,0,255,255});
            }
            //MidHandle gfx\ball
            DrawCircle(arr_rot[i].chainx[2]-maprealx(),arr_rot[i].chainy[2]-maprealy(),32,(Color){0,0,255,255});
        }
	}
}

void inirot(int x,int y){
    
	for(int i=0;i<MAX_ROT;i++){
        if(arr_rot[i].active==false){
            arr_rot[i].active = true;
            arr_rot[i].ang = 90;
            arr_rot[i].ballx = x;
            arr_rot[i].bally = y+48;
            arr_rot[i].chainx[0] = 0.0f;
            arr_rot[i].chainy[0] = 0.0f;
            arr_rot[i].chainx[1] = 0.0f;
            arr_rot[i].chainy[1] = 0.0f;
            arr_rot[i].chainx[2] = 0.0f;
            arr_rot[i].chainy[2] = 0.0f;
            
            return;
        }
    }
}

void playerblinkingeffect(){
    
	if(p.blinkingdelay > GetTime()){

		if(p.blinktimer < GetTime()){
            //weasel=14298;
			if(p.blink==true){
                p.blink = false;
                }else{
                p.blink = true;
            }            
            p.blinktimer = GetTime() + 0.2f;;            
        }
			
		}else{
            
		                
        p.blink = true ;
	}

}

void drawhud(){
	//; hits
	int cnt = 0;
	for(int y=100;y>0;y-=34){
		if(p.hits > cnt)drawlifeimage(32,y);
		cnt += 1;
	}
	//;
	//; lives
	//SetFont font\hud
	//Color 155,155,155
	DrawText("Lives",10,screenHeight-128,28,(Color){155,155,155,255});

	//If Len(p\lives) = 1 Then Return "0"+p\lives Else Return p\lives
    
    
	DrawText(FormatText("0 %01i",p.lives),28,screenHeight-100,30,(Color){155,155,155,255});
	//; money
	//Color 155,155,155
	drawmoneyimage(32,screenHeight-70);
	//SetFont font\hud
	DrawText(FormatText("0 %01i",p.lives),28,screenHeight-42,30,(Color){155,155,155,255});
	//SetFont font\normal
}


void drawlifeimage(int x,int y){
	//Color 255,0,0
	DrawCircle(x,y,16,(Color){255,0,0,255});
	//Color 5,5,5
	//SetFont font\money
	DrawText("H",x-6,y-10,24,(Color){5,5,5,255});
	//SetFont font\normal
}

void playermoneycollision(){
	for(int i=0;i<MAX_MONEY;i++){
        if(arr_money[i].active){
            if(RectsOverlap(p.x,p.y,p.w,p.h,arr_money[i].x,arr_money[i].y,32,32)){ 
                p.money += 1;
                arr_money[i].active = false;
            }            
        }
	}
}

void playeraikill(){
	if(p.fall){
		for(int i=0;i<MAX_AI;i++){
			if(RectsOverlap(p.x,p.y,48,80,arr_ai[i].x,arr_ai[i].y,arr_ai[i].w,arr_ai[i].h)){
                arr_ai[i].active = false;
			}
		}
	}
}

void playerdecreasehit(){
	p.hits -= 1;
	p.blinkingdelay = GetTime() + 3;
	if(p.hits == 0){
		p.hits = 3;
		p.lives -= 1;
		if(p.lives == 0)inigame();
	}
}

void rotplayerkill(){
	if(p.blinkingdelay > GetTime())return;
	if(p.cheat)return;
	for(int i=0;i<MAX_ROT;i++){
        if(arr_rot[i].active){
            if(p.state == 1){
                if(circlerectcollide(arr_rot[i].chainx[2],arr_rot[i].chainy[2],32,p.x,p.y+(80-32),p.w,p.h)){
                    playerdecreasehit();
                }
            }else{
                if(circlerectcollide(arr_rot[i].chainx[2],arr_rot[i].chainy[2],32,p.x,p.y,p.w,p.h)){
                    playerdecreasehit();
                }
            }
        }
	}
}


bool circlerectcollide(int cx,int cy,int cr, int rx, int ry,int rw,int rh){
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

void spikesplayerkill(){
	if(p.cheat)return;
	if(p.blinkingdelay > GetTime())return;
	int x1 = p.x / 32;
	int y1 = (p.y+64) / 32;
	if(RectsOverlap(x1,y1,1,1,0,0,170,15)){
		if(map[x1][y1] == 2)playerdecreasehit();
	}
}


void bulletsplayerkill(){
	if(p.cheat)return;
	if(p.blinkingdelay > GetTime())return;
	for(int i=0;i<MAX_BULLETS;i++){
		switch (p.state){
		case 0:
		if(RectsOverlap(p.x,p.y,p.w,p.h,arr_bullet[i].x,arr_bullet[i].y,arr_bullet[i].w,arr_bullet[i].h)){
			playerdecreasehit();
		}
        break;
		case 1:
		if(RectsOverlap(p.x,p.y,p.w,p.h,arr_bullet[i].x,arr_bullet[i].y+(80-32),arr_bullet[i].w,arr_bullet[i].h)){
			playerdecreasehit();
		}
        break;
		}
	}
}

void aiplayerkill(){
	if(p.blinkingdelay > GetTime())return;
    for(int i=0;i<MAX_AI;i++){
        if(arr_ai[i].active){
            if(RectsOverlap(p.x,p.y,p.w,p.h,arr_ai[i].x,arr_ai[i].y,arr_ai[i].w,arr_ai[i].h)){
                playerdecreasehit();
            }
        }
    }
}
