#include "jeu.h"


int main(int argc, char* argv[])
{
    Input in ;
    realPos bubbleReal ;
    screen game ;
    image graphics ;
    Time check ;

    initScreen(&game) ;
	initPos(&graphics, &bubbleReal) ;
	initTime(&check) ;
 
    memset(&in, 0, sizeof(in)); // Fill the whole "array" of zeros

    while ( !escape(&in) ){

        HandleEvent( &in ) ;
        updateScreen( &graphics, &bubbleReal, &game, &check ) ;

        if ( timereached(&check) ) /* controls the speed of the launcher (especially the FPS) */
        	launchermov( &in, &graphics, &check ) ;

        if ( bubblelaunched(&in) )
            bubblemov( &bubbleReal, &graphics, &in ) ;

        SDL_UpdateRect(game.screen, 0, 0, 0, 0) ;

    }

    /* Clean up */
    SDL_FreeSurface(game.launcher) ;
    SDL_FreeSurface(game.frame) ;
    SDL_Quit() ;

    return 0 ;
}
