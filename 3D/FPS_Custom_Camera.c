//
// If this will not conpile than look for the files raylib.h and raymath.h and copy these
// into the folder where this file is. You can find these files in the raylib /src folder.
//
//


#include "raylib.h"
#include "raymath.h"
#include <stdlib.h>


#define PLAYER_MOVEMENT_SENSITIVITY 3
#define CAMERA_MOUSE_MOVE_SENSITIVITY 0.01f

static Vector2 lastMousePos;
static Vector3 movement = {0, -0.2f, 0};
static Vector3 rotation ;


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

 
    camera.position = (Vector3){50.0f, 1.0f, 50.0f};
    camera.target = (Vector3){50.0f, 1.0f, 50.0f};
    camera.up = (Vector3){0.0f, 1.0f, 0.0f};
    camera.fovy = 60.0f;
    camera.type = CAMERA_PERSPECTIVE;
    rotation = (Vector3){0.0f, 0.0f, 0.0f};
    //world = world;
    lastMousePos = GetMousePosition();
 
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        //UpdateCamera(&camera);      // Update internal camera and our camera
        // -------------------- Rotation stuff --------------------
        Vector2 mouseMovement = Vector2Subtract(GetMousePosition(), lastMousePos);

        lastMousePos = GetMousePosition();

        rotation.x += (mouseMovement.x*-CAMERA_MOUSE_MOVE_SENSITIVITY);
        rotation.y += (mouseMovement.y*-CAMERA_MOUSE_MOVE_SENSITIVITY);
        
        // -------------------- Movement stuff --------------------

        float direction[] = { IsKeyDown(KEY_W),
                              IsKeyDown(KEY_S),
                              IsKeyDown(KEY_D),
                              IsKeyDown(KEY_A)};

        //movement = (Vector3){0, movement.y + GetFrameTime() * -0.5, 0};
        movement = (Vector3){0, 0, 0};
        movement.x = (sinf(rotation.x)*direction[1] -
                      sinf(rotation.x)*direction[0] -
                      cosf(rotation.x)*direction[3] +
                      cosf(rotation.x)*direction[2])/PLAYER_MOVEMENT_SENSITIVITY;

        movement.z = (cosf(rotation.x)*direction[1] -
                      cosf(rotation.x)*direction[0] +
                      sinf(rotation.x)*direction[3] -
                      sinf(rotation.x)*direction[2])/PLAYER_MOVEMENT_SENSITIVITY;

        if (IsKeyDown(KEY_F))
            movement.y -= 0.12f;
        if (IsKeyDown(KEY_R))
            movement.y += 0.12f;

        camera.position.x += movement.x / PLAYER_MOVEMENT_SENSITIVITY;

        camera.position.y += movement.y;

        camera.position.z += movement.z / PLAYER_MOVEMENT_SENSITIVITY;

        // -------------------- Retarget stuff --------------------

        Matrix translation = MatrixTranslate(0, 0, (10));
        Matrix rotation2 = MatrixRotateXYZ((Vector3){ PI*2 - rotation.y, PI*2 - rotation.x, 0 });
        Matrix transform = MatrixMultiply(translation, rotation2);

        camera.target.x = camera.position.x - transform.m12;
        camera.target.y = camera.position.y - transform.m13;
        camera.target.z = camera.position.z - transform.m14;
        
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
            
            DrawText("Controls - Mouse w/a/s/d/ r/f.",400,0,20,DARKGRAY);
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
