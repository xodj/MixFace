#ifndef DEBUGLIBRARY_H
#define DEBUGLIBRARY_H

#include <iostream>
#include <string.h>
#include <ctime>

#define def_caps "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define def_no_caps "abcdefghijklmnopqrstuvwxyz"

using namespace std;

class DebugLibrary {
public:
    DebugLibrary(int argc_ = 0, char *argv_[] = 0)
        : argc(argc_), argv(argv_)
    {
        int debugLevelArg = 0;
        if(argc != 0)
            for(int i = 0; i < argc; i++) {
                //no caps
                char caps[] = def_caps;
                char ncaps[] = def_no_caps;
                for(int x = 0; x < int(sizeof(argv[i])); x++)
                    for(int y = 0; y < int(sizeof (caps)); y++)
                        if (argv[i][x] == caps[y])
                            argv[i][x] = ncaps[y];
                //if debug define
                if (string(argv[i]) == "-debug")
                    if(atoi(argv[i + 1]) > -1 && atoi(argv[i + 1]) < 6)
                        debugLevelArg = atoi(argv[i + 1]);
                //if dpi define
                if (string(argv[i]) == "-dpi")
                    if(atoi(argv[i + 1]) > -1 && atoi(argv[i + 1]) < 6)
                        dpi = atof(argv[i + 1]);
            }
        sendMessage(string("DebugLibrary::DebugLibrary Debuger loaded..."),1);
        setDebugLevel(debugLevelArg);
    }

    void sendMessage(string message_, int debugLevel_)
    {
        string message = getCurrentTime() + " : " + message_;
        if (debugLevel_ <= debugLevel) {
            cout << message << endl;
        }
    }

    int argc;
    char **argv;
    float dpi = 1.f;

private:
    int debugLevel = 0;

    void setDebugLevel(int debugLevel_)
    {
        debugLevel = debugLevel_;
        sendMessage("DebugLibrary::setDebugLevel Debuger level set to " + to_string(debugLevel), 1);
    }

    string getCurrentTime()
    {
        time_t seconds = time(NULL);
        tm* timeinfo = localtime(&seconds);
        string curTime = asctime(timeinfo);
        curTime.resize(curTime.size() - 1);
        return curTime;
    }
};

#endif // DEBUGLIBRARY_H
