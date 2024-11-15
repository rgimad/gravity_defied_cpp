#pragma once

#include <stdint.h>
#include <vector>
#include <string>
#include <list>
#include <vector>
#include <algorithm>

#include "config.h"
#include "utils/Log.h"
#include "utils/Time.h"
#include "utils/FileStream.h"

class RecordManager {
public:
    void loadRecordInfo(const uint8_t level, const uint8_t track);
    std::array<std::string_view, RECORD_NO_MAX> getRecordDescription(const uint8_t league) const;
    uint8_t getPosOfNewRecord(const uint8_t league, const uint64_t timeMs) const;
    void writeRecordInfo(const uint8_t level, const uint8_t track);
    void addNewRecord(const uint8_t league, const char* playerName, const uint64_t timeMs);
    void deleteRecordStores() const;

private:
    struct Record {
        uint64_t timeMs;
        char playerName[PLAYER_NAME_MAX];
        uint8_t padding[5];
    };
    static_assert(sizeof(Record) == 16);

    struct LeagueRecords {
        Record records[RECORD_NO_MAX];
    };
    static_assert(sizeof(LeagueRecords) == 48);

    struct RecordsSaveData {
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
