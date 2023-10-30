#include "TCPReceiver.h"
#include "Global.h"
#include <cstring>

void TCPReceiver::receive(const Packet &packet) {
    pUtils->printPacket("接收方收到报文", packet);
    int checkSum = pUtils->calculateCheckSum(packet);
    if(packet.checksum == checkSum) {
        packetWindow.addPacket(packet);
        std::vector<Packet> ackedPackets = packetWindow.setAcked(packet.seqnum);
        for(const Packet &ack: ackedPackets) {
            Message msg;
            memcpy(msg.data, ack.payload, sizeof(msg.data));
            pUtils->printPacket("接收方将报文交付给应用层", ack);
            pns->delivertoAppLayer(RECEIVER, msg);
            lastAck = ack.seqnum; // 最后一次成功累计确认的seqNum
        }
        if(lastAck != -1) { // 发送最后一次成功累计确认的ack packet
            Packet ackPacket;
            ackPacket.acknum = lastAck;
            ackPacket.seqnum = -1;
            memset(ackPacket.payload, 0, sizeof(ackPacket.payload));
            ackPacket.checksum = pUtils->calculateCheckSum(ackPacket);
            pUtils->printPacket("接收方发送响应报文", ackPacket);
            pns->sendToNetworkLayer(SENDER, ackPacket);
        }
    }
}