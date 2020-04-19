//
// I used the message window from Earthbound as a example.
//

enum messages{MESSAGE1};

#define GUI_MESSAGE_WIDTH 320
#define GUI_MESSAGE_HEIGHT 200
#define GUI_KEY_DELAY 10

#include "raylib.h"
#include <string.h>


// This is our message. It is passed into the message function as MESSAGE1R(see switch)
// The \n characters in text that is shown in the message gui's are line feeds. They 
// start the next text a line down. (Set text 128 higher for larger chars.
static const char message_intro[128] = "This sample illustrates a\ntext writing animation\neffect!\n\nCheck it out! ;)";
// be sure to reset the framesCounter after every new message. (this is for the text effect.)
static int framesCounter = 0;
static bool messagedisplayed = false; // If this boolean is true than the message is completely shown.
static int waitbetweenkeypresses = 0;

// our global(static) functions.
static void drawmessage(int x,int y,int message);
static void drawroundedrect(int x, int y, int w,int h,Color col);//With this we draw our gui.


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
        
        waitbetweenkeypresses++;//There needs to be a time between key presses.
        
        // Here we replay the message.
        if(waitbetweenkeypresses>GUI_KEY_DELAY && messagedisplayed && IsKeyPressed(KEY_SPACE)){
            waitbetweenkeypresses = 0;//reset the between keypresses timer
            framesCounter = 0; // text display counter
            messagedisplayed = false;
        }
        
        //----------------------------------------------------------------------------------
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);
            
            drawmessage(200,100,MESSAGE1);
            
            DrawRectangle(0,0,screenWidth,20,DARKGRAY);
            DrawText("Press Space for finishing message or replaying message.!",0,0,20,RAYWHITE);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;


}

void drawmessage(int x,int y,int message){

    char text[128]="";
    
    switch (message){
        case MESSAGE1:
            strcat(text,message_intro);
        break;
    }
    framesCounter+=4;
    
    // If the user presses space than display the complete message.
    // (no time to wait for the typewriter effect.)
    if(framesCounter<strlen(text)*10){
        if(waitbetweenkeypresses>GUI_KEY_DELAY && IsKeyPressed(KEY_SPACE)){
            framesCounter=strlen(text)*10;
            messagedisplayed = true;
            waitbetweenkeypresses = 0;
        }        
    }else{
        messagedisplayed = true;
    }
    
    
    int w=GUI_MESSAGE_WIDTH;
    int h=GUI_MESSAGE_HEIGHT;
    drawroundedrect(x,y,w,h,BLACK);
    drawroundedrect(x+1,y+1,w-2,h-2,LIGHTGRAY);
    drawroundedrect(x+6,y+6,w-12,h-12,DARKGRAY);
    drawroundedrect(x+8,y+8,w-16,h-16,BLACK);
    DrawText(TextSubtext(text, 0, framesCounter/10), x+18, y+18, 20, WHITE);
    //
    // This here figures out when the message is displayed completely and then 
    // draws a white triangle pointing downwards.  (Wait for user input here.)
    if(framesCounter>strlen(text)*10){
        x+=w-48;
        y+=h-16;
        DrawTriangle((Vector2){x,y},(Vector2){x+10,y+14},(Vector2){x+20,y},BLACK);
        DrawTriangle((Vector2){x+1,y+1},(Vector2){x+9,y+13},(Vector2){x+18,y+1},WHITE);
    }
    
}

void drawroundedrect(int x, int y, int w,int h,Color col){

    // our round border size.
    int roundsize = 20;
    
    // if messy then look here..
    if(w<50)roundsize/=2;
    if(h<50)roundsize/=2;
    if(w<20)roundsize/=2;
    if(h<20)roundsize/=2;
    
    // first draw 4 circles at the corners
    DrawCircle(x+roundsize,y+roundsize,roundsize,col);
    DrawCircle(x+w-roundsize,y+roundsize,roundsize,col);
    DrawCircle(x+roundsize,y+h-roundsize,roundsize,col);
    DrawCircle(x+w-roundsize,y+h-roundsize,roundsize,col);

    // Draw two rectangles on top of the circles..
    DrawRectangle(x,y+roundsize,w,h-roundsize*2,col);
    DrawRectangle(x+roundsize,y,w-roundsize*2,h,col);
}
