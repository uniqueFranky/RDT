#ifndef MULTI_OUTPUT_H
#define MULTI_OUTPUT_H

#include "DataStructure.h"
#include <functional>
#include <vector>
#include <ostream>

class MultiOutput {
protected:
    std::vector<std::ostream *> streams;

public:
    MultiOutput(): streams() {};
    virtual ~MultiOutput() = default;
    void addStream(std::ostream *stream);
    void print(const std::string &msg);
    void printPacket(const Packet &packet);
};

#endif