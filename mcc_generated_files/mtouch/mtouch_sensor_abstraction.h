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
#ifndef MTOUCH_SENSOR_ABSTRACTION_H
#define MTOUCH_SENSOR_ABSTRACTION_H
  
    #include <stdint.h>
    #include <stdbool.h>
    
    #define MTOUCH_SENSORS      2
    
    enum mtouch_sensor_names
    {
        Sensor_AN5 = 0,
        Sensor_AN6 = 1
    };
    
    enum mtouch_sensor_error
    {
        MTOUCH_SENSOR_ERROR_none            =  0,
        MTOUCH_SENSOR_ERROR_invalid_index   = -1,
        MTOUCH_SENSOR_ERROR_interrupt_notEnabled = -2,
        MTOUCH_SENSOR_ERROR_invalid_calibrate= -3
    };

/*
 * =======================================================================
 * Typedefs / Data Types
 * =======================================================================
 */
    typedef uint16_t mtouch_sensor_sample_t;
    #define MTOUCH_SENSOR_SAMPLE_MIN (0)
    #define MTOUCH_SENSOR_SAMPLE_MAX (UINT16_MAX)

    typedef uint8_t mtouch_sensor_mask_t;
    #define MTOUCH_SENSOR_MASK_MIN (0)
    #define MTOUCH_SENSOR_MASK_MAX (UINT8_MAX)

/*
 * =======================================================================
 * Global Functions
 * =======================================================================
 */
    bool                        MTOUCH_Sensor_wasSampled     (enum mtouch_sensor_names sensor);
    mtouch_sensor_sample_t      MTOUCH_Sensor_RawSample_Get  (enum mtouch_sensor_names sensor);
    void                        MTOUCH_Sensor_Sampled_Reset(enum mtouch_sensor_names sensor); 
    inline void                 MTOUCH_Sensor_Sampled_ResetAll(void);

#endif