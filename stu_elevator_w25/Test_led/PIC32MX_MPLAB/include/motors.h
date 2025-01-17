/* 
 * File:   
 * Author: sh
 *
 */

#ifndef MOTORS_H
#define	MOTORS_H

#include <xc.h>
#include <stdint.h>
#include "configuration.h"

/* Initializes both motors */
void motors_init(void);

/* Sets the door motor state    */
/* dir = -1 closes the door         */
/* dir = 1  opens  the door         */
/* dir = 0  stops the door          */
void motor_door_set(int dir);

/* Returns the door motor state         */
/* returns -1 when the door is closing      */
/* returns 1 when the door is opening       */
/* returns 0 when the door is stopped       */
int motor_door_get(void);

/* Sets the level motor state   */
/* dir = -1 goes up                 */
/* dir = 1  goes down               */
/* dir = 0  stops it                */
void motor_level_set(int dir);

/* Returns the level motor state         */
/* returns -1 when moving down      */
/* returns 1 when moving up       */
/* returns 0 when stopped       */
int motor_level_get(void);

/* Initializes both encoders */
void encoders_init(void);

/* Sets the encoder value for the door */
void encoder_door_set(int cnt);

/* Returns the current encoder value for the door*/
int encoder_door_get(void);

/* Sets the encoder value for the level*/
void encoder_level_set(int cnt);

/* Returns the current encoder value for the level*/
int encoder_level_get(void);

#endif	

