// CA1 - Shane Cunningham
// import raylib library
#include "raylib.h"

// structs are used to group related variables to one place, struct Anim hold variables related to the animation logic
struct Anim {
    Rectangle rec;
    // Vector2 is the position of the line on X and Y axis
    Vector2 pos;
    int frame;
    float updateTime;
    float runningTime;
};

// main c++ function that contains the game application between the curly brackets
int main() {

    // Set Window height and window width as constant integer variables
    const int windowWidth{700};
    const int windowHeight{450};

    //Call initWindow method from the raylib library to create a new window to display the game on. 
    // include the following variable types within the brackets(int width, int height, const char *title) 
    InitWindow(windowWidth,windowHeight,"DuuM Fook'em 2D");

    // #########  Game Characters / Sprites  #############################

    // Locate and load the texture file in the resources folder  // Marine.png original DOOM content taken from https://spritedatabase.net/file/555
    Texture2D marine = LoadTexture("resources/marine_walking.png");
    Texture2D demon = LoadTexture("resources/demon.png");


    Anim marineAnim;
    marineAnim.rec.width = marine.width/4; 
    marineAnim.rec.height = marine.height;
    marineAnim.rec.x = 0;
    marineAnim.rec.y = 0;
    marineAnim.pos.x = windowWidth/2 -marineAnim.rec.width/2;
    marineAnim.pos.y = windowHeight - marineAnim.rec.height;
    marineAnim.frame = 0;
    marineAnim.updateTime = 1.0/12.0;
    marineAnim.runningTime = 0.0;

    //#########  Game Physics ###############################

    
    const int gravity{1000};
    // integer to move sprite
    int speed{250};
    // change jumpHeight to pixels per second
    int jumpHeight{500};
    bool IsJumping = false;
    bool collision{};


    // Update the animation speed, takes 1 second then divides it by 12
    const float updateTime{1.0/12};
    // float to contain the running time data
    float runningTime{};


    SetTargetFPS(60);
    // Create While loop, While WindowShouldClose is false, initiate methods within the while loop brackets
    while(!WindowShouldClose()){
        // Get time in seconds for the last frame drawn (delta time)
        const float deltaTime{GetFrameTime()};


        // Call raylib's BeginDrawing method, sets up canvas or (framebuffer) to begin drawing
        BeginDrawing();

        DrawTextureRec(marine,marineAnim.rec,marineAnim.pos,WHITE);




        // Call raylib's ClearBackground method, colour window white
        ClearBackground(BLACK);
        // Call raylib's EndDrawing method, this method ends the canvas drawing and swaps the buffers
        EndDrawing();
    }
}
