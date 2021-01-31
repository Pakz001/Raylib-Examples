
#include "raylib.h"

bool get_line_intersect(float p0_x, float p0_y, float p1_x, float p1_y, float p2_x ,float p2_y, float p3_x, float p3_y);
bool polypolycollide(Vector2 poly1[],int as1, Vector2 poly2[], int as2);

int debug=0;

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib example.");
 
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
            //
            // Create a Vector2 array and create some points in it
            //
            Vector2 pol1[3];
            pol1[0] = (Vector2){200,100};
            pol1[1] = (Vector2){20,300};
            pol1[2] = (Vector2){280,300};
            
            Vector2 pol2[3];
            pol2[0] = (Vector2){50+GetMouseX()-50,50+GetMouseY()-50};
            pol2[1] = (Vector2){20+GetMouseX()-50,150+GetMouseY()-50};
            pol2[2] = (Vector2){90+GetMouseX()-50,150+GetMouseY()-50};

            if(polypolycollide(     pol1,sizeof(pol1)/sizeof(pol1[0]),
                                    pol2,sizeof(pol2)/sizeof(pol2[0]))==true){
                DrawText(FormatText("Collision"),0,0,20,RED);
            }

            DrawText(FormatText("%i",debug),0,20,20,RED);
            
            DrawTriangle(pol1[0],pol1[1],pol1[2],RED);
            DrawTriangle(pol2[0],pol2[1],pol2[2],YELLOW);


        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;


}

bool polypolycollide(Vector2 poly1[],int as1, Vector2 poly2[], int as2){
    
	//'first check if the second polygon is inside the first polygon
	int x=-5000;
    int y=-5000;
	int cnt=0;
	for(int i=1;i<as1;i++){
        if(get_line_intersect(  x,
                                y,
                                poly2[0].x,
                                poly2[0].y,
                                poly1[i-1].x,
                                poly1[i-1].y,
                                poly1[i].x,
                                poly1[i].y))cnt++;
	}
    if(get_line_intersect(      x,y,
                                poly2[0].x,poly2[0].y,
                                poly1[as1-1].x,poly1[as1-1].y,
                                poly1[0].x,poly1[0].y))cnt++;
    
    if( (cnt % 2) > 0 )return true;
    
    

	//'Now check if the first polygon is inside the second polygon
	x=-5000;
	y=-5000;
	cnt=0;
	for(int i=1;i<as2;i++){
		if(get_line_intersect(      x,y,
                                    poly1[0].x,poly1[0].y,
                                    poly2[i-1].x,poly2[i-1].y,
                                    poly2[i].x,poly2[i].y))cnt++;
	}
	if(get_line_intersect(      x,y,
                                poly1[0].x,poly1[0].y,
                                poly2[as2-1].x,
                                poly2[as2-1].y,
                                poly2[0].x,poly2[0].y))cnt++;
	//'
	if((cnt % 2) > 0)return true;

	//'
	//' Now check if any of the lines of the two polygons touch
    for(int i=1;i<as1;i++){
    for(int j=1;j<as2;j++){
		if(get_line_intersect(      poly2[j-1].x,poly2[j-1].y,
                                    poly2[j].x,poly2[j].y,
                                    poly1[i-1].x,poly1[i-1].y,
                                    poly1[i].x,poly1[i].y))return true;
    }}
    for(int j=1;j<as2;j++){
		if(get_line_intersect(  poly2[j-1].x,poly2[j-1].y,
                                poly2[j].x,poly2[j].y,
                                poly1[0].x,poly1[0].y,
                                poly1[as1-1].x,poly1[as1-1].y))return true;
	}
    for(int j=1;j<as1;j++){
		if(get_line_intersect(  poly1[j-1].x,poly1[j-1].y,
                                poly1[j].x,poly1[j].y,
                                poly2[0].x,poly2[0].y,
                                poly2[as2-1].x,poly2[as2-1].y))return true;
    }
	//'There was no Collision between the two polygons ___ Return
	return false;
}

// This function was originally by andre la moth.
bool get_line_intersect(float p0_x, float p0_y, float p1_x, float p1_y, float p2_x ,float p2_y, float p3_x, float p3_y){
    float s1_x;
    float s1_y; 
    float s2_x; 
    float s2_y;
    
    s1_x = p1_x - p0_x;     
    s1_y = p1_y - p0_y;
    s2_x = p3_x - p2_x;
    s2_y = p3_y - p2_y;

    float s;
    float t;
    
    s = (-s1_y * (p0_x - p2_x) + s1_x * (p0_y - p2_y)) / (-s2_x * s1_y + s1_x * s2_y);
    t = ( s2_x * (p0_y - p2_y) - s2_y * (p0_x - p2_x)) / (-s2_x * s1_y + s1_x * s2_y);

    if(s >= 0 && s <= 1 && t >= 0 && t <= 1) return true;
    
    return false; // No collision    
}
