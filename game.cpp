// CA1 - Shane Cunningham
// import raylib library
#include "raylib.h"


// structs are used to group related variables to one place, each variable in the struct is known as a member 
// struct Anim hold variables related to the animation logic
struct Anim {
    Rectangle rec;
    // Vector2 is the position of the line on X and Y axis
    Vector2 pos;
    // int frame: this integer will hold the frame count created during frame incrementation (frame++)
    int frame;
    // float updateTime: the data stored here will aid in slowing down the animation 
    float updateTime;
    // float runningTime: float to contain the running time data
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

    // ----------------- Marine class ------------------------------------
    Anim marineAnim;
    marineAnim.rec.width = marine.width/4; 
    marineAnim.rec.height = marine.height;
    marineAnim.rec.x = 0;
    marineAnim.rec.y = 0;
    marineAnim.pos.x = windowWidth/2 -marineAnim.rec.width/2;
    marineAnim.pos.y = windowHeight - marineAnim.rec.height;
    marineAnim.frame = 0;
    // Update the animation speed, takes 1 second then divides it by 12, this slows the frame rate down
    marineAnim.updateTime = 1.0/12.0;
    marineAnim.runningTime = 0.0;

    //#########  Game Physics ###############################

    int velocity{10};
    const int gravity{1000};
    // integer to move sprite
    int speed{250};
    // change jumpHeight to pixels per second
    int jumpHeight{500};
    // boolean value to determine if character is jumping or not, set to false as default action
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
        // Include Rectangle marineRec class to all for updates to the variables during the game runtime, 
        // used for game collisions
        Rectangle marineRec{
	        marineAnim.pos.x,
	        marineAnim.pos.y,
	        marineAnim.rec.height,
	        marineAnim.rec.width	
	    };

    //  ############################### Character Movement ###########################
    // ------------------------------------- Marine ---------------------------------

        ////////////////////////////// Make Character Jump //////////////////////////////

        // if the marine's position on the y axis is greater than or equal to windowHeight - marine.height
        if (marineAnim.pos.y >= windowHeight - marine.height){
            // slow velocity
            velocity = 0;
            // Is jumping is false
            IsJumping = 0;
            // else 
        } else {
            // multiply gravity & velocity by delta time
            velocity += gravity * deltaTime;
            IsJumping = 1;
        }
        // velocity = 0; if space is pressed, velocity = -10, making object jump.  
        // and if IsJumping is not true, allow velocity(object) jump by -10
        if(IsKeyPressed(KEY_SPACE) && !IsJumping){
            // Move object up by 10
            velocity -= jumpHeight;
        } 
        // make marine position y equal to velocity, then multiply velocity by deltaTime
        // this statement makes marine sink on the Y axis or fly off the top of the screen
        // when space bar is pressed. an additional conditional is required to make the marine jump and land correctly
        marineAnim.pos.y += velocity * deltaTime;

        // Call raylib's BeginDrawing method, sets up canvas or (framebuffer) to begin drawing
        BeginDrawing();

        DrawTextureRec(marine,marineAnim.rec,marineAnim.pos,WHITE);




        // Call raylib's ClearBackground method, colour window white
        ClearBackground(BLACK);
        // Call raylib's EndDrawing method, this method ends the canvas drawing and swaps the buffers
        EndDrawing();
    }
}
