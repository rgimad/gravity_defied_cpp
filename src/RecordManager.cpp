#include "RecordManager.h"

#include "rms/RecordStore.h"
#include "rms/RecordStoreException.h"
#include "rms/RecordStoreNotOpenException.h"
#include "rms/InvalidRecordIDException.h"

void RecordManager::method_8(const uint8_t level, const uint8_t track)
{
    std::cout << "method_8 " << (int)level << (int)track << std::endl;
    resetRecordsTime();

    try {
        const std::string str = std::to_string(level) + std::to_string(track);
        std::cout << "str " << str << std::endl;
        recordStore = RecordStore::openRecordStore(str, true);
    } catch (RecordStoreException& var9) {
        return;
    }

    packedRecordInfoRecordId = -1;

    RecordEnumeration* recordEnum;
    try {
        recordEnum = recordStore->enumerateRecords(nullptr, nullptr, false);
    } catch (RecordStoreNotOpenException& var8) {
        return;
    }

    if (recordEnum->numRecords() > 0) {
        std::vector<int8_t> saveFileBytes;
        try {
            saveFileBytes = recordEnum->nextRecord();
            recordEnum->reset();
            packedRecordInfoRecordId = recordEnum->nextRecordId();
        } catch (RecordStoreNotOpenException& var5) {
            return;
        } catch (InvalidRecordIDException& var6) {
            return;
        } catch (RecordStoreException& var7) {
            return;
        }

        loadRecordInfo(saveFileBytes);
        // recordEnum->destroy();
    }
}

void RecordManager::loadRecordInfo(std::vector<int8_t> dataBytes)
{
    RecordsSaveDataConverter converter;
    std::copy(dataBytes.begin(), dataBytes.end(), converter.bytes);
    recordsSaveData = converter.recordsSaveData;
}

void RecordManager::resetRecordsTime()
{
    for (uint8_t league = 0; league < LEAGUES_MAX; ++league) {
        for (uint8_t pos = 0; pos < RECORD_NO_MAX; ++pos) {
            recordsSaveData.leagueRecords[league].records[pos].timeMs = 0L;
        }
    }
}

std::vector<std::string> RecordManager::getRecordDescription(const uint8_t league)
{
    std::vector<std::string> recordsDescription = std::vector<std::string>(RECORD_NO_MAX);

    for (uint8_t i = 0; i < RECORD_NO_MAX; ++i) {
        Record record = recordsSaveData.leagueRecords[league].records[i];

        if (record.timeMs != 0L) {
            std::stringstream ss;
            ss << record.playerName << ' ' << Time::timeToString(record.timeMs);
            recordsDescription[i] = ss.str();
        } else {
            recordsDescription[i].clear();
        }
    }

    return recordsDescription;
}

void RecordManager::writeRecordInfo()
{
    RecordsSaveDataConverter converter = {
        .recordsSaveData = recordsSaveData
    };
    std::vector<int8_t> packedRecordInfo(std::begin(converter.bytes), std::end(converter.bytes));

    if (packedRecordInfoRecordId == -1) {
        try {
            packedRecordInfoRecordId = recordStore->addRecord(packedRecordInfo, 0, 96);
        } catch (RecordStoreNotOpenException& var1) {
            return;
        } catch (RecordStoreException& var2) {
            return;
        }
    } else {
        try {
            recordStore->setRecord(packedRecordInfoRecordId, packedRecordInfo, 0, 96);
        } catch (RecordStoreNotOpenException& var3) {
            return;
        } catch (RecordStoreException& var4) {
            return;
        }
    }
}

uint8_t RecordManager::getPosOfNewRecord(const uint8_t league, const int64_t timeMs)
{
    for (uint8_t i = 0; i < RECORD_NO_MAX; ++i) {
        const Record record = recordsSaveData.leagueRecords[league].records[i];

        if (record.timeMs > timeMs || record.timeMs <= 0L) {
            return i;
        }
    }

    return 3;
}

void RecordManager::method_17(const uint8_t league, char* playerName, int64_t timeMs)
{
    std::cout << "method_17 " << (int)league << " " << playerName << " " << timeMs << std::endl;
    const uint8_t newRecordPos = getPosOfNewRecord(league, timeMs);
    std::cout << "record pos " << (int)newRecordPos << std::endl;

    if (newRecordPos >= 3) {
        // out of scope, we save only first 3 records
        return;
    }

    if (timeMs > 16777000L) {
        timeMs = 16777000L; // 3 int8_ts, not five, wtf?
    }

    addNewRecord(league, newRecordPos);
    recordsSaveData.leagueRecords[league].records[newRecordPos].timeMs = timeMs;

    for (uint8_t i = 0; i < PLAYER_NAME_MAX; ++i) {
        recordsSaveData.leagueRecords[league].records[newRecordPos].playerName[i] = playerName[i];
    }
}

void RecordManager::addNewRecord(const uint8_t league, const uint8_t position)
{
    std::cout << "addNewRecord " << (int)league << " " << (int)position << std::endl;
    LeagueRecords leagueRecs = recordsSaveData.leagueRecords[league];

    for (uint8_t i = 2; i > position; --i) {
        leagueRecs.records[i].timeMs = leagueRecs.records[i - 1].timeMs;

        for (uint8_t i = 0; i < PLAYER_NAME_MAX; ++i) {
            leagueRecs.records[i].playerName[i] = leagueRecs.records[i - 1].playerName[i];
        }
    }
}

void RecordManager::deleteRecordStores()
{
    std::vector<std::string> names = RecordStore::listRecordStores();

    for (std::size_t i = 0; i < names.size(); ++i) {
        if (names[i] != GlobalSetting::GlobalSaveFileName) {
            try {
                RecordStore::deleteRecordStore(names[i]);
            } catch (RecordStoreException& var3) {
                return;
            }
        }
    }
}

void RecordManager::closeRecordStore()
{
    if (recordStore != nullptr) {
        try {
            recordStore->closeRecordStore();
            return;
        } catch (RecordStoreException& var1) {
            return;
        }
    }
}
