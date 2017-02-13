/**
  Generated Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This is the main file generated using MPLAB(c) Code Configurator

  Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  MPLAB(c) Code Configurator - 4.15
        Device            :  PIC16F1823
        Driver Version    :  2.00
    The generated drivers are tested against the following:
        Compiler          :  XC8 1.35
        MPLAB             :  MPLAB X 3.40
*/

/*
    (c) 2016 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/

#include "mcc_generated_files/mcc.h"
#include "mcc_generated_files/mtouch/mtouch_button.h"

volatile uint8_t ticks;

const uint16_t expTable[] = {0 ,0 ,1 ,1 ,2 ,2 ,3 ,4 ,5 ,6 ,8 ,9 ,11 ,12 ,14 ,16 ,18 ,20 ,23 ,25
 ,28 ,30 ,33 ,36 ,39 ,42 ,46 ,49 ,53 ,56 ,60 ,64 ,68 ,72 ,77 ,81 ,86 ,90 ,95 ,100 ,105 ,110 ,116
 ,121 ,127 ,132 ,138 ,144 ,150 ,156 ,163 ,169 ,176 ,182 ,189 ,196 ,203 ,210 ,218 ,225 ,233 ,240
 ,248 ,256 ,264 ,272 ,281 ,289 ,298 ,306 ,315 ,324 ,333 ,342 ,352 ,361 ,371 ,380 ,390 ,400 ,410
 ,420 ,431 ,441 ,452 ,462 ,473 ,484 ,495 ,506 ,518 ,529 ,541 ,552 ,564 ,576 ,588 ,600 ,613 ,625
 ,638 ,650 ,663 ,676 ,689 ,702 ,716 ,729 ,743 ,756 ,770 ,784 ,798 ,812 ,827 ,841 ,856 ,870 ,885
 ,900 ,915 ,930 ,946 ,961 ,977 ,992 ,1008 ,1023
};

#define MAX_PWM_PERIOD (sizeof(expTable) / sizeof(uint16_t) - 1)

/*
                         Main application
 */
void main(void)
{
    mtouch_buttonmask_t bs, buttonsState = 0;
    bool b = false;
    uint8_t tBtnUp, tBtnDown;
    uint16_t pwmDutyPeriod = MAX_PWM_PERIOD / 2;
    
    ticks = 0;
    // initialize the device
    SYSTEM_Initialize();

//    Led1_SetLow();
//    Led2_SetHigh();
//    for(bs = 0; bs < 10; bs++) {
//        __delay_ms(100);
//        Led1_Toggle();
//        Led2_Toggle();
//    }
    
//    TMR2_SetInterruptHandler(timerCallback);
    // When using interrupts, you need to set the Global and Peripheral Interrupt Enable bits
    // Use the following macros to:

    // Enable the Global Interrupts
    INTERRUPT_GlobalInterruptEnable();

    // Enable the Peripheral Interrupts
    INTERRUPT_PeripheralInterruptEnable();

    // Disable the Global Interrupts
    //INTERRUPT_GlobalInterruptDisable();

    // Disable the Peripheral Interrupts
    //INTERRUPT_PeripheralInterruptDisable();

//    printf("---- Starting main loop ----");
    Led1_SetLow();
    Led2_SetLow();
    while (1)
    {
        // TODO add some error indication
        if(MTOUCH_Service_Mainloop() == false)
            continue;
        
        // save buttons state for press/release detection
        bs = buttonsState;
        buttonsState = MTOUCH_Button_Buttonmask_Get();
//        if(MTOUCH_Button_isInitialized(ButtonUp)) {
//            if(MTOUCH_Button_isPressed(ButtonUp)) {
//                Led1_SetHigh();
//                buttonsState |= 1 << ButtonUp;
//            } else {
//                Led1_SetLow();
//                buttonsState &= ~(1 << ButtonUp);
//            }
//        }
//        if(MTOUCH_Button_isInitialized(ButtonDown)) {
//            if(MTOUCH_Button_isPressed(ButtonDown)) {
//                Led2_SetHigh();
//                buttonsState |= 1 << ButtonDown;
//            } else {
//                Led2_SetLow();
//                buttonsState &= ~(1 << ButtonDown);
//            }
//        }
        
        // check for change in button up state
//        if((bs ^ buttonsState) & (1 << ButtonUp)) {
//            // timer snapshot if just pressed
//            if(bs & (1 << ButtonUp)) {
//                tBtnUp = ticks;
//                // start increasing brightness
//                if(pwmDutyPeriod < MAX_PWM_PERIOD)
//                    pwmDutyPeriod++;
//                EPWM_LoadDutyValue(expTable[pwmDutyPeriod]);
//            }
//        } else 
        if(buttonsState & (1 << ButtonUp)) {
            Led1_SetHigh();
        } else {
            Led1_SetLow();
        }
        
        if(buttonsState & (1 << ButtonDown)) {
            Led2_SetHigh();
        } else {
            Led2_SetLow();
        }
        
        if(buttonsState & (1 << ButtonUp) && (tBtnUp != ticks)) {
            // button kept pressed, and tick elapsed
            tBtnUp = ticks;
            if(pwmDutyPeriod < MAX_PWM_PERIOD)
                pwmDutyPeriod++;
            EPWM_LoadDutyValue(expTable[pwmDutyPeriod]);
        }
        
        // check for change in button down state
//        if((bs ^ buttonsState) & (1 << ButtonDown)) {
//            // timer snapshot if just pressed
//            if(bs & (1 << ButtonDown)) {
//                tBtnDown = ticks;
//                // start decreasing brightness
//                if(pwmDutyPeriod > 0)
//                    pwmDutyPeriod--;
//                EPWM_LoadDutyValue(expTable[pwmDutyPeriod]);
//            }
//        } else 
        if(buttonsState & (1 << ButtonDown) && (tBtnDown != ticks)) {
            // button kept pressed, and tick elapsed
            tBtnDown = ticks;
            if(pwmDutyPeriod > 0)
                pwmDutyPeriod--;
            EPWM_LoadDutyValue(expTable[pwmDutyPeriod]);
        }
    }
}

// increment ticks counter for button press time, and repeat synchronization
// approx 40Hz
void TMR2_CallBack(void) {
    ticks++;
}
/**
 End of File
*/