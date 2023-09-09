#pragma once

#include <vector>
#include <iostream>

#include "RecordEnumeration.h"
#include "../utils/FileStream.h"

class RecordEnumerationImpl : public RecordEnumeration {
private:
    // static const int64_t serialVersionUID = 123;
    int currentPos = 0;

public:
    RecordEnumerationImpl(std::vector<std::vector<int8_t>> data);
    RecordEnumerationImpl();
    ~RecordEnumerationImpl();

    int numRecords();
    std::vector<int8_t> nextRecord();
    int addRecord(std::vector<int8_t> bytes);
    void setRecord(int index, std::vector<int8_t> bytes);
    void reset();
    int nextRecordId();
    void destroy();

    void serialize(FileStream* outStream);
    void deserialize(FileStream* inStream);

    std::vector<std::vector<int8_t>> data;
};
