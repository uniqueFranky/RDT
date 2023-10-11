#ifndef PACKET_WINDOW_H
#define PACKET_WINDOW_H

#include "DataStructure.h"
#include <vector>

struct PacketWithAck {
    Packet packet;
    bool ack;

    PacketWithAck(const Packet &p, bool a): packet(p), ack(a) {};
    PacketWithAck(){};
};

class PacketWindow {

protected:
    int head;
    int tail;
    int *base;
    int *nextSeqNum;
    int windowSize;
    bool empty;
    PacketWithAck *window;

    int getWindowIndex(int seqNum) const;
    bool inWindow(int seqNum) const;

public:
    PacketWindow(int, int *, int *);
    virtual ~PacketWindow();

    int getWindowSize() const;
    const Packet &getPacket(int seqNum);
    bool addPacket(Packet);
    bool popPacket();
    bool isEmpty() const;
    bool isFull() const;
};

#endif