/**
 * agent.c - agent cell processing routines
 *
 * @author Jolin Qiu
 */

#include <stdlib.h>
#include <stdbool.h>
#include "agent.h"	

//to compute the happiness level
int num_newline;
int num_endline;
int total_neighbors;

/**
* Public functions
*/

/**
 * new_agent() - create a new agent
 */

Agent new_agent( char preference, bool satisfied, float curr_happiness, bool initially_vacant) {
    Agent new;
       new.preference = preference;
       new.satisfied = satisfied;
       new.curr_happiness = curr_happiness;
       new.initially_vacant = initially_vacant;

    return( new );
}
