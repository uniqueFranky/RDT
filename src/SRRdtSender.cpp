#include "SRRdtSender.h"
#include "DataStructure.h"
#include "Global.h"
#include "RandomEventEnum.h"
#include <cstring>
#include <iterator>
#include <vector>


bool SRRdtSender::getWaitingState() {
    return packetWindow.isFull();
}

bool SRRdtSender::send(const Message &msg) {
    if(getWaitingState()) {
        return false;
    }

    Packet packet;
    packet.seqnum = nextSeqNum;
    packet.acknum = -1;
    memcpy(packet.payload, msg.data, sizeof(msg.data));
    packet.checksum = 0;
    packet.checksum = pUtils->calculateCheckSum(packet);

    packetWindow.addPacket(packet);
    pUtils->printPacket("发送方发送报文到网络层", packet);
    pns->startTimer(SENDER, Configuration::TIME_OUT, packet.seqnum);
    pns->sendToNetworkLayer(RECEIVER, packet);
    return true;
}

void SRRdtSender::receive(const Packet &packet) {
    pUtils->printPacket("发送方接收到响应报文", packet);
    int checkSum = pUtils->calculateCheckSum(packet);
    if(packet.checksum == checkSum) {
        vector<Packet> ackedPackets = packetWindow.setAcked(packet.acknum);
        for(const Packet &ack: ackedPackets) {
            pns->stopTimer(SENDER, ack.seqnum);
        }
    }
}

void SRRdtSender::timeoutHandler(int seqNum) {
    const Packet &packet = packetWindow.getPacket(seqNum);
    pUtils->printPacket("报文超时，重新发送", packet);
    pns->stopTimer(SENDER, seqNum);
    pns->startTimer(SENDER, Configuration::TIME_OUT, seqNum);
    pns->sendToNetworkLayer(RECEIVER, packet);
}

