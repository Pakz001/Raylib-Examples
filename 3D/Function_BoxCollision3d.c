// Yet to be tested . . . working (star trek voice). .. 


#include "raylib.h"
#include <stdlib.h>
#include <math.h>


bool boxcollision(float x1,float y1,float z1,float w1,float h1,float d1, float x2, float y2, float z2,float w2,float h2,float d2);
    
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


    Vector3 bulletpos;
    bulletpos = camera.position;
    Vector3 bulletinc;
    bulletinc.x = 0.05;
    bulletinc.y = 0.00;
    bulletinc.z = 0.0;
    

    Model bullet = { 0 };
    bullet = LoadModelFromMesh(GenMeshCube(1.0f, 1.0f, 1.0f));
    
    // Set checked texture as default diffuse component for all models material
    bullet.materials[0].maps[MAP_DIFFUSE].texture = texture;

 
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

        
        // update the bullet.
        bulletpos.x += bulletinc.x;
        bulletpos.y += bulletinc.y;
        bulletpos.z += bulletinc.z;
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

            EndMode3D();
            
            DrawText("Press space and aim to shoot bullet.",100,100,30,YELLOW);
            DrawFPS(0,0);
            

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
   UnloadTexture(texture); // Unload texture

    // Unload models data (GPU VRAM)
    UnloadModel(model);
    UnloadModel(bullet);

    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;


}

bool boxcollision(  float x1, float y1, float z1,float w1,float h1,float d1, 
                    float x2, float y2, float z2,float w2,float h2,float d2){
    return  (x1 <= x2+w2 && x1+w1 >= x2) &&
            (y1 <= y2+h2 && y1+h1 >= y2) &&
            (z1 <= z2+d2 && z1+d1 >= z2);
}
