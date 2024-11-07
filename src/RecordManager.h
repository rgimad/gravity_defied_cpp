#pragma once

#include <stdint.h>
#include <vector>
#include <string>
#include <list>
#include <vector>

#include "config.h"
#include "utils/Time.h"

#include "rms/RecordStore.h"
#include "rms/RecordStoreException.h"
#include "rms/RecordStoreNotOpenException.h"
#include "rms/InvalidRecordIDException.h"

// class RecordStore;

class RecordManager {
public:
    enum {
        LEAGUES_MAX = 4,
        RECORD_NO_MAX = 3,
        PLAYER_NAME_MAX = 3,
    };

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

    // RecordStore* recordStore = nullptr;
    // int packedRecordInfoRecordId = -1;
    RecordsSaveDataConverter recordsSaveDataConverter;
    // RecordsSaveData recordsSaveData;

    void resetRecordsTime();
    // void shiftRecords(const uint8_t league, const uint8_t position);
};
