#include "PacketWindow.h"
#include "DataStructure.h"


PacketWindow::PacketWindow(int size, int *_base, int *_nextSeqNum): windowSize(size), head(1), tail(0), base(_base), nextSeqNum(_nextSeqNum), empty(true) {
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
    return true;
}

bool PacketWindow::popPacket() {
    if(isEmpty()) {
        return false;
    }

    head = (head + 1) % windowSize;
    if((tail + 1) % windowSize == head) {
        empty = true;
    }
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