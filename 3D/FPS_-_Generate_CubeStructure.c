//
// This example uses the included fps free movement camera. It needs the entire map(all models) to move up or down since the camera
// with this mode is locked to its y position.
//
//

#include "raylib.h"
#include <stdlib.h>


int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib example.");
 
     // We generate a checked image for texturing WALLS
    Image checked = GenImageChecked(2, 2, 1, 1, WHITE, DARKGRAY);
    Texture2D texture = LoadTextureFromImage(checked);
    UnloadImage(checked);
 
 
    Model model = { 0 };
    model = LoadModelFromMesh(GenMeshCube(1.0f, 1.0f, 1.0f));
    
    // Set checked texture as default diffuse component for all models material
    model.materials[0].maps[MAP_DIFFUSE].texture = texture;

    //////// WALKABLE

    Image checked2 = GenImageChecked(2, 2, 1, 1, (Color){100,0,0,255},RED);
    Texture2D texture2 = LoadTextureFromImage(checked2);
    UnloadImage(checked2);
 
    Model model2 = { 0 };
    model2 = LoadModelFromMesh(GenMeshCube(1.0f, 1.0f, 1.0f));
    
    // Set checked texture as default diffuse component for all models material
    model2.materials[0].maps[MAP_DIFFUSE].texture = texture2;

    ///////// GRASS
    
    Image checked3 = GenImageChecked(8, 8, 1, 1, (Color){10,150,20,255},GREEN);
    Texture2D texture3 = LoadTextureFromImage(checked3);
    UnloadImage(checked3);
 
    Model model3 = { 0 };
    model3 = LoadModelFromMesh(GenMeshCube(1.0f, 1.0f, 1.0f));
    
    // Set checked texture as default diffuse component for all models material
    model3.materials[0].maps[MAP_DIFFUSE].texture = texture3;


    // Define the camera to look into our 3d world
    Camera camera = { { 5.0f, 5.0f, 5.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, 45.0f, 0 };


    SetCameraMode(camera, CAMERA_FIRST_PERSON);     // Set camera mode
    //SetCameraMode(camera, CAMERA_ORBITAL);  // Set a orbital camera mode

 
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    float myy={0.0f};
    
    // drunkenwalk from top to down.
    int cavew=39;
    int caveh=29;
    int caved=39;
    int cave[cavew][caveh][caved];
    int caveb[cavew][caveh][caved];

    for(int x=0;x<cavew;x++){
        for(int y=0;y<caveh;y++){
            for(int z=0;z<caved;z++){
                cave[x][y][z] = 0;
                caveb[x][y][z] = 0;
            }
        }
    }
    int x=20;
    int y;
    int z=20;
    for(int y=caveh-1;y>0;y--){
        int oldx=x;
        int oldz=z;
        x+=GetRandomValue(-1,1);
        z+=GetRandomValue(-1,1);        
        if(x<0 || x>=cavew)x=oldx;
        if(z<0 || z>=caved)z=oldz;
        
        
//        cave[x][y][z]=1;
        
        for(int x1=-3;x1<3;x1++){
        for(int z1=-3;z1<3;z1++){
        for(int y1=-3;y1<3;y1++){
            if(x+x1<0 || x+x1>=cavew)continue;
            if(z+z1<0 || z+z1>=caved)continue;
            if(y+y1<0 || y+y1>=caveh)continue;
            cave[x+x1][y+y1][z+z1] = 1;
            
        }
        }
        }
                
        if(GetRandomValue(0,10)<9 && y<caveh)y+=1;
    }
    
    // 
    // Remove doubles
    for(int x1=0;x1<cavew;x1++){
    for(int z1=0;z1<caved;z1++){
    for(int y1=0;y1<caveh;y1++){
        if(cave[x1][y1][z1]==0){
            int cnt=0;
        for(int x2=-1;x2<2;x2++){
        for(int z2=-1;z2<2;z2++){
        for(int y2=-1;y2<2;y2++){
            if(x1+x2<0 || x1+x2>=cavew || y1+y2<0 || y1+y2>=caveh || z1+z2<0 || z1+z2>=caved)continue;

            if(cave[x1+x2][y1+y2][z1+z2]==1)cnt+=1;
        }
        }
        }
        if(cnt>0)caveb[x1][y1][z1]=1;
        }
    
    }
    }
    }


    // Walkable/roofs (model2)
    for(int x1=0;x1<cavew;x1++){
    for(int z1=0;z1<caved;z1++){
    for(int y1=0;y1<caveh;y1++){
        if(caveb[x1][y1][z1]==1){
            if(y1+1<=caveh && caveb[x1][y1+1][z1]==0){
                caveb[x1][y1][z1] = 2;
            }
        }
    }
    }
    }
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        UpdateCamera(&camera);      // Update internal camera and our camera

        // Move all models up or down.
        if(IsKeyDown(KEY_R))myy-=0.2f;
        if(IsKeyDown(KEY_F))myy+=0.2f;
        
        //----------------------------------------------------------------------------------
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);
 
            BeginMode3D(camera);

                for(int x=0;x<50;x++){
                for(int z=0;z<50;z++){
                    Vector3 position;
                    position.x = x;
                    position.y = 0+myy;
                    position.z = z;
                    DrawModel(model3, position, 1.0f, WHITE);
                }
                }

                
                for(int x=0;x<cavew;x++){
                for(int y=0;y<caveh;y++){
                for(int z=0;z<caved;z++){
                    if(caveb[x][y][z]>0){
                        Vector3 position;
                        position.x = x;
                        position.y = y+myy;
                        position.z = z;
                        if(caveb[x][y][z]==1){
                            DrawModel(model, position, 1.0f, WHITE);
                        }else{
                            DrawModel(model2, position, 1.0f, WHITE);
                        }
                    }
                }
                }
                }
                

            EndMode3D();
            
            DrawFPS(0,0);
            DrawText("W/A/S/D - R/F and Mouse = Controls.",100,0,20,BLACK);
            
               

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------

    UnloadTexture(texture); // Unload texture
    UnloadTexture(texture2); // Unload texture
    UnloadTexture(texture3); // Unload texture
    // Unload models data (GPU VRAM)
    UnloadModel(model);
    UnloadModel(model2);
    UnloadModel(model3);

    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;


}
