#ifndef GO_BACK_N_RDT_SENDER_H
#define GO_BACK_N_RDT_SENDER_H

#include "DataStructure.h"
#include "GBNPacketWindow.h"
#include "Global.h"

class GBNRdtSender: public RdtSender {
private:
    int base; // 最早未被确认的seqNum
    int nextSeqNum; // 下一次将要发送的seqNum
    GBNPacketWindow packetWindow; // 负责管理滑动窗口

public:
    GBNRdtSender(): base(1), nextSeqNum(1), packetWindow(Configuration::WINDOW_SIZE, &base, &nextSeqNum) {

    };
    ~GBNRdtSender() = default;

    bool getWaitingState() override;
	bool send(const Message &message) override;
	void receive(const Packet &ackPkt) override;	
	void timeoutHandler(int seqNum) override;			


};

#endif
