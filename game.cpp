// CA1 - Shane Cunningham
// import raylib library
#include "raylib.h"

// #include <iostream>

// #define PLAYER_MAX_SHELLS  1

// #define PLAYER_MAX_SHELLS_Left  1

// #define MAX_ENEMIES 200

// --------------------------------------------------------------------------------------------------------
//                                          Structures 
// --------------------------------------------------------------------------------------------------------

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

    Vector2 speed;
};

// marines shells 
struct Shells {
    Rectangle rec;
    Vector2 shot_speed;
    Color colour;
    bool active;
    int lifeSpawn;
};
// Code sourced from HE360, Raylib C/C++ Tutorial 6: Animated Enemy Sprites, A.I., Collisions, and How to  Defeat Your Enemy. https://www.youtube.com/watch?v=lCJrj_IEFlw
// marines shells left side
struct Shells_Left {
    Rectangle recLeft;
    Vector2 shot_speedLeft;
    Color colourLeft;
    bool activeLeft;
    int lifeSpawnLeft;
};



// Code sourced from HE360, Raylib C/C++ Tutorial 6: Animated Enemy Sprites, A.I., Collisions, and How to  Defeat Your Enemy. https://www.youtube.com/watch?v=lCJrj_IEFlw
// enemy type definition struct
typedef struct Enemy{
    // Used to determine the activity state of enemy
    bool active;
    // Used to determine if enemy is dying
    bool dying;
    // dying counter
    int dyingCounter;
    // collisions will happen with frameRec
    Rectangle EnemyFrameRec;
    // collision outcomes are determind from here
    Rectangle EnemyCollision;
    // determine where enemy will be placed on screen
    Vector2 EnemyPosition;
    // int frame: this integer will hold the frame count created during frame incrementation (EnemyFrameCounter++)
    int EnemyFrameCounter;
    // the speed of the animations
    int EnemyFrameSpeed;
    // current frame this keeps track of which animation will be used whenever the enemy
    //  is going to right or going to the left 
    int EnemyCurrentFrame;
// Variable for the class
} Enemy;




//---------------------------------------------------------------------------------------------------------
//                                      Program main entry point
//---------------------------------------------------------------------------------------------------------

// main c++ function that contains the game application between the curly brackets
int main() {

    // Set Window height and window width as constant integer variables, 700 pixels high, 450 pixels wide, 
    const int windowWidth{700};
    const int windowHeight{450};

    //Call initWindow method from the raylib library to create a new window to display the game on. 
    // include the following variable types within the brackets(int width, int height, const char *title) 
    InitWindow(windowWidth,windowHeight,"DuuM Fook'em 2D");

    InitAudioDevice();

    // --------------------------------------------------------------------------------
    //                          Game Characters / Textures / Audio 
    // --------------------------------------------------------------------------------

    // Locate and load the texture file in the resources folder  // Marine.png original DOOM content taken from https://spritedatabase.net/file/555
    Texture2D marine = LoadTexture("resources/space-marine-run.png");
    Texture2D demon = LoadTexture("resources/demon_run.png");

    // Audio
    Music music = LoadMusicStream("resources/shane_cunningham_Bosca_ceoil_1.wav");
    Sound shotgun = LoadSound("resources/266105__marregheriti__shotgun.wav");
    Sound growl = LoadSound("resources/angry-beast.wav");

    // Local variables

    // number of shells fired rightward
    int shell_num{50};
    // number of shells fired rightward
    int shell_left_num{50};
    // No of enemies, value shown in the DEMONS life bar
    int Enemy_Amount{200};
    // score created when marine shoots demons
    int Score_Count{0};

    

    // --------------------------------------------------------------------------------
    //                                  Marine class 
    // --------------------------------------------------------------------------------

    // Call struct Anim, create class marineAnim
    Anim marineAnim;
    // set rectangle width equal to Texture2d marine width (png image) and divide into 4 frames, This is used to contain the sprites
    marineAnim.rec.width = marine.width/11; 
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
    // marine facing right
    bool right;
    // marine facing left
    bool left;
    // bool to see if marine character is active
    bool marine_active = true;
    // speed of marine on x and y axis
    marineAnim.speed.x = 115;
    marineAnim.speed.y = 0;
    // Marine life bar 
    int life_bar = 200;

    // marines shells/ bullets rightward
    Shells shell[shell_num];
    for (int i = 0; i < shell_num; i++){
            shell[i].rec.height = 2;
            shell[i].rec.width = 50;
            shell[i].colour = ORANGE;
            shell[i].rec.x = (windowWidth/2) + (marineAnim.rec.width/2) - (shell[i].rec.width/2);
            shell[i].rec.y = (windowHeight - marineAnim.rec.height) - (shell[i].rec.height);
            shell[i].shot_speed.x = 100;
            shell[i].shot_speed.y = 100;
            shell[i].active = false;
            shell[i].lifeSpawn = 0;
            }
            int fire_rate = 0;

    
    Shells_Left shellLeft[shell_left_num];
    for (int i = 0; i < shell_left_num; i++){
            shellLeft[i].recLeft.height = 2;
            shellLeft[i].recLeft.width = 50;
            shellLeft[i].colourLeft = ORANGE;
            shellLeft[i].recLeft.x = (windowWidth/2) + (marineAnim.rec.width/2) - (shellLeft[i].recLeft.width/2);
            shellLeft[i].recLeft.y = (windowHeight - marineAnim.rec.height) - (shellLeft[i].recLeft.height);
            shellLeft[i].shot_speedLeft.x = 100;
            shellLeft[i].shot_speedLeft.y = 100;
            shellLeft[i].activeLeft = false;
            shellLeft[i].lifeSpawnLeft = 0;
    }   

        




    // --------------------------------------------------------------------------------------
    //                                  Enemy class 
    // --------------------------------------------------------------------------------------
    


    // this stores the where the enemies are placed on screen when the game launches
    // int EnemyX = 100;
    // int EnemyY = 250;


    // Enemy Code sourced from HE360, Raylib C/C++ Tutorial 6: Animated Enemy Sprites, A.I., Collisions, and How to  Defeat Your Enemy. https://www.youtube.com/watch?v=lCJrj_IEFlw
    // Enemy will store the enemy Array
    // Setting up the enemy's position and where they start
    // on screen, We are also setting up the enemy's invisable animation rectangle that hides
    // the other frames while a current animation is played. here we will set up 
    // whether the enemy is active or not, the frame counter, the frame speed and current frame.
    // Using a for loop to create enemies will simplify the process of duplicating more enimies
    Enemy enemy[Enemy_Amount];
    for (int i = 0; i < Enemy_Amount; i++){
        // enemy[i].EnemyPosition = (Vector2){EnemyX, EnemyY};

        enemy[i].EnemyFrameRec.x = 290.0f;
        enemy[i].EnemyFrameRec.y = 0.0f;

        enemy[i].EnemyFrameRec.width = demon.width/10;
        enemy[i].EnemyFrameRec.height = demon.height;

        enemy[i].EnemyPosition.x = windowWidth- enemy[i].EnemyFrameRec.width;
        enemy[i].EnemyPosition.y = windowHeight- enemy[i].EnemyFrameRec.height;
        // Used to determine the activity state of enemy
        enemy[i].active = true;
        // //used to determine when enemy is dying
        // enemy[i].dying = false; // ?
        // //used for when enemy is dying
        // enemy[i].dyingCounter = 0; // ?
        // FrameCounter holds information on what frame the enemy is on
        enemy[i].EnemyFrameCounter = 0;
        // How fast the animation frame will move
        enemy[i].EnemyFrameSpeed = 8;
        // The current frame will keep track of which frame is currently being used
        enemy[i].EnemyCurrentFrame = 0;

    }


    // --------------------------------------------------------------------------------------
    //                                  Game Physics Variables
    // --------------------------------------------------------------------------------------

    //-------- variables for jumping --------------
    // the velocity is used to control the speed of the jump
    int velocity{20};
    // force of gravity in the game, brings player back to the ground
    const int gravity{1800};
    // the height of the jump under the constraints of velocity and gravity
    int jumpHeight{650};
    // Boolean value to determine if the player is jumping or not, if the jump is true or false (0 or 1). 
    // set to false as default action
    bool IsJumping = false;

    // integer to move sprite
    // int speed{115};
    
    // boolean value to determine if character has had a collision or not, set as a null value
    // bool collision{};
    bool marine_and_enemy_Collide;
    // This constant character type variable will contain the text concerned with the games outcome
    // the null pointer is used here to point the Game_Over_Text value to a location that represents 
    // invalid memory. A Null value is 0, but it is not an equivalant to the integer 0, both use 
    // different memory locations. It is good practice to use nullptr on if a value is not pointing 
    // to a valid memory address, this will help a program run more efficiently
    const char* Game_Over_Text = nullptr; 
    // char value that contains text to display when game is over
    const char* replay = "Press R To Play Again";



    SetTargetFPS(60);
    // Create While loop, While WindowShouldClose is false, initiate methods within the while loop brackets
    while(!WindowShouldClose()){

        UpdateMusicStream(music);
        PlayMusicStream(music);
        // Get time in seconds for the last frame drawn (delta time)
        const float deltaTime{GetFrameTime()};
        // -------------------------------------------------------------------------------------------------
        //                                   Collision Rectangles 
        // -------------------------------------------------------------------------------------------------
        // Include Rectangle marineRec class to all for updates to the variables during the game runtime, 
        // used for game collisions

        // Rectangle demonicRec{
		//     demonPos.x,
		//     demonPos.y,
		//     demonRec.height,
		//     demonRec.width,
	    // };
        Rectangle marineRec{
	        marineAnim.pos.x,
	        marineAnim.pos.y,
	        marineAnim.rec.height,
	        marineAnim.rec.width	
	    };
        // // Enemy Code sourced from HE360, Raylib C/C++ Tutorial 6: Animated Enemy Sprites, A.I., Collisions, and How to  Defeat Your Enemy. https://www.youtube.com/watch?v=lCJrj_IEFlw

        // loop Enemy amount to get enemy[i]
        for (int i = 0; i < Enemy_Amount; i++){
            // set the boolean value to store CheckCollisionRecs() method to check if there are collisions between marine rectangle and enemy rectangle
            marine_and_enemy_Collide = CheckCollisionRecs(marineRec, enemy[i].EnemyCollision);
           
        }
            // std::cout, part of the <iostream> library included at the start of the file
            // it is used to log data results to the console, useful for debugging
            // below logs data from marine_and_enemy_Collide value to console, useful for viewing data when game is running
            // std::cout << marine_and_enemy_Collide << " :marine_and_enemy_Collide\n";
            // output: 1 :marine_and_enemy_Collide or 0 :marine_and_enemy_Collide, 1 for collision, 0 for none



        BeginDrawing();

        //  ############################### Character Movement ###########################

        // -----------------------------------------------------------------------
        //                                       Marine 
        // -----------------------------------------------------------------------

        // Draw Life Bar for Marine
        DrawRectangle(15,20,200,12,LIGHTGRAY);
        DrawRectangle(15,20,life_bar,12,RED);
        DrawRectangleLines(15,20,200,12,GRAY);
        DrawText("HEALTH",100,22,10,BLACK);

        DrawRectangle(windowWidth - 215,20,200,12,LIGHTGRAY);
        DrawRectangle(windowWidth - 215,20,Enemy_Amount,12,RED);
        DrawRectangleLines(windowWidth - 215,20,200,12,GRAY);
        DrawText("DEMONS",windowWidth -135,22,10,BLACK);


        if(marine_active == true){
            // Call DrawTextureRec() method from the raylib.h library and include the Texture2D texture, Rectangle source, Vector2 position, Color tint
            // Draw the marine for the game
            DrawTextureRec(marine,marineAnim.rec,marineAnim.pos,WHITE);
        }

        // stop marine from sticking to top and bottom of game screen
        // if statment conditional Code sourced from JonesRepo, Pong-Tutorial, https://github.com/JonesRepo/Pong-Tutorial , Rights: MIT 
        // if (marineAnim.pos.y  > windowHeight){
        //     marineAnim.pos.y = windowHeight;
        // };
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
        if(IsKeyDown(KEY_D) /*&& !IsJumping*/){ 
            // then the marine position on the x-axis is equal to the speed variable multiplied by the deltaTime  
            marineAnim.pos.x += marineAnim.speed.x*deltaTime;        // This uses addition to move the character right on the x axis to the speed set in the int variable above
            // marine facing right is true
            right = 1;
            // marine facing left is false
            left = 0;
            // then the animation rectangle width is equal to reveresed (flip/change direction of frame)
            // marine texture with its width divided into 11 frames
            marineAnim.rec.width = marine.width/11;
            
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
                // if the frame count gets greater than 11
             if (marineAnim.frame > 10){
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
        if(IsKeyDown(KEY_A) /*&& !IsJumping*/){
            // marineAnim.pos.x = marineAnim.pos.x - speed * deltaTime
	        marineAnim.pos.x -= marineAnim.speed.x * deltaTime;          // This uses subtraction to move the character Left on the x axis to the speed set in the int variable above
	        // marine texture with its width divided into 11 frames
            marineAnim.rec.width = -marine.width/11;
            // synchronise the marine animation runningtime equal to the deltaTime
            marineAnim.runningTime += deltaTime;
            // marine facing right is false
            right = 0;
            // marine facing left is true
            left = 1;
            // If the marine Animation runningTime Greater or Equal to the updateTime, then:
            if(marineAnim.runningTime >= marineAnim.updateTime){
                // set animation runningTime back to float 0.0
                marineAnim.runningTime = 0.0;
                // Create sprite animation, character rectangle on x axis is equal to frame integer multiplied rec width
                marineAnim.rec.x = marineAnim.frame* marineAnim.rec.width;
                // Increment frame by 1, cycle through .png frames
                marineAnim.frame++;
                // if the frame count gets greater than 11
             if (marineAnim.frame > 10){
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

        // STOP MARINE FROM EXITING THE SCREEN EDGES
        // if(marineAnim.pos.x < windowWidth - marineAnim.rec.width){
        //     marineAnim.speed.x = 0;
        // }
        
                

        

        //---------------------------  Shooting   -------------------------------------- 
        
        
        // Draw Shells right
        if(marine_active){
            for (int i = 0; i < shell_num; i++){
                if(shell[i].active){
                    DrawRectangleRec(shell[i].rec, shell[i].colour);
                }
            }
        
            // Shoot to the right of the x axis

            if(IsKeyDown(KEY_RIGHT_CONTROL) && right){
                fire_rate += 1;
                for (int i = 0; i < shell_num; i++){
                    if (!shell[i].active && fire_rate % 90 == 0){
                        // add sfx here PlaySound(sound);
                        PlaySound(shotgun);
                        shell[i].rec.x = marineAnim.pos.x + marineAnim.rec.width /5;
                        shell[i].rec.y = marineAnim.pos.y + marineAnim.rec.height / 2;
                        shell[i].active = true;
                        break;
                    }
                }
            }


            for (int i = 0; i < shell_num; i++){
                    if (shell[i].active) {
                    shell[i].rec.x += shell[i].shot_speed.x;

                    if (shell[i].rec.x <= 0) {
                        shell[i].active = false;
                        fire_rate = 0;
                    }


                }
            }

        

            // Draw Shells left
            for (int i = 0; i < shell_left_num; i++){
                if(shellLeft[i].activeLeft){
                    DrawRectangleRec(shellLeft[i].recLeft, shellLeft[i].colourLeft);
                }
            }

            // Shoot to the left of the x axis
            if(IsKeyDown(KEY_RIGHT_CONTROL) && left){
                fire_rate += 1;
                for (int i = 0; i < shell_left_num; i++){
                    if (!shellLeft[i].activeLeft && fire_rate % 90 == 0){
                        // add sfx here PlaySound(sound);
                        PlaySound(shotgun);
                        shellLeft[i].recLeft.x = marineAnim.pos.x + marineAnim.rec.width /5; //(float)0.5;
                        shellLeft[i].recLeft.y = marineAnim.pos.y + marineAnim.rec.height / 2;
                        shellLeft[i].activeLeft = true;
                        break;
                    }
                }
            }


            for (int i = 0; i < shell_num; i++){
                    if (shellLeft[i].activeLeft) {
                    shellLeft[i].recLeft.x -= shellLeft[i].shot_speedLeft.x;

                    if (shellLeft[i].recLeft.x <= 0) {
                        shellLeft[i].activeLeft = false;
                        fire_rate = 0;
                    }
                }
            }
        }   

        // -----------------------------------------------------------------------
        //                               Demon 
        // -----------------------------------------------------------------------


        // Call DrawTextureRec() method from the raylib.h library and include the Texture2D texture, Rectangle source, Vector2 position, Color tint
        // Draw the marine for the game
        // DrawTextureRec(marine,marineAnim.rec,marineAnim.pos,WHITE);

        // // Enemy Code sourced from HE360, Raylib C/C++ Tutorial 6: Animated Enemy Sprites, A.I., Collisions, and How to  Defeat Your Enemy. https://www.youtube.com/watch?v=lCJrj_IEFlw

        // // Attaching the collision rectangle to the enemy's position
        // // The collision rectangle will make it possible for marine to defeat the enemy
        // Use for loop to start looping through Enemy_amount
        for (int i = 0; i < Enemy_Amount; i++){
            enemy[i].EnemyCollision.x = enemy[i].EnemyPosition.x;
            enemy[i].EnemyCollision.y = enemy[i].EnemyPosition.y;
            enemy[i].EnemyCollision.width = enemy[i].EnemyFrameRec.width;
            enemy[i].EnemyCollision.height = enemy[i].EnemyFrameRec.height;
        }

        // Enemy A.I and enemy movement with Animation
        // Move Right
        // Use for loop to start looping through Enemy_amount
        for (int i = 0; i < Enemy_Amount; i++){
            // if enemy active and marine and enemy are not colliding
            if (enemy[i].active && !marine_and_enemy_Collide){
                // if the marine position on x axis is greater than the enemies
                if (marineAnim.pos.x > enemy[i].EnemyPosition.x){
                // move enemy rightward towards marines position on the x axis
                    enemy[i].EnemyPosition.x += 1;
                    // begin to increment the frame count in order to begin animating
                    enemy[i].EnemyFrameCounter++;
                    // Call PlaySound Method from the raylib.h library, include variable connected to resources .wav file
                    PlaySound(growl);
                    // if the enemies frame count is greater or equal to the 60 / enemy frame speed(8), another way of setting frame speed to 60 per second
                    if (enemy[i].EnemyFrameCounter >= (60/enemy[i].EnemyFrameSpeed)){
                        // reset frame count to 0
                        enemy[i].EnemyFrameCounter = 0;
                        // increment enemy current frame by 1
                        enemy[i].EnemyCurrentFrame++;
                        // if the frame count gets greater than 10
                        if(enemy[i].EnemyCurrentFrame > 9) {
                            // set frame count back to 6 in order to restart cycle through .png frames
                            enemy[i].EnemyCurrentFrame = 5;
                        }
                        // this part helps to hide the frames
                        enemy[i].EnemyFrameRec.x = enemy[i].EnemyCurrentFrame * demon.width/10;
                    }
                }
            }
        }


        // // Move Left
        // Use for loop to start looping through Enemy_amount
        for (int i = 0; i < Enemy_Amount; i++){
            // if enemy active and marine and enemy are not colliding
            if (enemy[i].active && !marine_and_enemy_Collide){
                // move enemy leftward
                // if the marine position on x axis is less than the enemies 
                if (marineAnim.pos.x < enemy[i].EnemyPosition.x){
                    // move enemy left towards marines position on the x axis
                    enemy[i].EnemyPosition.x -= 1;
                    // begin to increment the frame count in order to begin animating
                    enemy[i].EnemyFrameCounter++;
                    // Call PlaySound Method from the raylib.h library, include variable connected to resources .wav file
                    PlaySound(growl);
                    // if the enemies frame count is greater or equal to the 60 / enemy frame speed(8), another way of setting frame speed to 60 per second
                    if (enemy[i].EnemyFrameCounter >= (60/enemy[i].EnemyFrameSpeed)){
                        // reset frame count to 0
                        enemy[i].EnemyFrameCounter = 0;
                        // increment enemy current frame by 1
                        enemy[i].EnemyCurrentFrame++;

                        // if the frame count gets greater than 5
                        if(enemy[i].EnemyCurrentFrame > 4) {
                        // set frame count back to 1 in order to restart cycle through .png frames
                            enemy[i].EnemyCurrentFrame = 0;
                        }
                        // hide frames
                        enemy[i].EnemyFrameRec.x = enemy[i].EnemyCurrentFrame * demon.width/10;
                    }
                }
            }
        }


        // Check collisions with enemy and shells coming from the rightward direction

        for (int i = 0; i < shell_num; i++){

            if ((shell[i].active)){
                
                for (int a = 0; a < Enemy_Amount; a++) {
                    
                    if (enemy[a].active && CheckCollisionRecs(shell[i].rec, enemy[a].EnemyCollision)){
                        
                        shell[i].active = false;
                        shell[i].lifeSpawn = 0;
                        // enemy[a].active = false;
                        // decrease Enemy_Amount
                        Enemy_Amount--;
                        if(Enemy_Amount == 0 && !Game_Over_Text){
                            Enemy_Amount = GetRandomValue(1,5);
                            enemy[a].EnemyPosition.x = 800;
                            Score_Count++;
                        }
                        // std::cout << Score_Count << " :SCORE_COUNT\n";    
                        
                    }
                }
            }
        }


        // Check collisions with enemy and shells coming from the leftward direction

        for (int i = 0; i < shell_left_num; i++){

            if ((shellLeft[i].activeLeft)){
                
                for (int a = 0; a < Enemy_Amount; a++) {
                    
                    if (enemy[a].active && CheckCollisionRecs(shellLeft[i].recLeft, enemy[a].EnemyCollision)){
                        
                        shellLeft[i].activeLeft = false;
                        shellLeft[i].lifeSpawnLeft = 0;
                        // PlaySound(growl);
                        // enemy[a].active = false;
                        DrawText("Demon Hit", 10, 60, 20, WHITE);
                        Enemy_Amount--;
                        if(Enemy_Amount == 0 && !Game_Over_Text){
                            Enemy_Amount = GetRandomValue(1,5);
                            Score_Count++;
                            enemy[a].EnemyPosition.x = -100;
                        }
                        // enemy[a].EnemyPosition.x = -100;
                            
                        
                        
                    }
                }
            }
        }

        

        // Draw Enemy
        for (int i = 0; i < Enemy_Amount; i++){
            if (enemy[i].active) {
                DrawTextureRec(demon, enemy[i].EnemyFrameRec, enemy[i].EnemyPosition, WHITE);
                
            }
        }


        // Call raylib's ClearBackground method, colour window white
        ClearBackground(BLACK);


        

        // Enemy Code sourced from HE360, Raylib C/C++ Tutorial 6: Animated Enemy Sprites, A.I., Collisions, and How to  Defeat Your Enemy. https://www.youtube.com/watch?v=lCJrj_IEFlw
        // Collision

        for(int i = 0; i < marine_and_enemy_Collide; i++){
                life_bar-- ;
                
                // std::cout << life_bar << "\n";
                 break;
        }
        // if collision between marine + enemy and marine active
        if (marine_and_enemy_Collide && marine_active) {
        //    Draw message
            DrawText("Demons are draining your life!\n Move!!!",10, 60, 20, RED); 
        } 
        // if the marines life bar is equal to zero
        if (life_bar == 0){
            // The marine is no longer active
            marine_active = false;
            // loop throught the Enemy_Amount value to set enemy activity to false, making the demon disapeer
            for(int i = 0; i < Enemy_Amount; i++){
                enemy[i].active = false;
            }
        }
        //  if Marine is not active
        if (!marine_active){
            // Set Game over text to "You Are Dead!"
            Game_Over_Text ="You Are Dead!";
        }
        //  if Score_Count is greater than value
        if(Score_Count >= 10){
            // The marine is no longer active
            marine_active = false;

            for(int i = 0; i < Enemy_Amount; i++){
                enemy[i].active = false;
            }
            // Set Game over text to "You Win ! Max Score Achieved !"
            Game_Over_Text = "You Win ! Max Score Achieved !";
        }
        //  Original Code sourced from JonesRepo, Pong-Tutorial, https://github.com/JonesRepo/Pong-Tutorial , Rights: MIT 
        //  if the Game_Over_Text contains either char values "You Are Dead!" or "You Win ! Max Score Achieved !"
        if (Game_Over_Text){
            // create int called textWidth, value will contain MeasureText method holding the Game_over_text and font size as parameters
            int textWidth = MeasureText(Game_Over_Text, 60);
            // Draw game over text outcomes and place in large font in the center of the game screen
            DrawText(Game_Over_Text, GetScreenWidth() / 2 - textWidth / 3 + 10, GetScreenHeight() / 2 - 30, 40, YELLOW);
            // 
            DrawText(replay, GetScreenWidth() / 2 - textWidth / 4, GetScreenHeight() / 2 + 50, 20, YELLOW);
        }
        // Restart Game
        if(Game_Over_Text && IsKeyPressed(KEY_R)) {
            // set Game_Over_Text char back to null
            Game_Over_Text = nullptr;
            // redraw original game state
            // --------------------------- MARINE ----------------------------------------
            // this will set the player charaters position on the X axis when the game begins
            // the position x of the marine is equal to half of the windowWidth minus half of the rectangle width
            marineAnim.pos.x = windowWidth/2 -marineAnim.rec.width/2;
            // this will set the player charaters position on the Y axis when the game begins
            // the position Y of the marine is equal to the windowHeight minus the rectangle height
            marineAnim.pos.y = windowHeight - marineAnim.rec.height;
            // Marine life bar 
            life_bar = 200;
            // bool to see if marine character is active
            marine_active = true;
            // -------------------------- DEMON / ENEMY -------------------------------
            Score_Count = 0;
            Enemy_Amount = 1;
            for(int i = 0; i < Enemy_Amount; i++){
                enemy[i].EnemyPosition.x = 750;
                enemy[i].EnemyPosition.y;
                enemy[i].active = true;
            }


        }


         
        // else{
        //     //     marine_active = false;
        //     // }
        //     // marine_active = false;
        //     // for(int i = 0; i < Enemy_Amount; i++){
        //     //     if(enemy[i].active == true){enemy[i].active = false;}
        //     // }

        //     // ClearBackground(RED);

        //     // DrawText("You Are Dead!", 10, 60, 20, WHITE);

        //     // DrawText("Hit", 10, 60, 20, WHITE);

        //     // for(int i = 0; i < Enemy_Amount; i++){

        //     //     if (enemy[i].EnemyCurrentFrame >= 1 ){
        //     //         enemy[i].EnemyCurrentFrame = 1;
        //     //     }
        //     // }
          
        // } /*else if(IsKeyPressed(KEY_ENTER)){
        //     marine_active = true;
        // }*/


     




        // if(collision){
        //     // ClearBackground(RED);
            // DrawTextureRec(marine,marineAnim.rec,marineAnim.pos,WHITE);
        //     // Draw the demon
        //     // DrawTextureRec(demon, demonRec, demonPos, WHITE);

        //     // // Draw Shells
        //     // for (int i = 0; i < shell_num; i++){
        //     //     if(shell[i].active){
        //     //         DrawRectangleRec(shell[i].rec, shell[i].color )
        //     //     }
        //     // }

        // } else {
        // // Call DrawTextureRec() method from the raylib.h library and include the Texture2D texture, Rectangle source, Vector2 position, Color tint
        //     // Draw the marine for the game
        //     DrawTextureRec(marine,marineAnim.rec,marineAnim.pos,WHITE);
        //     // Draw the demon
        //     // DrawTextureRec(demon, demonRec, demonPos, WHITE); 

        //     // // Draw Shells
        //     // for (int i = 0; i < shell_num; i++){
        //     //     if(shell[i].active){
        //     //         DrawRectangleRec(shell[i].rec, shell[i].color )
        //     //     }
        //     // }
        // }

        // Call raylib's EndDrawing method, this method ends the canvas drawing and swaps the buffers
        EndDrawing();
    }
        UnloadTexture(marine);
        UnloadTexture(demon);
        

        CloseAudioDevice(); 
        CloseWindow();

        return 0;

}








