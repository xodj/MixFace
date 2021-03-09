#include "DebugLibrary.hpp"

DebugLibrary::DebugLibrary(int argc_, char *argv_[])
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
            if (string(argv[i]) == "-dpi"){
                dpi = atof(argv[i + 1]) / 96;
                if (dpi < 1) dpi = 1;
                if (dpi > 2) dpi = 2;
                sendMessage("DebugLibrary::DebugLibrary DPI ratio set to " + to_string(dpi),1);
            }
        }
    sendMessage(string("DebugLibrary::DebugLibrary Debuger loaded..."),1);
    setDebugLevel(debugLevelArg);
}

void DebugLibrary::sendMessage(string message_, int debugLevel_)
{
    string message = getCurrentTime() + " : " + message_;
    if (debugLevel_ <= debugLevel) {
        cout << message << endl;
    }
}

void DebugLibrary::setDebugLevel(int debugLevel_)
{
    debugLevel = debugLevel_;
    sendMessage("DebugLibrary::setDebugLevel Debuger level set to " + to_string(debugLevel), 1);
}

string DebugLibrary::getCurrentTime()
{
    time_t seconds = time(NULL);
    tm* timeinfo = localtime(&seconds);
    string curTime = asctime(timeinfo);
    curTime.resize(curTime.size() - 1);
    return curTime;
}
