
#include "raylib.h"
#include <stdlib.h>
#include <math.h>

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib example.");

    // We generate a checked image for texturing
    Image checked = GenImageChecked(2, 2, 1, 1, BLACK, DARKGRAY);
    Texture2D texture = LoadTextureFromImage(checked);
    UnloadImage(checked);

    Model model = { 0 };
    model = LoadModelFromMesh(GenMeshCube(1.0f, 1.0f, 1.0f));
    
    // Set checked texture as default diffuse component for all models material
    model.materials[0].maps[MAP_DIFFUSE].texture = texture;

    // Define the camera to look into our 3d world
    Camera camera = { { 5.0f, 5.0f, 5.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, 45.0f, 0 };


    SetCameraMode(camera, CAMERA_FIRST_PERSON);     // Set camera mode


    // Our bullet setup
    //
    Vector3 bulletpos;
    bulletpos = camera.position;
    Vector3 bulletinc;
    bulletinc.x = 0.05;
    bulletinc.y = 0.00;
    bulletinc.z = 0.0;
    
    // Our bullet model
    Model bullet = { 0 };
    bullet = LoadModelFromMesh(GenMeshCube(1.0f, 1.0f, 1.0f));
    
    // Set checked texture as default diffuse component for all models material
    bullet.materials[0].maps[MAP_DIFFUSE].texture = texture;

    // Our target model
    //
    Vector3 targetboxposition;
    targetboxposition.x = 20;
    targetboxposition.y = 10;
    targetboxposition.z = 20;
    
    Model targetbox = { 0 };
    targetbox = LoadModelFromMesh(GenMeshCube(1.0f, 1.0f, 1.0f));    
    // Set checked texture as default diffuse component for all models material
    targetbox.materials[0].maps[MAP_DIFFUSE].texture = texture;

 
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        UpdateCamera(&camera);      // Update internal camera and our camera

        // Shoot the bullet.
        if(IsKeyDown(KEY_SPACE)){
            bulletpos = camera.position;
            // Get the direction of where to shoot the bullet at.
            float x1 = camera.position.x;
            float y1 = camera.position.y;
            float z1 = camera.position.z;
            float x2 = camera.target.x;
            float y2 = camera.target.y;
            float z2 = camera.target.z;
            float delta_x = x2 - x1;
            float delta_y = y2 - y1;
            float delta_z = z2 - z1;
            
            float len = sqrt(delta_x*delta_x+delta_y*delta_y+delta_z*delta_z);
            delta_x = delta_x / len;
            delta_y = delta_y / len;            
            delta_z = delta_z / len;          
            bulletinc.x = delta_x;
            bulletinc.y = delta_y;
            bulletinc.z = delta_z;
        }

        
        // update the bullet position.
        bulletpos.x += bulletinc.x;
        bulletpos.y += bulletinc.y;
        bulletpos.z += bulletinc.z;
 
       // collision between the bullet and the targetbox.
       // Check collisions player vs enemy-box
        if (CheckCollisionBoxes(
            (BoundingBox){(Vector3){ bulletpos.x - 0.5f,
                                     bulletpos.y - 0.5f,
                                     bulletpos.z - 0.5f },
                          (Vector3){ bulletpos.x + 0.5f,
                                     bulletpos.y + 0.5f,
                                     bulletpos.z + 0.5f }},
            (BoundingBox){(Vector3){ targetboxposition.x - 0.5f,
                                     targetboxposition.y - 0.5f,
                                     targetboxposition.z - 0.5f },
                          (Vector3){ targetboxposition.x + 0.5f,
                                     targetboxposition.y + 0.5f,
                                     targetboxposition.z + 0.5f }})){
            // Put the target box at a new position.
            targetboxposition = (Vector3){(float)GetRandomValue(5,40),(float)GetRandomValue(2,10),(float)GetRandomValue(5,40)};
        }
        //----------------------------------------------------------------------------------
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            BeginMode3D(camera);

            // Draw a floor.
            for(int x=0;x<50;x++){
            for(int z=0;z<50;z++){
                Vector3 position;
                position.x = x;
                position.y = 0;
                position.z = z;
                DrawModel(model, position, 1.0f, WHITE);
            }
            }

            // Draw our bullet.
            DrawModel(bullet, bulletpos, 1.0f, WHITE);


            // Draw our targetbox.
            DrawModel(targetbox, targetboxposition, 1.0f, WHITE);

            EndMode3D();

            // Draw our Crosshair - Simply a PLUS character drawn with DrawText in RED.
            DrawText("+",screenWidth/2-20,screenHeight/2-20,40,RED);
            
            DrawText("Press space and aim to shoot bullet.",100,100,30,RED);
            DrawFPS(0,0);
            

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
   UnloadTexture(texture); // Unload texture

    // Unload models data (GPU VRAM)
    UnloadModel(model);
    UnloadModel(bullet);
    UnloadModel(targetbox);

    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;


}
