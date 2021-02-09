TEMPLATE = subdirs

SUBDIRS += \
    oscpack \
    DebugLibrary \
    MixFaceLibrary \
    MixFaceQtWidgets

MixFaceQtWidgets.depends = OSCPACK DebugLibrary MixFaceLibrary
MixFaceLibrary.depends = OSCPACK DebugLibrary
