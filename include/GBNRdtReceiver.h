#ifndef GBN_RDT_RECEIVER_H
#define GBN_RDT_RECEIVER_H

#include "RdtReceiver.h"
class GBNRdtReceiver: public RdtReceiver {
private:
    int expectSeqNum;

public:
    GBNRdtReceiver(): expectSeqNum(0) {};
    ~GBNRdtReceiver() = default;
    void receive(const Packet &packet) override;
};

#endif