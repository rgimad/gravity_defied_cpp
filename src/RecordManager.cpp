#include "RecordManager.h"

void RecordManager::loadRecordInfo(const uint8_t level, const uint8_t track)
{
    // resetRecordsTime();
    std::string saveFileName = "./saves/" + GlobalSetting::SavesPrefix + "_" + std::to_string(level) + std::to_string(track);
    std::cout << "loadRecordInfo " << saveFileName << std::endl;
    
    FileStream levelFileStream(saveFileName, std::ios::in | std::ios::binary);
    levelFileStream.readVariable(&recordsSaveDataConverter.bytes, false, sizeof(RecordsSaveData));
}

void RecordManager::resetRecordsTime()
{
    recordsSaveDataConverter = { .bytes = {} };
    // for (uint8_t league = 0; league < LEAGUES_MAX; ++league) {
    //     for (uint8_t pos = 0; pos < RECORD_NO_MAX; ++pos) {
    //         recordsSaveData.leagueRecords[league].records[pos].timeMs = 0L;
    //     }
    // }
}

std::vector<std::string> RecordManager::getRecordDescription(const uint8_t league)
{
    std::vector<std::string> recordsDescription = std::vector<std::string>(RECORD_NO_MAX);

    for (uint8_t i = 0; i < RECORD_NO_MAX; ++i) {
        Record record = recordsSaveDataConverter.recordsSaveData.leagueRecords[league].records[i];

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

void RecordManager::writeRecordInfo(const uint8_t level, const uint8_t track)
{
    std::string saveFileName = "./saves/" + GlobalSetting::SavesPrefix + "_" + std::to_string(level) + std::to_string(track);
    std::cout << "writeRecordInfo " << saveFileName << std::endl;

    FileStream levelFileStream(saveFileName, std::ios::out | std::ios::binary);
    levelFileStream.writeVariable(&recordsSaveDataConverter.bytes, sizeof(RecordsSaveData));
}

uint8_t RecordManager::getPosOfNewRecord(const uint8_t league, const int64_t timeMs) const
{
    for (uint8_t i = 0; i < RECORD_NO_MAX; ++i) {
        const Record record = recordsSaveDataConverter.recordsSaveData.leagueRecords[league].records[i];

        if (record.timeMs > timeMs || record.timeMs <= 0L) {
            return i;
        }
    }

    return 3;
}

void RecordManager::addNewRecord(const uint8_t league, char* playerName, int64_t timeMs)
{
    std::cout << "addNewRecord " << (int)league << " " << playerName << " " << timeMs << std::endl;
    const uint8_t newRecordPos = getPosOfNewRecord(league, timeMs);
    std::cout << "record pos " << (int)newRecordPos << std::endl;

    if (newRecordPos >= 3) {
        // out of scope, we save only first 3 records
        return;
    }

    if (timeMs > 16777000L) {
        timeMs = 16777000L; // 3 int8_ts, not five, wtf?
    }

    if (newRecordPos < (RECORD_NO_MAX - 1)) {
        std::rotate(
            std::begin(recordsSaveDataConverter.recordsSaveData.leagueRecords[league].records) + newRecordPos,
            std::end(recordsSaveDataConverter.recordsSaveData.leagueRecords[league].records) - 1,
            std::end(recordsSaveDataConverter.recordsSaveData.leagueRecords[league].records));
    }

    recordsSaveDataConverter.recordsSaveData.leagueRecords[league].records[newRecordPos] = {
        .timeMs = timeMs,
        .playerName = {}
    };
    strcpy(recordsSaveDataConverter.recordsSaveData.leagueRecords[league].records[newRecordPos].playerName, playerName);
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

// void RecordManager::closeRecordStore()
// {
//     // if (recordStore != nullptr) {
//     //     try {
//     //         recordStore->closeRecordStore();
//     //         return;
//     //     } catch (RecordStoreException& var1) {
//     //         return;
//     //     }
//     // }
// }
