#ifndef GBN_PACKET_WINDOW_H
#define GBN_PACKET_WINDOW_H

#include "DataStructure.h"
#include "PacketWindow.h"

class GBNPacketWindow: public PacketWindow {

public:
    GBNPacketWindow(int size, int *_base, int *_nextSeqNum): PacketWindow(size, _base, _nextSeqNum){};
    ~GBNPacketWindow() = default;
    std::vector<int> setAcked(int seqNum);
    std::vector<Packet> getGBNPackets();
};

#endif