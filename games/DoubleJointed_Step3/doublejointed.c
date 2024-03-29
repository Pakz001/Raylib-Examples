/*******************************************************************************************
*
*   Double Jointed - Double Dragon parody. - beat 'em up game
*   
*   *Added zorder drawing function (needs more testing..) other sprite drawing functions can be removed
*   // debugging phase sill in progress above...  
*   Added oneshot and loop anims to entities.. fixed damage anims.
*
*      
*
********************************************************************************************/

#include "raylib.h"
#include "math.h"

#define MAX_FRAME_SPEED     15
#define MIN_FRAME_SPEED      1

#define stateFlee 1
#define stateAttack 2
#define stateChase 3
#define stateIdle 4
#define stateSawplayer 5
#define stateDead 6
#define statemoveto 7 // for scattering if they are clumped up together.

#define animIdle 0
#define animKick  1
#define animHit1 2
#define animUcut 3
#define animHit2 4
#define animWalk 5
#define animDamage 6
#define animFlying 7
#define animNothing 8

#define MAX_SPRITES 1000
#define MAX_PLAYERS 1 
#define MAX_ENTITIES 3
#define MAX_ITEMS 3

typedef struct item{
    bool active;
    float facing;
    bool blinking;
    int blinkingtime;
    int blinkingcnt;
    bool visible; // sprite is drawn or not drawn.
    Vector2 position;
    int currentFrame;
    Rectangle frameRec;
    float angle;
    float incx;
    float incy;
    float shadey;
}item;

static struct item it[MAX_ITEMS];


typedef struct entity{
    bool active;
    float facing; //-1 left, 1 right
    Vector2 position;
    int w;
    int h;
    float health;
    bool blinking;
    int blinkingtime;
    int blinkingcnt;
    bool visible; // sprite is drawn or not drawn.
    
    bool flying; // for when nocked flying
    float flyingincx;
    float flyingincy;
    Vector2 target;
    int state;
    float shadey;//shadow coordinate for when physics. bottom of sprites last/start position
    int frameSpeed;
    int damagedelay;
    int currentFrame;
    int currentAnim; // which animation is currenty active
    bool animLoop;
    bool animOneshot;
    int lastAnim;
    int framesCounter;
    int frame_start;
    int frame_end;
    int lastFiretime;
    int lastFireAnim;
    Rectangle frameRec;
    int mod; //collumn of sprites (+5 = enemy1 +9 = enemy headless)
}entity;

static struct entity e[MAX_ENTITIES];


typedef struct player{
    bool active;
    float facing; //-1 left, 1 right
    Vector2 position;
    int w;
    int h;
    int enemyweareatattacking;
    int enemyattheright; // only 1 enemy at every side (integer of entity number)
    int enemyattheleft;
    int hitcombo;
    int score;
    int health;
    int currentFrame;
    int currentAnim; // which animation is currenty active
    int lastAnim;
    int framesCounter;
    int frame_start;
    int frame_end;
    int keynothingtime; // when key is down this is zero (used to disable animations..)
    int lastFiretime;
    int lastFireAnim;
    
}player;

static struct player p[MAX_PLAYERS];


// controls for player (true/false) ,,add function to read keyboard/joypad/touchscreen
bool LEFT;
bool RIGHT;
bool UP;
bool DOWN;
bool FIRE1;
bool FIRE2;

// currentframe hold the current position of the cells (typewriter style)
int currentFrame;
int framesSpeed = 8;            // Number of spritesheet frames shown by second

Rectangle frameRec = { 0.0f, 0.0f, (float)96, (float)96 };


int frame_start = 0;
int frame_end = 0;

int frame_kickstart = 1;
int frame_kickend = 3;

int frame_hit1start = 16;
int frame_hit1end = 18;

int frame_ucutstart = 31;
int frame_ucutend = 33;

int frame_hit2start = 46;
int frame_hit2end = 48;

int frame_walkstart = 61;
int frame_walkend = 62;

int frame_damagestart = 76;
int frame_damageend = 77;

int frame_flyingstart = 91;
int frame_flyingend = 93;

int frame_idlestart = 1;
int frame_idleend = 1;


// this sets the frame to start and sets start and end position(loop)
void updateplayer(int player);
void setplayeranimation(int player, int anim);
void playercontrols(int player);
void setentityanimation(int entity, int anim);
void updateentity(int entity);
void entityattack(int entity);
void updateitems();
void drawZordered(); //Draw every sprite z sorted infront/back from the feet
// Our rectsoverlap function. Returns true/false.
static bool rectsoverlap(int x1,int y1,int w1,int h1,int x2,int y2,int w2,int h2);
float distance(float x1,float y1,float x2,float y2);

Texture2D scarfy;
Texture2D backg;

int debug;

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 940;
    const int screenHeight = 720;

    InitWindow(screenWidth, screenHeight, "raylib [texture] example - texture rectangle");

    // NOTE: Textures MUST be loaded after Window initialization (OpenGL context is required)
    //Texture2D scarfy = LoadTexture("resources/mrskelly.png");        // Texture loading
    //Texture2D backg = LoadTexture("resources/onedragon.png");
    scarfy = LoadTexture("resources/mrskelly.png");        // Texture loading
    backg = LoadTexture("resources/onedragon.png");

    Vector2 position = { 350.0f, 480.0f };
    //Rectangle frameRec = { 0.0f, 0.0f, (float)96, (float)96 };
    //int currentFrame = frame_kickstart;

    int framesCounter = 0;

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    //setanimation(animKick);
    //setanimation(animFlying);
    it[0].active=false;
    it[0].frameRec = (Rectangle){ 0.0f, 0.0f, (float)96, (float)96 };
    it[1].active=false;
    it[1].frameRec = (Rectangle){ 0.0f, 0.0f, (float)96, (float)96 };
    it[2].active=false;
    it[2].frameRec = (Rectangle){ 0.0f, 0.0f, (float)96, (float)96 };


    e[0].active=true;
    e[0].state = stateIdle;
    e[0].visible=true;
    e[0].frameSpeed = 8;
    e[0].health = 10;
    e[0].frameRec = (Rectangle){ 0.0f, 0.0f, (float)96, (float)96 };
    e[0].facing=1;
    e[0].position.y = 470;
    e[0].position.x = 320;
    e[0].mod = 5;
    setentityanimation(0,animIdle);
    e[1].active=true;
    e[1].state = stateIdle;
    e[1].visible=true;
    e[1].frameSpeed = 8;
    e[1].health = 10;
    e[1].frameRec = (Rectangle){ 0.0f, 0.0f, (float)96, (float)96 };
    e[1].facing=1;
    e[1].position.y = 270;
    e[1].position.x = 320;
    e[1].mod = 5;
    setentityanimation(1,animIdle);
    e[2].active=true;
    e[2].state = stateIdle;
    e[2].visible=true;
    e[2].frameSpeed = 8;
    e[2].health = 10;
    e[2].frameRec = (Rectangle){ 0.0f, 0.0f, (float)96, (float)96 };
    e[2].facing=1;
    e[2].position.y = 370;
    e[2].position.x = 220;
    e[2].mod = 5;
    setentityanimation(2,animIdle);    

    p[0].active=true;
    p[0].enemyweareatattacking=-1;
    p[0].facing=1;
    p[0].position.y = 370;
    p[0].position.x = 320;
    p[0].enemyattheleft=-1;
    p[0].enemyattheright=-1;

    setplayeranimation(0,animIdle);

    //other sprites - 
    int mod=0;
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {

        /*
        // Update
        //----------------------------------------------------------------------------------
        */
        
        playercontrols(0);
        updateplayer(0);
        updateentity(0);
        updateentity(1);
        updateentity(2);
        entityattack(0);
        entityattack(1);
        entityattack(2);

        updateitems();



        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);
            DrawTexture(backg, 0, 0, WHITE);
            //DrawTexture(scarfy, 15, 40, WHITE);


            drawZordered();
            //drawplayers(true);
            //drawentities(true);
            //drawitems();

            // spceial case if player is near enemy check if he is below than draw infront.
            //for(int i=0;i<MAX_ENTITIES;i++){
            //if(rectsoverlap(p[0].position.x,p[0].position.y,96,96,e[i].position.x,e[i].position.y,96,96)){
            //    if(p[0].position.y>e[i].position.y){
            //        drawplayers(false);
            //    }
            //}
            //}
            
            DrawRectangle(0,2,screenWidth,23,WHITE);
            DrawText("Cursor LEFT/RIGHT/UP/DOWN Z(FIRE1)/X(FIRE2)", 0, 0, 30, BLACK);
            //DrawText(FormatText("hitcombo %i",p[0].hitcombo), 0, 20, 30, BLACK);
            //DrawText(FormatText("ca : %i",e[0].currentFrame), 0, 20, 30, BLACK);
            //DrawText(FormatText("ca : %i",e[1].currentFrame), 0, 50, 30, BLACK);
            //DrawText(FormatText("debug: %i",debug),210,20,30,BLACK);
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadTexture(scarfy);       // Texture unloading
    UnloadTexture(backg);

    CloseWindow();                // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}


void setentityanimation(int entity, int anim){
    //e[entity].lastAnim = anim;
    
    switch(anim){
            case animNothing:
            //e[entity].frame_start = frame_idlestart+e[entity].mod;
            //e[entity].frame_end = frame_idleend+e[entity].mod;
            //e[entity].currentFrame = frame_flyingend;    
            break;

            case animIdle:
            e[entity].frame_start = frame_idlestart+e[entity].mod;
            e[entity].frame_end = frame_idleend+e[entity].mod;
            e[entity].currentFrame = e[entity].frame_start;    
            e[entity].animLoop=true;
            e[entity].animOneshot=false;
            break;

            case animKick:
            e[entity].frame_start = frame_kickstart+e[entity].mod;
            e[entity].frame_end = frame_kickend+e[entity].mod;
            e[entity].currentFrame = e[entity].frame_start;
            e[entity].animLoop=false;
            e[entity].animOneshot=true;

            break;
            case animHit1:
            e[entity].frame_start = frame_hit1start+e[entity].mod;
            e[entity].frame_end = frame_hit1end+e[entity].mod;
            e[entity].currentFrame = e[entity].frame_start;
            e[entity].animLoop=false;
            e[entity].animOneshot=true;            
            break;
            case animHit2:
            e[entity].frame_start = frame_hit2start+e[entity].mod;
            e[entity].frame_end = frame_hit2end+e[entity].mod;
            e[entity].currentFrame = e[entity].frame_start;            
            e[entity].animLoop=false;
            e[entity].animOneshot=true;
            break;

            case animUcut:
            e[entity].frame_start = frame_ucutstart+e[entity].mod;
            e[entity].frame_end = frame_ucutend+e[entity].mod;
            e[entity].currentFrame = e[entity].frame_start;
            e[entity].animLoop=false;
            e[entity].animOneshot=true;

            break;

            case animWalk:
            e[entity].framesCounter = 8;
            e[entity].frame_start = frame_walkstart+e[entity].mod;
            e[entity].frame_end = frame_walkend+e[entity].mod;
            e[entity].currentFrame = e[entity].frame_start;            
            e[entity].animLoop=true;
            e[entity].animOneshot=false;

            break;
            case animDamage:
            e[entity].frame_start = frame_damagestart+e[entity].mod;
            e[entity].frame_end = frame_damageend+e[entity].mod;
            e[entity].currentFrame = e[entity].frame_start;
            e[entity].animLoop=false;
            e[entity].animOneshot=true;
            
            break;
            case animFlying:
            e[entity].framesCounter = 100;
            e[entity].frame_start = frame_flyingstart+e[entity].mod;
            e[entity].frame_end = frame_flyingend+e[entity].mod;
            e[entity].currentFrame = e[entity].frame_start;
            e[entity].animLoop=false;
            e[entity].animOneshot=true;
            
            break;

    
    }
    e[entity].currentAnim = anim;
}

void setplayeranimation(int player, int anim){
    p[player].lastAnim = anim;
    switch(anim){
            case animIdle:
            p[player].frame_start = frame_idlestart;
            p[player].frame_end = frame_idleend;
            p[player].currentFrame = p[player].frame_start;    
            break;

            case animKick:
            p[player].frame_start = frame_kickstart;
            p[player].frame_end = frame_kickend;
            p[player].currentFrame = p[player].frame_start;
            break;
            case animHit1:
            p[player].frame_start = frame_hit1start;
            p[player].frame_end = frame_hit1end;
            p[player].currentFrame = p[player].frame_start;
            break;

            case animUcut:
            p[player].frame_start = frame_ucutstart;
            p[player].frame_end = frame_ucutend;
            p[player].currentFrame = p[player].frame_start;
            break;

            case animHit2:
            p[player].frame_start = frame_hit2start;
            p[player].frame_end = frame_hit2end;
            p[player].currentFrame = p[player].frame_start;
            break;
            case animWalk:
            if(p[0].currentAnim!=animWalk){
            p[player].frame_start = frame_walkstart;
            p[player].frame_end = frame_walkend;
            p[player].currentFrame = p[player].frame_start;            
            }
            break;
            case animDamage:
            p[player].frame_start = frame_damagestart;
            p[player].frame_end = frame_damageend;
            p[player].currentFrame = p[player].frame_start;
            break;
            case animFlying:
            p[player].frame_start = frame_flyingstart;
            p[player].frame_end = frame_flyingend;
            p[player].currentFrame = p[player].frame_start;
            
            break;

    
    }
    p[player].currentAnim = anim;
}

void updateitems(){
    for(int i=0;i<MAX_ITEMS;i++){
        if(it[i].active==false)continue;
        if(it[i].incx!=0){            
            
            if(it[i].incx>0){
                it[i].incx-=.05;
                it[i].angle+=10;
            }
            if(it[i].incx<0){
                it[i].incx+=.05;
                it[i].angle-=10;
            }
            if(it[i].incx>-0.1 && it[i].incx<0.1){
                it[i].incx=0;
            }
            it[i].incy+=.42;
            it[i].position.x+=it[i].incx;
            if(it[i].incy<8)it[i].position.y+=it[i].incy;
        }else{
            if(it[i].blinking==false){
                it[i].blinking=true;
                it[i].blinkingtime=0;
                it[i].blinkingcnt=0;
            }
            
        }
               // For the blinking of the entity
        
        if(it[i].blinking==true)it[i].blinkingtime++;
        if(it[i].blinkingtime<150 && it[i].blinking==true){                    
            it[i].blinkingcnt++;
            if(it[i].blinkingcnt>10){
                
                it[i].blinkingcnt=0;
                if(it[i].visible==true){
                    it[i].visible=false;
                }else{
                    it[i].visible=true;                    
                }
            }            
        }
        if(it[i].blinking && it[i].blinkingtime>150)it[i].active=false;

    }
 
}

void updateentity(int entity){
        // animation
        if(e[entity].active!=true)return;
        if(e[entity].currentAnim==animNothing)return;
        
        e[entity].lastFiretime++;
        e[entity].framesCounter++;
        ;
        
        if(e[entity].flying){
            if(e[entity].flyingincx!=0){
                
                if(e[entity].flyingincx>0){
                    e[entity].flyingincx-=.05;
                }
                if(e[entity].flyingincx<0){
                    e[entity].flyingincx+=.05;
                }
                if(e[entity].flyingincx>-0.1 && e[entity].flyingincx<0.1){
                    e[entity].flyingincx=0;
                }
                e[entity].flyingincy+=.42;
                e[entity].position.x+=e[entity].flyingincx;
                if(e[entity].flyingincy<8)e[entity].position.y+=e[entity].flyingincy;
            }else{
                if(e[entity].blinking==false){
                    e[entity].blinking=true;
                    e[entity].blinkingtime=0;
                    e[entity].blinkingcnt=0;
                }
            }                
        }
        // For the blinking of the entity
        
        if(e[entity].blinking==true)e[entity].blinkingtime++;
        if(e[entity].blinkingtime<150 && e[entity].blinking==true){                    
            e[entity].blinkingcnt++;
            if(e[entity].blinkingcnt>10){
                
                e[entity].blinkingcnt=0;
                if(e[entity].visible==true){
                    e[entity].visible=false;
                }else{
                    e[entity].visible=true;                    
                }
            }            
        }
        if(e[entity].blinking && e[entity].blinkingtime>150)e[entity].active=false;


        
        // actual animation system for the entities
        if (e[entity].framesCounter >= (60/e[entity].frameSpeed))
        {
            e[entity].framesCounter = 0;
            e[entity].currentFrame++;

            if (e[entity].currentFrame > e[entity].frame_end){
                if(e[entity].animLoop==true){ 
                    e[entity].currentFrame = e[entity].frame_start;    
                }
                if(e[entity].animOneshot==true){
                    
                    if(e[entity].currentAnim==animFlying){
                        e[entity].currentFrame = e[entity].frame_end;
                        //e[entity].currentAnim = animNothing;
                    }else{
                        setentityanimation(entity,animIdle);    
                    }

                    
                    
                }
                
            }
            int ypos = e[entity].currentFrame/15;
            e[entity].frameRec.y = (float)(e[entity].currentFrame/15)*(float)96;

            e[entity].frameRec.x = (float)((e[entity].currentFrame)-ypos*15)*(float)96;
            
        }
         
        
        
        
        // states
        if(e[entity].state==stateDead)return;
        if(e[entity].currentAnim==animHit1)return;
        if(e[entity].currentAnim==animHit2)return;
        if(e[entity].currentAnim==animKick)return;
        if(e[entity].currentAnim==animUcut)return;
        if(e[entity].state==stateIdle && GetRandomValue(0,60)==1){
            e[entity].state=stateChase;
        }

        
        Vector2 oldpos=e[entity].position;
        if(p[0].enemyattheleft==entity && e[entity].currentAnim==animFlying)p[0].enemyattheleft=-1;
        if(p[0].enemyattheleft==entity && e[entity].mod==9)p[0].enemyattheleft=-1;
        if(p[0].enemyattheright==entity && e[entity].currentAnim==animFlying)p[0].enemyattheright=-1;
        if(p[0].enemyattheright==entity && e[entity].mod==9)p[0].enemyattheright=-1;
        
        if(e[entity].state==stateChase && e[entity].mod==5 && e[entity].currentAnim!=animFlying){
            //
            
            //
            if(e[entity].position.x<p[0].position.x && p[0].enemyattheleft==-1 && p[0].enemyattheright!=entity){
                if(e[entity].target.x<e[entity].position.x)e[entity].facing=-1;
                if(e[entity].target.x>e[entity].position.x)e[entity].facing=1;
                e[entity].target.x=p[0].position.x-48;
                e[entity].target.y=p[0].position.y;
                p[0].enemyattheleft = entity;
                //if(e[entity].currentAnim!=animWalk)setentityanimation(entity,animWalk);
                //e[entity].state = stateChase;
            }
            if(e[entity].position.x>p[0].position.x && p[0].enemyattheright==-1 && p[0].enemyattheleft!=entity){
                if(e[entity].target.x<e[entity].position.x)e[entity].facing=-1;
                if(e[entity].target.x>e[entity].position.x)e[entity].facing=1;
                
                e[entity].target.x=p[0].position.x+48;
                e[entity].target.y=p[0].position.y;
                p[0].enemyattheright = entity;
                
                //e[entity].state = stateChase;
            }
        
            if(p[0].enemyattheleft==entity || p[0].enemyattheright==entity){
                if(e[entity].position.x>e[entity].target.x)e[entity].position.x--;
                if(e[entity].position.x<e[entity].target.x)e[entity].position.x++;
                if(e[entity].position.y>e[entity].target.y)e[entity].position.y--;
                if(e[entity].position.y<e[entity].target.y)e[entity].position.y++;
                if(distance(e[entity].position.x,e[entity].position.y,p[0].position.x,p[0].position.y)<20){
                    if(p[0].position.x<e[entity].position.x)e[entity].facing=-1;
                    if(p[0].position.x>e[entity].position.x)e[entity].facing=1;
                    
                }
                //
                for(int i=0;i<MAX_ENTITIES;i++){
                    if(i==entity)continue;
                    if(e[i].state!=stateChase)continue;
                    if(e[i].mod==9)continue;
                    if(distance(e[i].position.x,e[i].position.y,e[entity].position.x,e[entity].position.y)<48){
                        if(e[i].position.x>e[entity].position.x)e[i].position.x+=2;
                        if(e[i].position.x<e[entity].position.x)e[i].position.x-=2;
                        if(e[i].position.y>e[entity].position.y)e[i].position.y+=2;
                        if(e[i].position.y<e[entity].position.y)e[i].position.y-=2;
                        if(p[0].enemyattheleft==entity)p[0].enemyattheleft=-1;
                        if(p[0].enemyattheright==entity)p[0].enemyattheright=-1;
                        e[entity].state = stateIdle;

                    }
                    
                      
                }
                //
                if(e[entity].currentAnim!=animWalk && e[entity].currentAnim!=animDamage)setentityanimation(entity,animWalk);
            }
            
            if(rectsoverlap(p[0].position.x,p[0].position.y,96,96,e[entity].position.x,e[entity].position.y,48,48)==false){
                
                if(p[0].enemyattheleft==entity)p[0].enemyattheleft=-1;
                if(p[0].enemyattheright==entity)p[0].enemyattheright=-1;
                //e[entity].state=stateIdle;
            }
        }
        
        // Scatter if clumped up together.
        
        for(int i=0;i<MAX_ENTITIES;i++){
            if(e[entity].state!=stateIdle)continue;
            if(i==entity)continue;
            if(distance(e[entity].position.x,e[entity].position.y,e[i].position.x,e[i].position.y)<80){
                e[entity].state=statemoveto;
                e[entity].target.x = e[entity].position.x+GetRandomValue(-50,50);
                e[entity].target.y = e[entity].position.y+GetRandomValue(-50,50);
                if(e[entity].target.x<48)e[entity].target.x+=48;
                if(e[entity].target.x>1000-48)e[entity].target.x-=48;
                if(e[entity].target.y<148)e[entity].target.y+=48;
                if(e[entity].target.y>600-48)e[entity].target.y-=48;
                setentityanimation(entity,animWalk);

            }
        }
        
        
        // Move to state
        if(e[entity].state==statemoveto){
            if(e[entity].position.x>e[entity].target.x)e[entity].position.x--;
            if(e[entity].position.x<e[entity].target.x)e[entity].position.x++;
            if(e[entity].position.y>e[entity].target.y)e[entity].position.y--;
            if(e[entity].position.y<e[entity].target.y)e[entity].position.y++;
            if(distance(e[entity].position.x,e[entity].position.y,e[entity].target.x,e[entity].target.y)<4){
                e[entity].state = stateIdle;
            }

        }
        
        
        
        
        // If we have not moved then revert back to the idle state.
        if(e[entity].position.x==oldpos.x && e[entity].position.y==oldpos.y){
            
            if(e[entity].currentAnim!=animDamage)setentityanimation(entity,animIdle);
        }

        
        
}


void updateplayer(int player){
        // animation
        p[player].lastFiretime++;
        p[player].keynothingtime++;
        p[player].framesCounter++;
        p[player].enemyweareatattacking=-1;
        

        if (p[0].framesCounter >= (60/framesSpeed))
        {
            p[0].framesCounter = 0;
            p[0].currentFrame++;

            if (p[0].currentFrame > p[0].frame_end) p[0].currentFrame = p[0].frame_start;

            int ypos = p[0].currentFrame/15;
            frameRec.y = (float)(p[0].currentFrame/15)*(float)96;

            frameRec.x = (float)((p[0].currentFrame)-ypos*15)*(float)96;
            
        }
 
        //
        // Player hits a enemy!
        //
        // Is he nearby
        for(int entity=0;entity<MAX_ENTITIES;entity++){
            if(e[entity].damagedelay>0)e[entity].damagedelay-=1;
            if(p[player].enemyweareatattacking!=entity && p[player].enemyweareatattacking!=-1)continue;
            if(rectsoverlap(p[player].position.x,p[player].position.y,70,60,e[entity].position.x,e[entity].position.y+10,60,40)){
                // Are we on the final damaging frame.        
                if(p[0].currentFrame == frame_hit1end || p[0].currentFrame == frame_hit2end || p[0].currentFrame == frame_kickend || p[0].currentFrame == frame_ucutend){
                    // Are we faced into the right direction
                    bool goahead=false;
                    if(p[player].facing==-1 && e[entity].position.x<p[player].position.x){
                        goahead=true;
                    }
                    if(p[player].facing==1 && e[entity].position.x>p[player].position.x){
                        goahead=true;
                    }
                    
                    // We have hit 'em
                    if(goahead && e[entity].damagedelay==0){
                        p[player].enemyweareatattacking=entity;
                        //
                        
                        e[entity].damagedelay=10;
                        //if(e[0].health>0)e[0].health-=2;
                        //if(e[0].health<4 && e[0].health>0 && p[player].hitcombo==4){
                        if(p[player].hitcombo==4){
                            if(GetRandomValue(0,10)>5 || e[entity].mod==9){
                                p[player].hitcombo=0;
                                e[entity].frameSpeed = 2;
                                e[entity].flying = true;
                                e[entity].flyingincx = 5;
                                e[entity].flyingincy = -5;
                                setentityanimation(entity,animFlying);
                                e[entity].state = stateDead;
                                e[entity].shadey = e[entity].position.y;
                                if(p[0].position.x>e[entity].position.x)e[entity].flyingincx=-e[entity].flyingincx;

                            }else{
                                p[player].hitcombo=0;
                                e[entity].health=0;
                                setentityanimation(entity,animDamage);                                
                                //
                                if(e[entity].mod==5){                                    
                                    e[entity].mod = 9;
                                    it[entity].active=true;
                                    it[entity].visible=true;
                                    it[entity].shadey=e[entity].position.y;
                                    it[entity].incx=5;
                                    it[entity].incy=-5;
                                    it[entity].position = e[entity].position;
                                    if(p[0].position.x>e[entity].position.x)it[entity].incx=-it[entity].incx;
                                }                                

                            }
                        }else{
                            if(p[player].hitcombo==4)p[player].hitcombo=0;
                            setentityanimation(entity,animDamage);
                            p[player].hitcombo++;
                        }
                    }
                }
            
            }
        }

        // player movement
        if(p[player].currentAnim == animWalk && p[player].keynothingtime>1){
            setplayeranimation(player,animIdle);
        }
        if(p[player].currentAnim == animHit1 && p[player].keynothingtime>15){
            
            setplayeranimation(player,animIdle);
        }
        if(p[player].currentAnim == animHit2 && p[player].keynothingtime>15){
            setplayeranimation(player,animIdle);
            
        }
        if(p[player].currentAnim == animUcut && p[player].keynothingtime>15){
            setplayeranimation(player,animIdle);
            
        }

        if(p[player].currentAnim == animKick && p[player].keynothingtime>15){
            setplayeranimation(player,animIdle);
        }
        
        
        
        if(LEFT==false && RIGHT==false && UP==false && DOWN==false && FIRE1){
            bool cont=true;
            if(p[player].lastFireAnim == animHit1 || p[player].lastFireAnim == animHit2){
                if(p[player].hitcombo==4){
                    //p[player].hitcombo=0;
                    setplayeranimation(player,animUcut);                    
                    p[player].lastFireAnim=animUcut;
                    cont=false;
                }
            }
            if(cont){
            if(p[player].lastFireAnim==animHit2){
                setplayeranimation(player,animHit1);
                p[player].lastFireAnim=animHit1;
            }else{
                setplayeranimation(player,animHit2);
                p[player].lastFireAnim=animHit2;
            }
            }
            FIRE1=false;
        }
        if(LEFT==false && RIGHT==false && UP==false && DOWN==false && FIRE2){
            p[player].hitcombo=0;
            setplayeranimation(player,animKick);
            FIRE2=false;
        }
        if (RIGHT){
            p[player].hitcombo=0;
            p[player].lastFiretime=50;
            p[0].facing = 1;
            p[0].position.x+=2;
            RIGHT=false;
            setplayeranimation(player,animWalk);
        }
        else if (LEFT){
            p[player].hitcombo=0;
            p[player].lastFiretime=50;
            p[0].facing=-1;
            p[0].position.x-=2;
            LEFT=false;
            setplayeranimation(player,animWalk);
        }
        if (DOWN){
            p[player].hitcombo=0;
            p[player].lastFiretime=50;
            p[0].position.y+=2;
            DOWN=false;
            setplayeranimation(player,animWalk);
        }
        else if (UP){
            p[player].hitcombo=0;
            p[player].lastFiretime=50;
            p[0].position.y-=2;
            UP=false;
            setplayeranimation(player,animWalk);
        }
        
        
}

void playercontrols(int player){
        if (p[player].lastFiretime>30 && IsKeyDown(KEY_Z) && FIRE1==false && FIRE2==false){
            FIRE1 = true;
            p[player].lastFiretime=0;
            p[player].keynothingtime = 0;
        }
        if (p[player].lastFiretime>30 && IsKeyDown(KEY_X) && FIRE1==false && FIRE2==false){
            FIRE2 = true;
            p[player].lastFiretime=0;
            p[player].keynothingtime = 0;
        }
        
        if(p[player].lastFiretime<20)return; //WATCH this return!!
        if (IsKeyDown(KEY_RIGHT)){
            RIGHT = true;
            LEFT = false;
            p[player].keynothingtime = 0;
        }
        else if (IsKeyDown(KEY_LEFT)){
            LEFT = true;            
            RIGHT = false;
            p[player].keynothingtime = 0;
        }
        if (IsKeyDown(KEY_DOWN)){
            DOWN = true;
            UP = false;
            p[player].keynothingtime = 0;
        }
        else if (IsKeyDown(KEY_UP)){
            UP = true;
            DOWN = false;
            p[player].keynothingtime = 0;
        }
    
}


//
// What this function does is create a list of every y position of each sprite on the screen. This list gets
// sorted(bubble sort) so that the first sprite to be drawn is the upper most sprite. The last sprite on the list
// is the sprite that is on the lowest position on the screen.
// This is done because if a sprite is drawn ontop of another we need to know if he is infront or behind it. Knowing
// the feet position is a good way of this. If two sprites are close by, the one which is lower(at the feet) needs
// to be drawn infront(I think anyways, testing still for bugs)
// There might be problems later on so any weirdness with drawing wil be needed to be fixed here(different sprite sizes(standing/laying/items))
//
void drawZordered(){

    int totalsprites; // how many sprites
    int sortedlist[MAX_SPRITES][3]; // max of 1000 sprites ( 1 = player, 2 = entity, 3 = items)(playerid, entityid, itemid)
    int position=0;
    for(int i=0;i<MAX_PLAYERS;i++){
        sortedlist[position][0]=1;
        sortedlist[position][1]=i;
        sortedlist[position][2]=p[i].position.y+96;
        position++;
    }
    for(int i=0;i<MAX_ENTITIES;i++){
        if(e[i].active==false)continue;
        sortedlist[position][0]=2;
        sortedlist[position][1]=i;
        sortedlist[position][2]=e[i].position.y+96;
        //102/98 - special case - sprites are lying down so they have different bottom potision
        if(e[i].currentFrame==102 || e[i].currentFrame==98)sortedlist[position][2]-=32;
        position++;
    }
    for(int i=0;i<MAX_ITEMS;i++){
        if(it[i].active==false)continue;
        sortedlist[position][0]=3;
        sortedlist[position][1]=i;
        sortedlist[position][2]=it[i].position.y+48;
        position++;
    }
    totalsprites=position;
    //bubble sort - infinitly loop through the list and move lower value one position up and the one there one down.
    //exit when one loop is done without a modification.
    bool exit=false;
    while(exit==false){
        exit=true;
        for(int i=1;i<totalsprites;i++){
            if(sortedlist[i][2]<sortedlist[i-1][2]){
                int temp[3];
                temp[0]=sortedlist[i-1][0];
                temp[1]=sortedlist[i-1][1];
                temp[2]=sortedlist[i-1][2];
                sortedlist[i-1][0]=sortedlist[i][0];
                sortedlist[i-1][1]=sortedlist[i][1];
                sortedlist[i-1][2]=sortedlist[i][2];
                sortedlist[i][0]=temp[0];
                sortedlist[i][1]=temp[1];
                sortedlist[i][2]=temp[2];
                exit=false;
            }
        }
    }
    //
    // Here we draw everything z ordered
    //
    for(int i=0;i<totalsprites;i++){
        if(sortedlist[i][0]==1){//draw player
            int pn = sortedlist[i][1];
            if(p[pn].facing==1){
                DrawEllipse(p[pn].position.x+40,p[pn].position.y+96,20,10,(Color){0,0,0,96});    
                DrawTextureRec(scarfy, frameRec, (Vector2){p[pn].position.x,p[pn].position.y}, WHITE);  // Draw part of the texture                
                }else{
                DrawEllipse(p[pn].position.x+48,p[pn].position.y+96,20,10,(Color){0,0,0,96});
                DrawTexturePro(scarfy,  (Rectangle){frameRec.x,frameRec.y,-96,96},// the -96 (-)means mirror on x axis
                                                (Rectangle){p[pn].position.x,p[pn].position.y,96,96},
                                                (Vector2){0,0},0,WHITE);
            }
       
        }
        if(sortedlist[i][0]==2){//draw entities            
            int en = sortedlist[i][1];
            if(e[en].active==false)continue;
            if(e[en].visible==false)continue;
            bool shade=true;
            if(e[en].facing==1){
            if(shade){
                if(e[en].currentAnim!=animFlying){
                    DrawEllipse(e[en].position.x+40,e[en].position.y+96,20,10,(Color){0,0,0,96});
                }else{
                    DrawEllipse(e[en].position.x+40,e[en].shadey+96,20,10,(Color){0,0,0,96});
                }
            }
            DrawTextureRec(scarfy, e[en].frameRec, (Vector2){e[en].position.x,e[en].position.y}, WHITE);  // Draw part of the texture
            }else{
            if(shade){
                if(e[en].currentAnim!=animFlying){
                    DrawEllipse(e[en].position.x+48,e[en].position.y+96,20,10,(Color){0,0,0,96});
                }else{
                    DrawEllipse(e[en].position.x+48,e[en].shadey+96,20,10,(Color){0,0,0,96});
                }
            }
            DrawTexturePro(scarfy,  (Rectangle){e[en].frameRec.x,e[en].frameRec.y,-96,96},// the -96 (-)means mirror on x axis
                                            (Rectangle){e[en].position.x,e[en].position.y,96,96},
                                            (Vector2){0,0},0,WHITE);
            }
 
        }
        if(sortedlist[i][0]==3){//Draw items
            int in = sortedlist[i][1];
            if(it[in].active==false)continue;
            if(it[in].visible==false)continue;
            DrawEllipse(it[in].position.x+47,it[in].shadey+106,16,8,(Color){0,0,0,96});
            // get the head cell
            it[in].frameRec.y = 0;

            it[in].frameRec.x = 96*4;
                
            //DrawTextureRec(scarfy, it[0].frameRec, (Vector2){it[0].position.x,it[0].position.y}, WHITE);  // Draw part of the texture
            DrawTexturePro(scarfy,  (Rectangle){it[in].frameRec.x,it[in].frameRec.y,-96,96},// the -96 (-)means mirror on x axis
                                            (Rectangle){it[in].position.x+48,it[in].position.y+48,96,96},
                                            (Vector2){96/2,96/2},it[in].angle,WHITE);
        }
       
        
    }
}

// Not build for 2 players yet p[x]
void entityattack(int entity){
    if(e[entity].currentAnim!=animIdle)return;
    if(GetRandomValue(0,100)>1)return;
    // if close to player or headless then attack
    if(rectsoverlap(e[entity].position.x,e[entity].position.y+24,64,20,p[0].position.x,p[0].position.y+24,64,32)==true || e[entity].mod==9){
        // If the entity is not headless then he turns into the direction of the player.
        if(e[entity].mod==5){
            if(p[0].position.x<e[entity].position.x)e[entity].facing=-1;
            if(p[0].position.x>e[entity].position.x)e[entity].facing=1;
        }
        switch(GetRandomValue(0,3)){
            case 0:
            setentityanimation(entity,animHit1);
            //debug = GetRandomValue(0,100);
            break;
            case 1:
            setentityanimation(entity,animHit2);
            break;
            case 2:
            setentityanimation(entity,animKick);
            break;
            case 3:
            setentityanimation(entity,animUcut);
            break;
        }
        
    }
}

// Rectangles overlap
bool rectsoverlap(int x1,int y1,int w1,int h1,int x2,int y2,int w2,int h2){
    if(x1 >= (x2 + w2) || (x1 + w1) <= x2) return false;
    if(y1 >= (y2 + h2) || (y1 + h1) <= y2) return false;
    return true;
}
// Manhattan Distance (less precise)
float distance(float x1,float y1,float x2,float y2){
    return (float)abs(x2-x1)+abs(y2-y1);
}
