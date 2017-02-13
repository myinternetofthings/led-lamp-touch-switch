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
#include <stdlib.h>

#include "mtouch_random.h"

#include "mtouch_sensor.h"
#include "mtouch_sensor_scan.h"


typedef uint16_t mtouch_sensor_adcsample_t;
#define MTOUCH_SENSOR_ADCSAMPLE_MIN ((mtouch_sensor_adcsample_t)0)
#define MTOUCH_SENSOR_ADCSAMPLE_MAX ((mtouch_sensor_adcsample_t)1023)

typedef uint8_t mtouch_sensor_packetcounter_t;
#define MTOUCH_SENSOR_PACKET_ADCSAMPLES ((mtouch_sensor_packetcounter_t)8)
#define MTOUCH_SENSOR_PACKETCOUNTER_MIN ((mtouch_sensor_packetcounter_t)0)
#define MTOUCH_SENSOR_PACKETCOUNTER_MAX ((mtouch_sensor_packetcounter_t)UINT8_MAX)

typedef uint16_t mtouch_sensor_packetsample_t;
#define MTOUCH_SENSOR_PACKETSAMPLE_MIN ((mtouch_sensor_packetsample_t)0)
#define MTOUCH_SENSOR_PACKETSAMPLE_MAX ((mtouch_sensor_packetsample_t)UINT16_MAX)

typedef uint8_t mtouch_sensor_sampledelay_t;


#define PIC_ADC_RESOLUTION ((mtouch_sensor_adcsample_t)1024)

typedef struct
{
    unsigned    packet_done:1;
} mtouch_sensor_globalflags_t;

/*
 * =======================================================================
 * LOCAL FUNCTIONS
 * =======================================================================
 */
static enum mtouch_sensor_error Sensor_Acquisition          (enum mtouch_sensor_names sensor);
static enum mtouch_sensor_error Sensor_Acq_ExecutePacket    (enum mtouch_sensor_names sensor);
static void                     Sensor_RawSample_Update     (enum mtouch_sensor_names sensor);
static mtouch_sensor_sample_t   Sensor_RawSample_Get_helper (enum mtouch_sensor_names sensor);
static void                     Sensor_Acq_ExecuteScan      (void);
static void                     Sensor_setScanAFunction     (void (*scanFunction)(void));
static void                     Sensor_setScanBFunction     (void (*scanFunction)(void));
static void                     Sensor_setSampled           (enum mtouch_sensor_names sensor);
//static void                     Sensor_DefaultCallback      (enum mtouch_sensor_names sensor);
static void                     MTOUCH_Delay                (void);

/*
 * =======================================================================
 *  Callback Function Pointers
 * =======================================================================
 */
//static void (*callback_sampled)(enum mtouch_sensor_names sensor) = Sensor_DefaultCallback;

/*
 * =======================================================================
 *  Local Variables
 * =======================================================================
 */
static mtouch_sensor_globalflags_t      sensor_globalFlags;
static mtouch_sensor_mask_t             sensor_sampled;
static mtouch_sensor_sample_t           sensor_rawSample[MTOUCH_SENSORS];
static uint8_t                          packet_sensor;
static mtouch_sensor_packetcounter_t    packet_counter;
static mtouch_sensor_packetsample_t     packet_sample;
static mtouch_sensor_sampledelay_t      sample_delay;
static void (*Sensor_scanA)() = MTOUCH_CVD_ScanA_0;
static void (*Sensor_scanB)() = MTOUCH_CVD_ScanB_0;

static void (*const Sensor_scanAList[MTOUCH_SENSORS])() = {
        MTOUCH_CVD_ScanA_0,
        MTOUCH_CVD_ScanA_1,
};

static void (*const Sensor_scanBList[MTOUCH_SENSORS])() = {
        MTOUCH_CVD_ScanB_0,
        MTOUCH_CVD_ScanB_1,
};

static const mtouch_sensor_packetcounter_t sensor_oversampling[MTOUCH_SENSORS] = {
        8,
        8,
};
/*
 * =======================================================================
 * MTOUCH_Sensor_Init()
 * =======================================================================
 */
void MTOUCH_Sensor_Initialize(enum mtouch_sensor_names sensor)
{
    switch(sensor)                                  /* Overwrite TRIS,ANSEL and WPU*/
    {
        case 0:  
            WPUCbits.WPUC1 = 0;
            ANSELCbits.ANSC1 = 0;
            TRISCbits.TRISC1 = 0;
            break;
        case 1:  
            WPUCbits.WPUC2 = 0;
            ANSELCbits.ANSC2 = 0;
            TRISCbits.TRISC2 = 0;
            break;
    }
}
/*
 * =======================================================================
 * MTOUCH_SensorScan_Initialize
 * =======================================================================
 *  initialization for ADC and Timer module
 */
void MTOUCH_Sensor_Scan_Initialize(void)
{

    ADCON0 = (uint8_t)0;                            /* overwrite the ADC configuration for mTouch scan */
    ADCON1 = (uint8_t)( 0x1<<7 | 0x1<<4 | 0x0 );
}

/*
 * =======================================================================
 * MTOUCH_Sensor_Service()
 * =======================================================================
 * This function acts as a try-catch block. Sensor acquisition is
 * attempted. If any step in the process is broken, an error will be
 * returned which can be processed here.
 * A return value of MTOUCH_SENSOR_ERROR_none means the acquisition was successful
 * and the new data can be integrated with the sensor's output value(s).
 */
enum mtouch_sensor_error MTOUCH_Sensor_Service(enum mtouch_sensor_names sensor)
{
    enum mtouch_sensor_error error = Sensor_Acq_ExecutePacket(sensor);

    /* Validate sensor output. Handle errors. */
    if(error == MTOUCH_SENSOR_ERROR_none)
    {
        Sensor_RawSample_Update(sensor);
        Sensor_setSampled(sensor);
    }
    return error;
}
inline void MTOUCH_Sensor_Sampled_ResetAll(){
    sensor_sampled = 0;
}
//void MTOUCH_Sensor_Sampled_Reset(enum mtouch_sensor_names sensor) {
//    sensor_sampled &= ~((mtouch_sensor_mask_t)1 << sensor);
//}
bool MTOUCH_Sensor_wasSampled(enum mtouch_sensor_names sensor) {
    return ((sensor_sampled & (mtouch_sensor_mask_t)(1 << sensor))==0)? false : true;
}
static void Sensor_setSampled(enum mtouch_sensor_names sensor) {
    sensor_sampled |= (mtouch_sensor_mask_t)(1 << sensor);
}

/*
 * =======================================================================
 * Sensor_Acquisition()
 * =======================================================================
 */
//static enum mtouch_sensor_error Sensor_Acquisition(enum mtouch_sensor_names sensor)
//{
//    enum mtouch_sensor_error error = MTOUCH_SENSOR_ERROR_none;

//    /* Input validation */
//    if (sensor >= MTOUCH_SENSORS)
//    {
//        error = MTOUCH_SENSOR_ERROR_invalid_index;
//    }
//
    /*
     * =======================================================================
     * Production Scan
     * =======================================================================
     */
//    if (error == MTOUCH_SENSOR_ERROR_none)
//    error = Sensor_Acq_ExecutePacket(sensor);
//
//    return error;
//}


/*
 * =======================================================================
 * Sensor_Acq_ExecutePacket()
 * =======================================================================
 */
static enum mtouch_sensor_error Sensor_Acq_ExecutePacket(enum mtouch_sensor_names sensor)
{
    enum mtouch_sensor_error        error = MTOUCH_SENSOR_ERROR_none;
    uint8_t ADCON0_temp;
    uint8_t ADCON1_temp;
    uint8_t GIE_state;

    ADCON0_temp = ADCON0;       /* store the current ADC configuration */
    ADCON1_temp = ADCON1;
    MTOUCH_Sensor_Scan_Initialize();
    
    GIE_state = INTCONbits.GIE;
                 
    Sensor_setScanAFunction(Sensor_scanAList[sensor]);  /* Setup the scan function */
    Sensor_setScanBFunction(Sensor_scanBList[sensor]);

    packet_sensor   = sensor;
    packet_counter  = sensor_oversampling[sensor];
    sensor_globalFlags.packet_done = 0;

    sample_delay = MTOUCH_Random();    /* Pick a random scan delay */
    packet_sample   = 0;
    /* Perform packet samples */
    do
    {
        INTCONbits.GIE = (uint8_t)0;
        Sensor_Acq_ExecuteScan();       /* Critical section */
        INTCONbits.GIE = GIE_state;       
        MTOUCH_Delay();                 
        while(ADCON0bits.GO_nDONE == 1); 
    } while(sensor_globalFlags.packet_done == 0);


    ADCON0 = ADCON0_temp;       /* restore the previous ADC configuration */
    ADCON1 = ADCON1_temp;
    return error;
}

static void MTOUCH_Delay(void)
{
    for(uint8_t i = sample_delay;i>0;i--)
    { }
}
/*
 * =======================================================================
 * Sensor_Acq_ExecuteScan()
 * =======================================================================
 * Perform a single sample on the sensor. This is a local function and
 * requires that the ExecutePacket() function guarantees the correct PIC
 * and scanning configuration.
 *
 * This function is written to be independent of mainloop vs ISR context.
 */
static void Sensor_Acq_ExecuteScan(void)
{
    uint16_t result = ADRES;       /* result from previous scan */
    if (packet_counter != sensor_oversampling[packet_sensor])
    {
        if (packet_counter & 0x01)
        {
            result = PIC_ADC_RESOLUTION -result;
        }
    }

    if (packet_counter != (uint8_t)0)
    {
        #pragma switch time
        switch(packet_counter & 0x01)
        {
            case 0: Sensor_scanA();break;
            case 1: Sensor_scanB();break;
//            default: break;
        }
        
        if (packet_counter != sensor_oversampling[packet_sensor])
        {
            packet_sample += result;
        }
        else
        {
            _delay(9);      /* Equalize branch execution time. */
        }
        packet_counter--;
    }
    else
    {
        packet_sample += result;
        sensor_globalFlags.packet_done = (uint8_t)1;
    }
}

/*
 * =======================================================================
 * Sensor Raw Sample
 * =======================================================================
 */ 
static bit gie;
mtouch_sensor_sample_t MTOUCH_Sensor_RawSample_Get(enum mtouch_sensor_names sensor) /* Global */
{
    mtouch_sensor_sample_t sample;
    
    gie = INTCONbits.GIE;
    INTCONbits.GIE = (uint8_t)0;
    
    sample = sensor_rawSample[sensor];
    
    if(gie == 1)
        INTCONbits.GIE = (uint8_t)1;
    
    return sample;
}
static void Sensor_RawSample_Update(enum mtouch_sensor_names sensor) /* Local */
{
    gie = INTCONbits.GIE;
    INTCONbits.GIE = (uint8_t)0;
 
    sensor_rawSample[sensor] = packet_sample;
    
    if(gie == 1)
        INTCONbits.GIE = (uint8_t)1;
}

/*
 * =======================================================================
 * Sensor Sampled Callback
 * =======================================================================
 */ 
//static void Sensor_DefaultCallback(enum mtouch_sensor_names sensor) { }
//void MTOUCH_Sensor_SetSampledCallback(void (*callback)(enum mtouch_sensor_names sensor))
//{
//    callback_sampled = callback;
//}

/*
 * =======================================================================
 * Scan Setter Function
 * =======================================================================
 */ 

static void Sensor_setScanAFunction(void(*scanFunction)(void))
{
    Sensor_scanA = scanFunction;
}

static void Sensor_setScanBFunction(void(*scanFunction)(void))
{
    Sensor_scanB = scanFunction;
}