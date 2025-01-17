/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Digilent

  @File Name
    ssd.h

  @Description
    This file groups the declarations of the functions that implement
        the SSD library (defined in ssd.c).
        Include the file in the project when this library is needed.
        Use #include "ssd.h" in the source files where the functions are needed.
 */
/* ************************************************************************** */

#ifndef _SSD_H    /* Guard against multiple inclusion */
#define _SSD_H

 // SSD - Seven Segment Display

    // Cathods
    #define tris_SSD_CA     TRISGbits.TRISG12
    #define  lat_SSD_CA     LATGbits.LATG12

    #define tris_SSD_CB     TRISAbits.TRISA14
    #define  lat_SSD_CB     LATAbits.LATA14

    #define tris_SSD_CC     TRISDbits.TRISD6
    #define  lat_SSD_CC     LATDbits.LATD6

    #define tris_SSD_CD    TRISGbits.TRISG13
    #define  lat_SSD_CD     LATGbits.LATG13

    #define tris_SSD_CE     TRISGbits.TRISG15
    #define  lat_SSD_CE     LATGbits.LATG15

    #define tris_SSD_CF     TRISDbits.TRISD7
    #define  lat_SSD_CF     LATDbits.LATD7

    #define tris_SSD_CG     TRISDbits.TRISD13
    #define  lat_SSD_CG     LATDbits.LATD13

    #define tris_SSD_DP     TRISGbits.TRISG14
    #define  lat_SSD_DP     LATGbits.LATG14

    // Anods
    #define tris_SSD_AN0     TRISBbits.TRISB12
    #define lat_SSD_AN0      LATBbits.LATB12
    #define msk_SSD_AN0      1<<12
    #define ansel_SSD_AN0    ANSELBbits.ANSB12

    #define tris_SSD_AN1     TRISBbits.TRISB13
    #define lat_SSD_AN1      LATBbits.LATB13
    #define msk_SSD_AN1      1<<13
    #define ansel_SSD_AN1    ANSELBbits.ANSB13

    #define tris_SSD_AN2     TRISAbits.TRISA9
    #define lat_SSD_AN2      LATAbits.LATA9
    #define msk_SSD_AN2      1<<9


    #define tris_SSD_AN3     TRISAbits.TRISA10
    #define lat_SSD_AN3      LATAbits.LATA10
    #define msk_SSD_AN3      1<<10

    #define lat_SSD_AN23     LATA
    #define msk_SSD_AN23     (msk_SSD_AN2 | msk_SSD_AN3)
    #define prt_SSD_AN23     PORTA

    #define lat_SSD_AN01     LATB
    #define msk_SSD_AN01     (msk_SSD_AN0 | msk_SSD_AN1)
    #define prt_SSD_AN01     PORTB

    // *****************************************************************************
    // *****************************************************************************
    // Section: Interface Functions
    // *****************************************************************************
    // *****************************************************************************

void ssd_init();
unsigned char SSD_GetDigitSegments(unsigned char d);


#endif /* _SSD_H */

/* *****************************************************************************
 End of File
 */
