//
// Wave Function Collapse - sort of...
//
// I barely understand how this works. It has taken me a number of tries and a whack load of hours
// top get to this current point. The tutorials and explanations on the internet are not that clear and easy
// to understand.
//
// In the example here I have a number of tiles. I pick a random tile from the valid ones and place it on the grid.
// I then fix the neighbour tiles to fit this current position. Connected/unconnected. I keep placing random tiles
// for a x amount of times.

// There is a array that contains a list of all possible tiles. There are functions that check if a position on the grid
// has connections to another position on the grid. This is used to unflag the tiles that are on the list to get
// selected(one random) as a tile to be placed.
//
// The MAX_TILES is the amount of tiles that I have created to create the map with. I check the middle top/left/right/bottom
// tile value to see if there is a connection(1). Experiment with making different tiles.
//
// I might(should) add comments to the code later.
//


#include "raylib.h"

#define MAP_WIDTH 14
#define MAP_HEIGHT 12
#define MAX_TILES 15

static void newmap(void);
static void resetmap(void);
// If the selected map position has a connection the l/r/u/d side than return true/false
static bool domainhasleftsidedconnections(int x,int y);
static bool domainhasrightsidedconnections(int x,int y);
static bool domainhastopsidedconnections(int x,int y);
static bool domainhasbottomsidedconnections(int x,int y);
// Remove every tile flag from l/r/u/d.
static void removeleftsidedconnections(int x,int y);
static void removerightsidedconnections(int x,int y);
static void removetopsidedconnections(int x,int y);
static void removebottomsidedconnections(int x,int y);
// Select a random tile from the list of available tiles.
static void setrandomtile(int x,int y);
// Update the current map position checking l/r/u/d for connections and setting
// the flag for can use or can not use with setrandomtile.
static void updatedomain(int x,int y);
// create our final tilemap.
static void finishmap(void);

// Our tile array. 3by3 grid of x amount of tiles
static    int tile[3][3][MAX_TILES] = {0};
// Our work map. We have every tile set up there. -1 if not usable or 1 if usable.
static    int map[MAP_WIDTH][MAP_HEIGHT][MAX_TILES] = {0};
// This is a array we use to draw the map on the screen
static    int tilemap[MAP_WIDTH][MAP_HEIGHT] = {0};

int main(void)
{    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;



    InitWindow(screenWidth, screenHeight, "raylib example.");

//
// These are the tiles with which the map will be generated. You can set the amount you want to use in the
// define MAX_TILES x
// The code checks the connections between these tiles by reading if a 1 is on the center top/left/right/bottom.
// Also the draw routine draws the 1's as black.
//
// You could change(experiment!) and for instance use maybe 4 tiles and see the output. You can change the depth in the 
// newmap() function for how many times a tile is going to be selected and fit into the map.
    int tile1[3][3] = {     {0,0,0},
                            {1,1,1},
                            {0,0,0}};
                            
    int tile2[3][3] = {     {0,1,0},
                            {0,1,0},
                            {0,1,0}};
    
    int tile3[3][3] = {     {1,1,1},
                            {1,1,1},
                            {1,1,1}};    
    
    int tile4[3][3] = {     {0,1,0},
                            {1,1,1},
                            {0,1,0}};       

    int tile5[3][3] = {     {0,1,0},
                            {1,1,0},
                            {0,0,0}};
                            
    int tile6[3][3] = {     {0,1,0},
                            {0,1,1},
                            {0,0,0}};

    int tile7[3][3] = {     {0,0,0},
                            {1,1,0},
                            {0,1,0}};
                            
    int tile8[3][3] = {     {0,0,0},
                            {0,1,1},
                            {0,1,0}};

    int tile9[3][3] = {     {0,1,0},
                            {1,1,1},
                            {0,0,0}};    
    
    int tile10[3][3] = {    {0,1,0},
                            {0,1,1},
                            {0,1,0}};   
    
    int tile11[3][3] = {    {0,0,0},
                            {1,1,1},
                            {0,1,0}};    
    
    int tile12[3][3] = {    {0,1,0},
                            {1,1,0},
                            {0,1,0}};   

    int tile13[3][3] = {    {1,1,0},
                            {1,1,0},
                            {1,1,0}};   
    
    int tile14[3][3] = {    {0,1,1},
                            {0,1,1},
                            {0,1,1}};    
    
    int tile15[3][3] = {    {1,1,1},
                            {1,1,1},
                            {1,1,1}};   
    
//
// Here we copy each individual tile into a single array that we will
// use in the code.
    for(int i=0;i<MAX_TILES;i++){

        for(int y=0;y<3;y++){
            for(int x=0;x<3;x++){
                switch (i){
                    case 0:
                    tile[x][y][i] = tile1[x][y];
                    break;
                    case 1:
                    tile[x][y][i] = tile2[x][y];
                    break;
                    case 2:
                    tile[x][y][i] = tile3[x][y];
                    break;
                    case 3:
                    tile[x][y][i] = tile4[x][y];
                    break;
                    case 4:
                    tile[x][y][i] = tile5[x][y];
                    break;
                    case 5:
                    tile[x][y][i] = tile6[x][y];
                    break;
                    case 6:
                    tile[x][y][i] = tile7[x][y];
                    break;
                    case 7:
                    tile[x][y][i] = tile8[x][y];
                    break;
                    case 8:
                    tile[x][y][i] = tile9[x][y];
                    break;
                    case 9:
                    tile[x][y][i] = tile10[x][y];
                    break;
                    case 10:
                    tile[x][y][i] = tile11[x][y];
                    break;
                    case 11:
                    tile[x][y][i] = tile12[x][y];
                    break;
                    case 12:
                    tile[x][y][i] = tile13[x][y];
                    break;
                    case 13:
                    tile[x][y][i] = tile14[x][y];
                    break;
                    case 14:
                    tile[x][y][i] = tile15[x][y];
                    break;

                }
            }
        }
    }
    
    // Here we generate the first map
    newmap();
    
    
    SetTargetFPS(10);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------
    int cnt=0;
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        //---------------------------------------------------------------------------------
        cnt++;
        if(IsKeyReleased(KEY_SPACE) || cnt>4){
            newmap();
            cnt=0;

        }
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);
            
            for (int y = 0; y< MAP_HEIGHT ; y++)
            {
                for (int x = 0; x< MAP_WIDTH ; x++)
                {
                    int x3 = x*48;
                    int y3 = y*32;

                    for(int y2=0;y2<3;y2++){
                    for(int x2=0;x2<3;x2++){
                        
                        if( tile[x2][y2][tilemap[x][y]]==1 )DrawRectangle(x3+x2*16,y3+y2*12,16,12,BLACK);
                    }
                    }

                }
            }

            DrawText("Press space for new map.", 100, screenHeight-40, 40, LIGHTGRAY);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

    void newmap(){
        // Here we reset the map. Meaning nothing will be in the memory.
        resetmap();
        
        // here we generate the new map. I have set the depth to a number where it generates
        // output that I thought was ok!
        int depth=GetRandomValue(100,400);
        for(int i=0;i<depth;i++){
            setrandomtile(GetRandomValue(0,MAP_WIDTH-1),GetRandomValue(0,MAP_HEIGHT-1));
        }
        
        // Here we copy the remaining tile into the tilemap that we use to draw it onto the screen.        
        finishmap();

    };
    
    void finishmap(){
        for(int y=0;y<MAP_HEIGHT;y++){
        for(int x=0;x<MAP_WIDTH;x++){
            for(int i=0;i<MAX_TILES;i++){
                if(map[x][y][i]>-1){
                    tilemap[x][y] = i;
                    
                }
            }
        }}
    };

    void resetmap(){
        for(int y=0;y<MAP_HEIGHT;y++){
        for(int x=0;x<MAP_WIDTH;x++){
            for(int i=0;i<MAX_TILES;i++){
                map[x][y][i] = 1; // set every tile flag to usable(1)
            }
        }
        }
    };

    bool domainhasleftsidedconnections(int x,int y){
        if(x<0 || y<0 || x>=MAP_WIDTH || y>=MAP_HEIGHT)return false;
        for(int i=0;i<MAX_TILES;i++){
            if(map[x][y][i]==1 && tile[0][1][i]==1){
                return true;//the tile flag has a left side
            }
        }
        return false;//the map array has no tiles connecting on the left side
    };
    bool domainhasrightsidedconnections(int x,int y){
        if(x<0 || y<0 || x>=MAP_WIDTH || y>=MAP_HEIGHT)return false;
        for(int i=0;i<MAX_TILES;i++){
            if(map[x][y][i]==1 && tile[2][1][i]==1){
                return true;
            }
        }
        return false;
    };
    bool domainhastopsidedconnections(int x,int y){
        if(x<0 || y<0 || x>=MAP_WIDTH || y>=MAP_HEIGHT)return false;
        for(int i=0;i<MAX_TILES;i++){
            if(map[x][y][i]==1 && tile[1][0][i]==1){
                return true;
            }
        }
        return false;
    };
    bool domainhasbottomsidedconnections(int x,int y){
        if(x<0 || y<0 || x>=MAP_WIDTH || y>=MAP_HEIGHT)return false;
        for(int i=0;i<MAX_TILES;i++){
            if(map[x][y][i]==1 && tile[1][2][i]==1){
                return true;
            }
        }
        return false;
    };

    void removeleftsidedconnections(int x,int y){
        if(x<0 || y<0 || x>=MAP_WIDTH || y>=MAP_HEIGHT)return;
        for(int i=0;i<MAX_TILES;i++){
            if(map[x][y][i]==1 && tile[0][1][i]==1){ //if the flag of the tile is enabled(1) and the tile has a connection on the left(x)and center(y) spot
                map[x][y][i]=-1; // disable the flag
            }
        }
    };
    void removerightsidedconnections(int x,int y){
        if(x<0 || y<0 || x>=MAP_WIDTH || y>=MAP_HEIGHT)return;
        for(int i=0;i<MAX_TILES;i++){
            if(map[x][y][i]==1 && tile[2][1][i]==1){
                map[x][y][i]=-1;
            }
        }
    };
    void removetopsidedconnections(int x,int y){
        if(x<0 || y<0 || x>=MAP_WIDTH || y>=MAP_HEIGHT)return;
        for(int i=0;i<MAX_TILES;i++){
            if(map[x][y][i]==1 && tile[1][0][i]==1){
                map[x][y][i]=-1;
            }
        }
    };
    void removebottomsidedconnections(int x,int y){
        if(x<0 || y<0 || x>=MAP_WIDTH || y>=MAP_HEIGHT)return;
        for(int i=0;i<MAX_TILES;i++){
            if(map[x][y][i]==1 && tile[1][2][i]==1){
                map[x][y][i]=-1;
            }
        }
    };

    void setrandomtile(int x,int y){
        bool exitloop=false;
        int count=0;
        while(exitloop==false){
            count++;
            int val=GetRandomValue(0,MAX_TILES-1);//get a random number
            if(map[x][y][val]>-1){ //if the random number inside the map flag position is enabled(on the list!)
                for(int i=0;i<MAX_TILES;i++){ //disable every flag
                    map[x][y][i]=-1;
                }
                map[x][y][val]=1;//enable the one we randomly created
                exitloop = true;
                break;
            }
            if(count>500)exitloop=true;//if anything goes wrong than exit
        }
        // Update around the new tile so that the connections make sense
        updatedomain(x-1,y);
        updatedomain(x+1,y);
        updatedomain(x,y-1);
        updatedomain(x,y+1);
    };

    void updatedomain(int x,int y){
        if(x<0 || y<0 || x>=MAP_WIDTH || y>=MAP_HEIGHT)return;
        // Enable every tile flag
        for(int i=0;i<MAX_TILES;i++){
            map[x][y][i]=1;
        }
        // check sides and disable tiles pieces with connections that are not present.    
        if(domainhasbottomsidedconnections(x,y-1)==false)removetopsidedconnections(x,y);
        if(domainhastopsidedconnections(x,y+1)==false)removebottomsidedconnections(x,y);
        if(domainhasleftsidedconnections(x+1,y)==false)removerightsidedconnections(x,y);
        if(domainhasrightsidedconnections(x-1,y)==false)removeleftsidedconnections(x,y);
   };
