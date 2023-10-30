#include "SRSenderPacketWindow.h"
#include "DataStructure.h"
#include <vector>
#include <iostream>

std::vector<Packet> SRSenderPacketWindow::moveWindow() {
    std::vector<Packet> ret;
    printPacketWindow();
    while(!isEmpty() && window[head].ack) {
        ret.push_back(window[head].packet);
        popPacket();
    }
    return std::move(ret);
}

std::vector<Packet> SRSenderPacketWindow::setAcked(int seqNum) {
    std::vector<Packet> ret;
    if(inWindow(seqNum)) {
        window[getWindowIndex(seqNum)].ack = true;
        ret.push_back(window[getWindowIndex(seqNum)].packet);
        moveWindow();
    }
    return std::move(ret);
}

std::vector<Packet> SRSenderPacketWindow::getResendPackets() {
    throw "Not Implemented.";
}
