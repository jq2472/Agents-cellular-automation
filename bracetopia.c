/**
 * bracetopia -  initializes simulation command line arguments. 
 * Program simulates the movements of 'e' (k&R) and 'n'(Eric Allman)
 * agents as they seek the satisfaction of being close to neighbors
 * who like the same brace placement.
 *
 * @author Jolin Qiu
 */

#define _DEFAULT_SOURCE

#include <stdio.h>
#include <stdlib.h> 		// srandom( 41 );
#include <stdbool.h>
#include <getopt.h> 		// processing for "-flag" cla
#include <unistd.h> 		//usleep function: usleep(  delay_value );
#include <curses.h>

#include "simulation.h" 	// file that runs the simulation
#include "agent.h" 		// agent data struct

/**
* main routine for the report generator
* 
* @param argc argument count
* @param argv array of strings containing command line arguments
* @return termination status for the program
*/
extern int size = 15;    	// dimesions of the square grid
extern int threshold = 50; 	// strength of preference
int vacancy_rate = 20;		// the percentage of the entire space that is to be vacant
int endl_percent = 60;		// proportion of e agents to n agents, default=60
int cycle_count = -1; 		// num simulation cycles
useconds_t sleep_delay; 	// delay between simulation cycles, default=900,000ms

/**
* percent of vacancy/occupancy
*/
int calculate_percent(int value, int total){
    int percent = (value * total) / 100;
    return percent;
}

/**
* shuffle 2d array using modern algorithm ver of Fisher-Yates Shuffle
*/
void shuffle(Agent agents[][size]) {
    srandom(41);

    // Start from the last element and swap with a randomly selected element
    for (int row = size - 1; row > 0; row--) {
        for (int col = size - 1; col > 0; col--) {
            int randRow = rand() % (row + 1);
            int randCol = rand() % (col + 1);

            swap(&agents[row][col], &agents[randRow][randCol]);
        }
    }
}

/**
* print_grid() - prints the grid using cursor controlled output
*/
void print_grid(Agent agents[][size]){
    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            mvaddch(row, col * 2, agents[row][col].preference);
        }
    }
}

/**
* print_status() - when the simulation starts, prints the status info
* + the current board cycle.
* 
* @param agents board
*/
void print_status(Agent agents[][size], int curr_cycle, int remaining_cells, bool curses){
    int num_moves;
    if (curr_cycle == 0){
        update_cycle(agents);
        num_moves = 0;
    } else {
        num_moves = update_cycle(agents);
    }
    float happiness = avg_happiness(agents, remaining_cells);

    if (curses){
        clear(); // Clear the screen
        print_grid(agents);
        mvprintw(size, 0, "Use [ Control + C ] to Quit");
        mvprintw(size + 2, 0, "Cycle: %i", curr_cycle);
        mvprintw(size + 3, 0, "Moves this cycle: %i", num_moves);
        mvprintw(size + 4, 0, "Overall \"happiness\": %.2f", happiness);
        mvprintw(size + 5, 0, "Size: %i, Threshold: %i, Vacancy rate: %i, Endline percent: %i",
                size, threshold, vacancy_rate, endl_percent);
        refresh();
        usleep(sleep_delay);

    } else {
    //prints grid w out curses
    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            printf("%c ", agents[row][col].preference);
        }
        printf("\n");
    }
    //print summary
    printf("Cycle: %i\n", curr_cycle);
    printf("Moves this cycle: %i\n", num_moves);
    printf("Overall \"happiness\": %.2f\n", happiness);
    printf("Size: %i, Threshold: %i%, Vacancy rate: %i%, Endline percent: %i%\n", size, threshold, vacancy_rate, endl_percent);
    }
}

int main (int argc, char *argv[]){
    int opt;
    while ((opt = getopt(argc, argv, "ht:c:d:s:v:e:")) != -1) {
        bool pass;
        switch (opt){ 
            case 'h':
                printf("usage:\n");
			 printf("bracetopia [-h] [-t N] [-c N] [-d D] [-s S] [-v V] [-e E]\n");
			 printf("Option      Default   Example   Description\n");
			 printf("\'-h\'        n/a       -h        print this usage message.\n");
			 printf("\'-t N\'      900000    -t 5000   cycle delay, in microseconds.\n");
			 printf("\'-c N\'      n/a       -c4       cycle count maximum value.\n");
			 printf("\'-d D\'      15        -d    7   width and height dimension.\n");
			 printf("\'-s S\'      50        -s 30     strength of preference.\n");
			 printf("\'-v V\'      20        -v30      percent vacancies.\n");
			 printf("\'-e E\'      60        -e75      percent of braces that are Endline.\n");
                break;
            case 't':
                sleep_delay = strtol( optarg, NULL, 10 );
                if (sleep_delay < 1){
                    sleep_delay = 900000;
                }
                usleep( sleep_delay );
                break;
            case 'c':
                if (optarg && *optarg){
                    cycle_count = strtol( optarg, NULL, 10 );
                    if (cycle_count < 0){
                        fputs("count (N) must be a non-negative integer", stderr);
                        return 2;
                    }
                } else {
                    cycle_count = -1; // run in infinite mode
                }
                break;
	    case 'd':
                size = strtol(optarg, NULL, 10 );
                pass = (size >= 5 && size <= 39);
                if (!pass){
                    fputs("dimension (N) must be a value in [5..39]", stderr);
                    return 2;
                }
                break;
	    case 's':
                threshold = strtol( optarg, NULL, 10 );
                pass = (1 <= threshold && threshold <= 99);
                if (!pass){
                    fputs("preference strength (N) must be a value in [1..99]", stderr);
                    return 2;
                }
                break;
	    case 'v':
                vacancy_rate = strtol( optarg, NULL, 10 );
                pass = (1 <= vacancy_rate && vacancy_rate <= 99);
                if (!pass){
                    fputs("vacancy_rate (N) must be a value in [1..99]", stderr);
                    return 2;
                }
                break;
	    case 'e':
                endl_percent = strtol( optarg, NULL, 10 );
                pass = (1 <= endl_percent && endl_percent <= 99);
                if (!pass){
                    fputs("endline proportion (N) must be a value in [1..99]", stderr);
                    return 2;
                }
                break;
            default:
                fprintf(stderr, "usage:\nbracetopia [-h] [-t N] [-c N] [-d D] [-s S] [-v V] [-e E]\n");
                return(EXIT_FAILURE);
        }
    }

    /* Get all of the non-option arguments */
    if (optind < argc) {
        printf("Non-option args: ");
        while (optind < argc)
            printf("%s ", argv[optind++]);
        printf("\n"); 
    }

    //creating random layout of agents distributed in the grid

    Agent agents[size][size];

    int total = size*size;
    int cells_vacant = calculate_percent(vacancy_rate, total);
    int remaining_cells = total - cells_vacant;
    int num_endline = calculate_percent(endl_percent, remaining_cells);
    int num_newline = total - num_endline;

    // populating the board
    for(int row = 0; row<size; row++){
        for(int col = 0; col<size; col++){
            if (cells_vacant > 0) {
                agents[row][col].preference = '.';
                cells_vacant--;
            } else if (num_endline > 0) {
                agents[row][col].preference = 'e';
                num_endline--;
            } else if (num_newline > 0) {
                agents[row][col].preference = 'n';
                num_newline--;
            }
          }
      }

    //initial grid;
    shuffle(agents);
    //populate status of cells that are initially vacant after the shuffle
    for(int row = 0; row<size; row++){
        for(int col = 0; col<size; col++){
            if (row >= 0 && row < size && col >= 0 && col < size) {
                if (agents[row][col].preference == '.'){
                    agents[row][col].initially_vacant = true;
                    agents[row][col].curr_happiness = 0.0;
                }
            }
        }
     }

    //print mode
    int curr_cycle = 0;
    if (cycle_count > 0){
        do {
            print_status(agents, curr_cycle, remaining_cells, false);
        } while (curr_cycle++ < cycle_count);
    } else {
    //infinite mode
        initscr();
        keypad(stdscr, TRUE);
        clear();
        while (1){
            print_status(agents, curr_cycle, remaining_cells, true);
            //refresh();
            curr_cycle++;
            usleep(sleep_delay);
        }
        endwin();
    }

  return 0;
}


