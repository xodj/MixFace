#ifndef DEBUGLIBRARY_H
#define DEBUGLIBRARY_H

#include <iostream>
#include <ctime>
#include <boost/signals2.hpp>

#define def_caps "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define def_no_caps "abcdefghijklmnopqrstuvwxyz"

using namespace std;

typedef boost::signals2::signal<void(string, int)> signal_debug;
typedef signal_debug::slot_type signal_type_debug;

class DebugLibrary {
public:
    DebugLibrary(int argc_ = 0, char *argv_[] = 0);
    void sendMessage(string message_ = "Empty message!", int debugLevel_ = 5);

    int argc;
    char **argv;
    float dpi = 1.f;

private:
    void setDebugLevel(int debugLevel_);
    string getCurrentTime();

    int debugLevel = 0;
};

#endif // DEBUGLIBRARY_H
