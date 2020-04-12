//
// Stack and List and other data structures are not build into the c language.
// If you need to convert code that has stacks or lists you need to write or re use code
// for that.
//
// As far as I have gotten to know Clang and sources about it you need to use
// arrays for these. A stack is a array with several functions to manipulate
// it. Most known are Pop And Push. With making you own stack from a array
// the Push adds the data to the highest location[0..highest] in the array. You actually
// need to keep track of this by a counter that holds the size of your stack. (You need to 
// create arrays in Clang at program start and thus need to estimate how large they might
// need to be.)
// A pop removes the highest location from the array and decreases the counter with one.
//
// You can write more functions to remove items from the middle of the list or stack. You then
// need to shift or "sort" the array. 

#define MAXSTACKSIZE 1024 // how large a stack do we need?

#include "raylib.h"

// Our stack
static int mystacklen=0;
typedef struct stack{
    int x;
    int y;
}stack;



static stack mystack[MAXSTACKSIZE]; // Here this stack is global to our program.

static void push(struct stack in);
static void pop(void);

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib example.");
 
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------
    struct stack banana;
    banana.x = 10;
    banana.y = 20;
    push(banana);

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        if(IsKeyReleased(KEY_SPACE)){
            struct stack banana;
            banana.x = GetRandomValue(0,100);
            banana.y = 20;
            push(banana);        
        }
        if(IsKeyReleased(KEY_ENTER)){
            pop();
        }
        
        //----------------------------------------------------------------------------------
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);
            DrawText("Press space to add to stack, enter to remove",200,10,20,DARKGRAY);
            DrawText(FormatText("Stack size : %i",mystacklen),320,30,20,DARKGRAY);
            for(int i=0;i<mystacklen;i++){
                DrawText(FormatText("stack %i",mystack[i].x),0,20+i*20,20,BLACK);
            }

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;


}

void pop(){
    if(mystacklen>0)mystacklen-=1;
}
void push(struct stack in){
    mystack[mystacklen] = in;
    mystacklen+=1;    
}
