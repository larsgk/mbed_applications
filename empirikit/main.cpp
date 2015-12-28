#include "mbed.h"
#include "empiriKit.h"  // Defines, etc for empiriKit

#if defined(TARGET_KL46Z)
#include "SLCD.h"
#endif

//////////////////////////////////////////////////////////////////////////
// empiriKit for Freescale FRDM-KL25Z board
//
// Connects to host using the second USB port (named KL25Z on board)
//
// Author:  Ingemar Larsson & Lars Knudsen
// Date:    2013-08-19
// Version: 14.03.001
// License: TBD
//
//////////////////////////////////////////////////////////////////////////

// Enable the following to use the flashing (debug) serial port
// #define USE_FLASH_SERIAL_PORT

#ifdef USE_FLASH_SERIAL_PORT
Serial* serial;
#else
USBSerial* serial;
#endif

// #define EK_DEBUG
#ifdef EK_DEBUG
Serial _debug(USBTX, USBRX);  // PC Debugging
#endif

int count = 0;

#define DEFAULT_SAMPLING_RATE 50 // Sampling rate in Hz
#define ACC_LOG_LENGTH (DEFAULT_SAMPLING_RATE*20) // Allow 20s sampling log for 50 Hz
#define ACC_LOG_SIZE (DEFAULT_SAMPLING_RATE*20*3) // Allow 20s sampling log for 50 Hz
#define SAMPLING_WAIT (1000/DEFAULT_SAMPLING_RATE)
#define SAMPLING_WAIT_US (1000*SAMPLING_WAIT)

int _stream_sampling_rate = DEFAULT_SAMPLING_RATE;
int _stream_sampling_wait_us = SAMPLING_WAIT_US;

// Accelerometer
// Move the rest of the accel extra code (see patch) to this file
// (or the app at least)
#if defined(TARGET_KL25Z) | defined(TARGET_KL46Z)
#define MMA8451_I2C_ADDRESS (0x1d<<1)
MMA8451Q acc(PTE25, PTE24, MMA8451_I2C_ADDRESS);
int16_t *accLog = 0;
int16_t *accLogPtr;
int16_t accXYZ[3];
int accLoggedDataLength = 0;
int _accelerometerRange = 8;
int accelerometerStreaming = 0;
#endif

//Timers
Timer timer;
Timer loopTimer;

// LCD
#if defined(TARGET_KL46Z)
SLCD lcd;
char lcdMessage[40];
#endif

// Touch sensor
int16_t touchValue = 0;
int touchStreaming = 0;
TSISensor tsi;

// Serial communication
#define BUFFER_SIZE 128
char inBuffer[BUFFER_SIZE];
char cmdBuffer[BUFFER_SIZE];
int cmdBufPtr=0;
int cmdBufLength=0;
int inBufPtr=0;
char inChar;
int sendNotifications = 0;

void setStreamSamplingRate(int rate) {
    if (rate < 1 || rate > 100)
        return;

    _stream_sampling_rate = rate;
    _stream_sampling_wait_us = (1000000 / rate);
}

// RGBLED
#if defined(TARGET_KL25Z)
PwmOut ledGreen(LED_GREEN);
PwmOut ledRed(LED_RED);
PwmOut ledBlue(LED_BLUE);
int redVal, greenVal, blueVal;
#endif

void setRGB(int red, int green, int blue) {
#if defined(TARGET_KL25Z)
    // Note: since we're using PWM, values are inverted so 1.0 == off

    // Converting to float as the PwmOut class does float->int conversion.
    // Code seems to be simple enough for GCC to convert (somehow)
    ledRed = 1.f - ((float)red / 255.f);
    ledGreen = 1.f - ((float)green / 255.f);
    ledBlue = 1.f - ((float)blue / 255.f);
#endif
}

char helpString[] = 
    "\r\n"
    "CMD      Description\r\n"
    "GETINF   Get hardware and firmware information, ({'GETINF':1})\r\n"
#if defined(TARGET_KL25Z)
    "SETRGB   Set LED RGB color, e.g. send \"{'SETRGB':[255,0,0]}\"\r\n"
#endif
    "NOTIFY   Send state change notifications ({'NOTIFY':x}, x = 0(off) or 1(on))\r\n"
    "SETRTE   Set sampling rate ({'SETRTE':x}, 1 <= x <= 100)\r\n"
    "STRTCH   Stream touch values ({'STRTCH':x}, x = 0(off) or 1(on))\r\n"
    "STRACC   Stream accelerometer values ({'STRTCH':x}, x = 0(off) or 1(on))\r\n"
    "LOGACC   Start logging accelerometer data ({'LOGACC':1})\r\n"
    "GETLOG   Get logged accelerometer data, ({'GETLOG':1})\r\n"
    "\r\n"
    "Visit www.empirikit.com for more information.\r\n";

void handleCmdBuffer() {
    // For now - all commands are 6 chars - do a quick and dirty compare

#ifdef EK_DEBUG
    _debug.printf("Command buffer contains: '%s'\r\n",cmdBuffer);
#endif
    char *tmpPtr;
    int tmpVal;

    // Example cmd:  {"SETRGB":[255,0,0]}
    // Cmd name starts at 2
    // Args start at 10
    tmpPtr = &cmdBuffer[2];

    if (strncmp(tmpPtr,"SETIDL",6) == 0){
        accelerometerStreaming = 0;
        touchStreaming = 0;
        setStreamSamplingRate(DEFAULT_SAMPLING_RATE);
        currentState = IDLE_STATE;
    } else if (strncmp(tmpPtr,"LOGACC",6) == 0){
        currentState = LOG_ACC_STATE;
    } else if (strncmp(tmpPtr,"NOTIFY",6) == 0){
        sscanf(&cmdBuffer[10],"%i",&sendNotifications);
#if defined(TARGET_KL25Z)
    } else if (strncmp(tmpPtr,"SETRGB",6) == 0){
        currentState = SET_RGB_STATE;
        sscanf(&cmdBuffer[11],"%i,%i,%i",&redVal, &greenVal, &blueVal);
#elif defined(TARGET_KL46Z)
    } else if (strncmp(tmpPtr,"SETLCD",6) == 0){
        // TODO:  Set LCD string...
#endif
    } else if (strncmp(tmpPtr,"SETRTE",6) == 0){
        sscanf(&cmdBuffer[10],"%i",&tmpVal);
        setStreamSamplingRate(tmpVal);
    } else if (strncmp(tmpPtr,"STRTCH",6) == 0){
        sscanf(&cmdBuffer[10],"%i",&touchStreaming);
    } else if (strncmp(tmpPtr,"STRACC",6) == 0){
        sscanf(&cmdBuffer[10],"%i",&accelerometerStreaming);
    } else if (strncmp(tmpPtr,"GETINF",6) == 0){
        currentState = GET_INFO_STATE;
    } else if (strncmp(tmpPtr,"GETLOG",6) == 0){
        currentState = GET_LOG_STATE;
    } else {
        currentState = GET_HELP_STATE;
    }
}

void serialCallback() {
    // This interrupt service routine is called when a character 
    // is received over USB.

    // Introduce start & stop char (possibly JSON... {"cmd":"SETRGB", "params":[100,0,0]} ) 
    // {"SETRGB":[100,0,0]}, {"SETIDL":1}  .. probably better for now.. more compact
    // And send back ACK/NACK ? (always nice))
#ifdef USE_FLASH_SERIAL_PORT
    while (serial->readable()) {
        inChar = serial->getc();
#else
    while (serial->available()) {
        inChar = serial->_getc();
#endif
        // Hack to detect if ModemManager tries to steal the device
        if (inChar == 'T' && inBufPtr == 1 && inBuffer[0] == 'A') {
            // for now... change the color to red
            setRGB(255,0,0);
            // restart as "arduino"
            serial->disconnect();
            wait_ms(2000);
            delete serial;
            setRGB(0,0,255);
            wait_ms(2000);
            serial = new USBSerial(0x2341,0xbeef,0x0001,false);
            setRGB(0,255,0);
            wait_ms(2000);
            inBufPtr = 0;            
        }

        if (inChar == '{') {
            inBufPtr = 0;
        }
        inBuffer[inBufPtr] = inChar;
        inBufPtr++;
        inBuffer[inBufPtr] = 0;

        if (inBufPtr >= BUFFER_SIZE) {
            inBufPtr = 0;  // For now ... but should spawn an error
        }
        if (inChar == '}') {
            // Quick sanity check
            if (inBuffer[0] == '{') {
                memcpy(cmdBuffer, inBuffer, inBufPtr+1);  // Do a check here
                cmdBufLength = inBufPtr;
                inBufPtr = 0;  // reset
                handleCmdBuffer();          
            }
        }
    }
}

/////////////////////////////////// MAIN /////////////////////////////////

int main(void) {

#if defined(TARGET_KL25Z)
    ledGreen.period(0.001);
    ledRed.period(0.001);
    ledBlue.period(0.001);
#endif

#ifdef EK_DEBUG
    _debug.printf("Main init\r\n");
#endif

#ifdef USE_FLASH_SERIAL_PORT
    serial = new Serial(USBTX, USBRX);
#else
    // VID=0x0425, PID=0x0408 - our official sub-licensed PID on Freescale VID
    serial = new USBSerial(0x0425, 0x0408, 0x0001, false);
#endif

    accLog = new int16_t[ACC_LOG_SIZE];

    currentState = IDLE_STATE;
    
    // Indicate power on with green LED
    if (accLog)
        setRGB(0,255,0);
    else {
        setRGB(255,0,0);
        while(1);
    }

    // Set accelerometer range to +/-8G
    acc.setRange(_accelerometerRange);

#ifdef EK_DEBUG
    _debug.printf("Attaching serial callback\r\n");    
#endif
    // Attach ISR function
    serial->attach(serialCallback);

#if defined(TARGET_KL46Z)
    lcd.printf("8888");
#endif

    loopTimer.reset();
    loopTimer.start();

    // Remain in state machine forever
    while(1)
    {
        switch (currentState) {
            case IDLE_STATE:
                // TODO add battery status monitoring, USB connected?
#if defined(XXTARGET_KL46Z)
                    sprintf(lcdMessage, "%04d", tsi.readDistance());
                    lcd.printf(lcdMessage);
#endif
                break;
            case LOG_ACC_STATE:
#if defined(TARGET_KL46Z)
                    lcd.printf("LACC");
#endif
                count = (count<3)?count+1:0;
                if (tsi.readDistance() > 20)  // Should do:  Proper swipe detection.
                    currentState = ACC_READY_STATE;
                else if (tsi.readDistance() > 0) {
#if defined(TARGET_KL25Z)
                    setRGB(0,0,tsi.readDistance() * 12);
#elif defined(TARGET_KL46Z)
                    sprintf(lcdMessage, "%04d", tsi.readDistance());
                    lcd.printf(lcdMessage);
#endif
                } else if (count == 0)
                    setRGB(0,255,0);
                else
                    setRGB(0,0,0);
                break;
            case ACC_READY_STATE:
#if defined(TARGET_KL46Z)
                    lcd.printf("ACCR");
#endif
#ifdef EK_DEBUG
                _debug.printf("case ACC_READY_STATE\r\n");
#endif
                setRGB(255,0,0);
                // Blink red LED for 5s to indicate logging will start
                for (int i=0; i<10; i++) { 
                    wait_ms(500);
#if defined(TARGET_KL25Z)
                    setRGB((i&1?0:255),0,0);
#endif
#if defined(TARGET_KL46Z)
                    sprintf(lcdMessage, "-%2ds", (10-i)>>1);
                    lcd.printf(lcdMessage);
#endif
                }
                // Constant red LED to indicate recording
                // The following line is commented out (for now) as we get a crash if we send data and are not connected.
                if (sendNotifications)
                    serial->printf("{\"datatype\":\"Notification\",\"data\":\"LoggingStarted\"}\r\n");
                setRGB(255,0,0);
                accLoggedDataLength = ACC_LOG_LENGTH*3;
                timer.reset();
                timer.start();
                accLogPtr = accLog; // Point at the beginning
#if defined(TARGET_KL46Z)
                lcd.DP2(1);
#endif
                for (int i=0; i<ACC_LOG_LENGTH; i++) {
                    acc.getAccAllAxis(accLogPtr);
                    accLogPtr += 3; // Check that this works... might need ++ three times.
#if defined(TARGET_KL46Z)
                    sprintf(lcdMessage, "%3ds", i/5);
                    lcd.printf(lcdMessage);
#endif
                    while( timer.read_us() < _stream_sampling_wait_us );
                    timer.reset();

                    // Check if user swiped to stop logging (TODO: actual swipe detection ;))
                    if (tsi.readDistance() > 20){
                        accLoggedDataLength = i*3;
                        break;
                    }
                }
                timer.stop();
#if defined(TARGET_KL46Z)
                lcd.DP2(0);
                lcd.printf("DONE");
#endif
                // The following line is commented out (for now) as we get a crash if we send data and are not connected.
                if (sendNotifications)
                    serial->printf("{\"datatype\":\"Notification\",\"data\":\"LoggingEnded\"}\r\n");
                // Set green LED to indicate logging is done
                setRGB(0,255,0);
                currentState = IDLE_STATE;  // Done, switch back
                break;
#if defined(TARGET_KL25Z)
            case SET_RGB_STATE:
                // Set LEDs with last successfully read values.
                setRGB(redVal,greenVal,blueVal);
                currentState = IDLE_STATE;  // Done, switch back
                break;
#endif                
            case GET_INFO_STATE:
                serial->printf("{\"datatype\":\"HardwareInfo\",\r\n");
#if defined(TARGET_KL25Z)
                serial->printf("\"devicetype\":\"empiriKit|MOTION\",\r\n");
#elif defined(TARGET_KL46Z)
                serial->printf("\"devicetype\":\"empiriKit|KL46Z\",\r\n");
#endif
                serial->printf("\"version\":\"%s\",\r\n",versionString);
                serial->printf("\"uid\":\"0x%04X%08X%08X\",\r\n",
                    *((uint32_t *)0x40048058),
                    *((uint32_t *)0x4004805C),
                    *((uint32_t *)0x40048060));
                serial->printf("\"capabilities\":[\r\n");
                serial->printf("\"accelerometer\",\r\n");
#if defined(TARGET_KL25Z)
                serial->printf("\"rgbled\",\r\n");
#elif defined(TARGET_KL46Z)
                serial->printf("\"magnetometer\",\r\n");
                serial->printf("\"lightsensor\",\r\n");
#endif
                serial->printf("\"touchsensor\"\r\n");
                serial->printf("]}\r\n");
                currentState = IDLE_STATE;  // Done, switch back
                break;
            case GET_HELP_STATE:
                serial->printf("{\"datatype\":\"HelpMessage\",\"data\":\"%s\"}\r\n",helpString);
                currentState = IDLE_STATE;  // Done, switch back
                break;
            case GET_LOG_STATE:
                serial->printf("{\"datatype\":\"AccelerometerLog\",\r\n");
                serial->printf("\"accelrange\":%d,\r\n", _accelerometerRange);
                serial->printf("\"accelfactor\":%d,\r\n", 8192 / _accelerometerRange);
                serial->printf("\"samplingrate\":%d,\r\n", _stream_sampling_rate);
                serial->printf("\"data\":[\r\n");
                for (int i=0; i<accLoggedDataLength; i=i+3) {
                    serial->printf("[%d,%d,%d]",accLog[i],accLog[i+1],accLog[i+2]);
                    if (i<(accLoggedDataLength-3))
                        serial->printf(",\r\n");
                    else
                        serial->printf("\r\n");
                }
                serial->printf("]}\r\n");
                currentState = IDLE_STATE;  // Done, switch back
                break;
            default:
                serial->printf("{\"datatype\":\"StatusMessage\",\"data\":\"Unexpected state.\"}\r\n");
        }
        
        if (touchStreaming || accelerometerStreaming) {
            // Use the high precision timer
            while( loopTimer.read_us() < _stream_sampling_wait_us );
            loopTimer.reset();
            if (touchStreaming)
                touchValue = tsi.readDistance();
            if (accelerometerStreaming)
                acc.getAccAllAxis(accXYZ);
            // Separate reading and printing for better precision
            serial->printf("{\"datatype\":\"StreamData\",\r\n");
            serial->printf("\"samplingrate\":%d", _stream_sampling_rate);
            if (touchStreaming)
                serial->printf(",\r\n\"touchsensordata\":%d", touchValue);
            if (accelerometerStreaming)
                serial->printf(",\r\n\"accelerometerdata\":[%d,%d,%d]",accXYZ[0],accXYZ[1],accXYZ[2]);
            serial->printf("\r\n}\r\n");
        } else {
            wait_ms(100);
            loopTimer.reset();  // keep it ready
        }
    }
}
