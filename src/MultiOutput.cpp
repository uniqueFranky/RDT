#include "MultiOutput.h"
#include "DataStructure.h"
#include <ostream>
#include <iostream>

void MultiOutput::addStream(std::ostream *stream) {
    streams.push_back(stream);
}


void MultiOutput::print(const std::string &msg) {
    for(std::ostream *stream: streams) {
        *stream << msg << std::endl;
    }
}

void MultiOutput::printPacket(const Packet &packet) {
    for(std::ostream *stream: streams) {
        *stream << "seqNum = " << packet.seqnum << " , ackNum = " << packet.acknum << " , checkSum = " << packet.checksum << std::endl;
        *stream << "payload = " << packet.payload << std::endl;
    }
}