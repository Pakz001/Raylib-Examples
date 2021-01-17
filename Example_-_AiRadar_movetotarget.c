//

#include "raylib.h"
#include <math.h>

#define MAX_PATH 1024
#define MAX_AGENT 32 // Maximum number of ai entities

#define SPEED 1

enum terrain{TREE=1,GROUND=0,PLAYER=2,AIPLAYER=3,GOAL=4,GOALREACHED=5};
enum flag{UP,DOWN,LEFT,RIGHT,WAIT};

//
// You can set any number of 3 and 2 on the map. more 4's untested. keep the borders(1) intact(memory errors)
//
//1=border(TREE),2=PLAYER,3=AIPLAYER,4=Path finding objective(GOAL)
int map1[10][10] = {    {1,1,1,1,1,1,1,1,1,1},
                        {1,0,0,0,0,1,0,0,3,1},
                        {1,0,0,0,0,1,0,0,0,1},
                        {1,3,0,0,0,1,0,0,0,1},
                        {1,0,0,0,0,1,1,0,1,1},
                        {1,0,0,0,0,1,0,0,0,1},
                        {1,1,1,0,1,1,0,0,0,1},
                        {1,0,0,0,0,0,2,0,0,1},
                        {1,0,0,0,0,0,0,0,0,1},
                        {1,1,1,1,1,1,1,1,1,1}
                        };  
int map[10][10] = {0};

int dijkstramap[10][10]={0};


typedef struct pathnode{
    int x;
    int y;    
}pathnode;

typedef struct agent{
    bool active;
    Vector2 position;
    Vector2 size;
    Vector2 ray;
    float rayangle;
}agent;

static struct agent arr_agent[MAX_AGENT];

typedef struct player{
    Vector2 position;
    Vector2 size;
}player;

player myPlayer = {0};

void makedijkstramap();
static float distance(float x1,float y1,float x2,float y2);
static float edistance(float x1,float y1,float x2,float y2);
//Unit collide with solid blocks true/false
bool recttilecollide(int x, int y, int w, int h, int offsetx,int offsety);
// Our rectsoverlap function. Returns true/false.
static bool rectsoverlap(int x1,int y1,int w1,int h1,int x2,int y2,int w2,int h2);
void updateplayer();
void updateagents();
    
int topdijkstralen=0;
int screenWidth;
int screenHeight;
int tileWidth,tileHeight,mapWidth,mapHeight;



int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    screenWidth = 800;
    screenHeight = 600;
    mapWidth = 10;
    mapHeight = 10;
    tileWidth = ceil((float)screenWidth/(float)mapWidth);
    tileHeight = ceil((float)screenHeight/(float)mapHeight);
    
    InitWindow(screenWidth, screenHeight, "raylib example.");
 
    SetTargetFPS(200);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------
    //makedijkstramap(); // The 4 = the goal that gets flooded from...

    //
    // Copy our original map into the active map...
    for(int y=0;y<mapHeight;y++){
    for(int x=0;x<mapHeight;x++){
        map[y][x]= map1[y][x];
    }}
    
    // set our player up..
    for(int y=0;y<mapHeight;y++){
    for(int x=0;x<mapHeight;x++){
        if(map[y][x]==PLAYER){
            myPlayer.position = (Vector2){x*tileWidth,y*tileHeight};
            myPlayer.size = (Vector2){tileWidth/2,tileHeight/2};
            map[y][x]=GROUND;
        }
    }}    

    // set our agents(ai) up..
    int agentcount=0;
    for(int y=0;y<mapHeight;y++){
    for(int x=0;x<mapHeight;x++){
        if(map[y][x]==AIPLAYER){
            arr_agent[agentcount].active=true;
            arr_agent[agentcount].position = (Vector2){x*tileWidth,y*tileHeight};
            arr_agent[agentcount].size = (Vector2){tileWidth/2,tileHeight/2};
            map[y][x]=GROUND;
            agentcount++;
        }
    }}    


    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        
        updateplayer();
        updateagents();
        //----------------------------------------------------------------------------------
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);
            //drawmap (tile positions)
            for(int y=0;y<10;y++){
                for(int x =0;x<10;x++){
                    int m=map[y][x];
                    if(m==TREE){
                        
                        DrawRectangle(x*tileWidth,y*tileHeight,tileWidth,tileHeight,(Color){40,90,20,255});
                        DrawCircle(x*tileWidth+tileWidth/1.5,y*tileHeight+tileHeight/1.5,tileWidth/5,(Color){20,50,20,255});
                        DrawRectangle(x*tileWidth+tileWidth/2.2,y*tileHeight+tileHeight/2,tileWidth/8,tileHeight/3,BROWN);
                        
                        DrawCircle(x*tileWidth+tileWidth/2,y*tileHeight+tileHeight/3,tileWidth/4,(Color){120,250,20,255});
                        DrawCircle(x*tileWidth+tileWidth/2.2,y*tileHeight+tileHeight/4,tileWidth/9,(Color){220,255,220,155});
                    }
                    if(m==GROUND){
                        DrawRectangle(x*tileWidth,y*tileHeight,tileWidth,tileHeight,DARKGREEN);                        
                        DrawRectangle(x*tileWidth+5,y*tileHeight+10,2,1,GREEN);                        
                        DrawRectangle(x*tileWidth+tileWidth/6,y*tileHeight+tileHeight/6,2,1,GREEN);                        
                        DrawRectangle(x*tileWidth+tileWidth/1.5,y*tileHeight+tileHeight/1.5,2,1,GREEN);                        
                        DrawRectangle(x*tileWidth+tileWidth/2,y*tileHeight+tileHeight/2,2,1,GREEN);                        
                        
                    }
                    if(m==PLAYER){
                        DrawRectangle(x*tileWidth,y*tileHeight,tileWidth,tileHeight,BLUE);                        
                        DrawText("Player",x*tileWidth,y*tileHeight+tileHeight/4,16,BLACK);                        
                    }
                    if(m==AIPLAYER){
                        DrawRectangle(x*tileWidth,y*tileHeight,tileWidth,tileHeight,RED); 
                        DrawText("Ai_Player",x*tileWidth,y*tileHeight+tileHeight/4,16,WHITE);
                    }
                    if(m==GOAL){
                       DrawRectangle(x*tileWidth,y*tileHeight,tileWidth,tileHeight,WHITE);      
                       DrawText("City",x*tileWidth+tileWidth/4,y*tileHeight+tileHeight/4,26,BLACK);                        
                    }
                    if(m==GOALREACHED){
                        DrawRectangle(x*tileWidth,y*tileHeight,tileWidth,tileHeight,YELLOW);                        
                        DrawText("Captured",x*tileWidth+4,y*tileHeight+tileHeight/4,16,BLACK);                        
                    }
                    DrawText(FormatText("%i",dijkstramap[y][x]),x*tileWidth+tileWidth/2-10,y*tileHeight+tileHeight/2-10,20,BLACK);

              }
            }
            
            // Draw our agents..
            for(int i=0;i<MAX_AGENT;i++){
                int x = arr_agent[i].position.x;
                int y = arr_agent[i].position.y;
                int w = arr_agent[i].size.x;
                int h = arr_agent[i].size.y;
                DrawRectangle(x,y,w,h,RED); 
                DrawText(FormatText("Agent %i",i),x,y+h/8,h/8,WHITE);
                // Draw our agents ray
                Vector2 p1 = (Vector2){ arr_agent[i].position.x+arr_agent[i].size.x/2,
                                        arr_agent[i].position.y+arr_agent[i].size.y/2};
                Vector2 p2 = (Vector2){ arr_agent[i].ray.x,
                                        arr_agent[i].ray.y};
                DrawLineEx(p1,p2,4,GRAY);
            }
  
 
            // Draw our player..
            int x = myPlayer.position.x;
            int y = myPlayer.position.y;
            int w = myPlayer.size.x;
            int h = myPlayer.size.y;
            DrawRectangle(x,y,w,h,BLUE);                        
            DrawText("Player",x,y+h/8,h/4,BLACK);                        
            
            
            DrawText("Cursors U/D/L/R",10,10,26,BLACK);
            DrawText("Cursors U/D/L/R",9,9,26,WHITE);
            
            
            

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;


}

void updateagents(){
    for(int i=0;i<MAX_AGENT;i++){
        if(arr_agent[i].active==false)continue;
        arr_agent[i].rayangle+=.005;
        arr_agent[i].ray = (Vector2){   arr_agent[i].position.x+arr_agent[i].size.x/2,
                                        arr_agent[i].position.y+arr_agent[i].size.y/2};
        for(int dist=0;dist<400;dist+=5){
            int w = 5;
            int h = 5;
            float x1 = arr_agent[i].ray.x;
            float y1 = arr_agent[i].ray.y;
            if(recttilecollide(x1,y1,w,h,0,0)==true){
                break;
            }
            arr_agent[i].ray.x+=cos(arr_agent[i].rayangle)*5;
            arr_agent[i].ray.y+=sin(arr_agent[i].rayangle)*5;
        }
    }
}

void updateplayer(){
    for(int sp=0;sp<SPEED;sp++){ // our movement speed (makes to check collision every step)
        Vector2 oldpos = myPlayer.position;
        if(IsKeyDown(KEY_LEFT)){
            myPlayer.position.x-=1;
        }
        if(IsKeyDown(KEY_RIGHT)){
            myPlayer.position.x+=1;
        }
        if(recttilecollide(myPlayer.position.x,myPlayer.position.y,tileWidth/2,tileHeight/2,0,0)){
            myPlayer.position = oldpos;
        }
        oldpos = myPlayer.position;
        if(IsKeyDown(KEY_UP)){
            myPlayer.position.y-=1;
        }
        if(IsKeyDown(KEY_DOWN)){
            myPlayer.position.y+=1;
        }
        if(recttilecollide(myPlayer.position.x,myPlayer.position.y,tileWidth/2,tileHeight/2,0,0)){
            myPlayer.position = oldpos;
        }

    }
}

void makedijkstramap(){
    
    int startx;
    int starty;
    // First make sure every map value is 0.
    for(int y=0;y<mapHeight;y++){
    for(int x=0;x<mapWidth;x++){
        dijkstramap[y][x] = 0;
        if(map1[y][x]==1)dijkstramap[y][x]=-1;
        if(map1[y][x]==4){
            startx = x;
            starty = y;
        }
    }}
    
    //
    
    //
    // Flood the map with distances from the start.
    //
    
    // We store the distance on each map cell if there is no wall there.
    //
    struct pathnode list[MAX_PATH];
    dijkstramap[starty][startx]=1;
    int listlen=0;    
    list[listlen].x=startx;
    list[listlen].y=starty;
    listlen+=1;        
    int failed=0;
    // 4 way search! left/up/down/right
    int dx[8]={ -1,0,1,-1,1,-1,0,1};
    int dy[8]={-1,-1,-1,0,0,1,1,1};    
    // While we have a list to work with
    while(listlen>0){
        // Take the first value from the array.
        int x1=list[0].x;
        int y1=list[0].y;
        
        // shift all up.
        for(int i=0;i<listlen;i++){
            list[i].x = list[i+1].x;
            list[i].y = list[i+1].y;
        }
        // Decrease list length
        listlen-=1;
        //
        // Here we check around our current position.
        for(int i=0;i<8;i++){
            int nx = x1+dx[i];
            int ny = y1+dy[i];
            if(nx<0 || ny<0 || nx>= mapWidth || ny>= mapHeight)continue;            
            // If we can get there then put the new distance there and add this position
            // to the list.
            if(dijkstramap[ny][nx]==0 && map[ny][ny]!=1){
                dijkstramap[ny][nx]=dijkstramap[y1][x1]+1;
                if(dijkstramap[ny][nx]>topdijkstralen)topdijkstralen=dijkstramap[ny][nx];
                // add to last
                //
                list[listlen].x = nx;
                list[listlen].y = ny;
                listlen++;
                //                
            }
        }
        // Error?
        failed+=1;
        if(failed>160000)return;
        
    }
    
 
}

// Manhattan Distance (less precise)
float distance(float x1,float y1,float x2,float y2){
    return (float)abs(x2-x1)+abs(y2-y1);
}

// Euclidean distance (more precise)
float edistance(float x1,float y1,float x2,float y2){
    return sqrt( (x1-x2)*(x1-x2)+(y1-y2)*(y1-y2) );
}

//Unit collide with solid blocks true/false
bool recttilecollide(int x, int y,int w, int h, int offsetx,int offsety){
    //y+=abs(mapy);//for scrolling
    int cx = (x+offsetx)/tileWidth;
    int cy = (y+offsety)/tileHeight;
    for(int y2=cy-1; y2<cy+2;y2++){//Note that the - and + are to be set differently with differently sized players
    for(int x2=cx-1; x2<cx+2;x2++){
        if(x2>=0 && x2<mapWidth && y2>=0 && y2<mapHeight){
            if(map[y2][x2] == 1){
                int x3 = (x2)*tileWidth;
                int y3 = (y2)*tileHeight;
                if(rectsoverlap(x+offsetx,y+offsety,w,h,x3,y3,tileWidth,tileHeight)){
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
