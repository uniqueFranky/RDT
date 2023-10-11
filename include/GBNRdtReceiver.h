#ifndef GBN_RDT_RECEIVER_H
#define GBN_RDT_RECEIVER_H

#include "RdtReceiver.h"
class GBNRdtReceiver: public RdtReceiver {
private:
    int expectSeqNum; // 期望接受的seqNum

public:
    GBNRdtReceiver(): expectSeqNum(1) {};
    ~GBNRdtReceiver() = default;
    void receive(const Packet &packet) override;
};

#endif