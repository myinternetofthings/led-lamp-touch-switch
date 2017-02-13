/*
    MICROCHIP SOFTWARE NOTICE AND DISCLAIMER:

    You may use this software, and any derivatives created by any person or
    entity by or on your behalf, exclusively with Microchip's products.
    Microchip and its subsidiaries ("Microchip"), and its licensors, retain all
    ownership and intellectual property rights in the accompanying software and
    in all derivatives hereto.

    This software and any accompanying information is for suggestion only. It
    does not modify Microchip's standard warranty for its products.  You agree
    that you are solely responsible for testing the software and determining
    its suitability.  Microchip has no obligation to modify, test, certify, or
    support the software.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE APPLY TO THIS SOFTWARE, ITS INTERACTION WITH MICROCHIP'S
    PRODUCTS, COMBINATION WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT, WILL MICROCHIP BE LIABLE, WHETHER IN CONTRACT, WARRANTY, TORT
    (INCLUDING NEGLIGENCE OR BREACH OF STATUTORY DUTY), STRICT LIABILITY,
    INDEMNITY, CONTRIBUTION, OR OTHERWISE, FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    EXEMPLARY, INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, FOR COST OR EXPENSE OF
    ANY KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWSOEVER CAUSED, EVEN IF
    MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE
    FORESEEABLE.  TO THE FULLEST EXTENT ALLOWABLE BY LAW, MICROCHIP'S TOTAL
    LIABILITY ON ALL CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED
    THE AMOUNT OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR
    THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF
    THESE TERMS.
*/

#include <xc.h>
#include <stdint.h>

#include "mtouch_sensor.h"
#include "mtouch_button.h"
#include "../mcc.h"

/*
 * =======================================================================
 *  Local Functions
 * =======================================================================
 */
static void     Sensor_InitializeAll    (void);
static bool     Sensor_SampleAll        (void);
static void     Button_InitializeAll        (void);
static void     Button_ServiceAll           (void);

/*
 * =======================================================================
 * MTOUCH_Init()
 * =======================================================================
 *  Root initialization routine for all enabled mTouch library modules.
 */
void MTOUCH_Initialize(void)
{   
    MTOUCH_Sensor_Scan_Initialize(); 
    Sensor_InitializeAll(); /* Loop through all sensors and initialize */  
    Button_InitializeAll(); /* Loop through all buttons and initialize */
    MTOUCH_Sensor_Sampled_ResetAll();
}

/*
 * =======================================================================
 * MTOUCH_Service_Mainloop()
 * =======================================================================
 *  Root mainloop service routine for all enabled mTouch library modules.
 */
bool MTOUCH_Service_Mainloop(void)
{
    if(Sensor_SampleAll() == false)     
        return false;           /* Please check the mtouch_sensor_error if Sensor_SampleAll() return false*/    
    Button_ServiceAll();           /* Execute state machine for all buttons w/scanned sensors */
    MTOUCH_Sensor_Sampled_ResetAll();  
    MTOUCH_Tick();
    return true;
}

/*
 * =======================================================================
 * Sensor Helper Functions
 * =======================================================================
 */
static void Sensor_InitializeAll(void)
{
    uint8_t i;    
    for (i = 0; i < MTOUCH_SENSORS; i++)
    {
        MTOUCH_Sensor_Initialize(i);
    }
}
static bool Sensor_SampleAll(void)
{
    uint8_t i;   
    for (i = 0; i < MTOUCH_SENSORS; i++)
    {
        if(MTOUCH_Sensor_Service(i)!= MTOUCH_SENSOR_ERROR_none)
            return false;
    }
    return true;
}

/*
 * =======================================================================
 * Button Helper Functions
 * =======================================================================
 */
static void Button_InitializeAll(void)
{
    uint8_t i;    
    for (i = 0; i < MTOUCH_BUTTONS; i++)
    {
        MTOUCH_Button_Initialize(i);
    }
}
static void Button_ServiceAll(void)
{
    uint8_t i; 
    for (i = 0; i < MTOUCH_BUTTONS; i++)
    {  
        MTOUCH_Button_Service(i);
    }
}

/*
 * =======================================================================
 * MTOUCH_Tick
 * =======================================================================
 */
void MTOUCH_Tick(void)
{
    MTOUCH_Button_Tick();
}
