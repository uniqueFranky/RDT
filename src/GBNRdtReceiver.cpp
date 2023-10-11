#include "GBNRdtReceiver.h"
#include "DataStructure.h"
#include "Global.h"
#include "RandomEventEnum.h"
#include <cstddef>
#include <cstdio>
#include <cstring>

void GBNRdtReceiver::receive(const Packet &packet) {
    pUtils->printPacket("接收方收到报文", packet);
    int checkSum = pUtils->calculateCheckSum(packet);
    if(packet.checksum == checkSum) {
        Packet ackPacket;
        ackPacket.checksum = 0;
        ackPacket.acknum = expectSeqNum;
        ackPacket.seqnum = -1;
        if(packet.seqnum == expectSeqNum) {
            ackPacket.acknum = expectSeqNum;
            ackPacket.checksum = pUtils->calculateCheckSum(ackPacket);
            expectSeqNum = (expectSeqNum + 1) % Configuration::MAX_SEQ_NUM;
            pUtils->printPacket("接收方发送确认报文", ackPacket);
            pns->sendToNetworkLayer(SENDER, ackPacket);

            Message msg;
            memcpy(msg.data, packet.payload, sizeof(msg.data));
            pns->delivertoAppLayer(RECEIVER, msg);
        } else {
            ackPacket.acknum = expectSeqNum ? expectSeqNum - 1 : Configuration::MAX_SEQ_NUM - 1;
            ackPacket.checksum = pUtils->calculateCheckSum(ackPacket);
            pUtils->printPacket("接收方收到冗余或乱序报文", packet);
            pns->sendToNetworkLayer(SENDER, ackPacket);
        }

    }
}