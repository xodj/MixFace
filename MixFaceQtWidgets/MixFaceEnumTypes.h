#ifndef MIXFACEENUMTYPES_H
#define MIXFACEENUMTYPES_H

enum MixMode {
    mCtrl,
    mSrc,
    mEq,
    mDyn
};

enum SendMode {
    sendLR,
    sendMC,
    sendBUS
};

enum AssignedFaderMode {
    ALL,
    A,
    B,
    C,
    D,
    E,
    F
};

enum FaderType {
    f_channel,
    f_auxin,
    f_fxreturn,
    f_bus,
    f_matrix,
    f_lr,
    f_mc,
    f_dca,
    f_error
};

class EnumTypes {
public:
    FaderType getFaderType(int idx){
        FaderType ftype = f_error;
        if (idx < 32) ftype = f_channel;
        else if (idx < 40) ftype = f_auxin;
        else if (idx < 48) ftype = f_fxreturn;
        else if (idx < 64) ftype = f_bus;
        else if (idx < 70) ftype = f_matrix;
        else if (idx == 70) ftype = f_lr;
        else if (idx == 71) ftype = f_mc;
        else if (idx < 80) ftype = f_dca;
        return ftype;
    }
};

#endif // MIXFACEENUMTYPES_H
