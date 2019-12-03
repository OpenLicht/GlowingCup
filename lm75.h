/*
 * lm75.h
 *
 *  Created on: Oct 27, 2017
 *      Author: Tobias
 */

#ifndef LM75_H_
#define LM75_H_

#include <DAVE.h>


#define TEMP_ADDRESS_W_N 0x90
#define TEMP_ADDRESS_W_SW 0x98
#define TEMP_ADDRESS_W_SE 0x94

#define TEMP_ADDRESS_T 0x00

unsigned int lm75();

#endif /* LM75_H_ */
