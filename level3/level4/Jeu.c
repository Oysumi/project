#include "jeu.h"


int main(int argc, char* argv[])
{
    Input in ;
    realPos bubbleReal ;
    screen game ;
    image graphics ;
    Time check ;
    int bub_array[BUB_NY][BUB_NX] ;
    int bub_array_centers[BUB_NY][BUB_NX][2] ;

    initScreen(&game) ;
    initPos(&graphics, &bubbleReal) ;
    initTime(&check) ;
    initDist(&bubbleReal) ;
 
    memset(&in, 0, sizeof(in)) ; // Fill the whole "array" of zeros
    memset(&bub_array, 0, sizeof(bub_array)) ;

    while ( !escape(&in) ){

        HandleEvent( &in ) ;
		updateScreen( &graphics, &bubbleReal, &game, &check, bub_array ) ;

        if ( timereached(&check) ) /* controls the speed of the launcher (especially the FPS) */
            launchermov( &in, &graphics, &check ) ;

        if ( bubblelaunched(&in) ){
	    	setDist( &bubbleReal ) ;
            bubblemov( &bubbleReal, &graphics, &in, bub_array ) ;
		}

        SDL_UpdateRect(game.screen, 0, 0, 0, 0) ;

    }

    /* Clean up */
    SDL_FreeSurface(game.launcher) ;
    SDL_FreeSurface(game.frame) ;
    SDL_Quit() ;

    return 0 ;
}
