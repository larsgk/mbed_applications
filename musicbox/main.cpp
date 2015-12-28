#include "mbed.h"
#include "Beeper.h"

#include "USBSerial.h"  // Virtual serial port
#include "TSISensor.h"  // Touch sensor
#include "MMA8451Q.h"   // Accelerometer

#define MMA8451_I2C_ADDRESS (0x1d<<1)

enum STATE_TYPE
{
    IDLE_STATE,
    SET_RGB_STATE,
    GET_INFO_STATE,
    GET_HELP_STATE
};

STATE_TYPE currentState;


// Virtual serial port over USB
USBSerial* serial;  //(0x2341, 0xbeef);  // Identify as an arduino VID (for now)

// #define _DEBUG
#ifdef _DEBUG
Serial _debug(USBTX, USBRX);  // PC Debugging
#endif

#define MMA8451_I2C_ADDRESS (0x1d<<1)

// Accelerometer instantiation
MMA8451Q acc(PTE25, PTE24, MMA8451_I2C_ADDRESS);

Timer timer;

// Touch sensor instantiation
TSISensor tsi;

// Serial input buffer (128 = 2^7)
#define BUFFER_SIZE 128

char inBuffer[BUFFER_SIZE];
char cmdBuffer[BUFFER_SIZE];
int cmdBufPtr=0;
int cmdBufLength=0;

int inBufPtr=0;
char inChar;

// Define LEDs
PwmOut ledGreen(LED_GREEN);
PwmOut ledRed(LED_RED);
PwmOut ledBlue(LED_BLUE);
int redVal, greenVal, blueVal;

char helpString[] = 
    "\r\n"
    "CMD      Description\r\n"
    "SETRGB   Set LED RGB color, e.g. {'SETRGB':[255,0,0]}\r\n"
    "TODO: rest of help message :)\r\n";

void handleCmdBuffer() {
    // For now - all commands are 6 chars - do a quick and dirty compare

#ifdef _DEBUG
    _debug.printf("Command buffer contains: '%s'\r\n",cmdBuffer);
#endif
    char *tmpPtr;

    // Example cmd:  {"SETRGB":[255,0,0]}
    // Cmd name starts at 2
    // Args start at 10
    tmpPtr = &cmdBuffer[2];

    if (strncmp(tmpPtr,"SETIDL",6) == 0){
        currentState = IDLE_STATE;
    } else if (strncmp(tmpPtr,"SETRGB",6) == 0){
        currentState = SET_RGB_STATE;
        // Copy content of input buffer for later processing
        tmpPtr = &cmdBuffer[11];
        sscanf(tmpPtr,"%i,%i,%i",&redVal, &greenVal, &blueVal);
    } else if (strncmp(tmpPtr,"GETINF",6) == 0){
        currentState = GET_INFO_STATE;
    } else {
        currentState = GET_HELP_STATE;
    }
}

void serialCallback() {
    while (serial->available()) {
        inChar = serial->_getc();

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

void setRGB(int red, int green, int blue) {
    // Note: since we're using PWM, values are inverted so 1.0 == off

    // Converting to float as the PwmOut class does float->int conversion.
    // Code seems to be simple enough for GCC to convert (somehow)
    ledRed = 1.f - ((float)red / 255.f);
    ledGreen = 1.f - ((float)green / 255.f);
    ledBlue = 1.f - ((float)blue / 255.f);
}

/////////////////////////////////// MAIN /////////////////////////////////
Beeper beeper(D3);

void playTwinkle() {
    setRGB(255,255,0);
    beeper.note(60,0.4,0.1);
    setRGB(0,0,0);
    beeper.note(60,0.4,0.1);
    setRGB(255,255,255);
    beeper.note(67,0.4,0.1);
    setRGB(0,0,0);
    beeper.note(67,0.4,0.1);
    setRGB(255,255,0);
    beeper.note(69,0.4,0.1);
    setRGB(0,0,0);
    beeper.note(69,0.4,0.1);
    setRGB(255,255,255);
    beeper.note(67,0.9,0.1);

    setRGB(255,255,0);
    beeper.note(65,0.4,0.1);
    setRGB(0,0,0);
    beeper.note(65,0.4,0.1);
    setRGB(255,255,255);
    beeper.note(64,0.4,0.1);
    setRGB(0,0,0);
    beeper.note(64,0.4,0.1);
    setRGB(255,255,255);
    beeper.note(62,0.4,0.1);
    setRGB(0,0,0);
    beeper.note(62,0.4,0.1);
    setRGB(255,255,255);
    beeper.note(60,0.9,0.1);

    setRGB(255,255,0);
    beeper.note(67,0.4,0.1);
    setRGB(0,0,0);
    beeper.note(67,0.4,0.1);
    setRGB(255,255,255);
    beeper.note(65,0.4,0.1);
    setRGB(0,0,0);
    beeper.note(65,0.4,0.1);
    setRGB(255,255,0);
    beeper.note(64,0.4,0.1);
    setRGB(0,0,0);
    beeper.note(64,0.4,0.1);
    setRGB(255,255,255);
    beeper.note(62,0.9,0.1);

    setRGB(255,255,0);
    beeper.note(67,0.4,0.1);
    setRGB(0,0,0);
    beeper.note(67,0.4,0.1);
    setRGB(255,255,255);
    beeper.note(65,0.4,0.1);
    setRGB(0,0,0);
    beeper.note(65,0.4,0.1);
    setRGB(255,255,0);
    beeper.note(64,0.4,0.1);
    setRGB(0,0,0);
    beeper.note(64,0.4,0.1);
    setRGB(255,255,255);
    beeper.note(62,0.9,0.1);

    setRGB(255,255,0);
    beeper.note(60,0.4,0.1);
    setRGB(0,0,0);
    beeper.note(60,0.4,0.1);
    setRGB(255,255,255);
    beeper.note(67,0.4,0.1);
    setRGB(0,0,0);
    beeper.note(67,0.4,0.1);
    setRGB(255,255,0);
    beeper.note(69,0.4,0.1);
    setRGB(0,0,0);
    beeper.note(69,0.4,0.1);
    setRGB(255,255,255);
    beeper.note(67,0.9,0.1);

    setRGB(255,255,0);
    beeper.note(65,0.4,0.1);
    setRGB(0,0,0);
    beeper.note(65,0.4,0.1);
    setRGB(255,255,255);
    beeper.note(64,0.4,0.1);
    setRGB(0,0,0);
    beeper.note(64,0.4,0.1);
    setRGB(255,255,0);
    beeper.note(62,0.4,0.1);
    setRGB(0,0,0);
    beeper.note(62,0.4,0.1);
    setRGB(255,255,255);
    beeper.note(60,0.9,0.1);    
    setRGB(0,0,0);
}

void playSpider() {
    setRGB(255,0,0);
    beeper.note(60,0.4,0.1);
    setRGB(0,0,255);
    beeper.note(60,0.4,0.1);
    setRGB(255,0,0);
    beeper.note(60,0.4,0.1);
    setRGB(0,0,255);
    beeper.note(62,0.4,0.1);
    setRGB(255,0,0);
    beeper.note(64,0.4,0.1);
    setRGB(0,0,255);
    beeper.note(64,0.4,0.1);
    setRGB(255,0,0);
    beeper.note(64,0.9,0.1);

    setRGB(0,0,255);
    beeper.note(62,0.4,0.1);
    setRGB(255,0,0);
    beeper.note(62,0.4,0.1);
    setRGB(0,0,255);
    beeper.note(62,0.4,0.1);
    setRGB(255,0,0);
    beeper.note(64,0.4,0.1);
    setRGB(0,0,255);
    beeper.note(60,0.9,0.1);
    setRGB(255,0,0);
    beeper.note(60,0.9,0.1);

    setRGB(0,0,255);
    beeper.note(64,0.9,0.1);
    setRGB(255,0,0);
    beeper.note(64,0.4,0.1);
    setRGB(0,0,255);
    beeper.note(65,0.4,0.1);
    setRGB(255,0,0);
    beeper.note(67,0.9,0.1);
    setRGB(0,0,255);
    beeper.note(67,0.4,0.1);
    setRGB(255,0,0);
    beeper.note(67,0.4,0.1);
    setRGB(0,0,255);
    beeper.note(65,0.4,0.1);
    setRGB(255,0,0);
    beeper.note(65,0.4,0.1);
    setRGB(0,0,255);
    beeper.note(65,0.4,0.1);
    setRGB(255,0,0);
    beeper.note(67,0.4,0.1);
    setRGB(0,0,255);
    beeper.note(64,1.9,0.1);

    setRGB(255,0,0);
    beeper.note(72,0.9,0.1);
    setRGB(0,0,255);
    beeper.note(72,0.9,0.1);
    setRGB(255,0,0);
    beeper.note(71,0.9,0.1);
    setRGB(0,0,255);
    beeper.note(71,0.4,0.1);
    setRGB(255,0,0);
    beeper.note(71,0.4,0.1);
    setRGB(0,0,255);
    beeper.note(69,0.4,0.1);
    setRGB(255,0,0);
    beeper.note(69,0.4,0.1);
    setRGB(0,0,255);
    beeper.note(69,0.4,0.1);
    setRGB(255,0,0);
    beeper.note(69,0.4,0.1);
    setRGB(0,0,255);
    beeper.note(67,1.9,0.1);

    setRGB(255,0,0);
    beeper.note(60,0.4,0.1);
    setRGB(0,0,255);
    beeper.note(60,0.4,0.1);
    setRGB(255,0,0);
    beeper.note(60,0.4,0.1);
    setRGB(0,0,255);
    beeper.note(62,0.4,0.1);
    setRGB(255,0,0);
    beeper.note(64,0.4,0.1);
    setRGB(0,0,255);
    beeper.note(64,0.4,0.1);
    setRGB(255,0,0);
    beeper.note(64,0.9,0.1);

    setRGB(0,0,255);
    beeper.note(62,0.4,0.1);
    setRGB(255,0,0);
    beeper.note(62,0.4,0.1);
    setRGB(0,0,255);
    beeper.note(62,0.4,0.1);
    setRGB(255,0,0);
    beeper.note(64,0.4,0.1);
    setRGB(0,0,255);
    beeper.note(60,1.9,0.1);
    setRGB(0,0,0);
}


int main(void) {

#ifdef EK_DEBUG
    _debug.printf("Main init\r\n");
#endif

    serial = new USBSerial(0x2341, 0xbeef);

    currentState = IDLE_STATE;
    
    // Indicate power on with green LED
    setRGB(0,255,0);

    // Set accelerometer range to +/-8G
    //acc.setRange(_accelerometerRange);

#ifdef _DEBUG
    _debug.printf("Attaching serial callback\r\n");    
#endif
    // Attach ISR function
    serial->attach(serialCallback);

    setRGB(0,255,0);

    // Remain in state machine forever
    while(1)
    {
        switch (currentState) {
            case IDLE_STATE:
                // TODO add battery status monitoring, USB connected?
                if (tsi.readDistance() > 30) {
                    playSpider();
                } else if (tsi.readDistance() > 0 && tsi.readDistance() < 10) {
                    playTwinkle();                    
                }
                break;
            case SET_RGB_STATE:
                // Interpret data in processing buffer and set LEDs
                setRGB(redVal,greenVal,blueVal);
                currentState = IDLE_STATE;  // Done, switch back
                break;
            case GET_INFO_STATE:
                serial->printf("{\"datatype\":\"StatusMessage\",\"data\":\"Music Box\"}\r\n");
                currentState = IDLE_STATE;  // Done, switch back
                break;
            case GET_HELP_STATE:
                serial->printf("{\"datatype\":\"HelpMessage\",\"data\":\"%s\"}\r\n",helpString);
                currentState = IDLE_STATE;  // Done, switch back
                break;
            default:
                serial->printf("This is unexpected and should not be seen\r\n");
        }
        wait_ms(100);
    }
}
