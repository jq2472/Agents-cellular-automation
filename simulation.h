/**
* simulation.h - functions that handle the movements of agents as
* they seek the satisfaction of being close to neighbors
* who like the same brace placement.
*
* @author Jolin Qiu
*/

#ifndef _SIMULATION_H_
#define _SIMULATION_H_

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "agent.h" 	// the agent data structure
int size; 	// dimensions of the square grid
int threshold; 	// threshold to be happy


/**
* swap() - swaps positions of two agents
*/
void swap(Agent *e, Agent *n);


/**
 * update_agent(): checks bounds of the current agent cell 
 * and updates the current attributes of the agent cell depending on its neighbors.
 * @param
 *      the agents grid to reference neighbors
 *      the current row
 *      the current column
 **/
void update_agent(Agent agents[][size], int r, int c);

/**
* update_cycle() - unsatisfied neighbors are moved to valid spaces
* 
* @param agent	
* @return the number of moves made
*/
int update_cycle(Agent agents[][size]);


/**
* avg_happiness() - calculates the current happiness of an agent.
* The current happiness is the ratio of occupied neighbors with the same preference
* : the total number of OCCUPIED neighbors.
* If the agent's current happiness is ≥ the preference threshold,
* the agent is satisfied; otherwise, the agent is dissatisfied.
* There is one special case: if an agent has no occupied neighbors,
* we consider that agent to be satisfied.
* 
* @param agent		agent grid
* @param vacant		number of vacant spaces
* @return	happiness value
*/
float avg_happiness(Agent agents[][size], int vacant);

#endif
