#ifndef SR_RDT_RECEIVER_H
#define SR_RDT_RECEIVER_H

#include "DataStructure.h"
#include "MultiOutput.h"
#include "RdtReceiver.h"
#include "SRReceiverPacketWindow.h"


class SRRdtReceiver: public RdtReceiver {
private:
    int base;
    int nextSeqNum;
    SRReceiverPacketWindow packetWindow;

public:
    SRRdtReceiver(): base(1), nextSeqNum(-1), packetWindow(Configuration::WINDOW_SIZE, &base) {

    };
    void receive(const Packet &packet) override;
};

#endif