#include "SRReceiverPacketWindow.h"
#include "DataStructure.h"
#include <utility>
#include <vector>
#include <iostream>

int SRReceiverPacketWindow::getPreviousWindowIndex(int windowIndex) const {
    return windowIndex ? windowSize - 1 : windowIndex - 1;
}

int SRReceiverPacketWindow::getPreviousWindowIndex(int windowIndex, int cnt) const {
    int now = windowIndex;
    while(cnt--) {
        now = getPreviousWindowIndex(now);
    }
    return now;
}

std::vector<Packet> SRReceiverPacketWindow::moveWindow() {
    std::vector<Packet> ret;
    while(window[head].ack) {
        ret.push_back(window[head].packet);
        popPacket();
    }
    printPacketWindow();
    return std::move(ret);
}

std::vector<Packet> SRReceiverPacketWindow::setAcked(int seqNum) {
    std::vector<Packet> ret;
    if(inWindow(seqNum)) {
        window[getWindowIndex(seqNum)].ack = true;
        ret = moveWindow();
    }
    return std::move(ret);
}

bool SRReceiverPacketWindow::popPacket() {
    multiOutput.print("pop packet");
    multiOutput.printPacket(window[head]);
    window[head].ack = false;
    window[head].packet.seqnum = -1;
    head = (head + 1) % windowSize;
    *base = ((*base) + 1) % Configuration::MAX_SEQ_NUM;
    multiOutput.print("base = ", *base);
    return true;
}

bool SRReceiverPacketWindow::addPacket(Packet packet) {
    if(inWindow(packet.seqnum)) {
        multiOutput.print("add packet");
        window[getWindowIndex(packet.seqnum)].packet = packet;
        window[getWindowIndex(packet.seqnum)].ack = true;
        multiOutput.printPacket(window[getWindowIndex(packet.seqnum)]);
        multiOutput.print("base = ", *base);
        printPacketWindow();
        return true;
    } else {
        return false;
    }
}

bool SRReceiverPacketWindow::inWindow(int seqNum) const {
    int ed = ((*base) + windowSize - 1) % Configuration::MAX_SEQ_NUM;
    int st = *base;
    if(st <= ed) {
        return seqNum >= st && seqNum <= ed;
    } else {
        return seqNum >= st || seqNum <= ed;
    }
}

bool SRReceiverPacketWindow::inPreviousWindow(int seqNum) const {
    int st = *base;
    for(int i = 1; i <= windowSize; i++) {
        st = st ? st - 1 : Configuration::MAX_SEQ_NUM - 1;
    }
    int ed = *base;
    if(st <= ed) {
        return seqNum >= st && seqNum <= ed;
    } else {
        return seqNum >= st || seqNum <= ed;
    }
}

std::vector<Packet> SRReceiverPacketWindow::getResendPackets() {
    throw "Not Implemented.";
}

void SRReceiverPacketWindow::printPacketWindow() {
    int now = head;
    bool first = true;
    multiOutput.print("=============start to print packet window==============");
    while(window[now].packet.seqnum != -1 && (first || now != getPreviousWindowIndex(head))) {
        first = false;
        multiOutput.printPacket(window[now]);
        now = (now + 1) % windowSize;
    }
    multiOutput.print("=================finish to print packet window===========");
}