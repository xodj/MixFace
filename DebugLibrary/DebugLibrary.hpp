#ifndef DEBUGLIBRARY_H
#define DEBUGLIBRARY_H

#include <iostream>
#include <ctime>

#define def_caps "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define def_no_caps "abcdefghijklmnopqrstuvwxyz"

using namespace std;

class DebugLibrary {
public:
    DebugLibrary(int argc_ = 0, char *argv_[] = 0);
    void sendMessage(string message_ = "Empty message!", int debugLevel_ = 5);

    int argc;
    char **argv;
    float dpi = -1.f;

private:
    void setDebugLevel(int debugLevel_);
    string getCurrentTime();

    int debugLevel = 0;
};

#endif // DEBUGLIBRARY_H
