//////////////////////////////////////////////////////////////////////////
// empiriKit.h for Freescale FRDM-KL25Z board
//
// Author:  Ingemar Larsson
// Date:    2013-08-19
// Version: 0.25
// License: TODO decide on license
//
//////////////////////////////////////////////////////////////////////////
#include "USBSerial.h"  // Virtual serial port
#include "TSISensor.h"  // Touch sensor
#include "MMA8451Q.h"   // Accelerometer

#define MMA8451_I2C_ADDRESS (0x1d<<1)

enum STATE_TYPE
{
    IDLE_STATE,
    LOG_ACC_STATE,
    ACC_READY_STATE,
    SET_RGB_STATE,
    STREAM_TOUCH_STATE,
    STREAM_ACC_STATE,
    GET_INFO_STATE,
    GET_HELP_STATE,
    GET_LOG_STATE,
};

const char versionString[] = "14.06.001";

STATE_TYPE currentState;
