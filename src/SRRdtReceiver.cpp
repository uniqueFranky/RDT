#include "SRRdtReceiver.h"
#include "DataStructure.h"
#include "Global.h"
#include "MultiOutput.h"
#include "RandomEventEnum.h"
#include <cstring>
#include <type_traits>
#include <vector>
#include <iostream>

void SRRdtReceiver::receive(const Packet &packet) {
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
        }
        if(packetWindow.inWindow(packet.seqnum) || packetWindow.inPreviousWindow(packet.seqnum)) {
            Packet ackPacket;
            ackPacket.acknum = packet.seqnum;
            ackPacket.seqnum = -1;
            ackPacket.checksum = pUtils->calculateCheckSum(ackPacket);
            pUtils->printPacket("接收方发送响应报文", ackPacket);
            pns->sendToNetworkLayer(SENDER, ackPacket);
        } else {
            std::cout << "base = " << base << " , seqnum = " << packet.seqnum << " , not in window or pre window" << std::endl;
        }
        
    }
}