/* 
 * File:   DCMotor_mode.h
 * Author: sh
 *
 * Created on December 10, 2020, 7:49 AM
 *
 * SH      July 2022            Add   print_pos() and print_pwm()  
 * SH      October 2022         Add   #ifdef  SIMULATION 
 *           
*/  

#ifndef DCMOTOR_MODEL_H
#define	DCMOTOR_MODEL_H

#include "configuration.h"

#ifdef  SIMULATION
int32_t get_pv(void);
unsigned int adc_read( int);
void set_pwm(int);
void pwm_door_set(int);
void pwm_level_set(int);
int pwm_door_get();
int pwm_level_get();
int32_t door_get(void);
void door_set(int pv);
int32_t level_get(void);
void level_set(int pv);
void timer2_init( void);
#endif
void print_bar(int con, int val, char character, int centre, int range);
void plot_two_int32(int con, int val1, int val2, int centre, int range);


#endif	/* DCMOTOR_MODE_H */

