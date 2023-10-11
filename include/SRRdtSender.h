#ifndef SR_RDT_SENDER_H
#define SR_RDT_SENDER_H

#include "DataStructure.h"
#include "RdtSender.h"
#include "SRSenderPacketWindow.h"

class SRRdtSender: public RdtSender {

private:
    int base;
    int nextSeqNum;
    SRSenderPacketWindow packetWindow;

public:
    SRRdtSender(): base(1), nextSeqNum(1), packetWindow(Configuration::WINDOW_SIZE, &base, &nextSeqNum) {};
    virtual ~SRRdtSender() = default;

    bool getWaitingState() override;
	bool send(const Message &message) override;
	void receive(const Packet &ackPkt) override;	
	void timeoutHandler(int seqNum) override;	

};

#endif