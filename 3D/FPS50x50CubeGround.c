
#include "raylib.h"
#include <stdlib.h>


int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib example.");

    // We generate a checked image for texturing
    Image checked = GenImageChecked(2, 2, 1, 1, RED, GREEN);
    Texture2D texture = LoadTextureFromImage(checked);
    UnloadImage(checked);

    Model model = { 0 };
    model = LoadModelFromMesh(GenMeshCube(1.0f, 1.0f, 1.0f));
    
    // Set checked texture as default diffuse component for all models material
    model.materials[0].maps[MAP_DIFFUSE].texture = texture;

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
        UpdateCamera(&camera);      // Update internal camera and our camera

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
                    position.y = 0;
                    position.z = z;
                    DrawModel(model, position, 1.0f, WHITE);
                }
                }

            EndMode3D();
            
            DrawFPS(0,0);
            

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
   UnloadTexture(texture); // Unload texture

    // Unload models data (GPU VRAM)
    UnloadModel(model);

    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;


}

