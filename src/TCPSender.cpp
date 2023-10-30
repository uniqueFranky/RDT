#include "TCPSender.h"
#include "DataStructure.h"
#include "Global.h"

bool TCPSender::getWaitingState() {
    return packetWindow.isFull();
}

bool TCPSender::send(const Message &msg) { // same as GBN
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
    pUtils->printPacket("发送方发送报文: ", packet);
    packetWindow.addPacket(packet); // 将即将发送的packet加入滑动窗口
    if(needSetTimer) {
        pns->startTimer(SENDER, Configuration::TIME_OUT, 0); // GBN只有一个timer，设置timer时seqNum不需要用到
    }
    pns->sendToNetworkLayer(RECEIVER, packet); // 将packet发送到网络层
    return true;
}

void TCPSender::receive(const Packet &packet) {
    int checkSum = pUtils->calculateCheckSum(packet);
    if(packet.checksum == checkSum) {
        pUtils->printPacket("发送方收到确认报文，checksum正确 ", packet);
        vector<Packet> acked = packetWindow.setAcked(packet.acknum); // 累计确认
        if(acked.size() > 0) { // 不是冗余确认
            pns->stopTimer(SENDER, 0);
            if(!packetWindow.isEmpty()) { // 累计确认后滑动窗口不为空，需要重启timer
                pns->startTimer(SENDER, Configuration::TIME_OUT, 0);
            }
        }
        if(packetWindow.needFastRensend()) {
            Packet fastResendPacket = packetWindow.getFastResendPacket();
            pUtils->printPacket("发送方接收到3次冗余确认，快速重传 ", fastResendPacket);
            pns->sendToNetworkLayer(RECEIVER, fastResendPacket);
        }
    }
}

void TCPSender::timeoutHandler(int seqNum) { // same as GBN
    vector<Packet> packets = packetWindow.getResendPackets(); // 获取需要重传的packet
    std::cout << "响应超时，GBN" << std::endl;
    pns->stopTimer(SENDER, 0);
    pns->startTimer(SENDER, Configuration::TIME_OUT, 0);
    for(const Packet &packet: packets) {
        pUtils->printPacket("resend 1 packet", packet);
        pns->sendToNetworkLayer(RECEIVER, packet);
    }
}