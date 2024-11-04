#include "RecordManager.h"

#include "rms/RecordStore.h"
#include "rms/RecordStoreException.h"
#include "rms/RecordStoreNotOpenException.h"
#include "rms/InvalidRecordIDException.h"

// method_8 0 9
// openRecordStore(ecff3f91391c012620ea3f638ef4f21d_09, 1)
// createRecordStore(ecff3f91391c012620ea3f638ef4f21d_09, 1)
// enumerateRecords()
// method_197 AAA 2476

void RecordManager::method_8(const int level, const int track)
{
    std::cout << "method_8 " << level << track << std::endl;
    resetRecordsTime();

    try {
        const std::string str = std::to_string(level) + std::to_string(track);
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
        std::vector<int8_t> var4;
        try {
            var4 = recordEnum->nextRecord();
            recordEnum->reset();
            packedRecordInfoRecordId = recordEnum->nextRecordId();
        } catch (RecordStoreNotOpenException& var5) {
            return;
        } catch (InvalidRecordIDException& var6) {
            return;
        } catch (RecordStoreException& var7) {
            return;
        }

        loadRecordInfo(var4);
        // recordEnum->destroy();
    }
}

int64_t RecordManager::load5BytesAsLong(std::vector<int8_t> var1, int offset)
{
    int64_t result = 0L;
    int64_t mult = 1L;

    for (int var7 = offset; var7 < 5 + offset; ++var7) {
        int var8 = (var1[var7] + 256) % 256;
        result += mult * (int64_t)var8;
        mult *= 256L;
    }

    return result;
}

void RecordManager::pushLongAs5Bytes(std::vector<int8_t> var1, int var2, int64_t var3)
{
    for (int var5 = var2; var5 < 5 + var2; ++var5) {
        var1[var5] = (int8_t)((int)(var3 % 256L));
        var3 /= 256L;
    }
}

void RecordManager::loadRecordInfo(std::vector<int8_t> var1)
{
    int offset = 0;
    int league;
    int pos;

    for (league = 0; league < 4; ++league) {
        for (pos = 0; pos < 3; ++pos) {
            recordTimeMs[league][pos] = load5BytesAsLong(var1, offset);
            offset += 5;
        }
    }

    for (league = 0; league < LEAGUES_MAX; ++league) {
        for (pos = 0; pos < RECORD_NO_MAX; ++pos) {
            for (auto i = 0; i < PLAYER_NAME_MAX; ++i) {
                recordName[league][pos][i] = var1[offset++];
            }
        }
    }
}

void RecordManager::getLevelInfo(std::vector<int8_t> var1)
{
    int shift = 0;
    uint8_t league;
    uint8_t recordNo;

    for (league = 0; league < 4; ++league) {
        for (recordNo = 0; recordNo < 3; ++recordNo) {
            pushLongAs5Bytes(var1, shift, recordTimeMs[league][recordNo]);
            shift += 5;
        }
    }

    for (league = 0; league < LEAGUES_MAX; ++league) {
        for (recordNo = 0; recordNo < RECORD_NO_MAX; ++recordNo) {
            for (auto i = 0; i < PLAYER_NAME_MAX; ++i) {
                var1[shift++] = recordName[league][recordNo][i];
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

std::vector<std::string> RecordManager::getRecordDescription(const int league)
{
    std::vector<std::string> recordsDescription = std::vector<std::string>(3);

    for (uint8_t record_number = 0; record_number < 3; ++record_number) {
        if (recordTimeMs[league][record_number] != 0L) {
            std::stringstream ss;
            ss << recordName[league][record_number] << ' '
               << Time::timeToString(recordTimeMs[league][record_number]);
            recordsDescription[record_number] = ss.str();
            // int var4 = (int)recordTimeMs[var1][var3] / 100;
            // int var5 = (int)recordTimeMs[var1][var3] % 100;
            // var2[var3] = "" + std::string(recordName[var1][var3]) + " ";

            // if (var4 / 60 < 10) {
            //     var2[var3] = var2[var3] + " 0" + std::to_string(var4 / 60);
            // } else {
            //     var2[var3] = var2[var3] + " " + std::to_string(var4 / 60);
            // }

            // if (var4 % 60 < 10) {
            //     var2[var3] = var2[var3] + ":0" + std::to_string(var4 % 60);
            // } else {
            //     var2[var3] = var2[var3] + ":" + std::to_string(var4 % 60);
            // }

            // if (var5 < 10) {
            //     var2[var3] = var2[var3] + ".0" + std::to_string(var5);
            // } else {
            //     var2[var3] = var2[var3] + "." + std::to_string(var5);
            // }
        } else {
            recordsDescription[record_number].clear();
        }
    }

    return recordsDescription;
}

void RecordManager::writeRecordInfo()
{
    getLevelInfo(packedRecordInfo);

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

int RecordManager::getPosOfNewRecord(int league, int64_t timeMs)
{
    for (int i = 0; i < 3; ++i) {
        if (recordTimeMs[league][i] > timeMs || recordTimeMs[league][i] == 0L) {
            return i;
        }
    }

    return 3;
}

void RecordManager::method_17(int league, char* playerName, int64_t timeMs)
{
    std::cout << "method_17 " << league << " " << playerName << " " << timeMs << std::endl;
    const int newRecordPos = getPosOfNewRecord(league, timeMs);
    std::cout << "record pos " << newRecordPos << std::endl;

    if (newRecordPos < 3) {
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
    if (position <= 0) {
        return;
    }

    std::cout << "addNewRecord " << gameLevel << " " << position << std::endl;

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
                // RecordStore *var10000 = recordStore;
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
