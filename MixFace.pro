TEMPLATE = subdirs

SUBDIRS += \
    OSCPACK \
    DebugLibrary \
    MixFaceLibrary \
    MixFaceQtWidgets

MixFace.depends = OSCPACK DebugLibrary MixFaceLibrary
MixFaceQtWidgets.depends = OSCPACK DebugLibrary MixFaceLibrary
MixFaceLibrary.depends = OSCPACK DebugLibrary
