#ifndef SR_RECEIVER_PACKET_WINDOW_H
#define SR_RECEIVER_PACKET_WINDOW_H

#include "DataStructure.h"
#include "PacketWindow.h"
#include <cstddef>
#include <vector>

class SRReceiverPacketWindow: public PacketWindow {

private:
    int getPreviousWindowIndex(int windowIndex) const;
    int getPreviousWindowIndex(int windowIndex, int cnt) const;
    std::vector<Packet> moveWindow();

    
public:
    SRReceiverPacketWindow(int size, int *base): PacketWindow(size, base, nullptr, Configuration::RECEIVER_LOG_PATH) {
        for(int i = 0; i < size; i++) {
            window[i].packet.seqnum = -1;
        }
    };
    ~SRReceiverPacketWindow() = default;
    std::vector<Packet> setAcked(int seqNum) override;
    bool inPreviousWindow(int seqNum) const;
    bool addPacket(Packet) override;
    bool popPacket() override;
    bool inWindow(int seqNum) const override;
    std::vector<Packet> getResendPackets() override;
    void printPacketWindow() override;
};

#endif