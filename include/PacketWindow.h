#ifndef PACKET_WINDOW_H
#define PACKET_WINDOW_H

#include "DataStructure.h"
#include "MultiOutput.h"
#include <fstream>
#include <vector>


class PacketWindow { // 使用循环队列实现的滑动窗口

protected:
    int head; // 队首指针
    int tail; // 队尾指针
    int *base; // Sender/Receiver的base
    int *nextSeqNum; // Sender/Receiver的base
    int windowSize; // 窗口大小
    bool empty; // 是否为空：当tail + 1 == head 时，队列既有可能是满的，又有可能是空的。
    PacketWithAck *window; // 窗口

    MultiOutput multiOutput;
    std::ofstream *outputFile;

    int getWindowIndex(int seqNum) const; // 根据seqNum获取对应packet在window中的下标
    virtual void printPacketWindow();
    virtual std::vector<Packet> setAcked(int) = 0;

public:
    PacketWindow(int, int *, int *, const std::string &);
    virtual ~PacketWindow();

    int getWindowSize() const;
    const Packet &getPacket(int seqNum);
    virtual bool addPacket(Packet); // 将一个packet加入滑动窗口，标记为已发送但未确认，加入成功时返回true
    virtual bool popPacket(); // 将最早的packet标记为已确认并且将其移出滑动窗口
    bool isEmpty() const; // 滑动窗口是否为空
    bool isFull() const; // 滑动窗口是否已满
    virtual bool inWindow(int seqNum) const; // 判断seqNum对应的packet是否在窗口中

    virtual std::vector<Packet> getResendPackets() = 0;
};

#endif