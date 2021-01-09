//
// I was following a tutorial but was not able to grasp how it worked. This example was ment to
// have racing cars following the track.

// Here I created a moving object that remembers it's last direction and chooses to continue in that
// direction. Else it chooses a random direction based on what it's whispers tell is blocked or open.
//

//
// This is a mod of the first version ..
// Here I added code that now prefers to go into one left or right of the last direction if the last direction 
// was blocked. If these are blocked than random.
// 


#include "raylib.h"
#include <math.h>

#define MAX_CARS 10
#define MAX_WHISKERS 8

   
int myMap[10][11] =  {  {1,1,1,1,1,1,1,1,1,1,1},
                        {1,0,0,0,0,0,0,0,0,0,1},
                        {1,0,1,1,1,1,1,1,1,0,1},
                        {1,0,1,1,1,1,1,0,0,0,1},
                        {1,0,1,0,0,0,1,0,1,1,1},
                        {1,0,1,0,1,0,1,0,0,0,1},
                        {1,0,1,0,1,0,1,1,1,0,1},
                        {1,0,1,0,1,0,1,1,1,0,1},
                        {1,0,0,0,1,0,0,0,0,0,1},
                        {1,1,1,1,1,1,1,1,1,1,1}
                    };  
int mapWidth = 11;
int mapHeight = 10;
float tileWidth;
float tileHeight;

typedef struct car{
    float x;
    float y;
    float r;
    int last;
    float angle;
    float destAngle;
    Vector2 chosenDir;
    float velocity;
    float acceleration;
    bool interest[8];
    bool danger[8]  ;
}car;

static struct car arr_car[MAX_CARS];

//Unit collide with solid blocks true/false
bool pointtilecollide(int x, int y, int offsetx,int offsety);
// Our rectsoverlap function. Returns true/false.
static bool rectsoverlap(int x1,int y1,int w1,int h1,int x2,int y2,int w2,int h2);
void drawwhiskers(int car);
                        
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 600;
    tileWidth = ceil((float)(float)screenWidth/(float)mapWidth);
    tileHeight = ceil((float)screenHeight/(float)mapHeight);

    InitWindow(screenWidth, screenHeight, "raylib example.");
 
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    for(int i=0;i<MAX_CARS;i++){
//    arr_car[0].x = 100.0f;
//    arr_car[0].y = 100.0f;
//    arr_car[0].r = tileWidth/8;
//    arr_car[0].angle = 0.0f;
        arr_car[i].x = 100.0f+GetRandomValue(0,10);
        arr_car[i].y = 100.0f;
        arr_car[i].r = tileWidth/8;
        arr_car[i].angle = 0.0f;
        arr_car[i].last = GetRandomValue(0,MAX_WHISKERS);

    }

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------

        for(int i=0;i<MAX_CARS;i++){
            // update interest array
            float angle = 0;
            float angleStep = (PI*2.0f)/MAX_WHISKERS;
            for(int j=0;j<MAX_WHISKERS;j++){
                arr_car[i].interest[j]=0;
                float x=arr_car[i].x;
                float y=arr_car[i].y;
                for(int dist=10;dist<32;dist++){
                    if(pointtilecollide(x+cos(angle)*dist,y+sin(angle)*dist,0,0)){
                        arr_car[i].interest[j]=1;
                    }
                }
                angle+=angleStep;
            }

            bool chosen=false;
            while(chosen==false){
                int num=0;
                // can we go the same direction?
                if(arr_car[i].interest[arr_car[i].last]==1){                
                    num=GetRandomValue(0,MAX_WHISKERS);

                    // can we go one step left or right
                    int pos = arr_car[i].last;
                    pos-=1;
                    if(pos==-1)pos=MAX_WHISKERS;
                    if(arr_car[i].interest[pos]==0){
                        num=pos;
                    }
                    pos = arr_car[i].last;
                    pos++;
                    if(pos>MAX_WHISKERS)pos=0;
                    if(arr_car[i].interest[pos]==0){
                        num=pos;
                    }                    
                }else{
                    num = arr_car[i].last;
                }
                if(arr_car[i].interest[num]==0){
                    if(pointtilecollide(arr_car[i].x,arr_car[i].y,cos(num*angleStep)*9,sin(num*angleStep)*9)==false){
                        arr_car[i].x += cos(num*angleStep)*3;
                        arr_car[i].y += sin(num*angleStep)*3;
                        arr_car[i].last = num;
                        chosen=true;
                    }else{
                        chosen=true;
                        arr_car[i].last=GetRandomValue(0,MAX_WHISKERS);
                    }
                }
            }
            
        }
        
        //----------------------------------------------------------------------------------
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);
            
            // Draw map
            for (int y = 0; y< mapHeight ; y++)
            {
                for (int x = 0; x< mapWidth ; x++)
                {
                    if (myMap[y][x] == 1)
                    {
                        DrawRectangle(x*tileWidth,y*tileHeight,tileWidth,tileHeight,BLUE);
                    }
                }
            }
            
            // draw car(s)
            for(int i=0;i<MAX_CARS;i++){
                DrawCircle(arr_car[i].x,arr_car[i].y,arr_car[i].r,RED);
                drawwhiskers(i);
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


void drawwhiskers(int car){
    
    float angle = 0;
    float angleStep = (PI*2.0f)/MAX_WHISKERS;
    for(int i=0;i<MAX_WHISKERS;i++){
        float x = arr_car[car].x;
        float y = arr_car[car].y;
        if(arr_car[car].interest[i]==0){            
            DrawLine(x,y,x+cos(angle)*32,y+sin(angle)*32,GREEN);
            }
            else
            {
            DrawLine(x,y,x+cos(angle)*32,y+sin(angle)*32,RED);
        }
        angle+=angleStep;
    }
}

//Unit collide with solid blocks true/false
bool pointtilecollide(int x, int y, int offsetx,int offsety){
    int cx = (x+offsetx)/tileWidth;
    int cy = (y+offsety)/tileHeight;
    for(int y2=cy-1; y2<cy+2;y2++){//Note that the - and + are to be set differently with differently sized players
    for(int x2=cx-1; x2<cx+2;x2++){
        if(x2>=0 && x2<mapWidth && y2>=0 && y2<mapHeight){
            if(myMap[y2][x2] == 1){
                int x3 = (x2)*tileWidth;
                int y3 = (y2)*tileHeight;
                if(rectsoverlap(x+offsetx,y+offsety,1,1,x3,y3,tileWidth,tileHeight)){
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
