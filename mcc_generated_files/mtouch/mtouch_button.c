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
#include <stdbool.h>

#include "mtouch.h"
#include "mtouch_button.h"


enum mtouch_button_state
{
    MTOUCH_BUTTON_STATE_initializing,
    MTOUCH_BUTTON_STATE_notPressed,
    MTOUCH_BUTTON_STATE_pressed,
};
    
/* 
 * =======================================================================
 * Button Type Structure
 * =======================================================================
 */
    typedef struct
    {
        const enum mtouch_button_names          name;
        const enum mtouch_sensor_names          sensor;
              enum mtouch_button_state          state;
              mtouch_button_reading_t           reading;
              mtouch_button_baseline_t          baseline;
              mtouch_button_deviation_t         deviation;
              mtouch_button_statecounter_t      counter;
              mtouch_button_baselinecounter_t   baseline_count;
              mtouch_button_deviation_t         threshold;
              mtouch_button_scaling_t           scaling;
    } mtouch_button_t;

    static mtouch_button_t mtouch_button[MTOUCH_BUTTONS] =
    {
        /* ButtonUp */
        {   ButtonUp, 
            Sensor_AN5,
            MTOUCH_BUTTON_STATE_initializing,0,0,0,0,0, /* non-const variables */
            (mtouch_button_deviation_t)MTOUCH_BUTTON_THRESHOLD_ButtonUp, /* threshold */
            (mtouch_button_scaling_t)MTOUCH_BUTTON_SCALING_ButtonUp /* scaling */
        },
        /* ButtonDown */
        {   ButtonDown, 
            Sensor_AN6,
            MTOUCH_BUTTON_STATE_initializing,0,0,0,0,0, /* non-const variables */
            (mtouch_button_deviation_t)MTOUCH_BUTTON_THRESHOLD_ButtonDown, /* threshold */
            (mtouch_button_scaling_t)MTOUCH_BUTTON_SCALING_ButtonDown /* scaling */
        }
    };
 
 
/*
 * =======================================================================
 *  Local Functions
 * =======================================================================
 */
static void                     Button_Deviation_Update     (enum mtouch_button_names button);
static void                     Button_Reading_Update       (mtouch_button_t* button);
static void                     Button_Baseline_Initialize  (mtouch_button_t* button);
static void                     Button_Baseline_Update      (mtouch_button_t* button);
static mtouch_button_reading_t  Button_Baseline_Get_helper  (enum mtouch_button_names button);
static void                     Button_Tick_helper          (mtouch_button_t* button);
static void                     Button_State_Initializing   (mtouch_button_t* button);
static void                     Button_State_NotPressed     (mtouch_button_t* button);
static void                     Button_State_Pressed        (mtouch_button_t* button);


/*
 * =======================================================================
 * Button State machine
 * =======================================================================
 */
typedef void (*button_statemachine_state_t)(mtouch_button_t*);
const button_statemachine_state_t Button_StateMachine[] = 
{
    Button_State_Initializing,
    Button_State_NotPressed,
    Button_State_Pressed
};
#define BUTTON_STATEMACHINE_COUNT (uint8_t)(sizeof(Button_StateMachine)/sizeof(button_statemachine_state_t))

/*
 * =======================================================================
 *  MTOUCH_Button_Initialize
 * =======================================================================
 */
void MTOUCH_Button_Initialize(enum mtouch_button_names name)
{
    mtouch_button_t* button = &mtouch_button[name];

    button->state           = MTOUCH_BUTTON_STATE_initializing;
    button->baseline        = (mtouch_button_baseline_t)0;
    button->counter         = (mtouch_button_statecounter_t)0;
    button->baseline_count  = (mtouch_button_baselinecounter_t)0;
}

//void MTOUCH_Button_Recalibrate(void)
//{
//    uint8_t i;
//    for (i = 0; i < MTOUCH_SENSORS; i++)
//    {
//        MTOUCH_Button_Initialize(i);
//    }
//}

/*
 * =======================================================================
 * Button Service
 * =======================================================================
 */
void MTOUCH_Button_Service(enum mtouch_button_names name)
{
    mtouch_button_t* button = &mtouch_button[name];
    
    if (MTOUCH_Sensor_wasSampled(button->sensor))
    {
        Button_Reading_Update(button);
        Button_Deviation_Update(name);

//        if (button->state >= BUTTON_STATEMACHINE_COUNT)
//        {
//            MTOUCH_Button_Initialize(button->name);
//        }
        Button_StateMachine[button->state](button);
    }
}
static void Button_State_Initializing(mtouch_button_t* button) 
{
    /* Initialize the baseline. */
    if ((button->counter) <= (mtouch_button_statecounter_t)MTOUCH_BUTTON_BASELINE_INIT/2)
    { 
        Button_Baseline_Initialize(button); /* Rough resolution */
    } else { 
        Button_Baseline_Update(button); /* Fine resolution */
    }
    
    /* Exit after a fixed number of baseline updates */
    (button->counter)++;
    if ((button->counter) >= (mtouch_button_statecounter_t)MTOUCH_BUTTON_BASELINE_INIT)
    {
        button->state   = MTOUCH_BUTTON_STATE_notPressed;
        button->counter = (mtouch_button_statecounter_t)0;
    }
}
static void Button_State_NotPressed(mtouch_button_t* button)
{
    /* Negative Capacitance check */
    if ((button->deviation) < (mtouch_button_deviation_t)0)
    {
        (button->counter)++;
        if ((button->counter) > (mtouch_button_statecounter_t)MTOUCH_BUTTON_NEGATIVEDEVIATION)
        {
            button->counter = (mtouch_button_statecounter_t)0;
            Button_Baseline_Initialize(button);
        }
    }
    /* Threshold check */
    else if ((button->deviation) > (button->threshold))
    {
        button->state   = MTOUCH_BUTTON_STATE_pressed;
        button->counter = (mtouch_button_statecounter_t)0;
    }
    else
    {
        /* Delta is positive, but not past the threshold. */
        button->counter = (mtouch_button_statecounter_t)0;
    }
    
    /* Baseline Update check */
    (button->baseline_count)++;
    if ((button->baseline_count) > MTOUCH_BUTTON_BASELINE_RATE)
    {
        button->baseline_count = (mtouch_button_baselinecounter_t)0;
        Button_Baseline_Update(button);
    }
}
static void Button_State_Pressed(mtouch_button_t* button)
{
    /* Timeout check */
    if ((button->counter) >= MTOUCH_BUTTON_PRESSTIMEOUT)
    {
        MTOUCH_Button_Initialize(button->name);
    }
    
    /* Threshold check */
    else if ((button->deviation) < (mtouch_button_deviation_t)((button->threshold) >> 1))
    {
        button->state   = MTOUCH_BUTTON_STATE_notPressed;
        button->counter = (mtouch_button_statecounter_t)0;
    }
}

/*
 * =======================================================================
 *  MTOUCH_Button_Tick
 * =======================================================================
 */
void MTOUCH_Button_Tick(void)
{
    uint8_t i;
    for (i = 0; i < MTOUCH_BUTTONS; i++)
    {
        /* Only pressed state counter is based on real time */
        if ((mtouch_button[i].state) == MTOUCH_BUTTON_STATE_pressed)
        {
            (mtouch_button[i].counter)++;
            if (mtouch_button[i].counter == (mtouch_button_statecounter_t)0)
            {
                mtouch_button[i].counter = (mtouch_button_statecounter_t)0xFFFF;
            }
        }
    }
}
/*
 * =======================================================================
 * Button State and Deviation
 * =======================================================================
 */

//mtouch_button_threshold_t MTOUCH_Button_Threshold_Get(enum mtouch_button_names name)
//{
//    return mtouch_button[name].threshold;
//}
//
//void MTOUCH_Button_Threshold_Set(enum mtouch_button_names name,mtouch_button_threshold_t threshold)
//{
//    if(threshold >= MTOUCH_BUTTON_THRESHOLD_MIN && threshold <= MTOUCH_BUTTON_THRESHOLD_MAX)
//        mtouch_button[name].threshold = threshold;
//}
//
//mtouch_button_scaling_t MTOUCH_Button_Scaling_Get(enum mtouch_button_names name)
//{
//    return mtouch_button[name].scaling;
//}

//void MTOUCH_Button_Scaling_Set(enum mtouch_button_names name,mtouch_button_scaling_t scaling)
//{
//    if(scaling <= MTOUCH_BUTTON_SCALING_MAX)
//        mtouch_button[name].scaling = scaling;
//}

bool MTOUCH_Button_isPressed(enum mtouch_button_names name)
{
    return (mtouch_button[name].state == MTOUCH_BUTTON_STATE_pressed) ? true : false;
}

bool MTOUCH_Button_isInitialized(enum mtouch_button_names name)
{
    return (mtouch_button[name].state == MTOUCH_BUTTON_STATE_initializing) ? false : true;
}

//mtouch_button_deviation_t MTOUCH_Button_Deviation_Get(enum mtouch_button_names name) /* Global */
//{
//    return mtouch_button[name].deviation;
//}

static void Button_Deviation_Update(enum mtouch_button_names name)
{
    int32_t deviation;
    mtouch_button_deviation_t devscaled;
    
    deviation = (int32_t)((int32_t)(MTOUCH_Button_Reading_Get(name)) - (int32_t)(MTOUCH_Button_Baseline_Get(name)));

    /* Delta scaling */
    deviation = deviation >> (mtouch_button[name].scaling); /* XC8 compiler will extend sign bit of signed values */

    devscaled = (mtouch_button_deviation_t)deviation;
    /* Bounds checking */
    if (deviation > MTOUCH_BUTTON_DEVIATION_MAX)
    {
        devscaled = MTOUCH_BUTTON_DEVIATION_MAX;
    }
    if (deviation < MTOUCH_BUTTON_DEVIATION_MIN)
    {
        devscaled = MTOUCH_BUTTON_DEVIATION_MIN;
    }

    mtouch_button[name].deviation = devscaled;
}

mtouch_buttonmask_t MTOUCH_Button_Buttonmask_Get(void)
{
    mtouch_buttonmask_t output = 0;

    for (uint8_t i = 0; i < MTOUCH_SENSORS; i++)
    {
        if (MTOUCH_Button_isPressed(i) == true)
        {
            output |= (mtouch_buttonmask_t)0x01 << i;
        }
    }
    return output;
}
 
/*
 * =======================================================================
 * Button Reading and Baseline
 * =======================================================================
 */
mtouch_button_reading_t MTOUCH_Button_Reading_Get(enum mtouch_button_names name) /* Global */
{
    return mtouch_button[name].reading;
}
static void Button_Reading_Update(mtouch_button_t* button)
{
    button->reading   -= button->reading>>MTCOUH_BUTTON_READING_GAIN;
    button->reading   += MTOUCH_Sensor_RawSample_Get(button->sensor)>>MTCOUH_BUTTON_READING_GAIN;
}
static void Button_Baseline_Initialize(mtouch_button_t* button)
{
    button->baseline   = MTOUCH_Sensor_RawSample_Get(button->sensor);
    button->baseline <<= MTOUCH_BUTTON_BASELINE_GAIN;
}
static void Button_Baseline_Update(mtouch_button_t* button)
{
    button->baseline -= (button->baseline) >> MTOUCH_BUTTON_BASELINE_GAIN;
    button->baseline += MTOUCH_Sensor_RawSample_Get(button->sensor);
}

bit gie;
mtouch_button_reading_t MTOUCH_Button_Baseline_Get(enum mtouch_button_names name) /* Global */
{
    mtouch_button_reading_t baseline;

    gie = INTCONbits.GIE;
    INTCONbits.GIE = (uint8_t)0;

    baseline = (mtouch_button_reading_t)((mtouch_button[name].baseline) >> MTOUCH_BUTTON_BASELINE_GAIN);

    if(gie == 1)
        INTCONbits.GIE = (uint8_t)1;

    return baseline;
}

