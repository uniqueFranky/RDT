#include "PacketWindow.h"
#include "DataStructure.h"
#include "Global.h"
#include <cstdio>
#include <fstream>
#include <iostream>
#include <ostream>


PacketWindow::PacketWindow(int size, int *_base, int *_nextSeqNum, const std::string &path): windowSize(size), head(1), tail(0), base(_base), nextSeqNum(_nextSeqNum), empty(true) {
    window = new PacketWithAck[size];

}

PacketWindow::~PacketWindow() {
    if(nullptr != window) {
        delete []window;
    }
}

int PacketWindow::getWindowSize() const {
    return windowSize;
}

int PacketWindow::getWindowIndex(int seqNum) const {
    int offset;
    if(*base <= seqNum) {
        offset = seqNum - *base;
    } else {
        offset = seqNum + Configuration::MAX_SEQ_NUM - *base;
    }
    return (head + offset) % windowSize;
}

const Packet &PacketWindow::getPacket(int seqNum) {
    return window[getWindowIndex(seqNum)].packet;
}

bool PacketWindow::addPacket(Packet packet) {
    if(isFull()) {
        return false;
    }
    *nextSeqNum = (*nextSeqNum + 1) % Configuration::MAX_SEQ_NUM;
    tail = (tail + 1) % windowSize;
    window[tail] = PacketWithAck(packet, false);
    empty = false;

    pUtils->printPacket("add one packet to window: ", packet);
    printPacketWindow();

    return true;
}

bool PacketWindow::popPacket() {
    if(isEmpty()) {
        return false;
    }

    pUtils->printPacket("pop one packet from window: ", window[head].packet);
    (*base) = ((*base) + 1) % Configuration::MAX_SEQ_NUM;
    head = (head + 1) % windowSize;
    if((tail + 1) % windowSize == head) {
        empty = true;
    }

    printPacketWindow();

    return true;
}

bool PacketWindow::inWindow(int seqNum) const {
    if(isEmpty()) {
        return false;
    }

    int st = window[head].packet.seqnum;
    int ed = window[tail].packet.seqnum;
    if(st <= ed) {
        return seqNum >= st && seqNum <= ed;
    } else {
        return seqNum >= st || seqNum <= ed;
    }
}

bool PacketWindow::isEmpty() const {
    return empty && (tail + 1) % windowSize == head;
}

bool PacketWindow::isFull() const {
    if(head <= tail) {
        return !empty && tail - head + 1 == windowSize;
    } else {
        return !empty && tail + windowSize - head + 1 == windowSize;
    }
}

void PacketWindow::printPacketWindow() {
    std::cout << std::endl << std::endl << "==============packet window begin============" << std::endl;
    if(!isEmpty()) {
        int now = head;
        int last;
        char msg[100];
        do {
            sprintf(msg, "idx = %d: ", now);
            pUtils->printPacket(msg, window[now].packet);
            last = now;
            now = (now + 1) % windowSize;
            std::cout << std::endl;
        } while(last != tail);
    }
    std::cout << "==============packet window end============" << std::endl << std::endl;
}