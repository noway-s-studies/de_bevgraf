/************************************************
* OpenSnake v1.0                                *
*-----------------------------------------------*
* control:                                      *
*  - navigation : W A S D                       *
*  - exit       : Enter                         *
*************************************************/

#include <time.h>
#include "snakeEngine.h"

int main()
{
    int gd = VGA;
    int gm = VGAMAX;
    int page = 0;
    int timer = 0;
    int haspickup = 0;
    int game = 1;
    int direction = 1;
    node* snake = NULL;
    segment pickup;

    int speed = 4;

    initgraph(&gd,&gm,"");

    // init
    for (int i = 0; i < 3; i++)
        snake = insert(snake, 600, 390);

    srand(time(NULL));

    while (1) {
		setactivepage(page);
		cleardevice();
		// entry point

        drawSnake(snake);
        drawSegment(pickup.x, pickup.y, CYAN);

        if (game && timer++ >= speed) {
            moveSnake(snake, direction);
            collisionDetection(snake, pickup, &haspickup, &game);
            timer = 0;
        }

        settextstyle(0, 0, 4);
        if (!game) outtextxy(getmaxx() / 2 - 160, getmaxy() / 2 - 30, "Game Over!");

        if (!haspickup)
        {
            pickup = getPickup(snake);
            haspickup = !haspickup;
        }

		if(kbhit()) {
            int inchar = getch();
            inchar = turnDetection(inchar, direction);
            if (inchar == -1) exit(0);
            else if (inchar >= 0 ) direction = inchar;
		}

        setvisualpage(page);
        page = !page;
        delay(25); // 40 fps
    }

    closegraph();
    return 0;
}
