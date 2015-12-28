#include "mbed.h"

#if defined(TARGET_KL46Z)            

/*  ------ sample usage------

    #include "mbed.h"
    #include "SLCD.h"
    
    SLCD slcd;
    
    main()
    {
        slcd.printf("1234");    // standard printf function, only charaters in ASCII_TO_WF_CODIFICATION_TABLE will display
        slcd.putc("A");         // prints a single character 
        slcd.Write_Char('A');   // prints a single character
        slcd.All_Segments(y);   // y=1 for ALL segments on, 0 for ALL segments off  
        slcd.DPx(y);            // x=DP1 to DP3, y=1 for on 0 for off
        slcd.Colon(y);          // y=1 for on, 0 for off
        slcd.CharPosition=x;    // x=0 to 3, 0 is start position
        slcd.Home();            // sets next charater to posistion 0 (start)
        slcd.Contrast (x);      // set contrast x=0 - 15, 0 lightest, 15 darkest
        slcd.blink(x);          // set display to blink, 0-7 is blink rate (default = 3), -1 disables blink
        slcd.deepsleepEnable(x);// true (default) keeps the lcd enabled in deepsleep, false disables its 4MHz internal oscillator clock. Total power consumption ~= 40uA
    }   
*/

class SLCD : public Stream {
    public:
    SLCD();
    
    void Home (void);
    void Contrast (uint8_t lbContrast);
    void All_Segments (int);     
    void DP1 (int);
    void DP2 (int);
    void DP3 (int);
    void Colon (int);     
    uint8_t CharPosition;
    void blink(int blink = 3);
    void deepsleepEnable(bool enable);
    
    private:
    void Write_Char(char lbValue);
    void init();
    virtual int _putc(int c);
    virtual int _getc() {
        return 0;
    }  
};

#endif
