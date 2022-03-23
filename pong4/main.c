#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#define WINDOW_WIDTH (640)
#define WINDOW_HEIGHT (480)

#define SCROLL_SPEED (400)
#define SPEED (200)

void set_pixel(SDL_Renderer *rend, int x, int y, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	SDL_SetRenderDrawColor(rend, r,g,b,a);
	SDL_RenderDrawPoint(rend, x, y);
}

void draw_circle(SDL_Renderer *s, int n_cx, int n_cy, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	double error = (double)-radius;
	double x = (double)radius - 0.5;
	double y = (double)0.5;
	double cx = n_cx - 0.5;
	double cy = n_cy - 0.5;

	while (x >= y)
	{
		set_pixel(s, (int)(cx + x), (int)(cy + y), r, g, b, a);
		set_pixel(s, (int)(cx + y), (int)(cy + x), r, g, b, a);

		if (x != 0)
		{
			set_pixel(s, (int)(cx - x), (int)(cy + y), r, g, b, a);
			set_pixel(s, (int)(cx + y), (int)(cy - x), r, g, b, a);
		}

		if (y != 0)
		{
			set_pixel(s, (int)(cx + x), (int)(cy - y), r, g, b, a);
			set_pixel(s, (int)(cx - y), (int)(cy + x), r, g, b, a);
		}

		if (x != 0 && y != 0)
		{
			set_pixel(s, (int)(cx - x), (int)(cy - y), r, g, b, a);
			set_pixel(s, (int)(cx - y), (int)(cy - x), r, g, b, a);
		}

		error += y;
		++y;
		error += y;

		if (error >= 0)
		{
			--x;
			error -= x;
			error -= x;
		}
	}
}

//Init of the SDL
int main(void){
    if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) != 0){
        printf("error initializing SDL: %s\n",SDL_GetError());
        return 1;
    }

   SDL_Window* win = SDL_CreateWindow("Pong 2 Time !",
                                        SDL_WINDOWPOS_CENTERED,
                                        SDL_WINDOWPOS_CENTERED,
                                        WINDOW_WIDTH, WINDOW_HEIGHT, 0);

    if(!win)
    {
        printf( ": %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    Uint32 render_flags = SDL_RENDERER_ACCELERATED;
    SDL_Renderer* rend = SDL_CreateRenderer(win, -1, render_flags);

    if(!rend)
    {
        printf("error creating renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
    }

    //Draw paddle 1
    SDL_Rect rect;
    rect.w = 10;
    rect.h = 100;
    rect.x = 30;
    rect.y = (WINDOW_HEIGHT/2)- rect.w/2;

    SDL_RenderFillRect(rend, &rect);

    //Draw paddle 2
    SDL_Rect rec;
    rec.w = 10;
    rec.h = 100;
    rec.x = 600;
    rec.y = (WINDOW_HEIGHT/2)- rec.w/2;
    
    SDL_RenderFillRect(rend, &rec);

    //start the ball in middle of the screen
    float rayon = 8;
    float x_ball = (WINDOW_WIDTH - rayon) / 2;
    float y_ball = (WINDOW_HEIGHT - rayon) / 2;
    //give sprite initial velocity
    float x_ballvel = -SPEED;
    float y_ballvel = SPEED;

    draw_circle(rend, x_ball, y_ball, rayon, 0x00, 0x00, 0xFF, 0xFF);
    
    //Score points
    int score1 = 0;
    int score2 = 0;

    //Data of input's players
    int up = 0;
    int down = 0;

    int up2 = 0;
    int down2 = 0;
    

    //Set to 1 when window close button is pressed
    int close_requested = 0;

    while ( !close_requested)
    {
        SDL_Event event;
        while(SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_QUIT:
                    close_requested = 1;
                    break;
                case SDL_KEYDOWN:
                    switch (event.key.keysym.scancode)
                    {
                        case SDL_SCANCODE_W:
                            up = 1;
                            break;
                        case SDL_SCANCODE_S:
                        
                            down = 1;
                            break;
                        case SDL_SCANCODE_UP:
                            up2 = 1;
                            break;
                        case SDL_SCANCODE_DOWN:
                            down2 = 1;
                            break;
                        default:
                            break;
                    }
                    break;
                case SDL_KEYUP:
                    switch (event.key.keysym.scancode)
                    {
                        case SDL_SCANCODE_W:
                        
                            up = 0;
                            break;
                        case SDL_SCANCODE_UP:
                            up2 = 0;
                            break;
                        
                        case SDL_SCANCODE_S:
                            down = 0;
                            break;
                        case SDL_SCANCODE_DOWN:
                            down2 = 0;
                            break;
                        default:
                            break;
                    }
                    break;
        
            }
        }

        //Clear the window
        SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
        SDL_RenderClear(rend);

        SDL_SetRenderDrawColor(rend, 255, 255, 255, 0);
        
        SDL_RenderFillRect(rend, &rect);

        SDL_RenderFillRect(rend, &rec);

        //Update positions
        x_ball += x_ballvel /60;
        y_ball += y_ballvel /60;
        draw_circle(rend, x_ball, y_ball, rayon, 0xFF, 0xFF, 0xFF, 0x00);
        
        float x_vel = 0;
        float y_vel = 0;

        float x2_vel = 0;
        float y2_vel = 0;


        //Determine speed
        x_vel = y_vel = 0;
        if (up && !down) y_vel = -SCROLL_SPEED;
        if (down && !up) y_vel = SCROLL_SPEED;

        if (up2 && !down2) y2_vel = -SCROLL_SPEED;
        if (down2 && !up2) y2_vel = SCROLL_SPEED;


        //Reset positions
        rect.x += x_vel / 60;
        rect.y += y_vel /60;

        rec.x += x2_vel / 60;
        rec.y += y2_vel / 60;


        //Paddle collision detection with bounds
        if(rect.x <= 0) rect.x = 0;
    
        if(rect.y <= 0) rect.y = 0;
        
        if(rect.x >= WINDOW_WIDTH - rect.w) rect.x = WINDOW_WIDTH - rect.w;
        
        if(rect.y >= WINDOW_HEIGHT - rect.h) rect.y = WINDOW_HEIGHT - rect.h;

        if(rec.x <= 0) rec.x = 0;
    
        if(rec.y <= 0) rec.y = 0;
        
        if(rec.x >= WINDOW_WIDTH - rec.w) rec.x = WINDOW_WIDTH - rec.w;
        
        if(rec.y >= WINDOW_HEIGHT - rec.h) rec.y = WINDOW_HEIGHT - rec.h;
          
        //Ball collision detection with bounds
        if(y_ball <= 0)
        {
            y_ball = 0 +rayon;
            y_ballvel = -y_ballvel;
        }
     
         if(y_ball >= WINDOW_HEIGHT - rayon)
        {
            y_ball = WINDOW_HEIGHT - rayon;
            y_ballvel = -y_ballvel;
        }

        //Collision ball paddle 1
        if(x_ball - rayon <= rect.x + rect.w && y_ball - rayon >= rect.y && y_ball + rayon <= rect.y + rect.h)
        {
            x_ball = rect.x + rect.w + rayon;
            x_ballvel = -x_ballvel;
        }

        //Collision ball paddle 2
        if(x_ball + rayon >= rec.x && y_ball - rayon >= rec.y && y_ball + rayon <= rec.y + rect.h)
        {
            x_ball = rec.x - rayon;
            x_ballvel = -x_ballvel;
        }
       
        if(x_ball <= 0)
        {   
            score2 += 1;
            x_ball = (WINDOW_WIDTH - rayon) / 2;
            y_ball = (WINDOW_HEIGHT - rayon) / 2;
            x_ballvel = -x_ballvel;
            SDL_Delay(1000);

        }
        else if (x_ball >= WINDOW_WIDTH)
        {
            score1 += 1;
            x_ball = (WINDOW_WIDTH - rayon) / 2;
            y_ball = (WINDOW_HEIGHT - rayon) / 2;
            x_ballvel = -x_ballvel;
            SDL_Delay(1000);
        }
    
        //Game Over message
        if((score1 == 3)  || (score2 == 3)){
            SDL_Delay(1000);
            if(score1 == 3){
                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
                         "Game Over",
                         "Player 1 Win",
                         NULL);
            }
            if(score2 == 3){
                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
                         "Game Over",
                         "Player 2 Win",
                         NULL);
            }
            break;
        }
       
        SDL_RenderPresent(rend);

        SDL_Delay(1000/60);

        
    }

    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    SDL_Quit();
}
