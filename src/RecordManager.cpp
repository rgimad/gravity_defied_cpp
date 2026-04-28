#include "RecordManager.h"

#include "rms/RecordStore.h"
#include "rms/RecordStoreException.h"
#include "rms/RecordStoreNotOpenException.h"
#include "rms/InvalidRecordIDException.h"

void RecordManager::openRecordStore(int playerId, int levelId)
{
    resetRecordsTime();

    try {
        str = std::to_string(playerId) + std::to_string(levelId);
        recordStore = RecordStore::openRecordStore(str, true);
    } catch (RecordStoreException& e) {
        return;
    }

    packedRecordInfoRecordId = -1;

    RecordEnumeration* recordEnum;
    try {
        recordEnum = recordStore->enumerateRecords(nullptr, nullptr, false);
    } catch (RecordStoreNotOpenException& e) {
        return;
    }

    if (recordEnum->numRecords() > 0) {
        std::vector<int8_t> recordData;
        try {
            recordData = recordEnum->nextRecord();
            recordEnum->reset();
            packedRecordInfoRecordId = recordEnum->nextRecordId();
        } catch (RecordStoreNotOpenException& e) {
            return;
        } catch (InvalidRecordIDException& e) {
            return;
        } catch (RecordStoreException& e) {
            return;
        }

        loadRecordInfo(recordData);
        recordEnum->destroy();
    }
}

int64_t RecordManager::load5BytesAsLong(std::vector<int8_t> buffer, int offset)
{
    int64_t result = 0L;
    int64_t mult = 1L;

    for (int i = offset; i < 5 + offset; ++i) {
        int byteVal = (buffer[i] + 256) % 256;
        result += mult * (int64_t)byteVal;
        mult *= 256L;
    }

    return result;
}

void RecordManager::pushLongAs5Bytes(std::vector<int8_t> buffer, int offset, int64_t value)
{
    for (int i = offset; i < 5 + offset; ++i) {
        buffer[i] = (int8_t)((int)(value % 256L));
        value /= 256L;
    }
}

void RecordManager::loadRecordInfo(std::vector<int8_t> buffer)
{
    int offset = 0;

    int league;
    int pos;
    for (league = 0; league < 4; ++league) {
        for (pos = 0; pos < 3; ++pos) {
            recordTimeMs[league][pos] = load5BytesAsLong(buffer, offset);
            offset += 5;
        }
    }

    for (league = 0; league < LEAGUES_MAX; ++league) {
        for (pos = 0; pos < RECORD_NO_MAX; ++pos) {
            for (auto i = 0; i < PLAYER_NAME_MAX; ++i) {
                recordName[league][pos][i] = buffer[offset++];
            }
        }
    }
}

void RecordManager::getLevelInfo(std::vector<int8_t> buffer)
{
    int shift = 0;

    int league;
    int recordNo;
    for (league = 0; league < 4; ++league) {
        for (recordNo = 0; recordNo < 3; ++recordNo) {
            pushLongAs5Bytes(buffer, shift, recordTimeMs[league][recordNo]);
            shift += 5;
        }
    }

    for (league = 0; league < LEAGUES_MAX; ++league) {
        for (recordNo = 0; recordNo < RECORD_NO_MAX; ++recordNo) {
            for (auto i = 0; i < PLAYER_NAME_MAX; ++i) {
                buffer[shift++] = recordName[league][recordNo][i];
            }
        }
    }
}

void RecordManager::resetRecordsTime()
{
    for (int league = 0; league < 4; ++league) {
        for (int pos = 0; pos < 3; ++pos) {
            recordTimeMs[league][pos] = 0L;
        }
    }
}

std::vector<std::string> RecordManager::getRecordDescription(int league)
{
    std::vector<std::string> descriptions = std::vector<std::string>(3);

    for (int i = 0; i < 3; ++i) {
        if (recordTimeMs[league][i] != 0L) {
            int seconds = (int)recordTimeMs[league][i] / 100;
            int centiseconds = (int)recordTimeMs[league][i] % 100;
            descriptions[i] = "" + std::string(recordName[league][i]) + " ";

            if (seconds / 60 < 10) {
                descriptions[i] = descriptions[i] + " 0" + std::to_string(seconds / 60);
            } else {
                descriptions[i] = descriptions[i] + " " + std::to_string(seconds / 60);
            }

            if (seconds % 60 < 10) {
                descriptions[i] = descriptions[i] + ":0" + std::to_string(seconds % 60);
            } else {
                descriptions[i] = descriptions[i] + ":" + std::to_string(seconds % 60);
            }

            if (centiseconds < 10) {
                descriptions[i] = descriptions[i] + ".0" + std::to_string(centiseconds);
            } else {
                descriptions[i] = descriptions[i] + "." + std::to_string(centiseconds);
            }
        } else {
            descriptions[i].clear();
        }
    }

    return descriptions;
}

void RecordManager::writeRecordInfo()
{
    getLevelInfo(packedRecordInfo);
    if (packedRecordInfoRecordId == -1) {
        try {
            packedRecordInfoRecordId = recordStore->addRecord(packedRecordInfo, 0, 96);
        } catch (RecordStoreNotOpenException& e) {
        } catch (RecordStoreException& e) {
        }
    } else {
        try {
            recordStore->setRecord(packedRecordInfoRecordId, packedRecordInfo, 0, 96);
        } catch (RecordStoreNotOpenException& e) {
        } catch (RecordStoreException& e) {
        }
    }
}

int RecordManager::getPosOfNewRecord(int league, int64_t timeMs)
{
    for (int i = 0; i < 3; ++i) {
        if (recordTimeMs[league][i] > timeMs || recordTimeMs[league][i] == 0L) {
            return i;
        }
    }

    return 3;
}

void RecordManager::addRecord(int league, char* playerName, int64_t timeMs)
{
    int newRecordPos;
    if ((newRecordPos = getPosOfNewRecord(league, timeMs)) != 3) {
        if (timeMs > 16777000L) {
            timeMs = 16777000L; // 3 int8_ts, not five, wtf?
        }

        addNewRecord(league, newRecordPos);
        recordTimeMs[league][newRecordPos] = timeMs;

        for (auto i = 0; i < PLAYER_NAME_MAX; ++i) {
            recordName[league][newRecordPos][i] = playerName[i];
        }
    }
}

void RecordManager::addNewRecord(int gameLevel, int position)
{
    for (auto pos = 2; pos > position; --pos) {
        recordTimeMs[gameLevel][pos] = recordTimeMs[gameLevel][pos - 1];
        for (auto i = 0; i < PLAYER_NAME_MAX; ++i) {
            recordName[gameLevel][pos][i] = recordName[gameLevel][pos - 1][i];
        }
    }
}

void RecordManager::deleteRecordStores()
{
    std::vector<std::string> names = RecordStore::listRecordStores();

    for (std::size_t i = 0; i < names.size(); ++i) {
        if (names[i] != "GDTRStates") {
            try {
                RecordStore::deleteRecordStore(names[i]);
            } catch (RecordStoreException& e) {
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
        } catch (RecordStoreException& e) {
        }
    }
}
