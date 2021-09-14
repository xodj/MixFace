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

static inline float c201logScaleToFrequency(float value) {
    float filterfreq;
    switch (int(value * 200)) {
    case 0: filterfreq = 20.0; break;
    case 1: filterfreq = 20.7; break;
    case 2: filterfreq = 21.4; break;
    case 3: filterfreq = 22.2; break;
    case 4: filterfreq = 23.0; break;
    case 5: filterfreq = 23.8; break;
    case 6: filterfreq = 24.6; break;
    case 7: filterfreq = 25.5; break;
    case 8: filterfreq = 26.4; break;
    case 9: filterfreq = 27.3; break;
    case 10: filterfreq = 28.3; break;
    case 11: filterfreq = 29.2; break;
    case 12: filterfreq = 30.3; break;
    case 13: filterfreq = 31.3; break;
    case 14: filterfreq = 32.4; break;
    case 15: filterfreq = 33.6; break;
    case 16: filterfreq = 34.8; break;
    case 17: filterfreq = 36.0; break;
    case 18: filterfreq = 37.2; break;
    case 19: filterfreq = 38.6; break;
    case 20: filterfreq = 39.9; break;
    case 21: filterfreq = 41.3; break;
    case 22: filterfreq = 42.8; break;
    case 23: filterfreq = 44.3; break;
    case 24: filterfreq = 45.8; break;
    case 25: filterfreq = 47.4; break;
    case 26: filterfreq = 49.1; break;
    case 27: filterfreq = 50.8; break;
    case 28: filterfreq = 52.6; break;
    case 29: filterfreq = 54.5; break;
    case 30: filterfreq = 56.4; break;
    case 31: filterfreq = 58.3; break;
    case 32: filterfreq = 60.4; break;
    case 33: filterfreq = 62.5; break;
    case 34: filterfreq = 64.7; break;
    case 35: filterfreq = 67.0; break;
    case 36: filterfreq = 69.3; break;
    case 37: filterfreq = 71.8; break;
    case 38: filterfreq = 74.3; break;
    case 39: filterfreq = 76.9; break;
    case 40: filterfreq = 79.6; break;
    case 41: filterfreq = 82.4; break;
    case 42: filterfreq = 85.3; break;
    case 43: filterfreq = 88.3; break;
    case 44: filterfreq = 91.4; break;
    case 45: filterfreq = 94.6; break;
    case 46: filterfreq = 98.0; break;
    case 47: filterfreq = 101.4; break;
    case 48: filterfreq = 105.0; break;
    case 49: filterfreq = 108.7; break;
    case 50: filterfreq = 112.5; break;
    case 51: filterfreq = 116.4; break;
    case 52: filterfreq = 120.5; break;
    case 53: filterfreq = 124.7; break;
    case 54: filterfreq = 129.1; break;
    case 55: filterfreq = 133.7; break;
    case 56: filterfreq = 138.4; break;
    case 57: filterfreq = 143.2; break;
    case 58: filterfreq = 148.3; break;
    case 59: filterfreq = 153.5; break;
    case 60: filterfreq = 158.9; break;
    case 61: filterfreq = 164.4; break;
    case 62: filterfreq = 170.2; break;
    case 63: filterfreq = 176.2; break;
    case 64: filterfreq = 182.4; break;
    case 65: filterfreq = 188.8; break;
    case 66: filterfreq = 195.4; break;
    case 67: filterfreq = 202.3; break;
    case 68: filterfreq = 209.4; break;
    case 69: filterfreq = 216.8; break;
    case 70: filterfreq = 224.4; break;
    case 71: filterfreq = 232.3; break;
    case 72: filterfreq = 240.5; break;
    case 73: filterfreq = 248.9; break;
    case 74: filterfreq = 257.6; break;
    case 75: filterfreq = 266.7; break;
    case 76: filterfreq = 276.1; break;
    case 77: filterfreq = 285.8; break;
    case 78: filterfreq = 295.8; break;
    case 79: filterfreq = 306.2; break;
    case 80: filterfreq = 317.0; break;
    case 81: filterfreq = 328.1; break;
    case 82: filterfreq = 339.6; break;
    case 83: filterfreq = 351.6; break;
    case 84: filterfreq = 363.9; break;
    case 85: filterfreq = 376.7; break;
    case 86: filterfreq = 390.0; break;
    case 87: filterfreq = 403.7; break;
    case 88: filterfreq = 417.9; break;
    case 89: filterfreq = 432.5; break;
    case 90: filterfreq = 447.7; break;
    case 91: filterfreq = 463.5; break;
    case 92: filterfreq = 479.8; break;
    case 93: filterfreq = 496.6; break;
    case 94: filterfreq = 514.1; break;
    case 95: filterfreq = 532.1; break;
    case 96: filterfreq = 550.8; break;
    case 97: filterfreq = 570.2; break;
    case 98: filterfreq = 590.2; break;
    case 99: filterfreq = 611.0; break;
    case 100: filterfreq = 632.5; break;
    case 101: filterfreq = 654.7; break;
    case 102: filterfreq = 677.7; break;
    case 103: filterfreq = 701.5; break;
    case 104: filterfreq = 726.2; break;
    case 105: filterfreq = 751.7; break;
    case 106: filterfreq = 778.1; break;
    case 107: filterfreq = 805.4; break;
    case 108: filterfreq = 833.7; break;
    case 109: filterfreq = 863.0; break;
    case 110: filterfreq = 893.4; break;
    case 111: filterfreq = 924.8; break;
    case 112: filterfreq = 957.3; break;
    case 113: filterfreq = 990.9; break;
    case 114: filterfreq = 1020.0; break;
    case 115: filterfreq = 1060.0; break;
    case 116: filterfreq = 1090.0; break;
    case 117: filterfreq = 1130.0; break;
    case 118: filterfreq = 1170.0; break;
    case 119: filterfreq = 1210.0; break;
    case 120: filterfreq = 1260.0; break;
    case 121: filterfreq = 1300.0; break;
    case 122: filterfreq = 1350.0; break;
    case 123: filterfreq = 1390.0; break;
    case 124: filterfreq = 1440.0; break;
    case 125: filterfreq = 1490.0; break;
    case 126: filterfreq = 1550.0; break;
    case 127: filterfreq = 1600.0; break;
    case 128: filterfreq = 1660.0; break;
    case 129: filterfreq = 1720.0; break;
    case 130: filterfreq = 1780.0; break;
    case 131: filterfreq = 1840.0; break;
    case 132: filterfreq = 1910.0; break;
    case 133: filterfreq = 1970.0; break;
    case 134: filterfreq = 2040.0; break;
    case 135: filterfreq = 2110.0; break;
    case 136: filterfreq = 2190.0; break;
    case 137: filterfreq = 2270.0; break;
    case 138: filterfreq = 2340.0; break;
    case 139: filterfreq = 2430.0; break;
    case 140: filterfreq = 2510.0; break;
    case 141: filterfreq = 2600.0; break;
    case 142: filterfreq = 2690.0; break;
    case 143: filterfreq = 2790.0; break;
    case 144: filterfreq = 2890.0; break;
    case 145: filterfreq = 2990.0; break;
    case 146: filterfreq = 3090.0; break;
    case 147: filterfreq = 3200.0; break;
    case 148: filterfreq = 3310.0; break;
    case 149: filterfreq = 3430.0; break;
    case 150: filterfreq = 3550.0; break;
    case 151: filterfreq = 3680.0; break;
    case 152: filterfreq = 3810.0; break;
    case 153: filterfreq = 3940.0; break;
    case 154: filterfreq = 4080.0; break;
    case 155: filterfreq = 4220.0; break;
    case 156: filterfreq = 4370.0; break;
    case 157: filterfreq = 4520.0; break;
    case 158: filterfreq = 4680.0; break;
    case 159: filterfreq = 4850.0; break;
    case 160: filterfreq = 5020.0; break;
    case 161: filterfreq = 5200.0; break;
    case 162: filterfreq = 5380.0; break;
    case 163: filterfreq = 5570.0; break;
    case 164: filterfreq = 5760.0; break;
    case 165: filterfreq = 5970.0; break;
    case 166: filterfreq = 6180.0; break;
    case 167: filterfreq = 6390.0; break;
    case 168: filterfreq = 6620.0; break;
    case 169: filterfreq = 6850.0; break;
    case 170: filterfreq = 7090.0; break;
    case 171: filterfreq = 7340.0; break;
    case 172: filterfreq = 7600.0; break;
    case 173: filterfreq = 7870.0; break;
    case 174: filterfreq = 8140.0; break;
    case 175: filterfreq = 8430.0; break;
    case 176: filterfreq = 8730.0; break;
    case 177: filterfreq = 9030.0; break;
    case 178: filterfreq = 9350.0; break;
    case 179: filterfreq = 9680.0; break;
    case 180: filterfreq = 10020.0; break;
    case 181: filterfreq = 10370.0; break;
    case 182: filterfreq = 10740.0; break;
    case 183: filterfreq = 11110.0; break;
    case 184: filterfreq = 11500.0; break;
    case 185: filterfreq = 11910.0; break;
    case 186: filterfreq = 12330.0; break;
    case 187: filterfreq = 12760.0; break;
    case 188: filterfreq = 13210.0; break;
    case 189: filterfreq = 13670.0; break;
    case 190: filterfreq = 14150.0; break;
    case 191: filterfreq = 14650.0; break;
    case 192: filterfreq = 15170.0; break;
    case 193: filterfreq = 15700.0; break;
    case 194: filterfreq = 16250.0; break;
    case 195: filterfreq = 16820.0; break;
    case 196: filterfreq = 17410.0; break;
    case 197: filterfreq = 18030.0; break;
    case 198: filterfreq = 18660.0; break;
    case 199: filterfreq = 19320.0; break;
    case 200: filterfreq = 20000.0; break;
    default: filterfreq = -1.0; break;
    }
    return filterfreq;
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

static inline string c201logScaleToFrequencyString(float value) {
    string filterfreq;
    switch (int(value * 200)) {
    case 0: filterfreq = "20.0Hz"; break;
    case 1: filterfreq = "20.7Hz"; break;
    case 2: filterfreq = "21.4Hz"; break;
    case 3: filterfreq = "22.2Hz"; break;
    case 4: filterfreq = "23.0Hz"; break;
    case 5: filterfreq = "23.8Hz"; break;
    case 6: filterfreq = "24.6Hz"; break;
    case 7: filterfreq = "25.5Hz"; break;
    case 8: filterfreq = "26.4Hz"; break;
    case 9: filterfreq = "27.3Hz"; break;
    case 10: filterfreq = "28.3Hz"; break;
    case 11: filterfreq = "29.2Hz"; break;
    case 12: filterfreq = "30.3Hz"; break;
    case 13: filterfreq = "31.3Hz"; break;
    case 14: filterfreq = "32.4Hz"; break;
    case 15: filterfreq = "33.6Hz"; break;
    case 16: filterfreq = "34.8Hz"; break;
    case 17: filterfreq = "36.0Hz"; break;
    case 18: filterfreq = "37.2Hz"; break;
    case 19: filterfreq = "38.6Hz"; break;
    case 20: filterfreq = "39.9Hz"; break;
    case 21: filterfreq = "41.3Hz"; break;
    case 22: filterfreq = "42.8Hz"; break;
    case 23: filterfreq = "44.3Hz"; break;
    case 24: filterfreq = "45.8Hz"; break;
    case 25: filterfreq = "47.4Hz"; break;
    case 26: filterfreq = "49.1Hz"; break;
    case 27: filterfreq = "50.8Hz"; break;
    case 28: filterfreq = "52.6Hz"; break;
    case 29: filterfreq = "54.5Hz"; break;
    case 30: filterfreq = "56.4Hz"; break;
    case 31: filterfreq = "58.3Hz"; break;
    case 32: filterfreq = "60.4Hz"; break;
    case 33: filterfreq = "62.5Hz"; break;
    case 34: filterfreq = "64.7Hz"; break;
    case 35: filterfreq = "67.0Hz"; break;
    case 36: filterfreq = "69.3Hz"; break;
    case 37: filterfreq = "71.8Hz"; break;
    case 38: filterfreq = "74.3Hz"; break;
    case 39: filterfreq = "76.9Hz"; break;
    case 40: filterfreq = "79.6Hz"; break;
    case 41: filterfreq = "82.4Hz"; break;
    case 42: filterfreq = "85.3Hz"; break;
    case 43: filterfreq = "88.3Hz"; break;
    case 44: filterfreq = "91.4Hz"; break;
    case 45: filterfreq = "94.6Hz"; break;
    case 46: filterfreq = "98.0Hz"; break;
    case 47: filterfreq = "101.4Hz"; break;
    case 48: filterfreq = "105.0Hz"; break;
    case 49: filterfreq = "108.7Hz"; break;
    case 50: filterfreq = "112.5Hz"; break;
    case 51: filterfreq = "116.4Hz"; break;
    case 52: filterfreq = "120.5Hz"; break;
    case 53: filterfreq = "124.7Hz"; break;
    case 54: filterfreq = "129.1Hz"; break;
    case 55: filterfreq = "133.7Hz"; break;
    case 56: filterfreq = "138.4Hz"; break;
    case 57: filterfreq = "143.2Hz"; break;
    case 58: filterfreq = "148.3Hz"; break;
    case 59: filterfreq = "153.5Hz"; break;
    case 60: filterfreq = "158.9Hz"; break;
    case 61: filterfreq = "164.4Hz"; break;
    case 62: filterfreq = "170.2Hz"; break;
    case 63: filterfreq = "176.2Hz"; break;
    case 64: filterfreq = "182.4Hz"; break;
    case 65: filterfreq = "188.8Hz"; break;
    case 66: filterfreq = "195.4Hz"; break;
    case 67: filterfreq = "202.3Hz"; break;
    case 68: filterfreq = "209.4Hz"; break;
    case 69: filterfreq = "216.8Hz"; break;
    case 70: filterfreq = "224.4Hz"; break;
    case 71: filterfreq = "232.3Hz"; break;
    case 72: filterfreq = "240.5Hz"; break;
    case 73: filterfreq = "248.9Hz"; break;
    case 74: filterfreq = "257.6Hz"; break;
    case 75: filterfreq = "266.7Hz"; break;
    case 76: filterfreq = "276.1Hz"; break;
    case 77: filterfreq = "285.8Hz"; break;
    case 78: filterfreq = "295.8Hz"; break;
    case 79: filterfreq = "306.2Hz"; break;
    case 80: filterfreq = "317.0Hz"; break;
    case 81: filterfreq = "328.1Hz"; break;
    case 82: filterfreq = "339.6Hz"; break;
    case 83: filterfreq = "351.6Hz"; break;
    case 84: filterfreq = "363.9Hz"; break;
    case 85: filterfreq = "376.7Hz"; break;
    case 86: filterfreq = "390.0Hz"; break;
    case 87: filterfreq = "403.7Hz"; break;
    case 88: filterfreq = "417.9Hz"; break;
    case 89: filterfreq = "432.5Hz"; break;
    case 90: filterfreq = "447.7Hz"; break;
    case 91: filterfreq = "463.5Hz"; break;
    case 92: filterfreq = "479.8Hz"; break;
    case 93: filterfreq = "496.6Hz"; break;
    case 94: filterfreq = "514.1Hz"; break;
    case 95: filterfreq = "532.1Hz"; break;
    case 96: filterfreq = "550.8Hz"; break;
    case 97: filterfreq = "570.2Hz"; break;
    case 98: filterfreq = "590.2Hz"; break;
    case 99: filterfreq = "611.0Hz"; break;
    case 100: filterfreq = "632.5Hz"; break;
    case 101: filterfreq = "654.7Hz"; break;
    case 102: filterfreq = "677.7Hz"; break;
    case 103: filterfreq = "701.5Hz"; break;
    case 104: filterfreq = "726.2Hz"; break;
    case 105: filterfreq = "751.7Hz"; break;
    case 106: filterfreq = "778.1Hz"; break;
    case 107: filterfreq = "805.4Hz"; break;
    case 108: filterfreq = "833.7Hz"; break;
    case 109: filterfreq = "863.0Hz"; break;
    case 110: filterfreq = "893.4Hz"; break;
    case 111: filterfreq = "924.8Hz"; break;
    case 112: filterfreq = "957.3Hz"; break;
    case 113: filterfreq = "990.9Hz"; break;
    case 114: filterfreq = "1k02Hz"; break;
    case 115: filterfreq = "1k06Hz"; break;
    case 116: filterfreq = "1k09Hz"; break;
    case 117: filterfreq = "1k13Hz"; break;
    case 118: filterfreq = "1k17Hz"; break;
    case 119: filterfreq = "1k21Hz"; break;
    case 120: filterfreq = "1k26Hz"; break;
    case 121: filterfreq = "1k30Hz"; break;
    case 122: filterfreq = "1k35Hz"; break;
    case 123: filterfreq = "1k39Hz"; break;
    case 124: filterfreq = "1k44Hz"; break;
    case 125: filterfreq = "1k49Hz"; break;
    case 126: filterfreq = "1k55Hz"; break;
    case 127: filterfreq = "1k60Hz"; break;
    case 128: filterfreq = "1k66Hz"; break;
    case 129: filterfreq = "1k72Hz"; break;
    case 130: filterfreq = "1k78Hz"; break;
    case 131: filterfreq = "1k84Hz"; break;
    case 132: filterfreq = "1k91Hz"; break;
    case 133: filterfreq = "1k97Hz"; break;
    case 134: filterfreq = "2k04Hz"; break;
    case 135: filterfreq = "2k11Hz"; break;
    case 136: filterfreq = "2k19Hz"; break;
    case 137: filterfreq = "2k27Hz"; break;
    case 138: filterfreq = "2k34Hz"; break;
    case 139: filterfreq = "2k43Hz"; break;
    case 140: filterfreq = "2k51Hz"; break;
    case 141: filterfreq = "2k60Hz"; break;
    case 142: filterfreq = "2k69Hz"; break;
    case 143: filterfreq = "2k79Hz"; break;
    case 144: filterfreq = "2k89Hz"; break;
    case 145: filterfreq = "2k99Hz"; break;
    case 146: filterfreq = "3k09Hz"; break;
    case 147: filterfreq = "3k20Hz"; break;
    case 148: filterfreq = "3k31Hz"; break;
    case 149: filterfreq = "3k43Hz"; break;
    case 150: filterfreq = "3k55Hz"; break;
    case 151: filterfreq = "3k68Hz"; break;
    case 152: filterfreq = "3k81Hz"; break;
    case 153: filterfreq = "3k94Hz"; break;
    case 154: filterfreq = "4k08Hz"; break;
    case 155: filterfreq = "4k22Hz"; break;
    case 156: filterfreq = "4k37Hz"; break;
    case 157: filterfreq = "4k52Hz"; break;
    case 158: filterfreq = "4k68Hz"; break;
    case 159: filterfreq = "4k85Hz"; break;
    case 160: filterfreq = "5k02Hz"; break;
    case 161: filterfreq = "5k20Hz"; break;
    case 162: filterfreq = "5k38Hz"; break;
    case 163: filterfreq = "5k57Hz"; break;
    case 164: filterfreq = "5k76Hz"; break;
    case 165: filterfreq = "5k97Hz"; break;
    case 166: filterfreq = "6k18Hz"; break;
    case 167: filterfreq = "6k39Hz"; break;
    case 168: filterfreq = "6k62Hz"; break;
    case 169: filterfreq = "6k85Hz"; break;
    case 170: filterfreq = "7k09Hz"; break;
    case 171: filterfreq = "7k34Hz"; break;
    case 172: filterfreq = "7k60Hz"; break;
    case 173: filterfreq = "7k87Hz"; break;
    case 174: filterfreq = "8k14Hz"; break;
    case 175: filterfreq = "8k43Hz"; break;
    case 176: filterfreq = "8k73Hz"; break;
    case 177: filterfreq = "9k03Hz"; break;
    case 178: filterfreq = "9k35Hz"; break;
    case 179: filterfreq = "9k68Hz"; break;
    case 180: filterfreq = "10k02Hz"; break;
    case 181: filterfreq = "10k37Hz"; break;
    case 182: filterfreq = "10k74Hz"; break;
    case 183: filterfreq = "11k11Hz"; break;
    case 184: filterfreq = "11k50Hz"; break;
    case 185: filterfreq = "11k91Hz"; break;
    case 186: filterfreq = "12k33Hz"; break;
    case 187: filterfreq = "12k76Hz"; break;
    case 188: filterfreq = "13k21Hz"; break;
    case 189: filterfreq = "13k67Hz"; break;
    case 190: filterfreq = "14k15Hz"; break;
    case 191: filterfreq = "14k65Hz"; break;
    case 192: filterfreq = "15k17Hz"; break;
    case 193: filterfreq = "15k70Hz"; break;
    case 194: filterfreq = "16k25Hz"; break;
    case 195: filterfreq = "16k82Hz"; break;
    case 196: filterfreq = "17k41Hz"; break;
    case 197: filterfreq = "18k03Hz"; break;
    case 198: filterfreq = "18k66Hz"; break;
    case 199: filterfreq = "19k32Hz"; break;
    case 200: filterfreq = "20k00Hz"; break;
    default: filterfreq = "ERROR!"; break;
    }
    return filterfreq;
}
#endif // MIXFACESTATICMATH_H
