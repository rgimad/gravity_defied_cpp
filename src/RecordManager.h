#pragma once

#include <stdint.h>
#include <vector>
#include <string>
#include <list>
#include <vector>

#include "config.h"
#include "utils/Log.h"
#include "utils/Time.h"
#include "utils/FileStream.h"

class RecordManager {
public:
    // inline static const int unused = 3;

    void loadRecordInfo(const uint8_t level, const uint8_t track);
    std::vector<std::string> getRecordDescription(const uint8_t league);
    uint8_t getPosOfNewRecord(const uint8_t league, const int64_t timeMs) const;
    void writeRecordInfo(const uint8_t level, const uint8_t track);
    void addNewRecord(const uint8_t league, char* playerName, int64_t timeMs);
    void deleteRecordStores();
    // void closeRecordStore();

private:
    // 4: league, 100, 175, 225, 350,
    // 3: three best times
    struct /*__attribute__((packed))*/ Record {
        int64_t timeMs;
        char playerName[PLAYER_NAME_MAX];
        uint8_t padding[5];
    };
    static_assert(sizeof(Record) == 16);

    struct /*__attribute__((packed))*/ LeagueRecords {
        Record records[RECORD_NO_MAX];
    };
    static_assert(sizeof(LeagueRecords) == 48);

    struct /*__attribute__((packed))*/ RecordsSaveData {
        LeagueRecords leagueRecords[LEAGUES_MAX];
    };
    static_assert(sizeof(RecordsSaveData) == 192);

    union RecordsSaveDataConverter {
        RecordsSaveData recordsSaveData;
        int8_t bytes[sizeof(RecordsSaveData)];
    };
    static_assert(sizeof(RecordsSaveDataConverter) == sizeof(RecordsSaveData));

    RecordsSaveDataConverter recordsSaveDataConverter;
};
