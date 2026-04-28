#pragma once

#include <stdint.h>
#include <vector>
#include <string>

class RecordStore;

class RecordManager {
public:
    enum {
        LEAGUES_MAX = 4,
        RECORD_NO_MAX = 3,
        PLAYER_NAME_MAX = 3,
    };

    inline static const int unused = 3;

    // Opens record store with player/level ID
    void openRecordStore(int playerId, int levelId);
    std::vector<std::string> getRecordDescription(int league);
    void writeRecordInfo();
    int getPosOfNewRecord(int league, int64_t timeMs);
    void addRecord(int league, char* playerName, int64_t timeMs);
    void deleteRecordStores();
    void closeRecordStore();

private:
    std::vector<std::vector<int64_t>> recordTimeMs = std::vector<std::vector<int64_t>>(4, std::vector<int64_t>(3));
    // 4: league, 100, 175, 225, 350,
    // 3: three best times
    char recordName[LEAGUES_MAX][RECORD_NO_MAX][PLAYER_NAME_MAX + 1];
    RecordStore* recordStore = nullptr;
    int packedRecordInfoRecordId = -1;
    std::vector<int8_t> packedRecordInfo = std::vector<int8_t>(96);
    std::string str;

    int64_t load5BytesAsLong(std::vector<int8_t> buffer, int offset);
    void pushLongAs5Bytes(std::vector<int8_t> buffer, int offset, int64_t value);
    void loadRecordInfo(std::vector<int8_t> buffer);
    void getLevelInfo(std::vector<int8_t> buffer);
    void resetRecordsTime();
    void addNewRecord(int gameLevel, int position);
};
