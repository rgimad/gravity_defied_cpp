#include "RecordManager.h"

void RecordManager::loadRecordInfo(const uint8_t level, const uint8_t track)
{
    recordsSaveDataConverter = { .bytes = {} };

    const std::string trackSaveName = std::to_string(level) + std::to_string(track) + ".dat";
    const std::filesystem::path saveFileName = GlobalSetting::SavesPath / GlobalSetting::SavesPrefix / trackSaveName;
    Log::write(Log::LogLevel::Info, "loadRecordInfo %s\n", saveFileName.c_str());

    FileStream levelFileStream(saveFileName, std::ios::in | std::ios::binary);
    levelFileStream.readVariable(&recordsSaveDataConverter.bytes, false, sizeof(RecordsSaveData));
}

std::array<std::string, RECORD_NO_MAX> RecordManager::getRecordDescription(const uint8_t league) const
{
    std::array<std::string, RECORD_NO_MAX> recordsDescription;

    for (uint8_t i = 0; i < RECORD_NO_MAX; ++i) {
        const Record record = recordsSaveDataConverter.recordsSaveData.leagueRecords[league].records[i];

        if (record.timeMs != 0L) {
            std::stringstream ss;
            ss << std::string(record.playerName, PLAYER_NAME_MAX)
               << ' '
               << Time::timeToString(record.timeMs);
            recordsDescription[i] = ss.str();
        } else {
            recordsDescription[i] = "";
        }
    }

    return recordsDescription;
}

void RecordManager::writeRecordInfo(const uint8_t level, const uint8_t track)
{
    const std::string trackSaveName = std::to_string(level) + std::to_string(track) + ".dat";
    const std::filesystem::path saveFileName = GlobalSetting::SavesPath / GlobalSetting::SavesPrefix / trackSaveName;
    Log::write(Log::LogLevel::Info, "writeRecordInfo %s\n", saveFileName.c_str());

    FileStream levelFileStream(saveFileName, std::ios::out | std::ios::binary);
    levelFileStream.writeVariable(&recordsSaveDataConverter.bytes, sizeof(RecordsSaveData));
}

uint8_t RecordManager::getPosOfNewRecord(const uint8_t league, const uint64_t timeMs) const
{
    for (uint8_t i = 0; i < RECORD_NO_MAX; ++i) {
        const Record record = recordsSaveDataConverter.recordsSaveData.leagueRecords[league].records[i];

        if (record.timeMs > timeMs || record.timeMs == 0L) {
            return i;
        }
    }

    return 3;
}

void RecordManager::addNewRecord(const uint8_t league, const char* playerName, const uint64_t timeMs)
{
    Log::write(Log::LogLevel::Info, "addNewRecord %d %s %d\n", (int)league, playerName, timeMs);
    const uint8_t newRecordPos = getPosOfNewRecord(league, timeMs);
    Log::write(Log::LogLevel::Info, "Record pos %d\n", (int)newRecordPos);

    if (newRecordPos >= 3) {
        // out of scope, we save only first 3 records
        return;
    }

    if (newRecordPos < (RECORD_NO_MAX - 1)) {
        std::rotate(
            std::begin(recordsSaveDataConverter.recordsSaveData.leagueRecords[league].records) + newRecordPos,
            std::end(recordsSaveDataConverter.recordsSaveData.leagueRecords[league].records) - 1,
            std::end(recordsSaveDataConverter.recordsSaveData.leagueRecords[league].records));
    }

    recordsSaveDataConverter.recordsSaveData.leagueRecords[league].records[newRecordPos] = {
        .timeMs = timeMs,
        .playerName = {},
        .padding = { 0, 0, 0, 0, 0 }
    };
    strncpy(recordsSaveDataConverter.recordsSaveData.leagueRecords[league].records[newRecordPos].playerName, playerName, PLAYER_NAME_MAX);
}

void RecordManager::deleteRecordStores() const
{
    Log::write(Log::LogLevel::Info, "deleteRecordStores\n");
    const std::filesystem::path saveFileName = GlobalSetting::SavesPath / GlobalSetting::SavesPrefix;

    for (const auto& file : std::filesystem::directory_iterator(saveFileName)) {
        const std::string filename = file.path().filename().string();
        Log::write(Log::LogLevel::Debug, "Found record %s\n", filename.c_str());

        if (GlobalSetting::GlobalSaveFileName == filename) {
            Log::write(Log::LogLevel::Debug, "Skipped deletion for %s\n", file.path().c_str());
            continue;
        }

        std::filesystem::remove(file);
    }
}

uint32_t RecordManager::getNumberOfRecordsForLevel(const uint8_t level)
{
    Log::write(Log::LogLevel::Info, "getNumberOfRecordsForLevel %d\n", level);

    const std::string prefix = std::to_string(level);
    const std::filesystem::path saveFileName = GlobalSetting::SavesPath / GlobalSetting::SavesPrefix;

    uint32_t count = 0;

    for (const auto& file : std::filesystem::directory_iterator(saveFileName)) {
        const std::string filename = file.path().filename().string();
        Log::write(Log::LogLevel::Debug, "Found record %s\n", filename.c_str());

        if (!file.is_regular_file()) {
            Log::write(Log::LogLevel::Debug, "Skipped record %s\n", file.path().c_str());
            continue;
        }

        if (filename.rfind(prefix, 0) == 0) {
            Log::write(Log::LogLevel::Debug, "Included record %s\n", file.path().c_str());
            ++count;
        }
    }

    return count;
}