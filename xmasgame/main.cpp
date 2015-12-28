// "Christmas game" .. 
// When switched on, a random XYZ-vector will be calculated
// Jingle Bells start playing the intro in a loop
// Volume = 1/distance between Accel reading and XYZ vector (above)
// When within range, the device plays the full song
// Touch area does: ?
// LED does: ?


// TODO:
//   * Play music from "playlist" (best: converted MIDI file)
//   * Don't use wait() - use tick counter directly (prevent blocking)
//   * Simple vector math to do the distance calc
//   * Playlist should include LED settings and beep frequency/duration
// Possibly:
//   * Stretch goal:  handle other actions in playlist, e.g. "loop until touch(area)..."
//   * Different songs
//   * Other user interaction (e.g. "swipe santa away to another destination you can locate (XYZ)")
//   * Fading volume...
//   * Colors match music volume.. or follow the beat, e.g. "Green, red, red, red" on jingle bells?

#include "mbed.h"
#include "Beeper.h"

#include "USBSerial.h"  // Virtual serial port
#include "TSISensor.h"  // Touch sensor
#include "MMA8451Q.h"   // Accelerometer

#include "lightnsound.h"

#include <math.h>

enum STATE_TYPE
{
    ASK_FOR_MUSIC,
    FIND_ANGLE,
    PLAY_MUSIC,
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
uint8_t tsiDist;

/// Serial input buffer (128 = 2^7)
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

int16_t accXYZ[3];
int16_t dirXYZ[3];
float accLen;
float dirLen;
float dotDiff;
float dotProd;
float addedTime;

char helpString[] = 
    "Christmas game by Lars Gunder Knudsen.\r\n"
    "\r\n"
    "So you got this far, eh?... well.. I guess we better show some help :-)\r\n"
    "CMD      Description\r\n"
    "SETIDL   Reset the box to the idle state, e.g. {'SETIDL':1}\r\n"
    "GETINF   Get info from the box, e.g. {'GETINF':1}\r\n"
    "\r\n"
    "Usage:\r\n"
    "\r\n"
    "When in 'idle' mode, the box will blink blue to indicate it's waiting for user input\r\n"
    "on the touch area.  If you touch either side of the area, the box will go into\r\n"
    "'search for santa' mode, where you have to find the right angle (turn the box) after\r\n"
    "which, the box will start playing some music appropriate for the selected season.\r\n"
    "\r\n"
    "ENJOY!\r\n"
    "- Lars\r\n"
    "\r\n";

void handleCmdBuffer() {
    // For now - all commands are 6 chars - do a quick and dirty compare

#ifdef EK_DEBUG
    _debug.printf("Command buffer contains: '%s'\r\n",cmdBuffer);
#endif
    char *tmpPtr;

    // Example cmd:  {"SETRGB":[255,0,0]}
    // Cmd name starts at 2
    // Args start at 10
    tmpPtr = &cmdBuffer[2];

    if (strncmp(tmpPtr,"SETIDL",6) == 0) {
        currentState = ASK_FOR_MUSIC;
    } else if (strncmp(tmpPtr,"GETINF",6) == 0) {
        currentState = GET_INFO_STATE;
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


void playMusic(Lights* lights, Music* music, int bpm)
{
    Lights* lightpt = lights;
    Music* musicpt = music;
    int playing = 1;
    int nextLightTime = 0;
    int nextMusicTime = 0;
    int delayFactor = 60 * 10000 / bpm;

    int playTime;  // in us

    timer.start();
    while(playing) {
        playTime = timer.read_us();
        if (lights) {
            if (nextLightTime < playTime) {
                setRGB((*lightpt).r, (*lightpt).g, (*lightpt).b);
                nextLightTime += (*lightpt).duration * delayFactor;
                lightpt++;
                if ((*lightpt).duration == -1)
                    lightpt = lights;  // Go back to the beginning
            }
        }
        if (music) {
            if (nextMusicTime < playTime) {
                if ((*musicpt).note == -1) {
                    beeper.silence();
                } else {
                    beeper.setTone((*musicpt).note);
                }
                nextMusicTime += (*musicpt).duration * delayFactor;
                musicpt++;
                if ((*musicpt).duration == -1)
                    playing = 0;  // Stop playing.
            }
        }
    }

    beeper.silence();
    setRGB(0,0,0);
}

unsigned short isqrt(unsigned long a) {
    /*unsigned long*/ int rem = 0;
    int root = 0;
    int i;

    for (i = 0; i < 16; i++) {
        root <<= 1;
        rem <<= 2;
        rem += a >> 30;
        a <<= 2;

        if (root < rem) {
            root++;
            rem -= root;
            root++;
        }
    }

    return (unsigned short) (root >> 1);
}

float lengthVect(int16_t* xyz)
{
    return sqrt(((float)xyz[0] * (float)xyz[0]) + ((float)xyz[1] * (float)xyz[1]) + ((float)xyz[2] * (float)xyz[2]));
}

float dotVect(int16_t* xyz1, int16_t* xyz2)
{
    return ((float)xyz1[0] * (float)xyz2[0]) + ((float)xyz1[1] * (float)xyz2[1]) + ((float)xyz1[2] * (float)xyz2[2]);
}

Lights* selectedLights = 0;
Music* selectedMusic = 0;
int selectedBpm = 144;


int scanLight = 0;


int main(void) {

    setRGB(0,0,0);

#ifdef EK_DEBUG
    _debug.printf("Main init\r\n");
#endif

    serial = new USBSerial(0x2341, 0xbeef);

    currentState = ASK_FOR_MUSIC;
    
    // Set accelerometer range to +/-8G
    //acc.setRange(_accelerometerRange);

#ifdef _DEBUG
    _debug.printf("Attaching serial callback\r\n");    
#endif
    // Attach ISR function
    serial->attach(serialCallback);

    // Remain in state machine forever
    timer.start();

    while(1)
    {
        switch (currentState) {
            case ASK_FOR_MUSIC:
                if (timer.read() >= 0.5)
                    timer.reset();
                if (scanLight == 0 && timer.read() > 0.4) {
                    scanLight = 1;
                    setRGB(0,0,255);
                } else if (scanLight == 1 && timer.read() < 0.4) {
                    scanLight = 0;
                    setRGB(0,0,0);
                }
                tsiDist = tsi.readDistance();
                if (tsiDist > 30) {
                    selectedLights = fivefourthsBlue;
                    selectedMusic = takeFive;
                    selectedBpm = 180;
                    srand(tsiDist);
                    currentState = FIND_ANGLE;
                } else if (tsiDist > 0 && tsiDist < 10) {
                    selectedLights = fourfourthsGreen;
                    selectedMusic = jingleBells;
                    selectedBpm = 200;
                    srand(tsiDist);
                    currentState = FIND_ANGLE;
                }
                break;
            case FIND_ANGLE:
                dotDiff = 0;
                dirXYZ[0] = (rand() & 255) - 127;
                dirXYZ[1] = (rand() & 255) - 127;
                dirXYZ[2] = (rand() & 255) - 127;
                dirLen = lengthVect(dirXYZ);
                accLen = 0.f;
                dotProd = 0.f;
                addedTime = 0.f;
                timer.start();
                do {
                    acc.getAccAllAxis(accXYZ);
                    accLen = lengthVect(accXYZ);
                    dotProd = dotVect(dirXYZ, accXYZ);
                    dotDiff = 128.f + ((dotProd * 127.f) / (dirLen * accLen));
                    if(dotDiff<15)
                        setRGB(0,0,255);
                    addedTime += (dotDiff + 10) * timer.read();
                    timer.reset();
                    if (addedTime > 60) {
                        setRGB(255,0,0);
                        beeper.note(72, 0.1);
                        setRGB(0,0,0);
                        addedTime = 0.f;
                    } 
                } while (dotDiff < 250.f);
                // Interpret data in processing buffer and set LEDs
                currentState = PLAY_MUSIC;  // Done, switch back
                break;
            case PLAY_MUSIC:
                playMusic(selectedLights, selectedMusic, selectedBpm);
                currentState = ASK_FOR_MUSIC;  // Done, switch back
                break;
            case GET_INFO_STATE:
                serial->printf("{\"datatype\":\"StatusMessage\",\"data\":\"Christmas Game, v.2013.11.001\"}\r\n");
                currentState = ASK_FOR_MUSIC;  // Done, switch back
                break;
            case GET_HELP_STATE:
                serial->printf("{\"datatype\":\"HelpMessage\",\"data\":\"%s\"}\r\n",helpString);
                currentState = ASK_FOR_MUSIC;  // Done, switch back
                break;
            default:
                serial->printf("This is unexpected and should not be seen\r\n");
        }
        //wait_ms(100);
    }
}
