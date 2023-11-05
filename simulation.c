/**
 * simulation -  functions that aid in the movements of 'e' (k&R) and 'n'(Eric Allman)
 *
 * @author Jolin Qiu
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <getopt.h> // processing for "-flag" cla

#include "simulation.h" // file that runs the simulation
#include "agent.h" // agent data struct

int prev_row = 0; // last moved row
int prev_col = 0;
int vacant_row;
int vacant_col;

extern int size;
extern int threshold; // pulled from main after processed from cmd line
/**
* private functions
*/

/**
* first_vacant_space() - first space valid to move into
* @param the previous vacant agent row/col position and the current
* first_vacant_space(agents);
*/

void first_vacant_space(Agent agents[][size]) {
    vacant_row = prev_row;
    vacant_col = prev_col;
    do {
        if ((agents[vacant_row][vacant_col].preference == '.') && (agents[vacant_row][vacant_col].initially_vacant)) {
            //update the global vacant-trackers to start from this space
            prev_row = vacant_row;
            prev_col = vacant_col;
            return;
        } else {
            // stay within the valid bounds
            vacant_col++;
            if (vacant_col == size) {
                vacant_col = 0;
                vacant_row++;
                if (vacant_row == size) {
                     vacant_row = 0;
                }
             }
        }
    } while (vacant_row != prev_row || vacant_col != prev_col);
}


/**
* public functions
*/

/**
* swap function
*/
void swap(Agent *e, Agent *n) {
    Agent temp = *e;
    *e = *n;
    *n = temp;
}

/**
 * update_agent(): checks bounds of the current agent cell 
 * and updates the current attributes of the agent cell depending on its neighbors.
 **/
void update_agent(Agent agents[][size], int r, int c){
    int row, col, same_preference = 0;
    int count = 0;

    for(row = r-1; row <= r+1; row++){
        for(col = c-1; col <= c+1; col++){
            // verify the bounds of the cell are valid
            if( (row == r && col == c) 
                || (row < 0 || col < 0) 
                || (row >= size || col >= size))
            {
                continue; // skip invalid neighbors
            }
            // cell checked contains a neighbor
            if(agents[row][col].preference != '.'){
                count++;
            }
            if(agents[row][col].preference == agents[r][c].preference){
                same_preference++;
            }
        }
    }
    // current happiness and satisfaction
    if (count != 0){
         agents[r][c].curr_happiness = ( (float)same_preference / (float)count) * 100;
    } else {
        // no neighbors at all = happy
        agents[r][c].curr_happiness = 100.00;
    }
    if (agents[r][c].curr_happiness >= threshold){
        agents[r][c].satisfied = true;
    } else {
        agents[r][c].satisfied = false;
    }
}


/**
* current cycle of the simulation
*/
int update_cycle(Agent agents[][size]){
    int num_moves = 0;
    prev_row = 0;
    prev_col = 0;
    vacant_row = 0;
    vacant_col= 0;
    
    for(int row = 0; row < size; row++){
        for(int col = 0; col < size; col++){
            if (agents[row][col].preference != '.'){
                agents[row][col].initially_vacant = false;
                update_agent(agents, row, col);
                // if agent isn't satisfied swap them with the next vacant cell.
                if (!agents[row][col].satisfied){
                    first_vacant_space(agents);
                    swap(&agents[row][col], &agents[vacant_row][vacant_col]);
                    // space is no longer able to be used in future
                    agents[vacant_row][vacant_col].initially_vacant = false;

                    prev_row = vacant_row;
                    prev_col = vacant_col;

                    num_moves++;
               }
            } else {
                agents[row][col].curr_happiness = 0.0;
            }
        }
    }
    return num_moves;
}


/**
* computes overall happiness
*/
float avg_happiness(Agent agents[][size], int remaining){
     float count = 0;
     if (remaining != 0){
         for(int row = 0; row < size; row++){
            for(int col = 0; col < size; col++){
                count += agents[row][col].curr_happiness;
             }
         }
     } 
     float avg_happiness = (count / (float)remaining);
     return avg_happiness;
}