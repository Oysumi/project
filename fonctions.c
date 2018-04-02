#include "jeu.h"

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

/* Function to check the time */

bool timereached(Time *t){
	if (t->currentTime - t->previousTime > 6)
		return true ;
	return false ;
}

/* Function to check if the bubble is launched */

bool bubblelaunched(Input *in){
	if ( in->launched )
		return true ;
	return false ;
}

/* Function to initialize the screen */

void initScreen(screen *S){

	/* Initialize SDL */
    SDL_Init (SDL_INIT_VIDEO) ;

    /* Set the title bar */
    SDL_WM_SetCaption("BubbleGame", "BubbleGame") ;

    /* Create window */
    S->screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0) ;

    /* Load the sprite of the launcher */
    S->temp = SDL_LoadBMP("frame_launcher.bmp") ;
    S->launcher = SDL_DisplayFormat(S->temp) ;
    SDL_FreeSurface(S->temp) ;

    /* Load the sprite of the bubble */
    S->temp = SDL_LoadBMP("bub_blue.bmp") ;
    S->bubble = SDL_DisplayFormat(S->temp) ;
    SDL_FreeSurface(S->temp) ;

    /* Load the frame */
    S->temp  = SDL_LoadBMP("frame_1p.bmp") ;
    S->frame = SDL_DisplayFormat(S->temp) ;
    SDL_FreeSurface(S->temp) ;

    /* Setup the sprites and the frame colorkey and turn on RLE */
    S->colorkey = SDL_MapRGB(S->screen->format, 255, 0, 255) ;
    SDL_SetColorKey(S->launcher, SDL_SRCCOLORKEY | SDL_RLEACCEL, S->colorkey) ;
    SDL_SetColorKey(S->frame, SDL_SRCCOLORKEY | SDL_RLEACCEL, S->colorkey) ;
    SDL_SetColorKey(S->bubble, SDL_SRCCOLORKEY | SDL_RLEACCEL, S->colorkey) ;

}

/* Function to initialize the time */

void initTime(Time *t){
	t->previousTime = 0 ;
}

/* Function to initialize the positions */

void initPos(image *graph, realPos *bub){

	/* First position of the bubble */
    bub->x = BUB_X ;
    bub->y = BUB_Y ;
    bub->edge = 0 ; /* At the beggining, the bubble is not on one of the edges */

    /* First sprite of the launcher when we start the game */
    graph->launcherSprite = 22 ;

    /* First sprite of the bubble when we start the game */
    graph->bubbleSprite = 0 ;

    /* Set the launcher sprite in the window */
    graph->launcherPos.x = (SCREEN_WIDTH - LAUNCHER_WIDTH) / 2 ;
    graph->launcherPos.y = SCREEN_HEIGHT - LAUNCHER_HEIGHT ;

    /* Set the bubble sprite in the window */
    graph->bubblePos.x = BUB_X ;
    graph->bubblePos.y = BUB_Y ;

    /* Set rectangle dimensions and position to refresh sprite animations */
    graph->black_bg.w = SCREEN_WIDTH ;
    graph->black_bg.h = SCREEN_HEIGHT ;
    graph->black_bg.x = 0 ;
    graph->black_bg.y = 0 ;

}

/* Function to move the launcher */

void launchermov(Input *in, image *graph, Time *t){

	if (in->key[SDLK_LEFT]){
        if (!in->launched){ /* so we can't change the direction of the bubble during the movement */
            if ( graph->launcherSprite > 0 )
                    graph->launcherSprite += DIR_LEFT ;
        }
    }

    if (in->key[SDLK_RIGHT]){
        if (!in->launched){ /* so we can't change the direction of the bubble during the movement */
            if ( graph->launcherSprite < 44 )
                graph->launcherSprite += DIR_RIGHT ;
        }
    }
            
    t->previousTime = t->currentTime ;
}

/* Function to move the bubble */

int bubblemov(realPos *bub, image *graph, Input *in){

	if (bub->edge){
        graph->bubblePos.x = BUB_X ;
        bub->x = BUB_X ;
        graph->bubblePos.y = BUB_Y ;
        bub->y = BUB_Y ;
        in->launched = 0 ;
        bub->edge = 0 ;
        return 0 ;
        }

    if ( !bubbleEdges(&graph->bubblePos) ){
        bub->x -= cos( PI * (bub->launcherimg + 2) / LAUNCHER_DIV ) * VELOCITY ;
        bub->y -= sin( PI * (bub->launcherimg + 2) / LAUNCHER_DIV ) * VELOCITY ;
        graph->bubblePos.x = (int)bub->x ;
        graph->bubblePos.y = (int)bub->y ;
        if ( graph->bubbleSprite < 23 )
            graph->bubbleSprite += 1 ;
        else
            graph->bubbleSprite = 0 ;
        }

    else{
        in->launched = 0 ;
        bub->edge = 1 ; /* The bubble is on one of the edges */
    	}

    return 0 ;

}

/* Function to refresh the elements of the screen with the time */

void updateScreen(image *graph, realPos *pos, screen *bub, Time *t){

	/* Update the black background */
    SDL_FillRect(bub->screen, &graph->black_bg, SDL_MapRGB(bub->screen->format, 0, 0 ,0)) ;

    /* Draw the frame */
    SDL_BlitSurface(bub->frame, NULL, bub->screen, NULL) ;

    /* Parameters of the launcher's sprites for the BlitSurface */
    graph->launcherImage.x = 0 ;
    graph->launcherImage.w = LAUNCHER_WIDTH ;
    graph->launcherImage.h = LAUNCHER_HEIGHT ;

    /* choose the launcher's orientation according to direction */
    graph->launcherImage.y = LAUNCHER_HEIGHT * graph->launcherSprite ;

    SDL_BlitSurface(bub->launcher, &graph->launcherImage, bub->screen, &graph->launcherPos) ;

    /* Same operation for the Bubble */
    graph->bubbleImage.x = 0 ;
    graph->bubbleImage.w = BUB_SIZE ;
    graph->bubbleImage.h = BUB_SIZE ;
    graph->bubbleImage.y = BUB_SIZE * graph->bubbleSprite ;
    pos->launcherimg = graph->launcherSprite ;

    SDL_BlitSurface(bub->bubble, &graph->bubbleImage, bub->screen, &graph->bubblePos) ;

    t->currentTime = SDL_GetTicks() ;

}

/* Handle events coming from the user */

void HandleEvent(Input *inkey)
{
    SDL_Event event ;

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