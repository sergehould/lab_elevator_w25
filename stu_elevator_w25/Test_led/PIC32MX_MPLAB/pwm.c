/*
 *		pwm.c
 * 
 * 
 *		Serge Hould     June 28  2024       v1.0   Tested ok
 */
#include <xc.h>
#include "include/pwm.h"
#include "include/configuration.h"
#include "../../../../common/Tick_core.h"
#include <sys/attribs.h>

/* Initializes the Timer3 and OC for PWM */
/*  init timer 3 */
#define TMR_FREQ_MOT   4000 // 4 kHz
int PR3_value;
void pwm_init(void){
#ifndef  SIMULATION
    /* directions */
    TRISCbits.TRISC2 =0;  // door
    TRISDbits.TRISD9 =0; // level
    
    /* OCs */
    unsigned char fTimer3Initialized = T3CONbits.ON;
    if(!fTimer3Initialized)    {
        // configure Timer3
        T3CONbits.TCKPS = 0;    // 1:1 prescale value
        T3CONbits.TGATE = 0;    // not gated input (the default)
        T3CONbits.TCS = 0;      // PCBLK input (the default)
        T3CONbits.ON = 1;       // turn on Timer3
        PR3 = (int)((float)((float)SYS_FREQ /TMR_FREQ_MOT) + 0.5);   
        PR3_value= (int)((float)((float)SYS_FREQ /TMR_FREQ_MOT) + 0.5); 
    }
    // Configure Output Compare Module 4
   OC4CONbits.ON = 0;       // Turn off OC2 while doing setup.
   OC4CONbits.OCM = 6;      // PWM mode on OC2; Fault pin is disabled
   OC4CONbits.OCTSEL = 1;   // Timer3 is the clock source for this Output Compare module
   OC4CONbits.ON = 1;       // Start the OC2 module
   
    // Configure Output Compare Module 3
   OC3CONbits.ON = 0;       // Turn off OC3 while doing setup.
   OC3CONbits.OCM = 6;      // PWM mode on OC3; Fault pin is disabled
   OC3CONbits.OCTSEL = 1;   // Timer3 is the clock source for this Output Compare module
   OC3CONbits.ON = 1;       // Start the OC3 module
   //ja2 as OC3  
    RPC1R= 0b00001011;//p.146 datasheets
    //jb2 as OC4
    RPD11R=0b00001011;

   //__builtin_enable_interrupts();
#endif
}

#ifndef  SIMULATION
/* Sets the PWM. 			
 * The parameter takes a range from -10000 to +10000
 * A negative value represents a CCW direction
 * and a positive value represents a CW direction.
 * A value of 10000 represents a duty cycle of 100.00% CW direction
 * A value of 0 represents a duty cycle of 0.00%
 * A value of -10000 represents a duty cycle of 100.00% CCW direction
 */
int on_time_door=0;
void pwm_door_set(int on){   
            on_time_door=on;
            int on_time=0;
            // Duty Cycle(%)= (OcxR+1) *100/ (Prx+1)=
            if(on<0){
                //DIR = FORWARD;
                DIR_DOOR = REVERSE;
                on_time = -on;
            }
            else{
                DIR_DOOR = FORWARD;
                on_time = on;
            }
            /******* Caps the pwm value **********/
            if(on_time >PR3_value-1   ) on_time = PR3_value-1  ;
            else if(on_time  < 0 ) on_time=0;
            OC3RS = on_time ;
}
int on_time_level=0;
void pwm_level_set(int on){
            on_time_level=on;
            int on_time=0;
            // Duty Cycle(%)= (OcxR+1) *100/ (Prx+1)=
            if(on<0){
                //DIR = FORWARD;
                DIR_LEVEL = REVERSE;
                on_time = -on;
            }
            else{
                DIR_LEVEL = FORWARD;
                on_time = on;
            }
            /******* Caps the pwm value **********/
            if(on_time >PR3_value-1   ) on_time = PR3_value-1  ;
            else if(on_time  < 0 ) on_time=0;
            OC4RS = on_time ;
}

int pwm_door_get(){
    return on_time_door;
}
int pwm_level_get(){
    return on_time_level;
}
#endif