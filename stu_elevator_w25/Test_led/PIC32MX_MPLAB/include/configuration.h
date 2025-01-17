/* 
 * File:   configuration.h
 * Description:
 *  Includes all common macros needed by many files
 *  Includes the important macro to swap from simulation mode to target mode and vis-versa.
 * 
 * Author: sh
 *
 * SH  June 2024     v1.0             
 */

#ifndef CONFIGURATION_H
#define	CONFIGURATION_H

/* Macro to swap from simulation mode to target mode and vis-versa */
/* To run target mode, comment out this line.                        */
#define SIMULATION

/* Macro to use or not real motors. When enabled,the LCD emulates the motors */
/* To run with real motors, comment out this line. */
//#define NO_MOTORS

/* When using the debugger, 2 push buttons cannot be used */
/* They are replaced by 2 switches */
/* To run with switches, keep this line enabled. */
/* To run with push buttons, comment out this line. */
#define DEBUG


#endif	/* CONFIGURATION_H */

