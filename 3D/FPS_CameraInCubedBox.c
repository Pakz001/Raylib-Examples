
#include "raylib.h"
#include <stdlib.h>


// On my i7-7700HQ with Nvidia 1050ti I get 60Fps with these box settings.
const int mapwidth = 30;
const int mapheight = 10;
const int mapdepth = 30;

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib example.");

    // We generate a checked image for texturing
    Image checked = GenImageChecked(2, 2, 1, 1, (Color){10,10,10,255}, (Color){50,50,50,255});
    Texture2D texture = LoadTextureFromImage(checked);
    UnloadImage(checked);
    
    Image checked2 = GenImageChecked(2, 2, 1, 1, (Color){40,10,10,255}, (Color){90,50,50,255});
    Texture2D texture2 = LoadTextureFromImage(checked2);
    UnloadImage(checked2);


    Model model = { 0 };
    model = LoadModelFromMesh(GenMeshCube(1.0f, 1.0f, 1.0f));
    Model model2 = { 0 };
    model2 = LoadModelFromMesh(GenMeshCube(1.0f, 1.0f, 1.0f));
    
    // Set checked texture as default diffuse component for all models material
    model.materials[0].maps[MAP_DIFFUSE].texture = texture;
    model2.materials[0].maps[MAP_DIFFUSE].texture = texture2;

    // Define the camera to look into our 3d world
    Camera camera = { { 5.0f, 5.0f, 5.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, 45.0f, 0 };


    SetCameraMode(camera, CAMERA_FIRST_PERSON);     // Set camera mode
    //SetCameraMode(camera, CAMERA_ORBITAL);  // Set a orbital camera mode

 
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // Here we store the old camera position and then check if the camera hits the edge
        // and if that happens we restore it to the old position. We check x and z different
        // so we get sliding in stead of complete stop when hitting an edge.
        // 
        Vector3 oldCamPos = camera.position;    // Store old camera position

        // Default walking speed
        int moveSpeed=1;
        // Sprint speed
        if (IsKeyDown(KEY_LEFT_SHIFT)){
            moveSpeed=2;
        }
        // We check collision and movement x amount of times. 
        for(int i=0;i<moveSpeed;i++){
            UpdateCamera(&camera);      // Update internal camera and our camera        
            if (camera.position.x<1 || camera.position.x>mapwidth-2){
            camera.position.x = oldCamPos.x;
            }
            if (camera.position.z<1 || camera.position.z>mapdepth-2){
            camera.position.z = oldCamPos.z;
            }
        }
        
        //----------------------------------------------------------------------------------
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            BeginMode3D(camera);

                //Our player box!!
                // 
                // We create our box by creating a tripple for loop and drawing cubes there
                // on the edges.
                // 
                for(int x=0;x<mapwidth;x++){
                for(int y=0;y<mapheight;y++){
                for(int z=0;z<mapdepth;z++){
                    if(x==0 || y==0 || x==mapwidth-1 || y==mapheight-1 || z==0 || z==mapdepth-1){
                        Vector3 position;
                        position.x = x;
                        position.y = y;
                        position.z = z;
                        if (y==0 || y==9 ){                    
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
            

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    UnloadTexture(texture); // Unload texture
    UnloadTexture(texture2); // Unload texture
    // Unload models data (GPU VRAM)
    UnloadModel(model);
    UnloadModel(model2);

    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;


}

