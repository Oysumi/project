#include "jeu.h"

/* Function to quit the program */

bool escape(Input *inkey){
    if ( (inkey->quit) || (inkey->key[SDLK_ESCAPE]) || (inkey->key[SDLK_a]) )
        return true ;
    return false ;
}

/* Function to know if the bubble is on one of the boards */

bool bubbleEdges(SDL_Rect *pos){
    if ( (pos->x + BUB_SIZE <= BOARD_RIGHT ) && ( pos->x >= BOARD_LEFT ) && ( pos->y >= BOARD_TOP ) )
        return false ;
    return true ; /* the bubble is on one of the boards */
}

/* Function to check the time */

bool timereached(Time *t){
	if (t->currentTime - t->previousTime > 20)
		return true ;
	return false ;
}

/* Function to check if the bubble is launched */

bool bubblelaunched(Input *in){
	if ( in->launched )
		return true ;
	return false ;
	printf("%d", in->launched );
}

/* Function to initialize the screen */

void initScreen(screen *S){

	unsigned int j ;

	/* Initialize SDL */
    SDL_Init (SDL_INIT_VIDEO) ;

    /* Set the title bar */
    SDL_WM_SetCaption("BubbleGame", "BubbleGame") ;

    /* Create window */
    S->screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0) ;

    /* Load the sprite of the launcher */
    S->temp = SDL_LoadBMP("launcher/frame_launcher.bmp") ;
    S->launcher = SDL_DisplayFormat(S->temp) ;
    SDL_FreeSurface(S->temp) ;

    /* Load the sprite of the bubble */
    init_bub_color(S) ;

    /* Load the frame */
    S->temp  = SDL_LoadBMP("gamebackground/frame_1p.bmp") ;
    S->frame = SDL_DisplayFormat(S->temp) ;
    SDL_FreeSurface(S->temp) ;

    /* Setup the sprites and the frame colorkey and turn on RLE */
    S->colorkey = SDL_MapRGB(S->screen->format, 255, 0, 255) ;
    SDL_SetColorKey(S->launcher, SDL_SRCCOLORKEY | SDL_RLEACCEL, S->colorkey) ;
    SDL_SetColorKey(S->frame, SDL_SRCCOLORKEY | SDL_RLEACCEL, S->colorkey) ;
    for ( j = 0 ; j < NUM_COLORS ; ++j ){
    	SDL_SetColorKey(S->bubble[j], SDL_SRCCOLORKEY | SDL_RLEACCEL, S->colorkey) ;
    }

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

    /* Set a color for the bubble */
    assign_color(bub) ;

}

/* Function to initialize the speed vectors */

void initDist(realPos *bub){
    bub->dx = 0 ;
    bub->dy = 0 ;
}

/* Function to calculate the speed vectors */

void setDist(realPos *bub){
  if ( (bub->dx == 0) && (bub->dy == 0) ){
      bub->dx = cos( PI * (bub->launcherimg + 2) / LAUNCHER_DIV ) ;
      bub->dy = sin( PI * (bub->launcherimg + 2) / LAUNCHER_DIV ) ;
    }
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

int bubblemov(realPos *bub, image *graph, Input *in, int Tab[BUB_NY][BUB_NX]){

    if ( bub->edge ){
      bub->x = BUB_X ;
      bub->y = BUB_Y ;
      bub->dx = 0 ;
      bub->dy = 0 ;
      graph->bubblePos.x = BUB_X ;
      graph->bubblePos.y = BUB_Y ;
      bub->edge = 0 ;
      in->launched = 0 ;
      assign_color(bub) ;
    }

    if ( !bubbleEdges(&graph->bubblePos) ){
        bub->x -= bub->dx ;
        bub->y -= bub->dy ;
        graph->bubblePos.x = (int)bub->x ;
        graph->bubblePos.y = (int)bub->y ;
        if ( graph->bubbleSprite < 23 )
            graph->bubbleSprite += 1 ;
        else
	  		graph->bubbleSprite = 0 ;
        }

    else{
       if ( graph->bubblePos.x < BOARD_LEFT ){
	    bub->x = 2 * BOARD_LEFT - bub->x ;
	    graph->bubblePos.x = (int)bub->x ; 
	    graph->bubblePos.y = (int)bub->y ;
	    }
	 
	if ( graph->bubblePos.x > BOARD_RIGHT - BUB_SIZE ){
	    bub->x = 2 * ( BOARD_RIGHT - BUB_SIZE ) - bub->x ;
	    graph->bubblePos.x = (int)bub->x ; 
	    graph->bubblePos.y = (int)bub->y ;
        }

	bub->dx = (-1)*bub->dx ;

	if ( graph->bubblePos.y <= BOARD_TOP ){

		maillage( graph ) ;
		fill_bubarray( Tab, graph ) ;
	  	bub->edge = 1 ;
	  	in->launched = 0 ;
	  	return 0 ;
		}
   	}	

    return 0 ;

}

/* Function to refresh the elements of the screen with the time */

void updateScreen(image *graph, realPos *pos, screen *bub, Time *t, int Tab[BUB_NY][BUB_NX]){

	int i ;
	image temp ;

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

    /* Faire une boucle qui affiche toutes les cases du tableau contenant un 1 */ 
    SDL_BlitSurface(bub->bubble[pos->current_color], &graph->bubbleImage, bub->screen, &graph->bubblePos) ;

    temp.bubbleImage.y = BUB_SIZE * 1 ;
  
    for ( i = 0 ; i<=BUB_NX ; ++i ){
    	if ( Tab[0][i] == 1 ){
    		temp.bubbleImage.x = 0 ;
    		temp.bubbleImage.w = BUB_SIZE ;
    		temp.bubbleImage.h = BUB_SIZE ;
    		temp.bubblePos.x = BOARD_LEFT + 40 * i ;
    		temp.bubblePos.y = BOARD_TOP ;
    		SDL_BlitSurface( bub->bubble[pos->current_color], &temp.bubbleImage, bub->screen, &temp.bubblePos ) ;
    	}
    }


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

/* Place the bubble on a precise position */

void maillage(image *graph){

	int dx ; /* distance between two points */
	int rest ; 

	dx = ( (BOARD_RIGHT - BUB_SIZE) - BOARD_LEFT ) / 7 ;
	rest = ( graph->bubblePos.x ) % dx ;

	if ( rest <= 20 ) {
		graph->bubblePos.x -= rest ;
	}
	else {
		graph->bubblePos.x += (dx - rest) ;
	}

}

/* Fills the bubarray of 1 when a bubble touched the ceiling */

void fill_bubarray(int Tab[BUB_NY][BUB_NX], image *graph){

	unsigned int i ;
	i = (graph->bubblePos.x - BOARD_LEFT) / 40 ;
	Tab[0][i] = 1 ;

}

/* Initialize all the colors for the bubbles */

void init_bub_color(screen *S){

	S->temp = SDL_LoadBMP("bubs/bub_blue.bmp") ;
    S->bubble[0] = SDL_DisplayFormat(S->temp) ;

    S->temp = SDL_LoadBMP("bubs/bub_black.bmp") ;
    S->bubble[1] = SDL_DisplayFormat(S->temp) ;

    S->temp = SDL_LoadBMP("bubs/bub_green.bmp") ;
    S->bubble[2] = SDL_DisplayFormat(S->temp) ;

    S->temp = SDL_LoadBMP("bubs/bub_orange.bmp") ;
    S->bubble[3] = SDL_DisplayFormat(S->temp) ;

    S->temp = SDL_LoadBMP("bubs/bub_purple.bmp") ;
    S->bubble[4] = SDL_DisplayFormat(S->temp) ;

    S->temp = SDL_LoadBMP("bubs/bub_red.bmp") ;
    S->bubble[5] = SDL_DisplayFormat(S->temp) ;

    S->temp = SDL_LoadBMP("bubs/bub_white.bmp") ;
    S->bubble[6] = SDL_DisplayFormat(S->temp) ;

    S->temp = SDL_LoadBMP("bubs/bub_yellow.bmp") ;
    S->bubble[7] = SDL_DisplayFormat(S->temp) ;

    SDL_FreeSurface(S->temp) ;

}

/* Generate a random number to define the color of the bubble */

unsigned int rand_col(void){

	unsigned int rand_number ;

	srand(time(NULL)) ; /* initialization of rand */
	rand_number = rand() % ( NUM_COLORS - 1 ) ; /* NOTE : current_col doit être compris entre 1 et 8, faire modif plus tard */
	
	return rand_number ;

}

/* Assign a color to a bubble */

void assign_color(realPos *bub){

	unsigned int i ;

	i = rand_col() ;
	bub->current_color = i ;

}