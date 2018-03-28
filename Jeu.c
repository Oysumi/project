#include "SDL.h"
#include <math.h>
#include <stdbool.h>

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

/* Order of the different directions in the picture: */
#define DIR_LEFT -1
#define DIR_RIGHT 1

/* Size and position of one bubble image */
#define BUB_SIZE 40
#define BUB_X 339 // SCREEN_WIDTH - BUB_SIZE / 2
#define BUB_Y 457

/* Velocity of bubbles */
#define VELOCITY 2

/* Declaration of a double (point) */

typedef struct{
    double x ;
    double y ;
    int launcherimg ;
    int edge ; /* This field allows to store the good sprite's launcher image to calculate the trajectory of the bubble */
}realPos;

/* This structure records if a touch of the keyboard was rested or not */

typedef struct{
    char key[SDLK_LAST] ;
    int quit ;
    int launched ; /* This field allows to calculate the trajectory of the bubble when we press space */
}Input;

/* Function to quit the program */

bool escape(Input *inkey){
    if ( (inkey->quit) || (inkey->key[SDLK_ESCAPE]) || (inkey->key[SDLK_a]) )
        return true ;
    return false ;
}

/* Function to know if the bubble is on one of the boards */

bool bubbleEdges(SDL_Rect *pos){
    if ( (pos->x < BOARD_RIGHT - BUB_SIZE ) && ( pos->x > BOARD_LEFT) && ( pos->y > BOARD_TOP ) )
        return false ;
    return true ; /* the bubble is on one of the boards */
}

/* Handle events coming from the user */

void HandleEvent(Input *inkey)
{
    SDL_Event event ; /* When we press a key, we create an event in this function */

    while ( SDL_PollEvent(&event) )
    {
        switch ( event.type )
        {
        case SDL_KEYDOWN :
            inkey->key[event.key.keysym.sym] = 1 ;
	    if (event.key.keysym.sym == SDLK_SPACE)
	      inkey->launched = 1 ;
            break ;
        case SDL_KEYUP :
	    inkey->key[event.key.keysym.sym] = 0 ;
            break ;
        case SDL_QUIT :
            inkey->quit = 1 ;
            break ;
        default :
            break ;
        }
    }
}

int main(int argc, char* argv[])
{
    Input in ;
    realPos bubbleReal ;

    SDL_Surface *screen, *temp, *launcher, *frame, *bubble ;
    SDL_Rect black_bg ; /* Will 'refresh' the black background */
    SDL_Rect launcherPos ; /* Rectangle to store the position of the launcher in the window. */
    SDL_Rect bubblePos ; /* Rectangle to store the position of the bubble in the window. */
    SDL_Rect launcherImage ; /* Allows to select the right sprite */
    SDL_Rect bubbleImage ;  /* Same thing */

    int colorkey ;
    int launcherSprite ; /* 45 sprites */
    int bubbleSprite ; /* 24 sprites */
    int previousTime ; /* Delay */
    int currentTime ;

    previousTime = 0 ;

    /* Initialize SDL */
    SDL_Init (SDL_INIT_VIDEO) ;

    /* Set the title bar */
    SDL_WM_SetCaption("BubbleGame", "BubbleGame") ;

    /* Create window */
    screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0) ;

    /* Load the sprite of the launcher */
    temp = SDL_LoadBMP("frame_launcher.bmp") ;
    launcher = SDL_DisplayFormat(temp) ;
    SDL_FreeSurface(temp) ;

    /* Load the sprite of the bubble */
    temp = SDL_LoadBMP("bub_blue.bmp") ;
    bubble = SDL_DisplayFormat(temp) ;
    SDL_FreeSurface(temp) ;

    /* Load the frame */
    temp  = SDL_LoadBMP("frame_1p.bmp") ;
    frame = SDL_DisplayFormat(temp) ;
    SDL_FreeSurface(temp) ;

    /* Setup the sprites and the frame colorkey and turn on RLE */
    colorkey = SDL_MapRGB(screen->format, 255, 0, 255) ;
    SDL_SetColorKey(launcher, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey) ;
    SDL_SetColorKey(frame, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey) ;
    SDL_SetColorKey(bubble, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey) ;

    /* First position of the bubble */
    bubbleReal.x = BUB_X ;
    bubbleReal.y = BUB_Y ;
    bubbleReal.edge = 0 ; /* At the beggining, the bubble is not on one of the boards */

    /* First sprite of the launcher when we start the game */
    launcherSprite = 22 ;

    /* First sprite of the bubble when we start the game */
    bubbleSprite = 0 ;

    /* Set the launcher sprite in the window */
    launcherPos.x = (SCREEN_WIDTH - LAUNCHER_WIDTH) / 2 ;
    launcherPos.y = SCREEN_HEIGHT - LAUNCHER_HEIGHT ;

    /* Set the bubble sprite in the window */
    bubblePos.x = BUB_X ;
    bubblePos.y = BUB_Y ;

    /* Set rectangle dimensions and position to refresh sprite animations */
    black_bg.w = SCREEN_WIDTH ;
    black_bg.h = SCREEN_HEIGHT ;
    black_bg.x = 0 ;
    black_bg.y = 0 ;

    /* Set the parameters of the launche's sprite for the BlitSurface */
    launcherImage.x = 0 ;
    launcherImage.w = LAUNCHER_WIDTH ;
    launcherImage.h = LAUNCHER_HEIGHT ;

    /* Same operation for the Bubble */
    bubbleImage.x = 0 ;
    bubbleImage.w = BUB_SIZE ;
    bubbleImage.h = BUB_SIZE ;

    memset(&in, 0, sizeof(in)); // Fill the whole "array" of zeros

    while ( !escape(&in) ){

        HandleEvent( &in ) ;

        /* Update the black background */
        SDL_FillRect(screen, &black_bg, SDL_MapRGB(screen->format, 0, 0 ,0)) ;

        /* Draw the frame */
        SDL_BlitSurface(frame, NULL, screen, NULL) ;

        /* Set the parameters of the launche's sprite for the BlitSurface */
        launcherImage.x = 0 ;
        launcherImage.w = LAUNCHER_WIDTH ;
        launcherImage.h = LAUNCHER_HEIGHT ;

        /* choose the launcher's orientation according to direction */
        launcherImage.y = LAUNCHER_HEIGHT * launcherSprite ;

        SDL_BlitSurface(launcher, &launcherImage, screen, &launcherPos) ;

        /* Same operation for the Bubble */
        bubbleImage.x = 0 ;
        bubbleImage.w = BUB_SIZE ;
        bubbleImage.h = BUB_SIZE ;
        bubbleImage.y = BUB_SIZE * bubbleSprite ;
        bubbleReal.launcherimg = launcherSprite ;

        SDL_BlitSurface(bubble, &bubbleImage, screen, &bubblePos) ;

        currentTime = SDL_GetTicks() ;

        if (currentTime - previousTime > 4){
            if (in.key[SDLK_LEFT]){
                if (!in.launched){ /* so we can't change the direction of the bubble during the movement */
                    if ( launcherSprite > 0 )
                        launcherSprite += DIR_LEFT ;
                }
            }

            if (in.key[SDLK_RIGHT]){
                if (!in.launched){ /* so we can't change the direction of the bubble during the movement */
                    if ( launcherSprite < 44 )
                        launcherSprite += DIR_RIGHT ;
                }
            }
            previousTime = currentTime ;
        }

        if (in.launched){
            if (bubbleReal.edge){
                bubblePos.x = BUB_X ;
                bubbleReal.x = BUB_X ;
                bubblePos.y = BUB_Y ;
                bubbleReal.y = BUB_Y ;
                in.launched = 0 ;
                bubbleReal.edge = 0 ;
                continue ;
            }
            if ( !bubbleEdges(&bubblePos) ){
                bubbleReal.x -= cos( PI * (bubbleReal.launcherimg + 2) / LAUNCHER_DIV ) * VELOCITY ;
                bubbleReal.y -= sin( PI * (bubbleReal.launcherimg + 2) / LAUNCHER_DIV ) * VELOCITY ;
                bubblePos.x = (int)bubbleReal.x ;
                bubblePos.y = (int)bubbleReal.y ;
                if ( bubbleSprite < 23 )
                    bubbleSprite += 1 ;
                else
                    bubbleSprite = 0 ;
            }
            else{
                in.launched = 0 ;
                bubbleReal.edge = 1 ; /* The bubble is on one of the edges */
            }
        }

        SDL_UpdateRect(screen, 0, 0, 0, 0) ;
    }

    /* Clean up */
    SDL_FreeSurface(launcher) ;
    SDL_FreeSurface(frame) ;
    SDL_Quit() ;

    return 0 ;
}
