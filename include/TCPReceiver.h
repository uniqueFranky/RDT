#ifndef TCP_RECEIVER_H
#define TCP_RECEIVER_H
#include "SRRdtReceiver.h"
class TCPReceiver: public SRRdtReceiver {
private:
    int lastAck;
public:
    TCPReceiver(): lastAck(-1), SRRdtReceiver() {};
    ~TCPReceiver() = default;
    void receive(const Packet &packet) override;
};
#endif