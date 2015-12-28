#ifndef LIGHTNSOUND_H
#define LIGHTNSOUND_H

#define nC2  36
#define nDb2 37
#define nD2  38
#define nEb2 39
#define nE2  40
#define nF2  41
#define nGb2 42
#define nG2  43
#define nAb2 44
#define nA2  45
#define nBb2 46
#define nB2  47
#define nC3  48
#define nDb3 49
#define nD3  50
#define nEb3 51
#define nE3  52
#define nF3  53
#define nGb3 54
#define nG3  55
#define nAb3 56
#define nA3  57
#define nBb3 58
#define nB3  59
#define nC4  60
#define nDb4 61
#define nD4  62
#define nEb4 63
#define nE4  64
#define nF4  65
#define nGb4 66
#define nG4  67
#define nAb4 68
#define nA4  69
#define nBb4 70
#define nB4  71
#define nC5  72
#define nDb5 73
#define nD5  74
#define nEb5 75
#define nE5  76
#define nF5  77
#define nGb5 78
#define nG5  79
#define nAb5 80
#define nA5  81
#define nBb5 82
#define nB5  83

struct Lights {
    int r;
    int g;
    int b;
    int duration;  // 100 = "one beat"
};

struct Music {
    int note;  // -1 = pause
    int duration;   // 100 = "one beat"
};

struct Lights fivefourthsBlue[] =
{
    {255,   0,   0, 20},
    {  0,   0,   0, 80},
    {  0,   0, 255, 20},
    {  0,   0,   0, 80},
    {  0,   0, 255, 20},
    {  0,   0,   0, 80},
    {  0,   0, 255, 20},
    {  0,   0,   0, 80},
    {  0,   0, 255, 20},
    {  0,   0,   0, 80},
    {  0,   0,   0, -1}
};

struct Lights fourfourthsGreen[] =
{
    {255,   0,   0, 20},
    {  0,   0,   0, 80},
    {  0, 255,   0, 20},
    {  0,   0,   0, 80},
    {  0, 255,   0, 20},
    {  0,   0,   0, 80},
    {  0, 255,   0, 20},
    {  0,   0,   0, 80},
    {  0,   0,   0, -1}
};

struct Music jingleBells[] =
{
    {nG4, 80},
    {-1, 20},
    {nE5, 80},
    {-1, 20},
    {nD5, 80},
    {-1, 20},
    {nC5, 80},
    {-1, 20},

    {nG4, 280},
    {-1, 20},
    {nG4, 40},
    {-1, 10},
    {nG4, 40},
    {-1, 10},

    {nG4, 80},
    {-1, 20},
    {nE5, 80},
    {-1, 20},
    {nD5, 80},
    {-1, 20},
    {nC5, 80},
    {-1, 20},

    {nA4, 280},
    {-1, 20},
    {-1, 40},
    {-1, 10},
    {nA4, 40},
    {-1, 10},

    {nB4, 80},
    {-1, 20},
    {nF5, 80},
    {-1, 20},
    {nE5, 80},
    {-1, 20},
    {nD5, 80},
    {-1, 20},

    {nB4, 280},
    {-1, 20},
    {-1, 50},
    {nB4, 40},
    {-1, 10},

    {nG5, 80},
    {-1, 20},
    {nG5, 80},
    {-1, 20},
    {nF5, 80},
    {-1, 20},
    {nD5, 80},
    {-1, 20},

    {nE5, 280},
    {-1, 20},
    {nG4, 40},
    {-1, 10},
    {nG4, 40},
    {-1, 10},

    {nG4, 80},
    {-1, 20},
    {nE5, 80},
    {-1, 20},
    {nD5, 80},
    {-1, 20},
    {nC5, 80},
    {-1, 20},

    {nG4, 280},
    {-1, 20},
    {nG4, 40},
    {-1, 10},
    {nG4, 40},
    {-1, 10},

    {nG4, 80},
    {-1, 20},
    {nE5, 80},
    {-1, 20},
    {nD5, 80},
    {-1, 20},
    {nC5, 80},
    {-1, 20},

    {nA4, 280},
    {-1, 20},
    {-1, 50},
    {nA4, 40},
    {-1, 10},

    {nB4, 80},
    {-1, 20},
    {nF5, 80},
    {-1, 20},
    {nE5, 80},
    {-1, 20},
    {nD5, 80},
    {-1, 20},

    {nG5, 80},
    {-1, 20},
    {nG5, 80},
    {-1, 20},
    {nG5, 130},
    {-1, 20},
    {nG5, 40},
    {-1, 10},

    {nA5, 80},
    {-1, 20},
    {nG5, 80},
    {-1, 20},
    {nF5, 80},
    {-1, 20},
    {nD5, 80},
    {-1, 20},

    {nC5, 280},
    {-1, 20},
    {nG5, 80},
    {-1, 20},

    {nE5, 80},
    {-1, 20},
    {nE5, 80},
    {-1, 20},
    {nE5, 180},
    {-1, 20},

    {nE5, 80},
    {-1, 20},
    {nE5, 80},
    {-1, 20},
    {nE5, 180},
    {-1, 20},

    {nE5, 80},
    {-1, 20},
    {nG5, 80},
    {-1, 20},
    {nC5, 130},
    {-1, 20},
    {nD5, 40},
    {-1, 10},

    {nE5, 280},
    {-1, 20},
    {-1, 100},

    {nF5, 80},
    {-1, 20},
    {nF5, 80},
    {-1, 20},
    {nF5, 130},
    {-1, 20},
    {nF5, 40},
    {-1, 10},

    {nF5, 80},
    {-1, 20},
    {nE5, 80},
    {-1, 20},
    {nE5, 80},
    {-1, 20},
    {nE5, 40},
    {-1, 10},
    {nE5, 40},
    {-1, 10},

    {nE5, 80},
    {-1, 20},
    {nD5, 80},
    {-1, 20},
    {nD5, 80},
    {-1, 20},
    {nE5, 80},
    {-1, 20},

    {nD5, 180},
    {-1, 20},
    {nG5, 180},
    {-1, 20},

    {nE5, 80},
    {-1, 20},
    {nE5, 80},
    {-1, 20},
    {nE5, 180},
    {-1, 20},

    {nE5, 80},
    {-1, 20},
    {nE5, 80},
    {-1, 20},
    {nE5, 180},
    {-1, 20},

    {nE5, 80},
    {-1, 20},
    {nG5, 80},
    {-1, 20},
    {nC5, 130},
    {-1, 20},
    {nD5, 40},
    {-1, 10},

    {nE5, 280},
    {-1, 20},
    {-1, 100},

    {nF5, 80},
    {-1, 20},
    {nF5, 80},
    {-1, 20},
    {nF5, 130},
    {-1, 20},
    {nF5, 40},
    {-1, 10},

    {nF5, 80},
    {-1, 20},
    {nE5, 80},
    {-1, 20},
    {nE5, 80},
    {-1, 20},
    {nE5, 40},
    {-1, 10},
    {nE5, 40},
    {-1, 10},

    {nG5, 80},
    {-1, 20},
    {nG5, 80},
    {-1, 20},
    {nF5, 80},
    {-1, 20},
    {nD5, 80},
    {-1, 20},

    {nC5, 280},
    {-1, 20},
    {-1, 100},

    {-1, -1}  // Terminate on duration = -1
};

struct Music takeFive[] =
{
    // B part
    {nEb2, 70},
    {nGb3, 10},
    {nBb3, 10},
    {nEb4, 20},
    {-1, 50},
    {nEb2, 40},
    {nGb3, 10},
    {nBb3, 10},
    {nEb4, 40},
    {-1, 40},
    {nBb2, 100},
    {nF3, 10},
    {nGb3, 10},
    {nDb4, 80},

    {nEb2, 70},
    {nGb3, 10},
    {nBb3, 10},
    {nEb4, 20},
    {-1, 50},
    {nEb2, 40},
    {nGb3, 10},
    {nBb3, 10},
    {nEb4, 40},
    {-1, 40},
    {nBb2, 100},
    {nF3, 10},
    {nGb3, 10},
    {nDb4, 80},


    // A part
    {nEb2, 70},
    {nGb3, 10},
    {nBb3, 10},
    {nEb4, 20},
    {-1, 50},
    {nEb2, 40},
    {nGb3, 10},
    {nBb3, 10},
    {nEb4, 40},
    {-1, 40}, // End accomp.
    {nBb3, 45},
    {-1, 10},
    {nEb4, 35},
    {-1, 10},
    {nGb4, 45},
    {-1, 10},
    {nAb4, 35},
    {-1, 10},

    {nA4, 45},
    {-1, 10},
    {nBb4, 35},
    {-1, 10},
    {nA4, 45},
    {-1, 10},
    {nAb4, 35},
    {-1, 10},
    {nGb4, 40},
    {-1, 10},
    {-1, 50},

    {nBb3, 80},
    {nB3, 10},
    {nC4, 10},
    {nDb4, 40},
    {-1, 10},
    {-1, 50},
    {nEb4, 180},
    {-1, 20},
    {-1, 100},

    {nF4, 25},
    {nGb4, 25},
    {nF4, 25},
    {nEb4, 25},
    {nDb4, 40},
    {-1, 10},
    {-1, 50},
    {nEb4, 180},
    {-1, 20},
    {-1, 100},

    {nC4, 25},
    {nDb4, 25},
    {nC4, 25},
    {nBb3, 25},
    {nAb3, 40},
    {-1, 10},
    {-1, 50},
    {nBb3, 180},
    {-1, 20},
    {-1, 100},

    // A part (repeat)
    {nBb3, 45},
    {-1, 10},
    {nEb4, 35},
    {-1, 10},
    {nGb4, 45},
    {-1, 10},
    {nAb4, 35},
    {-1, 10},

    {nA4, 45},
    {-1, 10},
    {nBb4, 35},
    {-1, 10},
    {nA4, 45},
    {-1, 10},
    {nAb4, 35},
    {-1, 10},
    {nGb4, 40},
    {-1, 10},
    {-1, 50},

    {nBb3, 80},
    {nB3, 10},
    {nC4, 10},
    {nDb4, 40},
    {-1, 10},
    {-1, 50},
    {nEb4, 180},
    {-1, 20},
    {-1, 100},

    {nC4, 25},
    {nDb4, 25},
    {nC4, 25},
    {nBb3, 25},
    {nAb3, 40},
    {-1, 10},
    {-1, 50},
    {nBb3, 180},
    {-1, 20},
    {-1, 100},

    {nF4, 25},
    {nGb4, 25},
    {nF4, 25},
    {nEb4, 25},
    {nDb4, 40},
    {-1, 10},
    {-1, 50},
    {nEb4, 280},
    {-1, 20},
    
    {-1, 200},  // long pause

    // B part
    {nEb5, 70},
    {nGb5, 40},
    {-1, 50},
    {nEb5, 40},
    {nB4, 60},
    {-1, 40},
    {nAb4, 60},
    {nBb4, 40},
    {nB4, 60},
    {nC5, 40},

    {nDb5, 70},
    {nF5, 40},
    {-1, 50},
    {nDb5, 40},
    {nBb4, 60},
    {-1, 40},
    {nGb4, 60},
    {nAb4, 40},
    {nA4, 60},
    {nBb4, 40},

    {nB4, 70},
    {nEb5, 40},
    {-1, 50},
    {nB4, 40},
    {nAb4, 60},
    {-1, 40},
    {nF4, 60},
    {nGb4, 40},
    {nAb4, 60},
    {nA4, 40},

    {nBb4, 60},
    {nA4, 40},
    {nBb4, 60},
    {nB4, 40},
    {nDb5, 60},
    {-1, 40},
    {nDb5, 60},
    {nC5, 40},
    {nDb5, 60},
    {nD5, 40},

    // B part (repeat)
    {nEb5, 70},
    {nGb5, 40},
    {-1, 50},
    {nEb5, 40},
    {nB4, 60},
    {-1, 40},
    {nAb4, 60},
    {nBb4, 40},
    {nB4, 60},
    {nC5, 40},

    {nDb5, 70},
    {nF5, 40},
    {-1, 50},
    {nDb5, 40},
    {nBb4, 60},
    {-1, 40},
    {nGb4, 60},
    {nAb4, 40},
    {nA4, 60},
    {nBb4, 40},

    {nB4, 70},
    {nEb5, 40},
    {-1, 50},
    {nB4, 40},
    {nAb4, 60},
    {-1, 40},
    {nF4, 60},
    {nGb4, 40},
    {nDb5, 60},
    {nB4, 40},

    // End of B + A final
    {nBb4, 300},
    {-1, 100},
    {nGb4, 45},
    {-1, 10},
    {nAb4, 35},
    {-1, 10},

    {nA4, 45},
    {-1, 10},
    {nBb4, 35},
    {-1, 10},
    {nA4, 45},
    {-1, 10},
    {nAb4, 35},
    {-1, 10},
    {nGb4, 40},
    {-1, 10},
    {-1, 50},

    {nBb3, 80},
    {nB3, 10},
    {nC4, 10},
    {nDb4, 40},
    {-1, 10},
    {-1, 50},
    {nEb4, 180},
    {-1, 20},
    {-1, 100},

    {nF4, 25},
    {nGb4, 25},
    {nF4, 25},
    {nEb4, 25},
    {nDb4, 40},
    {-1, 10},
    {-1, 50},
    {nEb4, 180},
    {-1, 20},
    {-1, 100},

    {nC4, 25},
    {nDb4, 25},
    {nC4, 25},
    {nBb3, 25},
    {nAb3, 40},
    {-1, 10},
    {-1, 50},
    {nBb3, 180},
    {-1, 20},
    {-1, 100},

    // A part (repeat)
    {nBb3, 45},
    {-1, 10},
    {nEb4, 35},
    {-1, 10},
    {nGb4, 45},
    {-1, 10},
    {nAb4, 35},
    {-1, 10},

    {nA4, 45},
    {-1, 10},
    {nBb4, 35},
    {-1, 10},
    {nA4, 45},
    {-1, 10},
    {nAb4, 35},
    {-1, 10},
    {nGb4, 40},
    {-1, 10},
    {-1, 50},

    {nBb3, 80},
    {nB3, 10},
    {nC4, 10},
    {nDb4, 40},
    {-1, 10},
    {-1, 50},
    {nEb4, 180},
    {-1, 20},
    {-1, 100},

    {nC4, 25},
    {nDb4, 25},
    {nC4, 25},
    {nBb3, 25},
    {nAb3, 40},
    {-1, 10},
    {-1, 50},
    {nBb3, 180},
    {-1, 20},
    {-1, 100},

    {nF4, 25},
    {nGb4, 25},
    {nF4, 25},
    {nEb4, 25},
    {nDb4, 40},
    {-1, 10},
    {-1, 50},
    {nEb4, 280},
    {-1, 20},

    {-1, 500},

    {-1, -1}
};


#endif