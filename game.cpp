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

    // Set Window height and window width as constant integer variables, 700 pixels high, 450 pixels wide, 
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
    // Call struct Anim, create class marineAnim
    Anim marineAnim;
    // set rectangle width equal to Texture2d marine width (png image) and divide into 4 frames, This is used to contain the sprites
    marineAnim.rec.width = marine.width/4; 
    // set rectangle width equal to Texture2d marine height (png image), This will be the sprite height 
    marineAnim.rec.height = marine.height;
    // set rectangle x and y axis to 0, data for the rectangles will be stored in these two variables
    marineAnim.rec.x = 0;
    marineAnim.rec.y = 0;
    // this will set the player charaters position on the X axis when the game begins
    // the position x of the marine is equal to half of the windowWidth minus half of the rectangle width
    marineAnim.pos.x = windowWidth/2 -marineAnim.rec.width/2;
    // this will set the player charaters position on the Y axis when the game begins
    // the position Y of the marine is equal to the windowHeight minus the rectangle height
    marineAnim.pos.y = windowHeight - marineAnim.rec.height;
    // this integer variable will contain the frame rate values when the character makes an action
    marineAnim.frame = 0;
    // Update the animation speed, takes 1 second then divides it by 12, this slows the frame rate down
    marineAnim.updateTime = 1.0/12.0;
    // This variable wil contain the runningTime data, this is used in conjuction with the updateTime variable to regulate the animation speed
    marineAnim.runningTime = 0.0;

    //#########  Game Physics ###############################
    //-------- variables for jumping --------------
    // the velocity is used to control the speed of the jump
    int velocity{20};
    // force of gravity in the game, brings player back to the ground
    const int gravity{2000};
    // the height of the jump under the constraints of velocity and gravity
    int jumpHeight{550};
    // Boolean value to determine if the player is jumping or not, if the jump is true or false (0 or 1). 
    // set to false as default action
    bool IsJumping = false;

    // integer to move sprite
    int speed{115};
    
    // boolean value to determine if character has had a collision or not, set as a null value
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
        // // Include Rectangle marineRec class to all for updates to the variables during the game runtime, 
        // // used for game collisions
        // Rectangle marineRec{
	    //     marineAnim.pos.x,
	    //     marineAnim.pos.y,
	    //     marineAnim.rec.height,
	    //     marineAnim.rec.width	
	    // };

    //  ############################### Character Movement ###########################
    // ------------------------------------- Marine ---------------------------------

        ////////////////////////////// Make Character Jump - Y-AXIS //////////////////////////////

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

        ////////////////////////////// Movement - X-AXIS ///////////////////////////////////////

        //---------------------------  Move to the Right  KEY_D --------------------------------------
        
        // When the D key is pressed and character is not jumping 
        if(IsKeyDown(KEY_D) && !IsJumping){ 
            // then the marine position on the x-axis is equal to the speed variable multiplied by the deltaTime  
            marineAnim.pos.x += speed*deltaTime;        // This uses addition to move the character right on the x axis to the speed set in the int variable above
            // then the animation rectangle width is equal to reveresed (flip/change direction of frame)
            // marine texture with its width divided into 4 frames
            marineAnim.rec.width = -marine.width/4;
            
            // synchronise the marine animation runningtime equal to the deltaTime
            marineAnim.runningTime += deltaTime; 

            // If the marine Animation runningTime is Greater or Equal to the updateTime, then:
             if(marineAnim.runningTime >= marineAnim.updateTime){
                // set animation runningTime back to float 0.0
                marineAnim.runningTime = 0.0;
                // Create sprite animation, character rectangle on x axis is equal to frame integer multiplied rec width
                marineAnim.rec.x = marineAnim.frame* marineAnim.rec.width;
                // Increment frame by 1, cycle through .png frames
                marineAnim.frame++;
                // if the frame count gets greater than 4
             if (marineAnim.frame > 3){
                // set frame count back to zero in order to restart cycle through .png frames
                marineAnim.frame = 0;
             }
	        }
        }

        // if the D key is released and sprite is not jumping
        if(IsKeyReleased(KEY_D) && !IsJumping){
            // reset frame back to zero, revert sprite to first frame
            marineAnim.frame = 0;
            // then the marine animation rectangle x position = is equal to the marine animation frame multiplied by the rectangle width
            marineAnim.rec.x = marineAnim.frame * marineAnim.rec.width;
        }

        //---------------------------  Move to the Left KEY_A --------------------------------------

        // When the A key is pressed and character is not jumping
        if(IsKeyDown(KEY_A) && !IsJumping){
            // marineAnim.pos.x = marineAnim.pos.x - speed * deltaTime
	        marineAnim.pos.x -= speed * deltaTime;          // This uses subtraction to move the character Left on the x axis to the speed set in the int variable above
	        // marine texture with its width divided into 4 frames
            marineAnim.rec.width = marine.width/4;
            // synchronise the marine animation runningtime equal to the deltaTime
            marineAnim.runningTime += deltaTime;

            // If the marine Animation runningTime Greater or Equal to the updateTime, then:
            if(marineAnim.runningTime >= marineAnim.updateTime){
                // set animation runningTime back to float 0.0
                marineAnim.runningTime = 0.0;
                // Create sprite animation, character rectangle on x axis is equal to frame integer multiplied rec width
                marineAnim.rec.x = marineAnim.frame* marineAnim.rec.width;
                // Increment frame by 1, cycle through .png frames
                marineAnim.frame++;
                // if the frame count gets greater than 4
             if (marineAnim.frame > 3){
                // set frame count back to zero in order to restart cycle through .png frames
                marineAnim.frame = 0;
             }
	        }
        }

        // When A key is released and sprite is not jumping
        if(IsKeyReleased(KEY_A) && !IsJumping){
            // reset frame back to zero, revert sprite to first frame
            marineAnim.frame = 0;
             // then the marine animation rectangle x position = is equal to the marine animation frame multiplied by the rectangle width
            marineAnim.rec.x = marineAnim.frame * marineAnim.rec.width;
        }


        // Call raylib's BeginDrawing method, sets up canvas or (framebuffer) to begin drawing
        BeginDrawing();

        // Call DrawTextureRec() method from the raylib.h library and include the Texture2D texture, Rectangle source, Vector2 position, Color tint
        // Draw the marine for the game
        DrawTextureRec(marine,marineAnim.rec,marineAnim.pos,WHITE);




        // Call raylib's ClearBackground method, colour window white
        ClearBackground(BLACK);
        // Call raylib's EndDrawing method, this method ends the canvas drawing and swaps the buffers
        EndDrawing();
    }
}
