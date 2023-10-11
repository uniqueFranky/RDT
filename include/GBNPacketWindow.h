#ifndef GBN_PACKET_WINDOW_H
#define GBN_PACKET_WINDOW_H

#include "DataStructure.h"
#include "PacketWindow.h"

class GBNPacketWindow: public PacketWindow {

public:
    GBNPacketWindow(int size, int *_base, int *_nextSeqNum): PacketWindow(size, _base, _nextSeqNum, Configuration::LOG_PATH){};
    ~GBNPacketWindow() = default;
    std::vector<int> setAcked(int seqNum); // 累计确认
    std::vector<Packet> getGBNPackets(); // 获取所有未确认的packet
};

#endif