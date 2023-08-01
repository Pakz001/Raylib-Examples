//
// Here I tried to learn more about how to not let moving units go through each other when moving
// on the map.
//
// Note : I think when just checking for collide between units they should just both stop and plan
// a new path (for one?) towards the original destination.

#include "raylib.h"
#include <math.h>

typedef struct unit {
    float x, y;
    int asidex, asidey;
    int wait;
    bool moveaside;
    int pathloc;
    int pathlen;
} unit;

int path1x[12] = { 0, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 0 };
int path1y[12] = { 0, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 0 };

int path2x[12] = { 0, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 0 };
int path2y[12] = { 0, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 0 };

unit unit1 = { 0 };
unit unit2 = { 0 };
float tileWidth, tileHeight;

void iniunits();

int main(void) {
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib example.");

    tileWidth = 24.0f;
    tileHeight = 24.0f;
    iniunits();

    SetTargetFPS(60); // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------

        // Move unit 1
        if (unit1.wait > 0)
            unit1.wait -= 1;
        if (unit1.pathloc < unit1.pathlen && unit1.wait <= 0) {
            // get the next location in the path
            float x = (float)path1x[unit1.pathloc] * tileWidth;
            float y = (float)path1y[unit1.pathloc] * tileHeight;
            // If we are not moving aside currently
            if (unit1.moveaside == false) {
                // see if our next position is colliding with unit2
                int ax = path2x[unit2.pathloc + 1];
                int ay = path2y[unit2.pathloc + 1];
                int bx = path1x[unit1.pathloc];
                int by = path1y[unit1.pathloc];
                if (ax == bx && ay == by) {
                    // We will collide
                    unit1.moveaside = true;

                    // Get a position on the map to move aside to
                    bool found = false;
                    while (found == false) {
                        unit1.asidex = bx + GetRandomValue(-1, 1);
                        unit1.asidey = by + GetRandomValue(-1, 1);
                        found = true;
                        // if new position not on path of unit 1
                        for (unsigned int i = (unsigned int)unit1.pathloc - 1; i < (unsigned int)unit1.pathloc + 1; i++) {
                            if (unit1.asidex == path1x[i] && unit1.asidey == path1y[i])
                                found = false;
                        }
                        // if new position not on path of unit 2
                        for (unsigned int i = (unsigned int)unit2.pathloc - 1; i < (unsigned int)unit2.pathloc + 1; i++) {
                            if (unit1.asidex == path2x[i] && unit1.asidey == path2y[i])
                                found = false;
                        }
                        // Add checks for obstacles here...
                    }
                }
            }
            // if we are moving aside currently then adjust were we are moving to.
            if (unit1.moveaside) {
                x = (float)unit1.asidex * tileWidth;
                y = (float)unit1.asidey * tileHeight;
            }

            // Move our unit1
            if (unit1.x < x)
                unit1.x += 1;
            if (unit1.y < y)
                unit1.y += 1;
            if (unit1.x > x)
                unit1.x -= 1;
            if (unit1.y > y)
                unit1.y -= 1;

            // If we are at the destination..
            if ((int)unit1.x == (int)x && (int)unit1.y == (int)y) {
                // If we are moving aside then disable this mode and increase our location on the path.
                if (unit1.moveaside) {
                    unit1.moveaside = false;
                    unit1.pathloc += 3;
                    unit1.wait = 60;
                    //  If we are on the destination and not moving aside then increase our path location.
                } else {
                    unit1.pathloc++;
                }
            }
        }

        // Move unit 2
        if (unit2.pathloc < unit2.pathlen) {
            float x = (float)path2x[unit2.pathloc] * tileWidth;
            float y = (float)path2y[unit2.pathloc] * tileHeight;

            if (unit2.x < x)
                unit2.x += 1;
            if (unit2.y < y)
                unit2.y += 1;
            if (unit2.x > x)
                unit2.x -= 1;
            if (unit2.y > y)
                unit2.y -= 1;

            if ((int)unit2.x == (int)x && (int)unit2.y == (int)y) {
                unit2.pathloc++;
            }
        } else {
            iniunits();
        }

        //----------------------------------------------------------------------------------
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawRectangle((int)unit1.x, (int)unit1.y, (int)tileWidth, (int)tileHeight, RED);
        DrawRectangle((int)unit2.x, (int)unit2.y, (int)tileWidth, (int)tileHeight, BLUE);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

void iniunits() {
    unit1.pathloc = 1;
    unit1.pathlen = 10;
    unit1.x = (float)path1x[unit1.pathloc] * tileWidth;
    unit1.y = (float)path1y[unit1.pathloc] * tileHeight;
    unit1.moveaside = false;
    unit2.pathloc = 1;
    unit2.pathlen = 10;
    unit2.x = (float)path2x[unit2.pathloc] * tileWidth;
    unit2.y = (float)path2y[unit2.pathloc] * tileHeight;
    unit2.moveaside = false;
    return;
}
