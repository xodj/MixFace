#ifndef MIXFACESTATICMATH_H
#define MIXFACESTATICMATH_H
#include <math.h>

#define CLAMP(x, min, max) ((x) < (min) ? (min) : ((x) > (max) ? (max) : (x)))

static inline float round2(float number, int scale) {
        int pow = 10;
        for (int i = 1; i < scale; i++) pow *= 10;
        float tmp = number * pow;
        return (float) (int) ((tmp - (int) tmp) >= 0.5f ? tmp + 1 : tmp) / pow;
}

static inline float float2db(float value) {
    //Calculate db value
    float db = 0;
    if (value >= 0.5) db = value * 40. - 30.;
    else if (value >= 0.25) db = value * 80. - 50.;
    else if (value >= 0.0625) db = value * 160. - 70.;
    else if (value >= 0.0) db = value * 480. - 90.;
    return db;
}

static inline float db2float(float d) {
    //Calculate float value
    float f = 0;
    if (d < -60.) f = (d + 90.) / 480.;
    else if (d < -30.) f = (d + 70.) / 160.;
    else if (d < -10.) f = (d + 50.) / 80.;
    else if (d <= 10.) f = (d + 30.) / 40.;
    return f;
}

static inline float meterLogToFloatRatio(float meter) {
    meter = log10(meter) * 20;
    meter += 95;
    meter /= 100;
    if (meter > 0.949f) meter = 1.f;
    if (meter < 0.1f) meter = 0.f;
    return meter;
}

static inline float gateGainReductionToFloatRatio(float gate_) {
    float f = 0.f;
    if (gate_ < 0.708000f) {
        gate_ = log10(gate_) * 20; //gain reduction
        gate_ = -gate_; //inverse to positive
        gate_ += 1; // + 1 db for visibility
        gate_ = gate_ / 60; //decrease to 60 db
        if(gate_ > 1.f) gate_ = 1.f; //disable overload
        f = gate_;
    }
    return f;
}

static inline float compGainReductionToFloatRatio(float comp_) {
    float f = 0.f;
    if (comp_ < 0.998f) {
        comp_ = log10(comp_) * 20; //gain reduction
        comp_ = -comp_; //inverse to positive
        comp_ = comp_ / 18; //decrease to 18 db
        if(comp_ > 1.f) comp_ = 1.f; //disable overload
        f = comp_;
    }
    return f;
}

static inline float int2ratio(int value) {
    float ratio = 100.0;
    switch (value) {
    case 0: ratio = 1.1; break;
    case 1: ratio = 1.3; break;
    case 2: ratio = 1.5; break;
    case 3: ratio = 2.0; break;
    case 4: ratio = 2.5; break;
    case 5: ratio = 3.0; break;
    case 6: ratio = 4.0; break;
    case 7: ratio = 5.0; break;
    case 8: ratio = 7.0; break;
    case 9: ratio = 10.0; break;
    case 10: ratio = 20.0; break;
    case 11: ratio = 100.0; break;
    }
    return ratio;
}

static inline int ratio2int(float value) {
    int ratio = 0;
    if (value==1.1) ratio = 0;
    else if (value==1.3) ratio = 1;
    else if (value==1.5) ratio = 2;
    else if (value==2.0) ratio = 3;
    else if (value==2.5) ratio = 4;
    else if (value==3.0) ratio = 5;
    else if (value==4.0) ratio = 6;
    else if (value==5.0) ratio = 7;
    else if (value==7.0) ratio = 8;
    else if (value==10.0) ratio = 9;
    else if (value==20.0) ratio = 10;
    else if (value==100.0) ratio = 11;
    return ratio;
}

static inline float hold2ms(int value) {
    float ms = 0;
    switch (value) {
    case 0: ms = 0.02; break;
    case 1: ms = 0.02; break;
    case 2: ms = 0.03; break;
    case 3: ms = 0.03; break;
    case 4: ms = 0.03; break;
    case 5: ms = 0.04; break;
    case 6: ms = 0.04; break;
    case 7: ms = 0.04; break;
    case 8: ms = 0.05; break;
    case 9: ms = 0.06; break;
    case 10: ms = 0.06; break;
    case 11: ms = 0.07; break;
    case 12: ms = 0.08; break;
    case 13: ms = 0.09; break;
    case 14: ms = 0.10; break;
    case 15: ms = 0.11; break;
    case 16: ms = 0.13; break;
    case 17: ms = 0.14; break;
    case 18: ms = 0.16; break;
    case 19: ms = 0.18; break;
    case 20: ms = 0.20; break;
    case 21: ms = 0.22; break;
    case 22: ms = 0.25; break;
    case 23: ms = 0.28; break;
    case 24: ms = 0.32; break;
    case 25: ms = 0.36; break;
    case 26: ms = 0.40; break;
    case 27: ms = 0.45; break;
    case 28: ms = 0.50; break;
    case 29: ms = 0.56; break;
    case 30: ms = 0.63; break;
    case 31: ms = 0.71; break;
    case 32: ms = 0.60; break;
    case 33: ms = 0.89; break;
    case 34: ms = 1.00; break;
    case 35: ms = 1.12; break;
    case 36: ms = 1.26; break;
    case 37: ms = 1.42; break;
    case 38: ms = 1.59; break;
    case 39: ms = 1.78; break;
    case 40: ms = 2.00; break;
    case 41: ms = 2.24; break;
    case 42: ms = 2.52; break;
    case 43: ms = 2.83; break;
    case 44: ms = 3.17; break;
    case 45: ms = 3.56; break;
    case 46: ms = 3.99; break;
    case 47: ms = 4.48; break;
    case 48: ms = 5.02; break;
    case 49: ms = 5.64; break;
    case 50: ms = 6.32; break;
    case 51: ms = 7.10; break;
    case 52: ms = 7.96; break;
    case 53: ms = 8.93; break;
    case 54: ms = 10.0; break;
    case 55: ms = 11.2; break;
    case 56: ms = 12.6; break;
    case 57: ms = 14.1; break;
    case 58: ms = 15.8; break;
    case 59: ms = 17.8; break;
    case 60: ms = 20.0; break;
    case 61: ms = 22.4; break;
    case 62: ms = 25.1; break;
    case 63: ms = 28.2; break;
    case 64: ms = 31.7; break;
    case 65: ms = 35.5; break;
    case 66: ms = 39.9; break;
    case 67: ms = 44.7; break;
    case 68: ms = 50.2; break;
    case 69: ms = 56.3; break;
    case 70: ms = 63.2; break;
    case 71: ms = 70.9; break;
    case 72: ms = 79.6; break;
    case 73: ms = 89.3; break;
    case 74: ms = 100; break;
    case 75: ms = 112; break;
    case 76: ms = 126; break;
    case 77: ms = 141; break;
    case 78: ms = 158; break;
    case 79: ms = 178; break;
    case 80: ms = 200; break;
    case 81: ms = 224; break;
    case 82: ms = 251; break;
    case 83: ms = 282; break;
    case 84: ms = 316; break;
    case 85: ms = 355; break;
    case 86: ms = 399; break;
    case 87: ms = 447; break;
    case 88: ms = 502; break;
    case 89: ms = 563; break;
    case 90: ms = 632; break;
    case 91: ms = 709; break;
    case 92: ms = 796; break;
    case 93: ms = 893; break;
    case 94: ms = 1002; break;
    case 95: ms = 1124; break;
    case 96: ms = 1261; break;
    case 97: ms = 1415; break;
    case 98: ms = 1588; break;
    case 99: ms = 1782; break;
    case 100: ms = 2000; break;
    }
    return ms;
}

static inline int release2ms(int value) {
    int ms = 0;
    switch (value) {
    case 0: ms = 5; break;
    case 1: ms = 5; break;
    case 2: ms = 6; break;
    case 3: ms = 6; break;
    case 4: ms = 7; break;
    case 5: ms = 7; break;
    case 6: ms = 7; break;
    case 7: ms = 8; break;
    case 8: ms = 9; break;
    case 9: ms = 9; break;
    case 10: ms = 10; break;
    case 11: ms = 10; break;
    case 12: ms = 11; break;
    case 13: ms = 12; break;
    case 14: ms = 13; break;
    case 15: ms = 14; break;
    case 16: ms = 15; break;
    case 17: ms = 16; break;
    case 18: ms = 17; break;
    case 19: ms = 18; break;
    case 20: ms = 19; break;
    case 21: ms = 20; break;
    case 22: ms = 22; break;
    case 23: ms = 23; break;
    case 24: ms = 25; break;
    case 25: ms = 27; break;
    case 26: ms = 28; break;
    case 27: ms = 30; break;
    case 28: ms = 32; break;
    case 29: ms = 35; break;
    case 30: ms = 37; break;
    case 31: ms = 40; break;
    case 32: ms = 42; break;
    case 33: ms = 45; break;
    case 34: ms = 49; break;
    case 35: ms = 52; break;
    case 36: ms = 55; break;
    case 37: ms = 59; break;
    case 38: ms = 63; break;
    case 39: ms = 68; break;
    case 40: ms = 72; break;
    case 41: ms = 77; break;
    case 42: ms = 83; break;
    case 43: ms = 89; break;
    case 44: ms = 95; break;
    case 45: ms = 101; break;
    case 46: ms = 108; break;
    case 47: ms = 116; break;
    case 48: ms = 124; break;
    case 49: ms = 132; break;
    case 50: ms = 141; break;
    case 51: ms = 151; break;
    case 52: ms = 162; break;
    case 53: ms = 173; break;
    case 54: ms = 185; break;
    case 55: ms = 198; break;
    case 56: ms = 211; break;
    case 57: ms = 226; break;
    case 58: ms = 241; break;
    case 59: ms = 258; break;
    case 60: ms = 276; break;
    case 61: ms = 295; break;
    case 62: ms = 315; break;
    case 63: ms = 337; break;
    case 64: ms = 361; break;
    case 65: ms = 385; break;
    case 66: ms = 412; break;
    case 67: ms = 441; break;
    case 68: ms = 471; break;
    case 69: ms = 504; break;
    case 70: ms = 538; break;
    case 71: ms = 576; break;
    case 72: ms = 615; break;
    case 73: ms = 658; break;
    case 74: ms = 703; break;
    case 75: ms = 752; break;
    case 76: ms = 804; break;
    case 77: ms = 860; break;
    case 78: ms = 919; break;
    case 79: ms = 983; break;
    case 80: ms = 1051; break;
    case 81: ms = 1123; break;
    case 82: ms = 1201; break;
    case 83: ms = 1284; break;
    case 84: ms = 1373; break;
    case 85: ms = 1468; break;
    case 86: ms = 1569; break;
    case 87: ms = 1677; break;
    case 88: ms = 1793; break;
    case 89: ms = 1917; break;
    case 90: ms = 2050; break;
    case 91: ms = 2192; break;
    case 92: ms = 2343; break;
    case 93: ms = 2505; break;
    case 94: ms = 2678; break;
    case 95: ms = 2864; break;
    case 96: ms = 3062; break;
    case 97: ms = 3273; break;
    case 98: ms = 3499; break;
    case 99: ms = 3741; break;
    case 100: ms = 4000; break;
    }
    return ms;
}

#include <string>

using namespace std;

static inline string index2filtertype(int value) {
    string filtertype;
    switch (value) {
    case 0: filtertype = "LC6"; break;
    case 1: filtertype = "LC12"; break;
    case 2: filtertype = "HC6"; break;
    case 3: filtertype = "HC12"; break;
    case 4: filtertype = "1.0"; break;
    case 5: filtertype = "2.0"; break;
    case 6: filtertype = "3.0"; break;
    case 7: filtertype = "5.0"; break;
    case 8: filtertype = "10.0"; break;
    }
    return filtertype;
}
//dosnt works!
static inline float float2filterfreq(float value) {
    float filterfreq = value * 201;
    switch (int(value * 201)) {
    case 0: filterfreq = 20.f; break;
    case 1: filterfreq = 20.7; break;
    }
    return filterfreq;
}
#endif // MIXFACESTATICMATH_H
