#include "TCPPacketWindow.h"
#include "DataStructure.h"

Packet TCPPacketWindow::getFastResendPacket() {
    if(redundantAckCnt < 3) {
        throw "No Packet Need Fast Resend.";
    }
    if(isEmpty()) {
        throw "No Packet Can Be Resent.";
    }
    redundantAckCnt = 0;
    return window[head].packet;
}

std::vector<Packet> TCPPacketWindow::setAcked(int seqNum) {
    std::vector<Packet> ret;

    if(inWindow(seqNum)) { // 当确认的seqNum在滑动窗口中时才是有效确认
        int lastSeqNum;
        do {
            window[head].ack = true;
            lastSeqNum = window[head].packet.seqnum;
            ret.push_back(window[head].packet);
            popPacket();
            redundantAckCnt = 0;
        } while(lastSeqNum != seqNum);

    } else { // 冗余ack
        redundantAckCnt++;
    }
    return std::move(ret);
}

bool TCPPacketWindow::needFastRensend() const {
    return redundantAckCnt >= 3 && !isEmpty();
}
