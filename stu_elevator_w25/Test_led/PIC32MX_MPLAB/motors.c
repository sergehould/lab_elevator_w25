/*
 *	motors.c
 * 
 *  Description: 
 *      Contains the code for the motors and the encoders. It uses multiple 
 *      internal and external interrupts. Also, it displays the motor and 
 *      encoder states on UART3 or LCD depending on the mode.
 * 
 *	Serge Hould     June 28 2024       v1.0      	
 * 
 *  DO NOT MODIFY THIS FILE!	
 */ 

#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/attribs.h>
//#include "peripheral/int.h"
#include "include/configuration.h"
#include "include/motors.h"
#include "include/util.h"
#include "include/console.h"
#include "include/Tick_core.h"



static int32_t pos_door=0;  // door position
static int32_t pos_level=0;  

/**************** encoder section ***********************/
#define     ENCA_DOOR        _RC4         
#define     ENCB_DOOR        _RG6   

#define     ENCA_LEVEL        _RD10         
#define     ENCB_LEVEL        _RD8  

#define     REDUCTION   256      // reduction ratio to reduce motor overshoot

/* Sets the encoder value for the door */
void encoder_door_set(int cnt){
    pos_door= cnt;
}
/* Returns the current encoder value for the door*/
int encoder_door_get(void){
    return pos_door;
}

/* Sets the encoder value for the level*/
void encoder_level_set(int cnt){
    pos_level= cnt;
}

/* Returns the current encoder value for the level*/
int encoder_level_get(void){
    return pos_level;
}

/* print from isr function*/
static void printUart3FromISR(char *str){
    while(*str != '\0'){
        while(U3STAbits.TRMT == 0);
        U3TXREG = *str; 
        str++;
    }
}


/* Function that initializes timer2 */
static void timer2_init( void){
// Initialize and enable Timer2
    T2CONbits.TON = 0; // Disable Timer
    T2CONbits.TCS = 0; // Select internal instruction cycle clock
    T2CONbits.TGATE = 0; // Disable Gated Timer mode
    //    TCKPS<2:0>: Timer Input Clock Prescale Select bits(3)
    //111 = 1:256 prescale value
    //110 = 1:64 prescale value
    //101 = 1:32 prescale value
    //100 = 1:16 prescale value
    //011 = 1:8 prescale value
    //010 = 1:4 prescale value
    //001 = 1:2 prescale value
    //000 = 1:1 prescale value
    //T2CONbits.TCKPS = 0b111; // Select 1:256 Prescaler
    //T2CONbits.TCKPS = 0b110; // Select 1:64 Prescaler
    //T2CONbits.TCKPS = 0b011; // Select 1:8 Prescaler
//#if defined(NO_MOTORS) || defined(SIMULATION) 
#if defined(SIMULATION) 
    T2CONbits.TCKPS = 0b011; // Select  Prescaler
    PR2=50000; // Load the period value for 100 Hz
//        T2CONbits.TCKPS = 0b101; // Select  Prescaler
//        PR2=62500; // Load the period value for 20 Hz
#else // 2.5 Hz
    T2CONbits.TCKPS = 0b111; // Select  Prescaler
    PR2=62500; // Load the period value 
#endif
    TMR2 = 0x00; // Clear timer register
    __builtin_disable_interrupts();   // disable interrupts
    IPC2bits.T2IP = 0x01; // Set Timer 2 Interrupt Priority Level
    IFS0bits.T2IF = 0; // Clear Timer 2 Interrupt Flag
    INTCONbits.MVEC=1;
    IEC0bits.T2IE = 1; // Enable Timer 2 interrupt
   // _T2IP = 6; // Enable Timer 2 interrupt
    __builtin_enable_interrupts();   // enable interrupts
    T2CONbits.TON = 1; // Start Timer              
} // init

char spin[]={'\\', '|', '/', '-'}; // spinning motor characters

#define OVERSHOOT  2 // overshoot - the number of tics that the motor goes past when turned off
/* Periodically called to display values. */ 
/* In simulation mode, the encoder values are updated at 100 Hz*/
/* in target mode the display is updated at 2.5 Hz */
void __ISR( _TIMER_2_VECTOR, IPL1SOFT) T2InterruptHandler( void){
    char buff[100];
    static int cnt1=0, cnt2=0, door_moving=0, level_moving=0, lev_cnt=0, dr_cnt, pos_door_mem,  pos_level_mem;
    static int lev_cnt_mem=-100, dr_cnt_mem =-100;
#if defined(NO_MOTORS) || defined(SIMULATION) 
    if(motor_level_get()<0){ // motor CCW
        pos_level--;
        level_moving=-1;
        lev_cnt = --lev_cnt < 0  ? lev_cnt=3 : lev_cnt; 
    }        
    else if (motor_level_get()>0){ // motor CW
        pos_level++;
        level_moving=1;
        lev_cnt = ++lev_cnt >3  ? lev_cnt=0 : lev_cnt; 
    }
    else { // overshoot simulated when motor stopped
        if(level_moving !=0){
            if(cnt1++ < OVERSHOOT) {
                if(level_moving >0)pos_level++;
                else pos_level--;
            }
            else{
                level_moving=0;
                cnt1=0;
            }
        }
    }

    if(motor_door_get() <0) {
        pos_door--;
        door_moving =-1;
        dr_cnt = --dr_cnt < 0  ? dr_cnt=3 : dr_cnt; 
    }
    else if (motor_door_get() >0){
        pos_door++;
        door_moving =1;
        dr_cnt = ++dr_cnt > 3  ? dr_cnt=0 : dr_cnt; 
    }
    else { // overshoot simulated
        if(door_moving !=0){
            if(cnt2++ < OVERSHOOT) {
                if(door_moving >0)pos_door++;
                else pos_door--;
            }
            else{
                door_moving=0;
                cnt2=0;
            }
        }
    }
#endif
    T2CONbits.TON = 0;
#ifdef SIMULATION
    if(pos_door != pos_door_mem){ // displays only if position changes
        printUart3FromISR("Door  ");
        putc3(spin[dr_cnt]);
        putc3(' ');putc3(' ');
        itoa2(pos_door, buff, 10);
        printUart3FromISR(buff);
        putc3('\n');
    }
    if(pos_level != pos_level_mem){
        printUart3FromISR("Level ");
        putc3(spin[lev_cnt]);
        putc3(' ');putc3(' ');    
        itoa2(pos_level, buff, 10);
        printUart3FromISR(buff);
        putc3('\n');
    }


#else
#ifndef NO_MOTORS
        if(motor_door_get() >0) {
           dr_cnt = --dr_cnt < 0  ? dr_cnt=3 : dr_cnt; 
        }
        else if (motor_door_get() <0){
           dr_cnt = ++dr_cnt > 3  ? dr_cnt=0 : dr_cnt; 
        }
    	if(motor_level_get()>0){
            lev_cnt = --lev_cnt < 0  ? lev_cnt=3 : lev_cnt; 
        }        
        else if (motor_level_get()<0){
            lev_cnt = ++lev_cnt >3  ? lev_cnt=0 : lev_cnt; 
        }
#endif
        //if(pos_door != pos_door_mem){
    if((lev_cnt != lev_cnt_mem) || (dr_cnt != dr_cnt_mem)){
           lev_cnt_mem= lev_cnt;
           dr_cnt_mem = dr_cnt;
           LCDPos1(0);
           LCDPutString("Door   ");
           /* rotating bar */
           if(dr_cnt ==0){
               LCDPos1(7);
               LCD_WriteDataByte(1);// cgram pipe
           }
           else if(dr_cnt ==1)LCDPut('/');
           else if(dr_cnt ==2)LCDPut('-');
           else{;
               LCDPos1(7);
               LCD_WriteDataByte(0); // cgram /
           }
           LCDPos1(10);
           itoa2(pos_door, buff, 10);
           LCDPutString(buff);
           LCDPutString("      ");
       // }
    //if(pos_level != pos_level_mem){
        LCDPos2(0);
        LCDPutString("Level  ");
        /* rotating bar */
        if(lev_cnt ==0){
            LCDPos2(7);
            LCD_WriteDataByte(1);
        }
        else if(lev_cnt ==1)LCDPut('/');
        else if(lev_cnt ==2)LCDPut('-');
        else{
            LCDPos2(7);
            LCD_WriteDataByte(0);
        }
        LCDPos2(10);
        itoa2(pos_level, buff, 10);
        LCDPutString(buff);
        LCDPutString("      ");
    }
    //}
#endif
    /* memorizes latest value */
    pos_door_mem= pos_door ;
    pos_level_mem= pos_level ;
    T2CONbits.TON = 1;    
	IFS0bits.T2IF = 0; // Clear Timer2 Interrupt
}

#ifdef SIMULATION

#else 
/* Door encoder isr  */
void __ISR(_EXTERNAL_4_VECTOR, IPL2SOFT) ext_int4_isr(void){ 
    static int cnt=0;
    if(ENCB_DOOR) {
        cnt++;
    }
    else cnt--;
    pos_door = cnt / REDUCTION ; // reduces encoder overshoot
    IFS0bits.INT4IF = 0; // Clear the interrupt flag 
}

/* Level encoder isr  */
void __ISR(_EXTERNAL_3_VECTOR, IPL2SOFT) ext_int3_isr(void){
    static int cnt=0;
    if(ENCB_LEVEL) {
        cnt++;
    }
    else cnt--;
    pos_level = cnt / REDUCTION ; // reduces encoder overshoot
    
    IFS0bits.INT3IF = 0; // Clear the interrupt flag 
}
/* Initializes the INT3 and INT4 encoder interrupts */
static void interrupt_init(void){
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
}
#endif

/************* pwm section ************************/
#define REVERSE         0
#define FORWARD         1
#define DIR_DOOR             LATCbits.LATC2  //dir
#define DIR_LEVEL            LATDbits.LATD9  //dir

/* local functions */
static void pwm_door_set(int);
static void pwm_level_set(int);
static int pwm_door_get();
static int pwm_level_get();

/* local variables */
static int on_time_door=0;
static int on_time_level=0;

/* Sets the door motor state    */
/* dir = -1 closes the door         */
/* dir = 1  opens  the door         */
/* dir = 0  stops the door          */
void motor_door_set(int dir){
    if (dir > 0) pwm_door_set(-4000);// opens door
    else if (dir < 0) pwm_door_set(4000);// closes door
    else pwm_door_set(0);
}

/* Returns the door motor state         */
/* returns -1 when the door is closing      */
/* returns 1 when the door is opening       */
/* returns 0 when the door is stopped       */
int motor_door_get(void){
    if(on_time_door > 0) return -1;
    if(on_time_door < 0) return 1;
    else return 0;
}

/* Sets the level motor state   */
/* dir = 1 goes up                 */
/* dir = -1  goes down               */
/* dir = 0  stops it                */
void motor_level_set(int dir){
    if(dir > 0) pwm_level_set(-5000); // up
    else if(dir<0) pwm_level_set(5000); // dwn
    else pwm_level_set(0); 
}

/* Returns the level motor state         */
/* returns -1 when moving down      */
/* returns 1 when moving up       */
/* returns 0 when stopped       */
int motor_level_get(void){
    if(on_time_level > 0) return -1;
    if(on_time_level < 0) return 1;
    else return 0;
}


#ifdef SIMULATION

static void pwm_door_set(int on){   
            on_time_door=on;
}

static void pwm_level_set(int on){
            on_time_level=on;
}

void motors_init(void){
    // empty in simulation mode
}
#elif defined(NO_MOTORS)
    static void pwm_door_set(int on){   
                on_time_door=on;
    }

    static void pwm_level_set(int on){
                on_time_level=on;
    }

    void motors_init(void){
        // empty in simulation mode
    }
#else 
/* Initializes the Timer3 and OC for PWM */
/*  init timer 3 */
#define TMR_FREQ_MOT   4000 // 4 kHz
int PR3_value;
void motors_init(void){
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

}

/* Sets the PWM. 			
 * The parameter takes a range from -10000 to +10000
 * A negative value represents a CCW direction
 * and a positive value represents a CW direction.
 * A value of 10000 represents a duty cycle of 100.00% CW direction
 * A value of 0 represents a duty cycle of 0.00%
 * A value of -10000 represents a duty cycle of 100.00% CCW direction
 */

static void pwm_door_set(int on){   
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

static void pwm_level_set(int on){
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


#endif

static char backslash[] = {0,16,8,4,2,1,0,0,0}; // backslash character
static char pipe[] = {0,4,4,4,4,4,0,0,0};// short pipe character
void encoders_init(void){
    
#ifndef NO_MOTORS
    #ifndef  SIMULATION
        interrupt_init();
    #endif
#endif

//#if !defined(NO_MOTORS) || !defined(SIMULATION) 
//        interrupt_init();
//#endif
#ifdef  SIMULATION
    uart3_init(115200);   // for simulation
    //stdio_set(C_UART3);
#else
    lcd_init();
    stdio_set(C_LCD);
#endif
    LCD_WriteBytesAtPosCgram(pipe,8, 8); // cgram pipe character
    LCD_WriteBytesAtPosCgram(backslash,8, 0); // cgram backslah character
    timer2_init();
}
