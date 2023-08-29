#pragma once

#include <stdint.h>
#include <vector>
#include <string>

class RecordStore;

class RecordManager {
private:
    std::vector<std::vector<int64_t>> recordTimeMs = std::vector<std::vector<int64_t>>(4, std::vector<int64_t>(3));
    // 4: league, 100, 175, 225, 350,
    // 3: three best times
    std::vector<std::vector<std::vector<int8_t>>> recordName = std::vector<std::vector<std::vector<int8_t>>>(4, std::vector<std::vector<int8_t>>(3, std::vector<int8_t>(3)));
    RecordStore* recordStore = nullptr;
    int packedRecordInfoRecordId = -1;
    std::vector<int8_t> packedRecordInfo = std::vector<int8_t>(96);
    std::string str;

    int64_t load5BytesAsLong(std::vector<int8_t> var1, int offset);
    void pushLongAs5Bytes(std::vector<int8_t> var1, int var2, int64_t var3);
    void loadRecordInfo(std::vector<int8_t> var1);
    void getLevelInfo(std::vector<int8_t> var1);
    void resetRecordsTime();
    void addNewRecord(int gameLevel, int position);

public:
    inline static const int unused = 3;

    void method_8(int var1, int var2);
    std::vector<std::string> getRecordDescription(int var1);
    void writeRecordInfo();
    int getPosOfNewRecord(int league, int64_t timeMs);
    void method_17(int league, std::vector<int8_t> values, int64_t timeMs);
    void deleteRecordStores();
    void closeRecordStore();
};
