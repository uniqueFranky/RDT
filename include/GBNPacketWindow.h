#ifndef GBN_PACKET_WINDOW_H
#define GBN_PACKET_WINDOW_H

#include "DataStructure.h"
#include "PacketWindow.h"

class GBNPacketWindow: public PacketWindow {

private:

public:
    GBNPacketWindow(int size, int *_base, int *_nextSeqNum): PacketWindow(size, _base, _nextSeqNum, Configuration::SENDER_LOG_PATH){};
    ~GBNPacketWindow() = default;
    std::vector<Packet> getResendPackets() override; // 获取所有未确认的packet
    virtual std::vector<Packet> setAcked(int seqNum) override; // 累计确认
};

#endif