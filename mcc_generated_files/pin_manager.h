/**
  @Generated Pin Manager Header File

  @Company:
    Microchip Technology Inc.

  @File Name:
    pin_manager.h

  @Summary:
    This is the Pin Manager file generated using MPLAB(c) Code Configurator

  @Description:
    This header file provides implementations for pin APIs for all pins selected in the GUI.
    Generation Information :
        Product Revision  :  MPLAB(c) Code Configurator - 4.15
        Device            :  PIC16F1823
        Version           :  1.01
    The generated drivers are tested against the following:
        Compiler          :  XC8 1.35
        MPLAB             :  MPLAB X 3.40

    Copyright (c) 2013 - 2015 released Microchip Technology Inc.  All rights reserved.

    Microchip licenses to you the right to use, modify, copy and distribute
    Software only when embedded on a Microchip microcontroller or digital signal
    controller that is integrated into your product or third party product
    (pursuant to the sublicense terms in the accompanying license agreement).

    You should refer to the license agreement accompanying this Software for
    additional information regarding your rights and obligations.

    SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
    EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
    MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
    IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
    CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
    OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
    INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
    CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
    SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
    (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.

*/


#ifndef PIN_MANAGER_H
#define PIN_MANAGER_H

#define INPUT   1
#define OUTPUT  0

#define HIGH    1
#define LOW     0

#define ANALOG      1
#define DIGITAL     0

#define PULL_UP_ENABLED      1
#define PULL_UP_DISABLED     0

// get/set Led2 aliases
#define Led2_TRIS               TRISAbits.TRISA4
#define Led2_LAT                LATAbits.LATA4
#define Led2_PORT               PORTAbits.RA4
#define Led2_WPU                WPUAbits.WPUA4
#define Led2_ANS                ANSELAbits.ANSA4
#define Led2_SetHigh()            do { LATAbits.LATA4 = 1; } while(0)
#define Led2_SetLow()             do { LATAbits.LATA4 = 0; } while(0)
#define Led2_Toggle()             do { LATAbits.LATA4 = ~LATAbits.LATA4; } while(0)
#define Led2_GetValue()           PORTAbits.RA4
#define Led2_SetDigitalInput()    do { TRISAbits.TRISA4 = 1; } while(0)
#define Led2_SetDigitalOutput()   do { TRISAbits.TRISA4 = 0; } while(0)
#define Led2_SetPullup()      do { WPUAbits.WPUA4 = 1; } while(0)
#define Led2_ResetPullup()    do { WPUAbits.WPUA4 = 0; } while(0)
#define Led2_SetAnalogMode()  do { ANSELAbits.ANSA4 = 1; } while(0)
#define Led2_SetDigitalMode() do { ANSELAbits.ANSA4 = 0; } while(0)

// get/set Led1 aliases
#define Led1_TRIS               TRISAbits.TRISA5
#define Led1_LAT                LATAbits.LATA5
#define Led1_PORT               PORTAbits.RA5
#define Led1_WPU                WPUAbits.WPUA5
#define Led1_SetHigh()            do { LATAbits.LATA5 = 1; } while(0)
#define Led1_SetLow()             do { LATAbits.LATA5 = 0; } while(0)
#define Led1_Toggle()             do { LATAbits.LATA5 = ~LATAbits.LATA5; } while(0)
#define Led1_GetValue()           PORTAbits.RA5
#define Led1_SetDigitalInput()    do { TRISAbits.TRISA5 = 1; } while(0)
#define Led1_SetDigitalOutput()   do { TRISAbits.TRISA5 = 0; } while(0)
#define Led1_SetPullup()      do { WPUAbits.WPUA5 = 1; } while(0)
#define Led1_ResetPullup()    do { WPUAbits.WPUA5 = 0; } while(0)

// get/set Input1 aliases
#define Input1_TRIS               TRISCbits.TRISC0
#define Input1_LAT                LATCbits.LATC0
#define Input1_PORT               PORTCbits.RC0
#define Input1_WPU                WPUCbits.WPUC0
#define Input1_ANS                ANSELCbits.ANSC0
#define Input1_SetHigh()            do { LATCbits.LATC0 = 1; } while(0)
#define Input1_SetLow()             do { LATCbits.LATC0 = 0; } while(0)
#define Input1_Toggle()             do { LATCbits.LATC0 = ~LATCbits.LATC0; } while(0)
#define Input1_GetValue()           PORTCbits.RC0
#define Input1_SetDigitalInput()    do { TRISCbits.TRISC0 = 1; } while(0)
#define Input1_SetDigitalOutput()   do { TRISCbits.TRISC0 = 0; } while(0)
#define Input1_SetPullup()      do { WPUCbits.WPUC0 = 1; } while(0)
#define Input1_ResetPullup()    do { WPUCbits.WPUC0 = 0; } while(0)
#define Input1_SetAnalogMode()  do { ANSELCbits.ANSC0 = 1; } while(0)
#define Input1_SetDigitalMode() do { ANSELCbits.ANSC0 = 0; } while(0)

// get/set RC1 procedures
#define RC1_SetHigh()    do { LATCbits.LATC1 = 1; } while(0)
#define RC1_SetLow()   do { LATCbits.LATC1 = 0; } while(0)
#define RC1_Toggle()   do { LATCbits.LATC1 = ~LATCbits.LATC1; } while(0)
#define RC1_GetValue()         PORTCbits.RC1
#define RC1_SetDigitalInput()   do { TRISCbits.TRISC1 = 1; } while(0)
#define RC1_SetDigitalOutput()  do { TRISCbits.TRISC1 = 0; } while(0)
#define RC1_SetPullup()     do { WPUCbits.WPUC1 = 1; } while(0)
#define RC1_ResetPullup()   do { WPUCbits.WPUC1 = 0; } while(0)
#define RC1_SetAnalogMode() do { ANSELCbits.ANSC1 = 1; } while(0)
#define RC1_SetDigitalMode()do { ANSELCbits.ANSC1 = 0; } while(0)

// get/set RC2 procedures
#define RC2_SetHigh()    do { LATCbits.LATC2 = 1; } while(0)
#define RC2_SetLow()   do { LATCbits.LATC2 = 0; } while(0)
#define RC2_Toggle()   do { LATCbits.LATC2 = ~LATCbits.LATC2; } while(0)
#define RC2_GetValue()         PORTCbits.RC2
#define RC2_SetDigitalInput()   do { TRISCbits.TRISC2 = 1; } while(0)
#define RC2_SetDigitalOutput()  do { TRISCbits.TRISC2 = 0; } while(0)
#define RC2_SetPullup()     do { WPUCbits.WPUC2 = 1; } while(0)
#define RC2_ResetPullup()   do { WPUCbits.WPUC2 = 0; } while(0)
#define RC2_SetAnalogMode() do { ANSELCbits.ANSC2 = 1; } while(0)
#define RC2_SetDigitalMode()do { ANSELCbits.ANSC2 = 0; } while(0)

// get/set RC5 procedures
#define RC5_SetHigh()    do { LATCbits.LATC5 = 1; } while(0)
#define RC5_SetLow()   do { LATCbits.LATC5 = 0; } while(0)
#define RC5_Toggle()   do { LATCbits.LATC5 = ~LATCbits.LATC5; } while(0)
#define RC5_GetValue()         PORTCbits.RC5
#define RC5_SetDigitalInput()   do { TRISCbits.TRISC5 = 1; } while(0)
#define RC5_SetDigitalOutput()  do { TRISCbits.TRISC5 = 0; } while(0)
#define RC5_SetPullup()     do { WPUCbits.WPUC5 = 1; } while(0)
#define RC5_ResetPullup()   do { WPUCbits.WPUC5 = 0; } while(0)

#define LedEn_SetHigh()            do { LATCbits. LATC4 = 1; } while(0)
#define LedEn_SetLow()             do { LATCbits.LATC4 = 0; } while(0)
/**
   @Param
    none
   @Returns
    none
   @Description
    GPIO and peripheral I/O initialization
   @Example
    PIN_MANAGER_Initialize();
 */
void PIN_MANAGER_Initialize (void);

/**
 * @Param
    none
 * @Returns
    none
 * @Description
    Interrupt on Change Handling routine
 * @Example
    PIN_MANAGER_IOC();
 */
void PIN_MANAGER_IOC(void);



#endif // PIN_MANAGER_H
/**
 End of File
*/