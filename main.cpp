//  main.cpp for Tetris project
//  Tetris_Using_NCurses

//  Created by Forrest Miller on 12/12/17.
//  Version 0.1
//  Version 1.0 Finished December 21, 2017
//  Version 1.1 -- Added more god mode functionaliy
//  Version 1.11 -- Made it so hitting esc also makes the game quit    Jan/2/2018

//-----------------------------------------------------------------------------
// README!!
// Currently I have only tested this program on my MacBook Pro using xCode and
// having it launch in the program Terminal. Other Terminal emulators may behave
// differently in ways that I have not tested. In order for this application to
// run correctly in Terminal, there are some default settings in Terminal that
// need to be set manually for this program to open and display correctly.
//
// Setup:
// Open Terminal and go to preferences. At the top of the preferences window,
// switch tabs to the one called 'Profiles', and then select the default profile
// (it is called "Basic").
// Within "Basic", make sure the font is set to Menlo at 9 pt size. Even if it
// is already set to this, click where it says 'change' next to fonts. Set
// character spacing to 1.5 and line spacing to 0.7, otherwise the square blocks
// will be not be square (they will be rectangular).
// Once this is set, exit out of fonts, and then change the active tab within
// "Basic" from Text to Window. Set the Window dimensions to 180 columns by 96
// rows.
//
// I realize that this is a lot of setup to just get this program running
// correctly, but I have been unable to find a way to have my code tell Terminal
// to open with these settings. Note that making these changes will change how
// Terminal opens each time it is opened, even if this code it not running.
// Feel free to switch it back when you are done with this project.
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// #includes
//-----------------------------------------------------------------------------
//#include <stdlib.h>
#include <cstdlib>
//#include <time.h> //Used for getting system time
//#include <ctime> //Alternatie to time.h (I use <chrono> instead)
#include <math.h>
#include <chrono> //c++11 to get nano time accuratly
#include <thread> //Used for thisThread::sleepFor() //(Bringing this whole thing in seems kinda overkill)

#include <string.h> //Used in string manipulation
//#include <stdint.h> //Allows for additional integer types
//#include <vector> //Allows the use of the container 'std::vector'
//#include <curses.h> //Old version of curses, supposidly obsolete in 1995
#include <ncurses.h> //New curses (probably still pretty much obsolete)
#include "Square.hpp" //Square object
#include "HalfPlus.hpp" //HalfPlus object
#include "LShape.hpp" //LShape object
#include "InvLShape.hpp" //InvLShape
#include "StraightShape.hpp" //The Straight I shape
#include "SBlock.hpp" //S Block
#include "ZBlock.hpp" //Z block
#include "GameGrid.hpp" //For managing the game grid
#include "ShapeQueue.hpp" //Queue for shapes

//-----------------------------------------------------------------------------
// #defines (pre-processor constants)  (these don't have type-checking)
//-----------------------------------------------------------------------------
#define SCREEN_WIDTH 184 //Screen X size max
#define SCREEN_HEIGHT 88 //Screen Y size max
//Actual console size (But this is too small for what I set up)
//#define SCREEN_WIDTH 80
//#define SCREEN_HEIGHT 24
#define GAME_GRID_WIDTH 22 //Size of the game grid  (22 is a good value, 10 is real tetris)
#define GAME_GRID_HEIGHT 30 //Size of the game height (30 is a good value, 20 is real tetris)
#define SHAPEONDECK_X_OFFSET 8 //Offset from left side of screen of where to begin drawing shapes on deck

//Note that I didn't implement this next constant quite right, so changing the dist actally just changes the distance that my text is drawn, without moving the shapes themselves further apart. oops
#define DIST_BETWEEN_SHAPES_ON_DECK 6 //Distance between shapes on deck (vertical)

#define SCORE_PER_SHAPE 4 //Score for each successfully played shape
#define SCORE_PER_COMPLETE_ROW 50 //Score for each completely filled in row (note that doing multiple rows at once gives exponential score increase)

//These next three quantities are related
#define USER_INPUT_DELAY_MILLISECONDS 110  //~100 seems to be about the right value for this
//I use a time between frames constant to make frames with user input process at pretty close to the same rate as frames without user input (there is still a slight difference, but you wouldn't have noticed unless I told you)
#define TIME_BETWEEN_FRAMES_IN_NANOSECONDS 110000000 //8 zeros corresponds to two zeros in milliseconds
#define NANOSECONDS_PER_SLEEP_CALL 1000000 //See the loop in main to see how this is used

//Shape spawn parameters (see shape header files for orientation documentation)
#define SQUARE_SPAWN_ORIENTATION 0 //Value should be between 0-3
#define HALFPLUS_SPAWN_ORIENTATION 0 //Value should be between 0-3
#define LSHAPE_SPAWN_ORIENTATION 2 //Value should be between 0-3
#define INVLSHAPE_SPAWN_ORIENTATION 2 //Value should be between 0-3
#define STRAIGHTSHAPE_SPAWN_ORIENTATION 2 //Value should be between 0-3
#define SBLOCK_SPAWN_ORIENTATION 0 //Value should be between 0-3
#define ZBLOCK_SPAWN_ORIENTATION 0 //Value should be between 0-3

#define CHAR_TO_PRINT_ON_SHAPES_PLACED_IN_GRID "~"  //Needs to be a string of length 1


#define FRAMES_BETWEEN_GRAVITY 6 //So gravity is processed every ___ frames
#define FRAMES_BETWEEN_GRAVITY_2 5 //Once score threshold 1 has been reached, frames between gravity becomes ___
#define FRAMES_BETWEEN_GRAVITY_3 4 //Once score threshold 2 has been reached, frames between gravity becomes ___
#define FRAMES_BETWEEN_GRAVITY_4 3 //Once score threshold 3 has been reached, frames between gravity becomes ___
#define FRAMES_BETWEEN_GRAVITY_5 2 //Once score threshold 4 has been reached, frames between gravity becomes ___

#define SCORE_FOR_SPEED_INCREASE_1 560 //Score threshold 1 (above this is level 2)
#define SCORE_FOR_SPEED_INCREASE_2 1200 //Score threshold 2 (above this is level 3)
#define SCORE_FOR_SPEED_INCREASE_3 5500 //Score threshold 3 (above this is level 4)
#define SCORE_FOR_SPEED_INCREASE_4 10000 //Score threshold 4 (above this is level 5)


//-----------------------------------------------------------------------------
// Function Prototypes (These are just to let compiler know function signatures)
//-----------------------------------------------------------------------------
void setup();
void setupColors();
void setupNCurses();
void draw();
void logic();
void cleanup();
bool checkCollisions();
void userInput();
void addMoreShapesToShapeQ();
void drawBoxAroundPlayArea(int, int);
void drawShapesOnDeck();
void drawGameGrid(int y, int x);
void processGameOver();

//-----------------------------------------------------------------------------
// Constants   (These DO have type checking)
//-----------------------------------------------------------------------------
//Color pair constants (Probably could have used an enum for this. Oh well)
constexpr int BACKGROUND_CP = 1; //background colorpair
constexpr int SQUARE_CP = 2; //Square colorpair
constexpr int HALFPLUS_CP = 3; //HalfPlus color pair
constexpr int LSHAPE_CP = 4; //LShape Color pair number
constexpr int INVLSHAPE_CP = 5; //InvLShape Color pair number
constexpr int STRAIGHTSHAPE_CP = 6; //Straight Shape color pair
constexpr int SBLOCK_CP = 7; //SBlock color pair
constexpr int ZBLOCK_CP = 8; //ZBlock color pair
constexpr int TOPLINE_CP = 9; //Color pair for drawing the color of the top line

constexpr int SHAPE_VARIANTS = 7; //7 different types of shape
constexpr short LIGHT_BLUE = 33; //27 gave a light blue, 28 a green, 53 a purple, 33 light blue again...
constexpr short TOP_LINE_COLOR = 12; //Should be grey
constexpr int NUM_SHAPES_TO_ADD_ON_QUEUE_REPOPULATE = 10; //Number of shapes to be added to queue when necessary
constexpr int QUEUE_SIZE_TO_TRIGGER_REPOPULATE = 4;
constexpr int SHAPE_SPAWN_X_COOR = SCREEN_WIDTH / 2;
constexpr int SHAPE_SPAWN_Y_COOR = (SCREEN_HEIGHT / 2) - (GAME_GRID_HEIGHT / 2) - 3;
constexpr int RIGHTLEFT_MOVESPEED = 1;
constexpr int FALL_RATE_BASE = 1;
constexpr int NO_COMPLETE_ROWS = -1; //Code returned by gamegrid when no comeplete rows are detected

//For curser visibility
constexpr int NOT_VISIBLE = 0;
//const int NORMAL_VISIBLE = 1;
//const int VERY_VISIBLE = 2;

//-----------------------------------------------------------------------------
// Global variables (These will be visible to all functions in this file)
//-----------------------------------------------------------------------------
bool isTerminalAvailable = false; // Check this global variable before ncurses call
bool loopShouldEnd; //This keeps the game loop running until true

//bool gameCrashed; //Maybe have this flag set later so I can know if the game
//                   loop stopped running because of an error/exception

unsigned long long frameCounter;
ShapeQueue * shapeQ;
Shape * currentShape;
GameGrid * gameGrid; //The gamegrid
//short testGlobalVar = 0; //No longer used
int gameGridTopLeft_y; //Top left corner of game box
int gameGridTopLeft_x; //Top right corner of game box
long scoreCounter;
bool userMovedBlockDownThisTick; //this sorta works?
//bool blockMovedDownFromGravityThisTick; //This is other side of issue I am encountering? No gravity is ALWAYS computed after user input
bool gameOver;
short difficultyLevelCounter;

//Hehe I added this after main game was finished
bool godModeActive;
bool godModeUsed; //Will become true for always starting with the instant god mode is turned on


//-----------------------------------------------------------------------------
// MAIN (The entrypoint of the program)
//-----------------------------------------------------------------------------
int main(int argc, const char * argv[], const char* envp[]) {
    //Set random seed based off system time
    std::srand((unsigned int) std::time(0ul));
    
    //The next 35 or so lines are just to make sure the console opens correctly
    for (int argi = 1; argi < argc; argi++)
    {
        if (strcmp(argv[argi], "--debug-in-terminal") == 0)
        {
            printf("Debugging in terminal enabled\n");
            getchar(); // Without this call debugging will be skipped
            break;
        }
    }
    
    char *term = getenv("TERM");
    
    isTerminalAvailable = (term != NULL);
    
    if (isTerminalAvailable)
        isTerminalAvailable = (initscr() != NULL);
    
    if(has_colors() == FALSE) //Make sure the terminal window supports colors
    {    endwin(); //Tell the window to close
        printf("Your terminal does not support color. Join this century already!\n(Press Enter to crash gracefully)");
        getch();
        exit(5); //Program exits (crashes) because colors not supported (EXIT_CODE 5) [UNDOCUMENTED]
    }
    //--------------------------------------------------------------------------
    // Open Terminal and begin Game Loop
    // Here is where everything starts once we are sure the terminal will actually open
    //--------------------------------------------------------------------------
    try {
        if (isTerminalAvailable) {
            setup();
            do {
                //For time measurement I got code from https://stackoverflow.com/questions/14337278/precise-time-measurement
                auto begin = std::chrono::high_resolution_clock::now();  //Time Measurement (To smooth out the frame rate)
                draw();
                logic();
                frameCounter++;
                if (gameOver) {
                    timeout(-1);
                    processGameOver();
                    break;
                }
                auto end = std::chrono::high_resolution_clock::now();
                
                //If not enough time has elapsed since the last frame was processed, wait until enough time has passed
                if (std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count() < TIME_BETWEEN_FRAMES_IN_NANOSECONDS) {
                    do {
                        std::this_thread::sleep_for(std::chrono::nanoseconds(NANOSECONDS_PER_SLEEP_CALL));
                        end = std::chrono::high_resolution_clock::now();
                    } while (std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count() < TIME_BETWEEN_FRAMES_IN_NANOSECONDS);
                }
            } while (!loopShouldEnd);
            timeout(-1); //Remove the delay from skipping past user input (so code won't progress until user gives input)
            printw("Game Loop Exited. Press Enter (a few times) again to really quit for realz\n\n");
            refresh();
            
            //Clear any additional input in the input buffer
            timeout(0); //Remove any delay asking for user input
            while (wgetch(stdscr) != ERR) { //while input is still being detected (ERR means no input detected from user)
                wgetch(stdscr); //eat that input without doing anything to it
                //printw("Key Pressed: %d |", c); //for debug
            }
            timeout(-1); //Set delay for user input back to infinite delay
            getch();
            cleanup();
            move(SCREEN_HEIGHT - 7, (SCREEN_WIDTH / 2) - 18);
            printw("Completed Cleanup Routines (PRESS RETURN ONE MORE TIME!)\n\n");
            refresh();
            getch();
            printw("GOODBYE!");
        }
        return EXIT_SUCCESS;
    } catch (const std::exception& e) {
        move(SCREEN_HEIGHT / 2, SCREEN_WIDTH / 2);
        clear();
        printw("Exception Caught: %s", e.what());
        refresh();
    }
}

void setup() {
    setupNCurses(); //This function sets NCurses flags/parameters
    //Set Global Variables to initial values
    loopShouldEnd = false;
    gameOver = false;
    frameCounter = 0ll;
    currentShape = nullptr;
    gameGridTopLeft_y = (SCREEN_HEIGHT / 2) - (GAME_GRID_HEIGHT / 2) - 1;
    gameGridTopLeft_x = (SCREEN_WIDTH / 2) - (GAME_GRID_WIDTH / 2) - 1;
    scoreCounter = 0l;
    userMovedBlockDownThisTick = false;
   // blockMovedDownFromGravityThisTick = false; //Unused
    difficultyLevelCounter = 1;
    
    //GodMode
    godModeActive = false;
    godModeUsed = false;
    
    //Print Loading message
    printw("Welcome!\nLoading...");
    refresh(); //Refresh the screen
    //Create objects needed to faciliate the gameplay
    shapeQ = new ShapeQueue; //holds the upcoming shapes
    gameGrid = new GameGrid(GAME_GRID_HEIGHT, GAME_GRID_WIDTH, gameGridTopLeft_y, gameGridTopLeft_x);
    
    //For debug (Don't use with deleteRows on because will cause crash)
    //gameGrid->setUpDebugTest();
    
    timeout(USER_INPUT_DELAY_MILLISECONDS); //Set the timeout counter to ___ milliseconds
    
    //This function will add a collection of random shapes to the shapeQ
    addMoreShapesToShapeQ();
    
    //Set a starting shape
    currentShape = shapeQ->pop();
}

void setupNCurses() {
    //setupColors is a function I wrote, every other function is a built-in NCurses function (or macro)
    setupColors(); //color setup subroutine
    
    //Set operating conditions for ncurses to run with:
    noecho(); //Set echo to noecho;
    
    keypad(stdscr, TRUE); //This let's the code get special keyboard keys (i.e. f1, f2, arrow keys, etc...)
    
    //Set curser visibility
    curs_set(NOT_VISIBLE);
    //curs_set(NORMAL_VISIBLE); //Don't want the curser to be visible
    //curs_set(VERY_VISIBLE); //Don't want the curser to be very visible either
}

void setupColors() {
    start_color(); //http://tldp.org/HOWTO/NCURSES-Programming-HOWTO/color.html
    //Also see: https://stackoverflow.com/questions/27642696/changing-color-definitions-ncurses-c
    //Make sure that more than 8 colors are supported
    if (COLORS <= 8) { //Crash if 8 or less supported
        move(SCREEN_HEIGHT / 2, SCREEN_WIDTH / 2);
        clear();
        printw("OOPS! YOUR TERMINAL DOESN'T SUPPORTS MORE THAN 8 COLORS!\nWhat the heck sort of computer are you using?\n");
        refresh();
        loopShouldEnd = true;
        timeout(10000); //Leave this message on the screen for (10 x 2) seconds
        getch();
        getch();
        timeout(2); //Make any additional waits for user input to go fast
    }
    else {
        //init_color(COLOR_PINK, 999, 752, 796); //This gives grey?
        //init_color(COLOR_PINK, 900, 7, 300); //This was a light blue
        
        //To use colors, need to initialize two colors together into a 'color pair'.
        //Colors are then activated/deactivated by turning on/off color pairs while writing text
        
        //Syntax is INIT_PAIR(OBJ_CODE, TEXT COLOR, BACKGROUND (SHAPE) COLOR)
        init_pair(BACKGROUND_CP, COLOR_WHITE, COLOR_WHITE);
        init_pair(SQUARE_CP, COLOR_WHITE, LIGHT_BLUE);
        init_pair(HALFPLUS_CP, COLOR_BLUE, COLOR_WHITE);
        init_pair(LSHAPE_CP, COLOR_WHITE, COLOR_MAGENTA);
        init_pair(INVLSHAPE_CP, COLOR_WHITE, COLOR_YELLOW);
        init_pair(STRAIGHTSHAPE_CP, COLOR_WHITE, COLOR_CYAN);
        init_pair(SBLOCK_CP, COLOR_WHITE, COLOR_GREEN);
        init_pair(ZBLOCK_CP, COLOR_WHITE, COLOR_RED);
        init_pair(TOPLINE_CP, TOP_LINE_COLOR, COLOR_GREEN);
    }
}

void draw() {
    clear(); //Clear the screen
    
    //Draw frame counter
    move(0,0); //Set curser back to (0,0)
    //printw("My terminal supports %d colors.\n", COLORS);
    
    //Debug header print
    printw("\nFrame Number: %9lld   Shape Queue Size: %2d    SCORE: %ld ", frameCounter, shapeQ->getSize(), scoreCounter);
    //Release version of header
    //printw("\nFrame Number: %9lld    SCORE: %ld ", frameCounter, scoreCounter);
    if (godModeUsed) {
        printw(" (GMODE WAS USED)");
    }
    
    move(2,0);
    printw("\nCurrent Difficulty Level: %d\n", difficultyLevelCounter);
    move(1, SCREEN_WIDTH - 27);
    printw("(PRESS RETURN TO QUIT)\n\n");
    
    if (godModeActive) {
        move(SCREEN_HEIGHT - 3, SCREEN_WIDTH - 20);
        printw("GOD MODE ACTIVE!");
    }
    
    drawShapesOnDeck();
    
    //    int gameGridTopLeft_y = (SCREEN_HEIGHT / 2) - (GAME_GRID_HEIGHT / 2) - 1;
    //    int gameGridTopLeft_x = (SCREEN_WIDTH / 2) - (GAME_GRID_WIDTH / 2) - 1;
    drawBoxAroundPlayArea(gameGridTopLeft_y, gameGridTopLeft_x); //Will move curser to top left corner after completion
    
    drawGameGrid(gameGridTopLeft_y, gameGridTopLeft_x); //Draws all shapes currently in play aside from user controlled shape
    
    //Check to make sure currentShape is not null
    if (currentShape != nullptr) {
        //If currentShape is not null, draw it onto the screen
        attron(COLOR_PAIR(currentShape->getColorPair()));
        mvprintw(currentShape->getOrginY(), currentShape->getOrginX(), "*");
        for (int i = 0; i < (currentShape->getSize() - 1); i++) {
            mvprintw(currentShape->getOrginY() + currentShape->yCoorOffsets[i],
                     currentShape->getOrginX() + currentShape->xCoorOffsets[i],
                     ".");
        }
        attroff(COLOR_PAIR(currentShape->getColorPair()));
    }
    else { //CRASH!
        move(SCREEN_HEIGHT / 2, SCREEN_WIDTH / 2);
        printw("ERROR! Shape queue was empty!\n\n"); //This should never happen
        loopShouldEnd = true;
    }
    refresh(); //Have screen update all changes made to it
}

void logic() {
    userMovedBlockDownThisTick = false; //Set to false at the start of each tick
    //blockMovedDownFromGravityThisTick = false; //Set this to false also at start of each tick //Don't need this
    
    //Keep the shapeQ populated
    if (shapeQ->getSize() <= QUEUE_SIZE_TO_TRIGGER_REPOPULATE) {
        addMoreShapesToShapeQ();
    }
    
    //Process user input
    userInput(); //could set userMovedBlockDownThisTick to true
    //Check collisions
    checkCollisions(); //This just checks edge collisions
    if (gameGrid->checkForGridCollosion(currentShape)) {
        gameOver = gameGrid->addShapeToGrid(currentShape); //addShapeToGrid returns true if adding the shape to the grid results in the shape being over the top of the grid
        scoreCounter += SCORE_PER_SHAPE;
        if (shapeQ->peek() != nullptr) {
            delete currentShape; //Delete the shape
            currentShape = shapeQ->pop();
        }
        else { //it must be that the head of the shapeQ is == nullptr, so
            addMoreShapesToShapeQ(); //Repopulate the shapeQ
            delete currentShape; //Delete the current Shape (prevent them memory leaks dawg)
            currentShape = shapeQ->pop(); //get the next shape
        }
    }
    
    //Check to see if any rows are complete
    int completeRow = gameGrid->checkForCompleteRows();
    int completeRowThisTickCounter = 0;
    if (completeRow != NO_COMPLETE_ROWS) {
        do {
            completeRowThisTickCounter++;
            gameGrid->removeRow(completeRow);
            scoreCounter += pow(SCORE_PER_COMPLETE_ROW, completeRowThisTickCounter);
            completeRow = gameGrid->checkForCompleteRows();
        } while (completeRow != NO_COMPLETE_ROWS);
    }
    
    if (!userMovedBlockDownThisTick) {
        //Compute Gravity
        if (scoreCounter < SCORE_FOR_SPEED_INCREASE_1) {
            if ((frameCounter % FRAMES_BETWEEN_GRAVITY) == (FRAMES_BETWEEN_GRAVITY - 1)) {
                if (currentShape != nullptr) {
                    currentShape->moveFromGravity(FALL_RATE_BASE);
                }
                
            }
        }
        else if (scoreCounter < SCORE_FOR_SPEED_INCREASE_2) {
            if ((frameCounter % FRAMES_BETWEEN_GRAVITY_2) == (FRAMES_BETWEEN_GRAVITY_2 - 1)) {
                if (currentShape != nullptr) {
                    currentShape->moveFromGravity(FALL_RATE_BASE);
                    difficultyLevelCounter = 2;
                }
                //For now, if shape reaches bottom of screen, make shape become next shape in shapeQ
                //if (currentShape->getOrginY() >= (gameGridTopLeft_y + GAME_GRID_HEIGHT - 1)) {
                //  gameGrid->addShapeToGrid(currentShape);
                // if (shapeQ->peek() != nullptr) {
                //   delete currentShape; //Delete the shape
                //   currentShape = shapeQ->pop();
                //}
                // else {
                //    //currentShape = nullptr;
                //   addMoreShapesToShapeQ();
                //  delete currentShape; //Delete the shape
                //  currentShape = shapeQ->pop();
                // }
                // }
            }
        }
        else if (scoreCounter < SCORE_FOR_SPEED_INCREASE_3) {
            if ((frameCounter % FRAMES_BETWEEN_GRAVITY_3) == (FRAMES_BETWEEN_GRAVITY_3 - 1)) {
                if (currentShape != nullptr) {
                    currentShape->moveFromGravity(FALL_RATE_BASE);
                    difficultyLevelCounter = 3;
                }
            }
        }
        else if (scoreCounter < SCORE_FOR_SPEED_INCREASE_4) {
            if ((frameCounter % FRAMES_BETWEEN_GRAVITY_4) == (FRAMES_BETWEEN_GRAVITY_4 - 1)) {
                if (currentShape != nullptr) {
                    currentShape->moveFromGravity(FALL_RATE_BASE);
                    difficultyLevelCounter = 4;
                }
            }
        }
        else {
            if ((frameCounter % FRAMES_BETWEEN_GRAVITY_5) == (FRAMES_BETWEEN_GRAVITY_5 - 1)) {
                if (currentShape != nullptr) {
                    currentShape->moveFromGravity(FALL_RATE_BASE);
                    difficultyLevelCounter = 5;
                }
            }
        }
    }
}

void userInput() {
    int c = wgetch(stdscr); //Get user input
    if (currentShape != nullptr) { //If currentShape isn't null
        switch (c) { //Find out what input was given and do appropriate steps
            case 'g': //Toggle god mode
            case 'G': //Toggle god mode
                godModeActive = !godModeActive; //Switch the state of God Mode
                godModeUsed = true;
                break;
            case KEY_UP:
                if (godModeActive) {
                    currentShape->moveShapeUpOne();
                }
                //pointsToDraw.at(0)->yCoor -= VERT_MOVE_SPEED;
                break;
            case '1':
                if (godModeActive) {
                    shapeQ->addToFront(new StraightShape(STRAIGHTSHAPE_CP, SHAPE_SPAWN_Y_COOR, SHAPE_SPAWN_X_COOR, STRAIGHTSHAPE_SPAWN_ORIENTATION));
                }
                break;
            case '2':
                if (godModeActive) {
                    shapeQ->addToFront(new Square(SQUARE_CP, SHAPE_SPAWN_Y_COOR, SHAPE_SPAWN_X_COOR, SQUARE_SPAWN_ORIENTATION));
                }
                break;
            case '3':
                if (godModeActive) {
                    shapeQ->addToFront(new HalfPlus(HALFPLUS_CP, SHAPE_SPAWN_Y_COOR, SHAPE_SPAWN_X_COOR, HALFPLUS_SPAWN_ORIENTATION));
                }
                break;
            case '4':
                if (godModeActive) {
                    shapeQ->addToFront(new LShape(LSHAPE_CP, SHAPE_SPAWN_Y_COOR, SHAPE_SPAWN_X_COOR, LSHAPE_SPAWN_ORIENTATION));
                }
                break;
            case '5':
                if (godModeActive) {
                    shapeQ->addToFront(new InvLShape(INVLSHAPE_CP, SHAPE_SPAWN_Y_COOR, SHAPE_SPAWN_X_COOR, INVLSHAPE_SPAWN_ORIENTATION));
                }
                break;
            case '6':
                if (godModeActive) {
                    shapeQ->addToFront(new SBlock(SBLOCK_CP, SHAPE_SPAWN_Y_COOR, SHAPE_SPAWN_X_COOR, SBLOCK_SPAWN_ORIENTATION));
                }
                break;
            case '7':
                if (godModeActive) {
                    shapeQ->addToFront(new ZBlock(ZBLOCK_CP, SHAPE_SPAWN_Y_COOR, SHAPE_SPAWN_X_COOR, ZBLOCK_SPAWN_ORIENTATION));
                }
                break;
            case KEY_DOWN:
            case 's':
                currentShape->moveFromGravity(FALL_RATE_BASE);
                userMovedBlockDownThisTick = true;
                break;
            case 'S': //Make Capital S make shape fall faster (NOTE I REMOVED THIS FEATURE DURING DEBUG)
                currentShape->moveFromGravity(FALL_RATE_BASE * 2); //PARAMETER NO LONGER AFFECTS FALL SPEED
                userMovedBlockDownThisTick = true;
                break;
            case ',':
            case 'q':
            case 'Q':
                currentShape->rotateLeft();
                break;
            case '.':
            case 'e':
            case 'E':
                currentShape->rotateRight();
                break;
            case KEY_LEFT:
            case 'a':
            case 'A':
                if (!(gameGrid->hasGridElemLeftOf(currentShape))) {
                    currentShape->moveLeft(RIGHTLEFT_MOVESPEED);
                }
                break;
            case KEY_RIGHT:
            case 'd':
            case 'D':
                if (!(gameGrid->hasGridElemRightOf(currentShape))) {
                    currentShape->moveRight(RIGHTLEFT_MOVESPEED);
                }
                break;
            case 10: //I believe 10 is 'enter' or 'return' but it could be more keys as well
            case 27: //27 is both 'alt' and 'escape' (not sure why but internet said it is both)
                move(SCREEN_HEIGHT - 8, (SCREEN_WIDTH / 2) - 18);
                loopShouldEnd = true;
                processGameOver();
                timeout(-1); //A negative parameter makes the program wait indefinitly for user input again
                break;
            case ERR: //wgetch returns ERR if no key was pressed during polling period
                break;
            default:
                break;
        }
    }
    
    
    //Print detected key presses out to the screen
    //move(SCREEN_HEIGHT - 2, SCREEN_WIDTH /2 - 20);
    //printw("Key Pressed: %d |", c);
    //else { //This was from before I could guarentee the shapeQ would keep repopulating currentShape
    //    if (c == 10) {
    //        loopShouldEnd = true;
    //    }
    //}
    
    //Clear any additional input in the input buffer
    timeout(0); //Remove any delay asking for user input
    while (wgetch(stdscr) != ERR) {
        c = wgetch(stdscr);
        //printw("Key Pressed: %d |", c);
    }
    timeout(USER_INPUT_DELAY_MILLISECONDS);
}

bool checkCollisions() { //This is just for wall/floor collisions, collsions between shapes and GridElems are handled elsewhere
    int activeShapeXCoor[currentShape->getSize()]; //Create int array to hold x coordinates
    int activeShapeYCoor[currentShape->getSize()]; //Create int array to hold y coordinates
    activeShapeXCoor[0] = currentShape->getOrginX(); //Set first array entry to the orgin coordinate
    activeShapeYCoor[0] = currentShape->getOrginY(); //Set first array entry to the orgin coordinate
    for (int i = 1; i < currentShape->getSize(); ++i) { //Set the remaining array entries to the coordinates of the shape's offsets
        activeShapeXCoor[i] = currentShape->getOrginX() + currentShape->xCoorOffsets[i-1];
        activeShapeYCoor[i] = currentShape->getOrginY() + currentShape->yCoorOffsets[i-1];
    }
    
    //So the way my logic here works is maybe a little unconventional, but it works great.
    //What I do is I check to see if the shape has been moved out of bounds by user input
    //and if it has, then I just shift it till it is inbounds again!
    
    //Check for any side collisions for each part of the shape (orgin and the offsets)
    for (int i = 0; i < currentShape->getSize(); ++i) {
        //Check against left wall
        if (activeShapeXCoor[i] <= gameGridTopLeft_x) {//If shape is going to go outside left wall
            currentShape->moveRight(RIGHTLEFT_MOVESPEED);
            checkCollisions();
            return true;
        }
        //Check against right wall
        //       shape's coordinate  >= top left corner + gamegrid width + 1 more to get to the right wall
        else if (activeShapeXCoor[i] >= gameGridTopLeft_x + GAME_GRID_WIDTH + 1) { //if shape is going to go outside right wall
            currentShape->moveLeft(RIGHTLEFT_MOVESPEED);
            checkCollisions();
            return true;
        }
        //Check against bottom of the screen
        if (activeShapeYCoor[i] > gameGridTopLeft_y + GAME_GRID_HEIGHT) {
            currentShape->moveShapeUpOne();
            return true;
        }
    }
    
     for (int i = 0; i < currentShape->getSize(); ++i) {
        //If at the bottom of the grid
        if(activeShapeYCoor[i] == gameGridTopLeft_y + GAME_GRID_HEIGHT) {
            gameOver = gameGrid->addShapeToGrid(currentShape);
            scoreCounter += SCORE_PER_SHAPE;
            if (shapeQ->peek() != nullptr) {
                delete currentShape; //Delete the shape
                currentShape = shapeQ->pop();
            }
            else { //it must be that currentShape == nullptr, so
                addMoreShapesToShapeQ();
                delete currentShape; //Delete the shape
                currentShape = shapeQ->pop();
            }
            return true;
        }
    }
    return false;
}

//This function repopulates the shapeQ with more shapes
void addMoreShapesToShapeQ() {
    int shapeType;
    for (int i = 0; i < NUM_SHAPES_TO_ADD_ON_QUEUE_REPOPULATE; ++i) {
        shapeType = rand() % SHAPE_VARIANTS;
        /*//COLOR DEBUG LINE:
         //shapeType = 1;
         //Debug (This is to make sure the shapes being added to the queue are actually random)
         timeout(40); //If removing this, also remove timeout at bottom of this func
         move(35,35);
         printw("Adding more shapes to the queue...");
         move(36,35);
         printw("Random got: %d", shapeType);
         refresh();
         for (int i = 0; i < 15; i++) {
         getch();
         }
         //end debug */
        
        //Add a shape to the queue depending on what rand() assigned to shapeType
        switch (shapeType) {
            case 0:
                shapeQ->add(new HalfPlus(HALFPLUS_CP, SHAPE_SPAWN_Y_COOR, SHAPE_SPAWN_X_COOR, HALFPLUS_SPAWN_ORIENTATION));
                break;
            case 1:
                shapeQ->add(new Square(SQUARE_CP, SHAPE_SPAWN_Y_COOR, SHAPE_SPAWN_X_COOR, SQUARE_SPAWN_ORIENTATION));
                break;
            case 2:
                shapeQ->add(new LShape(LSHAPE_CP, SHAPE_SPAWN_Y_COOR, SHAPE_SPAWN_X_COOR, LSHAPE_SPAWN_ORIENTATION));
                break;
            case 3:
                shapeQ->add(new InvLShape(INVLSHAPE_CP, SHAPE_SPAWN_Y_COOR, SHAPE_SPAWN_X_COOR, INVLSHAPE_SPAWN_ORIENTATION));
                break;
            case 4:
                shapeQ->add(new StraightShape(STRAIGHTSHAPE_CP, SHAPE_SPAWN_Y_COOR, SHAPE_SPAWN_X_COOR, STRAIGHTSHAPE_SPAWN_ORIENTATION));
                break;
            case 5:
                shapeQ->add(new SBlock(SBLOCK_CP, SHAPE_SPAWN_Y_COOR, SHAPE_SPAWN_X_COOR, SBLOCK_SPAWN_ORIENTATION));
                break;
            case 6:
                shapeQ->add(new ZBlock(ZBLOCK_CP, SHAPE_SPAWN_Y_COOR, SHAPE_SPAWN_X_COOR, ZBLOCK_SPAWN_ORIENTATION));
                break;
            default: //Default should never be called theoretically
                shapeQ->add(new StraightShape(STRAIGHTSHAPE_CP, SHAPE_SPAWN_Y_COOR, SHAPE_SPAWN_X_COOR, STRAIGHTSHAPE_SPAWN_ORIENTATION));
                break;
        }
    }
    //Reset timeout to normal (This isn't needed if not debugging)
    //timeout(USER_INPUT_DELAY_MILLISECONDS);
}

void drawShapesOnDeck() {
    int onDeckYCoor = SCREEN_HEIGHT / 3;
    move(onDeckYCoor - 1, SHAPEONDECK_X_OFFSET);
    printw("Up Next:\n");
    //Need to compute the translation from ShapeSpawn to OnDeck area
    int yTrans = abs(SHAPE_SPAWN_Y_COOR - onDeckYCoor) + 3;
    int xTrans = abs(SHAPE_SPAWN_X_COOR - SHAPEONDECK_X_OFFSET) - 8;
    //Draw shape 1
    attron(COLOR_PAIR(shapeQ->peek()->getColorPair()));
    mvprintw(shapeQ->peek()->getOrginY() + yTrans - 1, shapeQ->peek()->getOrginX() - xTrans, "*");
    for (int i = 0; i < (shapeQ->peek()->getSize() - 1); ++i) {
        mvprintw(shapeQ->peek()->getOrginY() + yTrans + shapeQ->peek()->yCoorOffsets[i] - 1,
                 shapeQ->peek()->getOrginX() -xTrans + shapeQ->peek()->xCoorOffsets[i],
                 ".");
    }
    attroff(COLOR_PAIR(shapeQ->peek()->getColorPair()));
    
    //Draw shape 2
    move(onDeckYCoor + DIST_BETWEEN_SHAPES_ON_DECK + 2, SHAPEONDECK_X_OFFSET - 4);
    printw("And After That: \n\n");
    yTrans += DIST_BETWEEN_SHAPES_ON_DECK;
    attron(COLOR_PAIR(shapeQ->peekOnDeck()->getColorPair()));
    mvprintw(shapeQ->peekOnDeck()->getOrginY() + yTrans + 3, shapeQ->peekOnDeck()->getOrginX() - xTrans, "*");
    for (int i = 0; i < (shapeQ->peekOnDeck()->getSize() - 1); i++) {
        mvprintw(shapeQ->peekOnDeck()->getOrginY() + yTrans + shapeQ->peekOnDeck()->yCoorOffsets[i] + 3,
                 shapeQ->peekOnDeck()->getOrginX() -xTrans + shapeQ->peekOnDeck()->xCoorOffsets[i],
                 ".");
    }
    attroff(COLOR_PAIR(shapeQ->peekOnDeck()->getColorPair()));
}

void drawBoxAroundPlayArea(int gameGridTopLeft_y, int gameGridTopLeft_x) {
    //Draw the box around the outside of the play area:
    //Draw the top line
    move(gameGridTopLeft_y, gameGridTopLeft_x);
    addch(ACS_ULCORNER);
    attron(COLOR_PAIR(TOPLINE_CP));
    for (int i = 0; i < GAME_GRID_WIDTH; i++) {
        addch(ACS_HLINE);
    }
    attroff(COLOR_PAIR(TOPLINE_CP));
    addch(ACS_URCORNER);
    //Draw the edges
    gameGridTopLeft_y += 1;
    for (int j = 0; j < GAME_GRID_HEIGHT; j++) {
        move(gameGridTopLeft_y + j, gameGridTopLeft_x);
        addch(ACS_VLINE);
        for (int i = 0; i < GAME_GRID_WIDTH; i++) {
            addch(' ');
        }
        addch(ACS_VLINE);
    }
    //Draw the bottom line
    move(gameGridTopLeft_y + GAME_GRID_HEIGHT, gameGridTopLeft_x);
    addch(ACS_LLCORNER);
    for (int i = 0; i < GAME_GRID_WIDTH; i++) {
        addch(ACS_HLINE);
    }
    addch(ACS_LRCORNER);
    //needs to move curser to top left corner after completion
    move(gameGridTopLeft_y, gameGridTopLeft_x + 1);
}

void drawGameGrid(int y, int x) {
    for (int i = 0; i < GAME_GRID_WIDTH; i++) {
        for (int j = 0; j < GAME_GRID_HEIGHT; j++) {
            if (gameGrid->hasGridElemAt(j, i)) {
                attron(COLOR_PAIR((gameGrid->getColorAt(j, i))));
                mvprintw(y + j + 1, x + i + 1, CHAR_TO_PRINT_ON_SHAPES_PLACED_IN_GRID);
                attroff(COLOR_PAIR((gameGrid->getColorAt(j, i))));
            }
        }
    }
}

void processGameOver() {
    move((SCREEN_HEIGHT / 2) + (GAME_GRID_HEIGHT / 2) + 4, (SCREEN_WIDTH / 2) - 5);
    //attron(A_STANDOUT);
    attron(A_UNDERLINE);
    printw("GAME OVER!!!");
    move((SCREEN_HEIGHT / 2) + (GAME_GRID_HEIGHT / 2) +
         6, (SCREEN_WIDTH / 2) - 5);
    if (godModeUsed) {
        //printw("Your Score: %ld (God mode was used)\n", scoreCounter);
        printw("Your Score: %ld", scoreCounter);
        attroff(A_UNDERLINE);
        printw(" (God mode was used)\n");
    }
    else {
        printw("Your Score: %ld\n", scoreCounter);
         attroff(A_UNDERLINE);
    }
    refresh();
    //attroff(A_STANDOUT);
   
    loopShouldEnd = true;
    timeout(-1);
    move(SCREEN_HEIGHT - 8, (SCREEN_WIDTH / 2) - 18);
}

void cleanup () {
    //Delete the current shape
    if (currentShape != nullptr) { delete currentShape; }
    //Delete the gameGrid
    if (gameGrid != nullptr) {
        delete gameGrid;
        gameGrid = nullptr;
    }
    //Delete all shapes remaining in shapeQ
    while (shapeQ->peek() != nullptr) {
        delete shapeQ->pop();
    }
    
    delete shapeQ; //Delete the Shape Queue itself
    //timeout(3000);
    getch(); //Make the user press enter yet another time before app quits
}

