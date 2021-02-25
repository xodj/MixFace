TEMPLATE = subdirs

SUBDIRS += \
    OSCPACK \
    DebugLibrary \
    MixFaceLibrary \
    MixFaceQtWidgets \
    MixFaceWXWidgets

MixFaceQtWidgets.depends = OSCPACK DebugLibrary MixFaceLibrary
MixFaceWXWidgets.depends = OSCPACK DebugLibrary MixFaceLibrary
MixFaceLibrary.depends = OSCPACK DebugLibrary
