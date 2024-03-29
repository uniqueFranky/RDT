#include "GBNPacketWindow.h"
#include "DataStructure.h"
#include <vector>

std::vector<Packet> GBNPacketWindow::setAcked(int seqNum) {
    std::vector<Packet> ret;

    if(inWindow(seqNum)) { // 当确认的seqNum在滑动窗口中时才是有效确认
        int lastSeqNum;
        do {
            window[head].ack = true;
            lastSeqNum = window[head].packet.seqnum;
            ret.push_back(window[head].packet);
            popPacket();
        } while(lastSeqNum != seqNum);

    }
    return std::move(ret);
}

std::vector<Packet> GBNPacketWindow::getResendPackets() {
    std::vector<Packet> ret;
    if(!isEmpty()) {
        int now = head;
        int last;
        do {
            ret.push_back(window[now].packet);
            last = now;
            now = (now + 1) % windowSize;
        } while(last != tail);
    }
    return std::move(ret);
}