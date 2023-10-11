
#include "Global.h"
#include "DataStructure.h"
#include "GBNRdtSender.h"
#include "RandomEventEnum.h"
#include <cstring>


bool GBNRdtSender::getWaitingState() {
    return packetWindow.isFull(); 
}

bool GBNRdtSender::send(const Message &msg) {

    if(getWaitingState()) { // 滑动窗口满时，不再接受新的发送请求
        return false;
    }
    Packet packet;
    packet.seqnum = nextSeqNum;
    memcpy(packet.payload, msg.data, sizeof(msg.data));
    packet.acknum = -1; // receiver并不发送packet，忽略该字段
    packet.checksum = 0;
    packet.checksum = pUtils->calculateCheckSum(packet);
    bool needSetTimer = packetWindow.isEmpty(); // 如果在发送前，滑动窗口为空，则需要启动timer
    packetWindow.addPacket(packet); // 将即将发送的packet加入滑动窗口
    pUtils->printPacket("发送方发送报文: ", packet);
    if(needSetTimer) {
        pns->startTimer(SENDER, Configuration::TIME_OUT, 0); // GBN只有一个timer，设置timer时seqNum不需要用到
    }
    pns->sendToNetworkLayer(RECEIVER, packet); // 将packet发送到网络层
    return true;
}

void GBNRdtSender::receive(const Packet &packet) {
    int checkSum = pUtils->calculateCheckSum(packet);
    pUtils->printPacket("发送方收到确认报文,开始计算checksum ", packet);
    if(packet.checksum == checkSum) {
        vector<int> acked = packetWindow.setAcked(packet.acknum); // 累计确认
        pUtils->printPacket("发送方收到确认报文，checksum正确 ", packet);
        if(acked.size() > 0) { // 不是冗余确认
            pns->stopTimer(SENDER, 0);
            if(!packetWindow.isEmpty()) { // 累计确认后滑动窗口不为空，需要重启timer
                pns->startTimer(SENDER, Configuration::TIME_OUT, 0);
            }
        }
    }
}

void GBNRdtSender::timeoutHandler(int seqNum) {
    vector<Packet> packets = packetWindow.getGBNPackets(); // 获取需要重传的packet
    pUtils->printPacket("响应超时, go back n ", Packet());
    pns->stopTimer(SENDER, 0);
    pns->startTimer(SENDER, Configuration::TIME_OUT, 0);
    for(const Packet &packet: packets) {
        pUtils->printPacket("resend 1 packet", packet);
        pns->sendToNetworkLayer(RECEIVER, packet);
    }
}