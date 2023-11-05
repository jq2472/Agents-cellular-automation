/**
* agent.h - data structure for an agent cell
*
* @author Jolin Qiu
*/

#ifndef _AGENT_H_
#define _AGENT_H_

#include <stdio.h>
#include <stdint.h>

/**
* Agent data structure
*/

typedef
    struct agent {
        char preference;            	// endline or newline programing style preference
        bool satisfied;            	// is the agent satisfied?
        float curr_happiness;     	//current happiness
        bool initially_vacant;		// was the space initially vacant.
    }
        Agent;

/**
* Public functions
*/

/**
* new_agent() - create a new agent
*
* @param preference     	endline/newline agent
* @param satisfied      	whether or not the agent is satisifed
* @param curr_happiness 	the current happiness level of the agent
* @param initially_vacant	whether or not space was initially vacant
*/

Agent new_agent( char preference, bool satisfied, float curr_happiness, bool initially_vacant);

#endif
