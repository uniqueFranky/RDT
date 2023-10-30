#ifndef TCP_SENDER_H
#define TCP_SENDER_H

#include "RdtSender.h"
#include "TCPPacketWindow.h"

class TCPSender: public RdtSender {
private:
    int base; // 最早未被确认的seqNum
    int nextSeqNum; // 下一次将要发送的seqNum
    TCPPacketWindow packetWindow; // 负责管理滑动窗口

public:
    TCPSender(): base(1), nextSeqNum(1), packetWindow(Configuration::WINDOW_SIZE, &base, &nextSeqNum) {};
    ~TCPSender() = default;

    bool getWaitingState() override;
	bool send(const Message &message) override;
	void receive(const Packet &ackPkt) override;
	void timeoutHandler(int seqNum) override;
};
#endif