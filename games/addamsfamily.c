
#include "raylib.h"
#include <math.h>

static int map[270][15];

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
    int blinkingdelay;
    int blink;
    int blinktimer;
}player;

static game g = {0};
static player p = {0};

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
static bool pmcollision2(int offx,int offy);
static void playercontrols(void);
static bool playerright(void);
static bool playerleft(void);
static bool playerduck(void);
    
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 640;
    const int screenHeight = 480;

    InitWindow(screenWidth, screenHeight, "raylib example.");
 
    
    g.back = LoadRenderTexture(640,480);    

    inigame();
 
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
            
        gravity();
        playercontrols();
        //----------------------------------------------------------------------------------
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(BLACK);
                
       //     BeginTextureMode(g.back);
       //     ClearBackground(BLACK);
            drawlevel();
            drawplayer();
       //     EndTextureMode();
            //DrawTextureEx(g.back.texture,-32,0,WHITE);
       //     DrawTextureRec(g.back.texture, (Rectangle){ 0, 0, g.back.texture.width, -g.back.texture.height }, (Vector2){ 0, 0 }, WHITE);
            
            //if(rectmapcollision(GetMouseX(),GetMouseY(),32,32,0,0)){
            if(pmcollision(0,0)){
                DrawText("ewkfh",0,0,20,RED);
                
            }
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
            //iniai(x*32,y*32-(48-32),0)
            break;
            case 3: //; ai - walker
            //iniai(x*32,y*32-32,1)
            break;
            case 4: //; rotating thing
            //inirot(x*32,y*32-32)
            break;
            case 5: //; spikes
            map[x][y] = 2;
            break;       
            case 6: //; money
            //inimoney(x*32,y*32)
            break;
		}
	}}

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

	//Restore level1subb
	for(int y=0; y<15 ; y++){
	for(int x=0; x<70 ; x++){
		int x1 = x + 69;
		int y1 = y; 
		int a = level1subb[y][x];
		switch (a){
            case 1:
            break;
            map[x1][y1] = a;
            case 2: //; ai - shooter
            //iniai(x1*32,y1*32-(48-32),0)
            break;
            case 3: //; ai - walker
            //iniai(x1*32,y1*32-32,1)
            break;
            case 4: //; rotating thing
            //inirot(x1*32+16,y1*32-32)
            break;
            case 5: //; spikes
            map[x1][y1] = 2;
            break;
            case 6: //; money
            //inimoney(x1*32,y1*32-32)
            break;
        }
    }}
}

void drawlevel(){
    int x1;
    int y1;
	for (int y=0;y<14;y++){
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
	p.blinkingdelay = GetTime() + 3000;
	p.blinktimer = 50;
	p.blink = true;

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

bool pmcollision2(int offx,int offy){
	switch (p.state){
		case 0:
        return rectmapcollision(GetMouseX(),GetMouseY(),48,80,offx,offy);
		break;
        case 1:
        return rectmapcollision(GetMouseX(),GetMouseY(),48,80,offx,offy);
        break;
    }
    return false;
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
