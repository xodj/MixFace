#include <iostream>
#include "osc/OscOutboundPacketStream.h"
#include "ip/UdpSocket.h"
#include "osc/OscPacketListener.h"

#define def_caps "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define def_no_caps "abcdefghijklmnopqrstuvwxyz"

#define ANY_PORT -1
#define OUTPUT_BUFFER_SIZE 1024
#define PORT 10023

using namespace std;

void sendShutdownMessage(string ip)
{
    UdpSocket *udpSocket = new UdpSocket;
    udpSocket->Bind(IpEndpointName(IpEndpointName::ANY_ADDRESS, ANY_PORT));
    IpEndpointName host(ip.c_str(),PORT);
    char buffer[OUTPUT_BUFFER_SIZE];
    osc::OutboundPacketStream p(buffer, OUTPUT_BUFFER_SIZE);
    p.Clear();
    p << osc::BeginMessage("/shutdown") << osc::EndMessage;
    udpSocket->SendTo(host, p.Data(), p.Size());
}

int main(int argc = 0, char *argv[] = 0)
{
    string ip;
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
            if (string(argv[i]) == "-ip") {
                ip = argv[i + 1];
                cout << "Shutdown " << ip << "!" << endl;
                sendShutdownMessage(ip);
            }
        }
    return 0;
}
