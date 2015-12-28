#ifndef MBED_BEEPER_H
#define MBED_BEEPER_H
 
#include "mbed.h"
 
namespace mbed {
 
/* Class: Beeper
 *  A class witch uses pwm to controle a beeper to generate sounds.
 */
class Beeper {
 
public:
    
    /* Constructor: Beeper
     *  Creates a new beeper object.
     * 
     * Variables:
     *  pin - The pin which is connected to the beeper.
     */
    Beeper (PinName pin);
 
    /* Function: note
     *  Plays a midi note for a given time.
     *
     * Variables:
     *  note - The note to play.
     *  time - The duration to play the note.
     */
    void note (int, float time, float pause=0.0);

    void setTone(int);
    void silence();

  private :    
    PwmOut _pwm;
  };
}
 
#endif

