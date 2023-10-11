#ifndef GO_BACK_N_RDT_SENDER_H
#define GO_BACK_N_RDT_SENDER_H

#include "DataStructure.h"
#include "GBNPacketWindow.h"
#include "Global.h"

class GBNRdtSender: public RdtSender {
private:
    int base;
    int nextSeqNum;
    GBNPacketWindow packetWindow;

public:
    GBNRdtSender(): base(0), nextSeqNum(0), packetWindow(Configuration::WINDOW_SIZE, &base, &nextSeqNum) {

    };
    ~GBNRdtSender() = default;

    bool getWaitingState();
	bool send(const Message &message);
	void receive(const Packet &ackPkt);	
	void timeoutHandler(int seqNum);			


};

#endif
