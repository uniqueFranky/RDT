#ifndef SR_SENDER_PACKET_WINDOW_H
#define SR_SENDER_PACKET_WINDOW_H

#include "DataStructure.h"
#include "PacketWindow.h"
#include <vector>

class SRSenderPacketWindow: public PacketWindow {

private:
    std::vector<Packet> moveWindow();
    int getPreviousWindowIndex(int windowIndex) const;
    int getPreviousWindowIndex(int windowIndex, int cnt) const;

public:
    SRSenderPacketWindow(int size, int *base, int *nextSeqNum): PacketWindow(size, base, nextSeqNum, Configuration::SENDER_LOG_PATH) {};
    virtual ~SRSenderPacketWindow() = default;
    std::vector<Packet> setAcked(int seqNum) override;
    std::vector<Packet> getResendPackets() override; 
    bool inPreviousWindow(int seqNum);

};

#endif