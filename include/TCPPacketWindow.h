#ifndef TCP_PACKET_WINDOW_H
#define TCP_PACKET_WINDOW_H

#include "DataStructure.h"
#include "GBNPacketWindow.h"
#include "PacketWindow.h"
#include <vector>
class TCPPacketWindow: public GBNPacketWindow {

private:
    int redundantAckCnt;

public:
    TCPPacketWindow(int size, int *_base, int *_nextSeqNum): redundantAckCnt(0), GBNPacketWindow(size, _base, _nextSeqNum) {};
    ~TCPPacketWindow() = default;
    Packet getFastResendPacket();
    std::vector<Packet> setAcked(int seqNum) override;
    bool needFastRensend() const;
};

#endif