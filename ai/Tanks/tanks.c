
#include "raylib.h"
#include <math.h>

Texture2D sprites;

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");
    sprites = LoadTexture("resources/sprites.png");
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    int maxcommand=3;
    int maxcommand2=3;

    int command[maxcommand];
    int value[maxcommand];
    

    int command2[maxcommand];
    int value2[maxcommand];



            for(int i=0;i<maxcommand;i++){
                command[i]=GetRandomValue(1,3);
                value[i]=GetRandomValue(0,100);
            }

            for(int i=0;i<maxcommand;i++){
                command2[i]=GetRandomValue(2,3);
                value2[i]=GetRandomValue(0,100);
            }

    Vector2 position={320.0f,200.0f};
    float angle=0;
    float angle2=0;
    int valuecount=-1;
    int valuecount2=-1;
    int pos=0;
    int pos2=0;
    int time=0;
    int timemax=1;
    int time2=0;
    int timemax2=1;


    int debug=0;
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------
        
        if(pos==maxcommand){
            pos=0;
            for(int i=0;i<maxcommand;i++){
                command[i]=GetRandomValue(1,3);
                value[i]=GetRandomValue(10,100);
            }
        }
        if(pos2==maxcommand2){
            debug = GetRandomValue(0,100);
            pos2=0;
            for(int i=0;i<maxcommand2;i++){
                command2[i]=GetRandomValue(2,3);
                value2[i]=GetRandomValue(10,100);
                
            }
        }
        
        time++;
        if(time>timemax && pos<maxcommand){
            time=0;
            //
            switch (command[pos]){
                case 1://forward
                    if(valuecount==-1){
                        valuecount=value[pos];
                    }
                    if(valuecount>0){
                        valuecount--;
                        position.x += cos(angle*DEG2RAD)*1;
                        position.y += sin(angle*DEG2RAD)*1;                        
                    }
                    if(valuecount==0){
                        pos++;
                        valuecount=-1;
                    }
                break;
                case 2://left
                    if(valuecount==-1){
                        valuecount=value[pos];
                    }
                    if(valuecount>0){
                        valuecount--;
                        angle--;                        
                    }
                    if(valuecount==0){
                        pos++;
                        valuecount=-1;
                    }
                break;
                case 3://right
                    if(valuecount==-1){
                        valuecount=value[pos];
                    }
                    if(valuecount>0){
                        valuecount--;
                        angle++;                        
                    }
                    if(valuecount==0){
                        pos++;
                        valuecount=-1;
                    }
                break;

            }
        }
        
        time2++;
        if(time2>timemax2 && pos2<maxcommand2){
            time2=0;
            //
            switch (command2[pos2]){
                case 2://left
                    if(valuecount2==-1){
                        valuecount2=value2[pos2];
                    }
                    if(valuecount2>0){
                        valuecount2--;
                        angle2--;                        
                    }
                    if(valuecount2==0){
                        pos2++;
                        valuecount2=-1;
                    }
                break;
                case 3://right
                    if(valuecount2==-1){
                        valuecount2=value2[pos2];
                    }
                    if(valuecount2>0){
                        valuecount2--;
                        angle2++;                        
                    }
                    if(valuecount2==0){
                        pos2++;
                        valuecount2=-1;
                    }
                break;

            }


            
        }

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);



        DrawTexturePro(sprites,  (Rectangle){0,0,16,16},// the -96 (-)means mirror on x axis
                                        (Rectangle){position.x,position.y,64,64},
                                        (Vector2){32,32},angle,WHITE);
        DrawTexturePro(sprites,  (Rectangle){16,0,16,16},// the -96 (-)means mirror on x axis
                                        (Rectangle){position.x,position.y,64,64},
                                        (Vector2){32,32},angle2,WHITE);


        DrawText(FormatText("%i",pos2), 10, 20, 20, BLACK);
        DrawText(FormatText("%i",debug), 100, 20, 20, BLACK);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    UnloadTexture(sprites);
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}