#include "GBNPacketWindow.h"
#include "DataStructure.h"
#include <vector>

std::vector<int> GBNPacketWindow::setAcked(int seqNum) {
    std::vector<int> ret;

    if(inWindow(seqNum)) {
        int lastSeqNum;
        do {
            window[head].ack = true;
            lastSeqNum = window[head].packet.seqnum;
            ret.push_back(lastSeqNum);
            head = (head + 1) % windowSize;
        } while(lastSeqNum != seqNum);

        if((tail + 1) % windowSize == head) {
            empty = true;
        }
    }
    return std::move(ret);
}

std::vector<Packet> GBNPacketWindow::getGBNPackets() {
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