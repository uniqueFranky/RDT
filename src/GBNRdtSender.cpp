
#include "Global.h"
#include "DataStructure.h"
#include "GBNRdtSender.h"
#include "RandomEventEnum.h"
#include <cstring>


bool GBNRdtSender::getWaitingState() {
    return packetWindow.isFull();
}

bool GBNRdtSender::send(const Message &msg) {

    if(getWaitingState()) {
        return false;
    }
    Packet packet;
    packet.seqnum = nextSeqNum;
    memcpy(packet.payload, msg.data, sizeof(msg.data));
    packet.acknum = -1;
    packet.checksum = 0;
    packet.checksum = pUtils->calculateCheckSum(packet);
    bool needSetTimer = packetWindow.isEmpty();
    packetWindow.addPacket(packet);
    pUtils->printPacket("发送方发送报文: ", packet);
    if(needSetTimer) {
        pns->startTimer(SENDER, Configuration::TIME_OUT, 0);
    }
    pns->sendToNetworkLayer(RECEIVER, packet);
    return true;
}

void GBNRdtSender::receive(const Packet &packet) {
    int checkSum = pUtils->calculateCheckSum(packet);
    pUtils->printPacket("发送方收到确认报文,开始计算checksum ", packet);
    if(packet.checksum == checkSum) {
        vector<int> acked = packetWindow.setAcked(packet.acknum);
        pUtils->printPacket("发送方收到确认报文，checksum正确 ", packet);
        if(acked.size() > 0) {
            pns->stopTimer(SENDER, 0);
            if(!packetWindow.isEmpty()) {
                pns->startTimer(SENDER, Configuration::TIME_OUT, 0);
            }
        }
    }
}

void GBNRdtSender::timeoutHandler(int seqNum) {
    vector<Packet> packets = packetWindow.getGBNPackets();
    pUtils->printPacket("响应超时, go back n ", Packet());
    pns->stopTimer(SENDER, 0);
    pns->startTimer(SENDER, Configuration::TIME_OUT, 0);
    if(packets.size() > 0) {
        for(const Packet &packet: packets) {
            pUtils->printPacket("resend 1 packet", packet);
            pns->sendToNetworkLayer(RECEIVER, packet);
        }
    }

}