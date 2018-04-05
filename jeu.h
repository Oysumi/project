#ifndef __JEU_H__
#define __JEU_H__
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <stdbool.h>
#include "SDL.h"

/* Approximate pi value */
#define PI 3.14159265359

/* Size of the window */
#define SCREEN_WIDTH  720
#define SCREEN_HEIGHT 540
#define BOARD_LEFT 200
#define BOARD_RIGHT 520
#define BOARD_TOP 31
#define BOARD_BOTTOM 422

/* In the launcher sprite, we have 45 images of a 187x157 picture, */
/* Size of one launcher image: */
#define LAUNCHER_WIDTH 187
#define LAUNCHER_HEIGHT 157
#define LAUNCHER_CENTER 94
#define LAUNCHER_DIV 48

/* Order of the different directions in the picture */
#define DIR_LEFT -1
#define DIR_RIGHT 1

/* Size and position of one bubble image */
#define BUB_SIZE 40
#define BUB_X 339 // SCREEN_WIDTH - BUB_SIZE / 2
#define BUB_Y 457

/* Velocity of bubbles */
#define VELOCITY 2

/* Size of bub_array[BUB_NY][BUB_NX] */
#define BUB_NX 8 // max. number of bubbles in horizontal direction
#define BUB_NY 11 // max. number of bubbles in vertical direction

/* ---------------------------------------------------------------------------------------------------------------- */
					       /* STRUCTURES */
/* ---------------------------------------------------------------------------------------------------------------- */


/* Declaration of a double (point) */

typedef struct{
    double x ;
    double y ;
    double dx ; /* Dx (for the movement of the bubble) */
    double dy ; /* Dy (for the movement of the bubble) */
    int launcherimg ;
    int edge ; /* Takes 1 if a bubble is on one of the edges of the frame (necessary to reset the bubble) */
}realPos;

/* This structure records if a touch of the keyboard was rested or not */

typedef struct{
    char key[SDLK_LAST] ;
    int quit ;
    int launched ; /* This field allows to calculate the trajectory of the bubble when we press space */
}Input;

/* This structure allows to group all the parameters defining the screen */

typedef struct{
    SDL_Surface *screen ;
    SDL_Surface *temp ;
    SDL_Surface *launcher ; /* used for the Display */
    SDL_Surface *frame ;    /* used for the Display */
    SDL_Surface *bubble ;   /* used for the Display */
    int colorkey ;
}screen;

/* This structure allows to control the frames of the launcher */

typedef struct{
    int previousTime ;
    int currentTime ;
}Time;

/* This structure allows to group all the parameters defining the images/sprites */

typedef struct{
	SDL_Rect black_bg ; /* Will 'refresh' the black background */
        SDL_Rect launcherPos ; /* Rectangle to store the position of the launcher in the window. */
	SDL_Rect bubblePos ; /* Rectangle to store the position of the bubble in the window. */
	SDL_Rect launcherImage ; /* Allow to select the right sprite */
	SDL_Rect bubbleImage ;  /* Same thing */
	int launcherSprite ; /* 45 sprites */
	int bubbleSprite ; /* 24 sprites */
}image;


/* ---------------------------------------------------------------------------------------------------------------- */
                                                 /* FUNCTIONS */
/* ---------------------------------------------------------------------------------------------------------------- */

/* NOTE : A TRIER SELON L'ORDRE D'APPARITION DANS LE MAIN */

/* Function to close the windows */
bool escape(Input *inkey) ;

/* Function to know if the bubble is on one of the boards */
bool bubbleEdges(SDL_Rect *pos) ;

/* Function to check the time */
bool timereached(Time *t) ;

/* Function to check if the bubble is launched */
bool bubblelaunched(Input *in) ;

/* Function to initialize the screen */
void initScreen(screen *S) ;

/* Function to initialize the time */
void initTime(Time *t) ;

/* Function to initialize the positions */
void initPos(image *graph, realPos *bub) ;

/* Function to initialize the speed vectors */
void initDist(realPos *bub) ;

/* Function to calculate the speed vectors */
void setDist(realPos *bub) ;

/* Function to move the launcher */
void launchermov(Input *in, image *graph, Time *t) ;

/* Function to move the bubble */
int bubblemov(realPos *bub, image *graph, Input *in, int *Tab) ;

/* Function to refresh the elements of the screen with the time */
void updateScreen(image *graph, realPos *pos, screen *bub, Time *t, int *Tab) ;

/* Handle events coming from the user */
void HandleEvent(Input *inkey) ;

/* Function to set the bubble in a right position */
int maillage(int **Tab, image *graph) ;

/* Function fill the bub_array with ones */
void fillbubarray(int **Tab, screen *bub, image *graph);


/* ---------------------------------------------------------------------------------------------------------------- */
/*                                          END OF STRUCTURES AND FUNCTIONS                                         */                  
/* ---------------------------------------------------------------------------------------------------------------- */

#endif
