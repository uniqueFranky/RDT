#include "MultiOutput.h"
#include "DataStructure.h"
#include "PacketWindow.h"
#include <ostream>
#include <iostream>
#include <string>

void MultiOutput::addStream(std::ostream *stream) {
    streams.push_back(stream);
}


void MultiOutput::print(const std::string &msg) {
    for(std::ostream *stream: streams) {
        *stream << msg << std::endl;
    }
}

void MultiOutput::printPacket(const PacketWithAck &packetWithAck) {
    const Packet &packet = packetWithAck.packet;
    for(std::ostream *stream: streams) {
        *stream << "seqNum = " << packet.seqnum << " , ackNum = " << packet.acknum << " , checkSum = " << packet.checksum << " , ack = " << packetWithAck.ack << std::endl;
        *stream << "payload = " << packet.payload << std::endl;
    }
}

void MultiOutput::print(const std::string &msg, int val) {
    for(std::ostream *stream: streams) {
        *stream << msg << val << std::endl;
    }
}