//
// I took this from a example source and slapped it together.
//
// It kind of works...


#include "raylib.h"
#include "rlgl.h"
#include <stdlib.h>
#include <string.h>

#define MAX_MESH_VBO 7

static float *GetCubeVertices(float x, float y, float z);
static float texcoordsRef[];
static float normalsRef[];
    
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib example.");


    // Define the camera to look into our 3d world
    Camera camera = { 0 };
    camera.position = (Vector3){ 10.0f, 10.0f, 10.0f }; // Camera position
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };      // Camera looking at point
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                // Camera field-of-view Y
    


    SetCameraMode(camera, CAMERA_FIRST_PERSON);     // Set camera mode
    //SetCameraMode(camera, CAMERA_ORBITAL);  // Set a orbital camera mode






    Mesh mesh = {0};
    mesh.vboId = (unsigned int *)RL_CALLOC(MAX_MESH_VBO, sizeof(unsigned int));
    float *vertices = RL_MALLOC(36 * 3 * 1 * sizeof(float));
    float *texcoords = RL_MALLOC(36 * 2 * 1 * sizeof(float));
    float *normals = RL_MALLOC(36 * 3 * 1 * sizeof(float));

    int verticesCount = 0;
    int texcoordsCount = 0;
    int normalsCount = 0;
    
    int x=0,y=0,z=0;
    
    float *blockVertices = GetCubeVertices(x, y, z);
    for (int v = 0; v < 36 * 3; v++)
    {
        vertices[verticesCount + v] = blockVertices[v];
    }
    for (int t = 0; t < 36 * 2; t++)
    {
        texcoords[texcoordsCount + t] = texcoordsRef[t];
    }
    for (int n = 0; n < 36 * 3; n++)
    {
        normals[normalsCount + n] = normalsRef[n];
    }
    verticesCount += 36 * 3;
    texcoordsCount += 36 * 2;
    normalsCount += 36 * 3;    
    

    mesh.vertices = (float *)RL_MALLOC(verticesCount * sizeof(float));
    memcpy(mesh.vertices, vertices, verticesCount * sizeof(float));

    mesh.texcoords = (float *)RL_MALLOC(texcoordsCount * sizeof(float));
    memcpy(mesh.texcoords, texcoords, texcoordsCount * sizeof(float));

    mesh.normals = (float *)RL_MALLOC(normalsCount * sizeof(float));
    memcpy(mesh.normals, normals, normalsCount * sizeof(float));

    mesh.vertexCount = verticesCount / 3;         // fixme: Why divide by 3 ???
    mesh.triangleCount = (verticesCount / 3) / 2; // fixme: Why divide by 3 and 2 ???

    RL_FREE(vertices);
    RL_FREE(texcoords);
    RL_FREE(normals);

    rlLoadMesh(&mesh, false);

    Model worldModel = LoadModelFromMesh(mesh);


    // We generate a checked image for texturing
    Image checked = GenImageChecked(4, 4, 1, 1, RED, GREEN);
    Texture2D texture = LoadTextureFromImage(checked);
    UnloadImage(checked);
    worldModel.materials[0].maps[MAP_DIFFUSE].texture = texture;

    //worldModel.materials[0].maps[MAP_DIFFUSE].texture = LoadTexture("resources/grass.png");







 
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

            Vector3 pos;
            pos.x = 9;
            pos.y = 2;
            pos.z = 13;
            DrawModel(worldModel, pos, 1.0f, WHITE);

            EndMode3D();
            
            DrawFPS(0,0);
            

        EndDrawing();
        //----------------------------------------------------------------------------------
    }


    //--------------------------------------------------------------------------------------
    UnloadModel(worldModel); 
    UnloadTexture(texture); // Unload texture
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;


}

static float texcoordsRef[] = {
    // face 1
    0.5f, 1.0f,
    0.25f, 1.0f,
    0.25f, 0.0f,

    0.25f, 0.0f,
    0.5f, 0.0f,
    0.5f, 1.0f,


    // face 2
    0.25f, 1.0f,
    0.25f, 0.0f,
    0.5f, 0.0f,

    0.5f, 0.0f,
    0.5f, 1.0f,
    0.25f, 1.0f,

    // face 3 (top)
    0.0f, 0.0f,
    0.25f, 0.0f,
    0.25f, 1.0f,

    0.25f, 1.0f,
    0.0f, 1.0f,
    0.0f, 0.0f,

    // face 4 (bottom)
    0.0f, 0.0f,
    0.25f, 0.0f,
    0.25f, 1.0f,

    0.25f, 1.0f,
    0.0f, 1.0f,
    0.0f, 0.0f,

    // face 5
    0.25f, 1.0f,
    0.25f, 0.0f,
    0.5f, 0.0f,

    0.5f, 0.0f,
    0.5f, 1.0f,
    0.25f, 1.0f,

    // face 6
    0.5f, 1.0f,
    0.25f, 1.0f,
    0.25f, 0.0f,

    0.25f, 0.0f,
    0.5f, 0.0f,
    0.5f, 1.0f,
};

static float normalsRef[] = {
    // face 1
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,

    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,

    // face 2
    0.0f, 0.0f, -1.0f,
    0.0f, 0.0f, -1.0f,
    0.0f, 0.0f, -1.0f,

    0.0f, 0.0f, -1.0f,
    0.0f, 0.0f, -1.0f,
    0.0f, 0.0f, -1.0f,

    // face 3
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,

    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,

    // face 4
    0.0f, -1.0f, 0.0f,
    0.0f, -1.0f, 0.0f,
    0.0f, -1.0f, 0.0f,

    0.0f, -1.0f, 0.0f,
    0.0f, -1.0f, 0.0f,
    0.0f, -1.0f, 0.0f,

    // face 5
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,

    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,

    // face 6
    -1.0f, 0.0f, 0.0f,
    -1.0f, 0.0f, 0.0f,
    -1.0f, 0.0f, 0.0f,

    -1.0f, 0.0f, 0.0f,
    -1.0f, 0.0f, 0.0f,
    -1.0f, 0.0f, 0.0f};

// note: Yes, the implementation of the world model is dirty.
// todo: reimplement as chunks

static float *GetCubeVertices(float x, float y, float z)
{
    // not using indices
    float width = 1.0f;
    float height = 1.0f;
    float length = 1.0f;
    float *cubeVertices = malloc(36 * 3 * sizeof(float));
    float ref[] = {
        // face 1
        -width / 2 + x, -height / 2 + y, length / 2 + z,
        width / 2 + x, -height / 2 + y, length / 2 + z,
        width / 2 + x, height / 2 + y, length / 2 + z,

        width / 2 + x, height / 2 + y, length / 2 + z,
        -width / 2 + x, height / 2 + y, length / 2 + z,
        -width / 2 + x, -height / 2 + y, length / 2 + z,

        // face 2
        -width / 2 + x, -height / 2 + y, -length / 2 + z,
        -width / 2 + x, height / 2 + y, -length / 2 + z,
        width / 2 + x, height / 2 + y, -length / 2 + z,

        width / 2 + x, height / 2 + y, -length / 2 + z,
        width / 2 + x, -height / 2 + y, -length / 2 + z,
        -width / 2 + x, -height / 2 + y, -length / 2 + z,

        // face 3
        -width / 2 + x, height / 2 + y, -length / 2 + z,
        -width / 2 + x, height / 2 + y, length / 2 + z,
        width / 2 + x, height / 2 + y, length / 2 + z,

        width / 2 + x, height / 2 + y, length / 2 + z,
        width / 2 + x, height / 2 + y, -length / 2 + z,
        -width / 2 + x, height / 2 + y, -length / 2 + z,

        // face 4
        -width / 2 + x, -height / 2 + y, -length / 2 + z,
        width / 2 + x, -height / 2 + y, -length / 2 + z,
        width / 2 + x, -height / 2 + y, length / 2 + z,

        width / 2 + x, -height / 2 + y, length / 2 + z,
        -width / 2 + x, -height / 2 + y, length / 2 + z,
        -width / 2 + x, -height / 2 + y, -length / 2 + z,

        // face 5
        width / 2 + x, -height / 2 + y, -length / 2 + z,
        width / 2 + x, height / 2 + y, -length / 2 + z,
        width / 2 + x, height / 2 + y, length / 2 + z,

        width / 2 + x, height / 2 + y, length / 2 + z,
        width / 2 + x, -height / 2 + y, length / 2 + z,
        width / 2 + x, -height / 2 + y, -length / 2 + z,

        // face 6
        -width / 2 + x, -height / 2 + y, -length / 2 + z,
        -width / 2 + x, -height / 2 + y, length / 2 + z,
        -width / 2 + x, height / 2 + y, length / 2 + z,

        -width / 2 + x, height / 2 + y, length / 2 + z,
        -width / 2 + x, height / 2 + y, -length / 2 + z,
        -width / 2 + x, -height / 2 + y, -length / 2 + z};

    for (int i = 0; i < 36 * 3; i++)
    {
        cubeVertices[i] = ref[i];
    }
    
    return cubeVertices;
}    
