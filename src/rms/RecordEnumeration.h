#pragma once

#include <vector>
#include <stdint.h>

class RecordEnumeration {
public:
    virtual int numRecords() = 0;
    virtual std::vector<int8_t> nextRecord() = 0;
    virtual void reset() = 0;
    virtual int nextRecordId() = 0;
    // virtual void destroy() = 0;
};