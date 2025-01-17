/*
 *		pv_measure.c
 * 
 *      Measure the position of the motor by reading the rotary encoder pulses.
 *      Implemented using an INT4 and INT3 interrupt.
 * 
 *		Serge Hould     June 28 2024       v1.0     interrupts tested ok   		
 */ 


#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/attribs.h>
//#include "peripheral/int.h"
#include "include/pv_measure.h"
#include "include/configuration.h"

#define     ENCA_DOOR        _RC4         
#define     ENCB_DOOR        _RG6   

#define     ENCA_LEVEL        _RD10         
#define     ENCB_LEVEL        _RD8  

#define     REDUCTION   256      // reduction ratio to reduce the overshoot

static inline int encState_door(void);
static void encoderTask(void);
//static void encoderTask2(void);

static int32_t pos_door=0;  // door position
static int32_t pos_level=0;  

/* Door */
void __ISR(_EXTERNAL_4_VECTOR, IPL2SOFT) ext_int4_isr(void){ 
//    if(ENCB_DOOR) pos_door++;
//    else pos_door--;
    
    static int cnt=0;
    if(ENCB_DOOR) {
        cnt++;
    }
    else cnt--;
    pos_door = cnt / REDUCTION ; // reduces encoder overshoot
    IFS0bits.INT4IF = 0; // Clear the interrupt flag 
}

/* Level */
void __ISR(_EXTERNAL_3_VECTOR, IPL2SOFT) ext_int3_isr(void){
//    if(ENCB_LEVEL) pos_level++;
//    else pos_level--;

    static int cnt=0;
    if(ENCB_LEVEL) {
        cnt++;
    }
    else cnt--;
    pos_level = cnt / REDUCTION ; // reduces encoder overshoot
    
    IFS0bits.INT3IF = 0; // Clear the interrupt flag 
}


/* returns the motor position in tics */

#ifndef  SIMULATION
/* In simulation mode, get_pv() is provided by DCMotor_model3.c */
/* In target mode, get_pv() is provided respectively by pv_measure.c */
//int get_pv(void){
//    return pos;
//}
//
//void set_pv(int pv){
//    pos =pv;
//}

/* Must be called at a constant period. e.g. every 10 mS*/
/* Reads the speed in RPM */
//int get_speed(void){
//    static int pos_mem;
//    int period, speed;
//    if(DIR == FORWARD)period = pos-pos_mem;
//    //if(pos) period = pos-pos_mem;
//    else period = pos_mem-pos;
//    pos_mem = pos;
//    speed = ((long)60* period *100)/64; // speed in RPM. Multiply by 100 for 1 second.
//    return speed;
////    if(speed != 0){
////        speed = 2000/speed;
////        return speed;
////    }else return 0;
//}
#endif



/* Initializes the INT3 and INT4 interrupts */
void position_init(void){
#ifndef  SIMULATION
    //JA3 - door channel A as int4
    INT4R = 0b00001010; //p.142 datasheets
    //JB3 - level channel A as int3
    INT3R = 0b00000011; 
    
    /* channel ports setting */
    TRISGbits.TRISG6 =1;
    ANSELGbits.ANSG6 = 0; // digital mode
    TRISDbits.TRISD8 =1;

    // Set up INT4 for negative edge triggering 
    IEC0bits.INT4IE = 0; // Disable INT
    IPC4bits.INT4IP = 2; // Set Interrupt for priority level 2 
    IPC4bits.INT4IS = 0; // Set Interrupt for sub-priority level 0 
    INTCONbits.INT4EP = 0; // Set for falling edge 
    IFS0bits.INT4IF = 0; // Clear the interrupt flag 
    IEC0bits.INT4IE = 1; // Enable 
    
    // Set up INT3 for negative edge triggering 
    IEC0bits.INT3IE = 0; // Disable INT
    IPC3bits.INT3IP = 2; // Set Interrupt for priority level 2 
    IPC3bits.INT3IS = 0; // Set Interrupt for sub-priority level 0 
    INTCONbits.INT3EP = 0; // Set for falling edge 
    IFS0bits.INT3IF = 0; // Clear the interrupt flag 
    IEC0bits.INT3IE = 1; // Enable 
    __builtin_enable_interrupts();   // enable interrupts
#endif
}


#ifndef  SIMULATION
/* In simulation mode, provided by DCMotor_model3.c */
/* In target mode, provided by pv_measure.c */
void door_set(int cnt){
    pos_door= cnt;
}

int door_get(void){
    return pos_door;
}

void level_set(int cnt){
    pos_level= cnt;
}

int level_get(void){
    return pos_level;
}
#endif
