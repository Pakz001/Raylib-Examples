
#include "raylib.h"

// width/height/centerx/centery
static void midptellipse(int rx, int ry, int xc, int yc);
static int map[200][200]={0};

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib example.");

    
 
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------
    // width/height/centerx/centery
    midptellipse(10,7,30,30); 
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        
        midptellipse(GetRandomValue(5,10),GetRandomValue(5,10),GetRandomValue(5,100),GetRandomValue(5,100));    
        //----------------------------------------------------------------------------------
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            for(int y=0;y<100;y++){
            for(int x=0;x<100;x++){
                if(map[x][y]==1)DrawRectangle(x*3,y*3,3,3,BLACK);
            }
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

void midptellipse(int rx, int ry,  
                  int xc, int yc) 

{ 
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
            if(tx[i]>=0 && tx[i]<100 && ty[i]>=0 && ty[i]<100)map[tx[i]][ty[i]] = 1;
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
            if(tx[i]>=0 && tx[i]<100 && ty[i]>=0 && ty[i]<100)map[tx[i]][ty[i]] = 1;
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
