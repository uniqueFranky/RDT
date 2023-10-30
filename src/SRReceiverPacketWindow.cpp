#include "SRReceiverPacketWindow.h"
#include "DataStructure.h"
#include "Global.h"
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
    pUtils->printPacket("pop one packet from window", window[head].packet);
    window[head].ack = false;
    window[head].packet.seqnum = -1;
    head = (head + 1) % windowSize;
    *base = ((*base) + 1) % Configuration::MAX_SEQ_NUM;
    return true;
}

bool SRReceiverPacketWindow::addPacket(Packet packet) {
    if(inWindow(packet.seqnum)) {
        window[getWindowIndex(packet.seqnum)].packet = packet;
        window[getWindowIndex(packet.seqnum)].ack = true;
        pUtils->printPacket("add one packet to packet window: ", window[getWindowIndex(packet.seqnum)].packet);
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
    std::cout << std::endl << std::endl << "==============receiver packet window begins============" << std::endl;
    int tms = windowSize;
    while(tms--) {
        pUtils->printPacket("", window[now].packet);
        now = (now + 1) % windowSize;
    }
    std::cout << "==============receiver packet window ends============" << std::endl << std::endl;
}