#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
/*
**  Author:         William Ockmore
**  Username:       wbo201
**  Date:           10/12/11
**
**  Name:           Independent project
**
**  Description:    This program is a turn-based lunar lander game, that uses accurate physics for a 2-dimensional
**                  projectile, including thrusters that impart kinetic energy via impulse. The aim is to reach a landing area
**                  an arbitrary distance from the starting point, and land without destroying the lander. It uses a graphical display
**                  within the console, using several character arrays, which include pre-generated terrain and background stars.
**                  The lander itself is a structure, and almost every function uses pointers to its elements.
*/
 
typedef struct flyer {
    float y;
    float vy;
    float x;
    float vx;
    float mass;
    int fuel;
} Flyer;
 
#define DIST 100
#define WINDOW 30
#define N 100*DIST
#define INITIAL_HEIGHT 50
 
 
void suvat (Flyer *b, float g, float dt);
void bird_input (Flyer *b);
void collision_check (Flyer *b, int heights[N]);
char thrusters(Flyer *b);
void game(Flyer *b);
void diff_menu(Flyer *b);
void draw(char stars[2*WINDOW][WINDOW], int heights[N], Flyer *b);
void terrain(int heights[N]);
void starfield(char stars[2*WINDOW][WINDOW]);
 
int main (void) {
    Flyer lander, *b;
    float dt=0.02;
    char c, stars[2*WINDOW][WINDOW];
    int i, heights[N];
 
    b = &lander;
    b->mass = 2000;
    b->y = INITIAL_HEIGHT;
    b->vx = 15;
    b->x = 0;
    b->vy = 0;
 
    srand(time(NULL));
 
    starfield(stars);                //Generates the starfield.
    do {                             //Generates the terrain.
        terrain(heights);
    } while (heights[N/2-DIST]>INITIAL_HEIGHT);  //checks that the terrain doesn't cause the lander to spawn underground.
 
    game(b);
 
    for (i = 0; 1==1; i++) {
 
        if (i%10 == 0) {
            draw(stars, heights, b);
            printf("distance=%g height=%g velocity(x,y) = %g,%g fuel = %i \n", b->x, b->y, b->vx, b->vy, b->fuel);
            c = thrusters(b);
            if (c == 'w') {
                printf("Vertical thrusters engaged.\n");
            }
            if (c == 'a' || c == 'd') {
                printf("Lateral thrusters engaged.\n");
            }
            if (c=='s') {
                printf("Thrusters ignored.\n");  //any key other than the specific thruster commands will ignore thrusters,
            }                                    //but s is part of wasd and hence the most convinient one to display a message for.
            if (c == '&') {
                printf("No fuel remaining.\n");
            }
        }
            suvat(b, -1.63, dt);   //Gravity on the moon has a value of 1.63 m/s^2
            collision_check(b, heights);
    }
}
 
char thrusters(Flyer *b) {
    char choice;
    float a;
    if (b->fuel <= 0) {
        choice = '&';
    }
    else {
        printf("Will you engage thrusters?\n");
        scanf(" %c", &choice);
        if (choice == 'w') {
            b->fuel = b->fuel - 10;
            a = (2000)/(b->mass);     //the lander's thrusters exert a force of 2000N when active. Each
            b->vy = b->vy + a;        //time they are fired, they burn for  1 second.
        }
        if (choice == 'a') {
            b->fuel = b->fuel - 10;
            a = (2000)/(b->mass);
            b->vx = b->vx - a;
        }
        if (choice == 'd') {
            b->fuel = b->fuel - 10;
            a = (2000)/(b->mass);
            b->vx = b->vx + a;
        }
    }
    return choice;
}
 
 
void bird_input (Flyer *b) {
    printf("What is the height?\n");
    scanf("%f", &b->y);
    printf("What is the upward velocity?\n");
    scanf("%f", &b->vy);
    printf("What is the horizontal velocity?\n");
    scanf("%f", &b->vx);
    printf("What is the mass?\n");
    scanf("%f", &b->mass);
 }  //Function only used for testing.
 
 
void suvat (Flyer *b, float g, float dt) {
    b->y = 0.5 * g * pow(dt,2) + (b->vy) * dt + b->y;   // s = 0.5at^2 + ut
    b->x = b->vx * dt + b->x;
 
    b->vy = b->vy + g * dt;        //v = u + at
}
 
 
void collision_check (Flyer *b, int heights[N]) {    //Checks for whether the lander is in contact with the terrain or landing zone.
    long int bx;
    bx = lroundf(b->x);
 
    if (b->y - heights[(N/2)-DIST+bx] <= 0) {
        if (b->x >= DIST-5              //Winning conditions.
            && b->x <= DIST+5
            && b->vy >= - 5
            && b->vx >= -5
            && b->vx <= 5) {
 
            printf("\n\n Congratulations! You've set your lander down successfully. Buzz and Neil would be proud.\n");
        }
        else {
            printf("\n\n You just crashed our 100 million dollar lander!\n\n"  //Message when player loses.
                   "Boy, are you inept! \n");
        }
 
        printf("The distance travelled is %f meters.\n", b->x);
        exit(0);
    }
}
 
 
void game(Flyer *b) {
 
    printf(" --------------\n"
           "| LUNAR LANDER |\n"
           " --------------\n\n");
    printf("Welcome to Lunar Lander! What difficulty would you like to play?\n\n");
 
    diff_menu(b);
 
    printf("The aim of the game is to land the lunar module within the landing area. The landing area "
           "is %i m east of your current position. The lander is equipped with a finite amount of "
           "fuel, and gives readouts in units of meters. If you run out of fuel, the lander will fall "
           "until it hits the ground.If you can't set the lander down within 5 meters of the landing "
           "site,or you land with too much force, you will destroy the lander. If you manage to get "
           "your vertical and horizontal velocity below 5 m/s, you and your lander will enjoy a smooth landing. "
           "Be careful not to waste fuel, and plan your trajectory.\n\n", DIST);
    printf("Controls: \n\n"
           "w : vertical thruster\n"
           "a : left thruster\n"
           "d : right thruster\n\n"
           "press s to choose not to fire your thrusters.\n");
 
}
 
 
void diff_menu(Flyer *b) {
 
    int option;
 
    printf("1. Trainee \n"
           "2. Cadet \n"
           "3. Space Veteran \n"
           "4. Masochist \n");
    scanf("%i", &option);
 
    switch (option) {
        case 1:
            b->fuel = 2000;
            printf("You have chosen to play at Trainee difficulty. Shouldn't be too tough.\n\n");
            break;
        case 2:
            b->fuel = 600;
            printf("You have chosen to play at Cadet difficulty.\n\n");
            break;
        case 3:
            b->fuel = 350;
            printf("You have chosen to play at Space Veteran difficulty. Good luck - you're gonna need it.\n\n");
            break;
        case 4:
            b->fuel = 200;
            printf("You have chosen to play on Masochist difficulty. Got something to prove?\n\n");
            break;
        default:
            break;
 
    }
}
 
 
void draw (char stars[2*WINDOW][WINDOW], int heights[N], Flyer *b) {   //procedurally generates the character array to the screen to display the lunar lander.
    int x, y, height, i;
    long int bx, by;
    char window[2*WINDOW][WINDOW];
    bx = lroundf(b->x);
    by = lroundf(b->y);
 
    for (x=0; x<(2*WINDOW); ++x) {
        for (y=0; y<WINDOW; ++y) {
            window[x][y]=stars[x][y];    //Loads the previously generated background stars. The function for generating them
        }                                //turned out to be quite intensive so I put it in main to only run once at the beginning.
    }
    height = 0;
    for (x=0; x<(2*WINDOW) ; ++x) {
        if (by-(WINDOW/2) <= heights[(N/2)-DIST+bx+(x-WINDOW)]) {                        //Draws the ground when within range.
            for (y=0; y<((WINDOW/2)-(by-heights[(N/2)-DIST+bx+(x-WINDOW)])); ++y) {
                window[x][y] = 'o
                ';
            }
        }
    }
    x=0;
    y=0;
    if ((DIST-(bx+WINDOW) < 0)&&(by-(WINDOW/2) <= heights[N/2])) {        //Draws the landing zone when in range.
        window[DIST-5-bx+WINDOW][(WINDOW/2)-(by-heights[N/2])] = '^';
        for (i=0; i<9; ++i) {
            x=DIST+4-i-bx+WINDOW;
            y=(WINDOW/2)-(by-heights[N/2])-1;
            window[x][y]='T';
        }
        window[DIST+5-bx+WINDOW][(WINDOW/2)-(by-heights[N/2])] = '^';
    }
    window[WINDOW][WINDOW/2] = 'A';    //The lander is displayed with an "A".
    for (y=(WINDOW-1); y>0; --y) {
        for (x=0; x<(2*WINDOW); ++x) {
            printf("%c", window[x][y]);
        }
        printf("\n");
    }
}
 
 
void terrain(int heights[N]) {            //This function draws the array of heights by
                                          //stepping along and using a random funtion to
    int i, n, f=0;                        //increase or decrease the height.
 
    for (i=(N/2 - 6); i!=0; --i) {        //starts from the landing zone and goes backwards to ensure
        n = rand() % 2;                   //the height of the landing zone is always 0.
        if (n == 1) {
            f = f + 1;
        }
        else if (n == 0) {
            f = f - 1;
        }
 
        heights[i] = f;
    }
    f=0;
    for (i=0; i<10; ++i) {                //Keeps the landing area the same height.
        heights[(N/2)-5+i] = f;
    }
    for (i=(N/2 + 5); i<N; ++i) {
        n = rand() % 2;
        if (n == 1) {
            f = f + 1;
        }
        else if (n == 0) {
            f = f - 1;
        }
        heights[i] = f;
    }
}
 
 
void starfield(char stars[2*WINDOW][WINDOW]) {
    int x, y, count, p, i;                   //these loops define the background
    for (x=0; x<(2*WINDOW); ++x) {           //starfield using a system of
        for (y=0; y<WINDOW; ++y) {           //determining if coordinate combinations
            p = (123779*x+127123*y);         //are prime numbers; I found this gave a cool
            for(i=2;i<=p/2;i++){             //pseudo-random constellation effect when the
                count = 0;                   //numbers were big enough.
                if(p%i==0){
                    count++;
                    break;
                }
            }
            if (count == 0) {
                stars[x][y] = '*';
            }
            else {
                stars[x][y] = ' ';
            }
        }
    }
}
