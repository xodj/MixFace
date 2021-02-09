TEMPLATE = subdirs

SUBDIRS += \
    OSCPACK \
    DebugLibrary \
    MixFaceLibrary \
    MixFaceQtWidgets

MixFaceQtWidgets.depends = OSCPACK DebugLibrary MixFaceLibrary
MixFaceLibrary.depends = OSCPACK DebugLibrary
