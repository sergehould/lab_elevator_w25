/* ************************************************************************** */
/** Descriptive File Name

  @File Name
    ssd.c

  @Description
        This file groups the functions that implement the SSD library.
        This library deals with seven segment display modules.
        The library provides functions for setting the information to be displayed.
        Include the file in the project, together with iniBoard.h, when this library is needed.	
 * 
 * DO NOT MODIFY THIS FILE!	
 */

/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
#include <xc.h>
#include <sys/attribs.h>
#include "include/initBoard.h"
#include "include/ssd.h"


/* ************************************************************************** */
// the array that contains the LEDs configuration for each value to be displayed
// Bit 0 corresponds to SSD_CA, 0 for segment ON, 1 for segment off
// Bit 1 corresponds to SSD_CB, 0 for segment ON, 1 for segment off
// Bit 2 corresponds to SSD_CC, 0 for segment ON, 1 for segment off
// Bit 3 corresponds to SSD_CD, 0 for segment ON, 1 for segment off
// Bit 4 corresponds to SSD_CE, 0 for segment ON, 1 for segment off
// Bit 5 corresponds to SSD_CF, 0 for segment ON, 1 for segment off
// Bit 6 corresponds to SSD_CG, 0 for segment ON, 1 for segment off
// Bit 7 corresponds to SSD_CH, 0 for segment ON, 1 for segment off
const unsigned char digitSegments[]= {
    0b1000000, // 0
    0b1111001, // 1
    0b0100100, // 2
    0b0110000, // 3
    0b0011001, // 4
    0b0010010, // 5
    0b0000010, // 6
    0b1111000, // 7
    0b0000000, // 8
    0b0010000, // 9
    0b0001000, // A
    0b0000011, // b
    0b1000110, // C
    0b0100001, // d
    0b0000110, // E
    0b0001110, // F
    0b0001001, // H 
    0b0111111, // 17 -
    0b1100011, // 18 down arrow 1 
    0b1011100, // 19 up arrow 1
    0b1111111, // 20 off 
    0b0011101, // 21 down arrow 2 
    0b0101011  // 22 up arrow 2 
            
};



/* ------------------------------------------------------------ */
/***	SSD_ConfigurePins
**
**	Parameters:
**		
**
**	Return Value:
**		
**
**	Description:
**		This function configures the IO pins involved in the SSD module as digital output pins. 
**      The function uses pin related definitions from config.h file.
**      This is a low-level function called by SSD_Init(), so user should avoid calling it directly.       
**          
*/
void SSD_ConfigurePins()
{
    // set pins as digital outputs.
    tris_SSD_CA = 0;
    tris_SSD_CB = 0;
    tris_SSD_CC = 0;
    tris_SSD_CD = 0;  
    tris_SSD_CE = 0;
    tris_SSD_CF = 0;
    tris_SSD_CG = 0;
    tris_SSD_DP = 0;  
    
    tris_SSD_AN0 = 0;
    tris_SSD_AN1 = 0;
    tris_SSD_AN2 = 0;
    tris_SSD_AN3 = 0;    
    
    // disable analog (set pins as digital))    
    ansel_SSD_AN0 = 0;
    ansel_SSD_AN1 = 0;

    PMCONbits.ON = 0;   // turn PM off
}

/* ************************************************************************** */
/* ************************************************************************** */
// Section: Local Functions                                                   */
/* ************************************************************************** */
/* ************************************************************************** */

/* ------------------------------------------------------------ */
/***	SSD_Init
**
**	Parameters:
**		
**
**	Return Value:
**		
**
**	Description:
**		This function initializes the hardware involved in the SSD module:
**      The pins corresponding to SSD module are initialized as digital outputs.
**      The Timer1 is initialized to generate interrupts every approx. 3 ms.
**      
**          
*/
void ssd_init()
{
    SSD_ConfigurePins();
}

/* ------------------------------------------------------------ */
/***	SSD_GetDigitSegments
**
**	Parameters:
**		unsigned char d - the value to be represented on a digit of SSD. Should be between 0 and 22:
**                               0 -  9     - corresponds to 0-9 letter
**                              10 - 15     - corresponds to A-F letters
**                              16          - corresponds to H letter
**                              17 - 22     - corresponds to special characters
**
**	Return Value:
**       unsigned char - value containing segments configuration. Bits 0-6 correspond to the seven segments:
**                          Bit 0 corresponds to SSD_CA, 0 for segment ON, 1 for segment off
**                          Bit 1 corresponds to SSD_CB, 0 for segment ON, 1 for segment off
**                          Bit 2 corresponds to SSD_CC, 0 for segment ON, 1 for segment off
**                          Bit 3 corresponds to SSD_CD, 0 for segment ON, 1 for segment off
**                          Bit 4 corresponds to SSD_CE, 0 for segment ON, 1 for segment off
**                          Bit 5 corresponds to SSD_CF, 0 for segment ON, 1 for segment off
**                          Bit 6 corresponds to SSD_CG, 0 for segment ON, 1 for segment off
**
**	Description:
**		This function returns the byte containing the segments configuration for the digit received as parameter.
**      The segments configuration digits are defined in digitSegments.
**          
*/
unsigned char SSD_GetDigitSegments(unsigned char d)
{
    return digitSegments[d];
}

/* *****************************************************************************
 End of File
 */
