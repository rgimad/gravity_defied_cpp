#include "RecordManager.h"

#include "rms/RecordStore.h"
#include "rms/RecordStoreException.h"
#include "rms/RecordStoreNotOpenException.h"
#include "rms/InvalidRecordIDException.h"

void RecordManager::method_8(int var1, int var2) {
    resetRecordsTime();

    try {
        str = "" + std::to_string(var1) + std::to_string(var2); // TODO check if it works
        recordStore = RecordStore::openRecordStore(str, true);
    } catch (RecordStoreException &var9) {
        return;
    }

    packedRecordInfoRecordId = -1;

    RecordEnumeration *recordEnum;
    try {
        recordEnum = recordStore->enumerateRecords(nullptr, nullptr, false);
    } catch (RecordStoreNotOpenException &var8) {
        return;
    }

    if (recordEnum->numRecords() > 0) {
        std::vector<int8_t> var4;
        try {
            var4 = recordEnum->nextRecord();
            recordEnum->reset();
            packedRecordInfoRecordId = recordEnum->nextRecordId();
        } catch (RecordStoreNotOpenException &var5) {
            return;
        } catch (InvalidRecordIDException &var6) {
            return;
        } catch (RecordStoreException &var7) {
            return;
        }

        loadRecordInfo(var4);
        recordEnum->destroy();
    }
}

int64_t RecordManager::load5BytesAsLong(std::vector<int8_t> var1, int offset) {
    int64_t result = 0L;
    int64_t mult = 1L;

    for (int var7 = offset; var7 < 5 + offset; ++var7) {
        int var8 = (var1[var7] + 256) % 256;
        result += mult * (int64_t) var8;
        mult *= 256L;
    }

    return result;
}

void RecordManager::pushLongAs5Bytes(std::vector<int8_t> var1, int var2, int64_t var3) {
    for (int var5 = var2; var5 < 5 + var2; ++var5) {
        var1[var5] = (int8_t) ((int) (var3 % 256L));
        var3 /= 256L;
    }
}

void RecordManager::loadRecordInfo(std::vector<int8_t> var1) {
    int offset = 0;

    int league;
    int pos;
    for (league = 0; league < 4; ++league) {
        for (pos = 0; pos < 3; ++pos) {
            recordTimeMs[league][pos] = load5BytesAsLong(var1, offset);
            offset += 5;
        }
    }

    for (league = 0; league < 4; ++league) {
        for (pos = 0; pos < 3; ++pos) {
            for (int i = 0; i < 3; ++i) {
                recordName[league][pos][i] = var1[offset++];
            }
        }
    }
}

void RecordManager::getLevelInfo(std::vector<int8_t> var1) {
    int shift = 0;

    int league;
    int recordNo;
    for (league = 0; league < 4; ++league) {
        for (recordNo = 0; recordNo < 3; ++recordNo) {
            pushLongAs5Bytes(var1, shift, recordTimeMs[league][recordNo]);
            shift += 5;
        }
    }

    for (league = 0; league < 4; ++league) {
        for (recordNo = 0; recordNo < 3; ++recordNo) {
            for (int var5 = 0; var5 < 3; ++var5) {
                var1[shift++] = recordName[league][recordNo][var5];
            }
        }
    }
}

void RecordManager::resetRecordsTime() {
    for (int league = 0; league < 4; ++league) {
        for (int pos = 0; pos < 3; ++pos) {
            recordTimeMs[league][pos] = 0L;
        }
    }
}

std::vector<std::string> RecordManager::getRecordDescription(int var1) {
    std::vector<std::string> var2 = std::vector<std::string>(3);

    for (int var3 = 0; var3 < 3; ++var3) {
        if (recordTimeMs[var1][var3] != 0L) {
            int var4 = (int) recordTimeMs[var1][var3] / 100;
            int var5 = (int) recordTimeMs[var1][var3] % 100;
            var2[var3] = "" + std::string(reinterpret_cast<char*>(recordName[var1][var3].data())) + " ";

            if (var4 / 60 < 10) {
                var2[var3] = var2[var3] + " 0" + std::to_string(var4 / 60);
            } else {
                var2[var3] = var2[var3] + " " + std::to_string(var4 / 60);
            }

            if (var4 % 60 < 10) {
                var2[var3] = var2[var3] + ":0" + std::to_string(var4 % 60);
            } else {
                var2[var3] = var2[var3] + ":" + std::to_string(var4 % 60);
            }

            if (var5 < 10) {
                var2[var3] = var2[var3] + ".0" + std::to_string(var5);
            } else {
                var2[var3] = var2[var3] + "." + std::to_string(var5);
            }
        } else {
            var2[var3] = nullptr;
        }
    }

    return var2;
}

void RecordManager::writeRecordInfo() {
    getLevelInfo(packedRecordInfo);
    if (packedRecordInfoRecordId == -1) {
        try {
            packedRecordInfoRecordId = recordStore->addRecord(packedRecordInfo, 0, 96);
        } catch (RecordStoreNotOpenException &var1) {
        } catch (RecordStoreException &var2) {
        }
    } else {
        try {
            recordStore->setRecord(packedRecordInfoRecordId, packedRecordInfo, 0, 96);
        } catch (RecordStoreNotOpenException &var3) {
        } catch (RecordStoreException &var4) {
        }
    }
}

int RecordManager::getPosOfNewRecord(int league, int64_t timeMs) {
    for (int i = 0; i < 3; ++i) {
        if (recordTimeMs[league][i] > timeMs || recordTimeMs[league][i] == 0L) {
            return i;
        }
    }

    return 3;
}

void RecordManager::method_17(int league, std::vector<int8_t> values, int64_t timeMs) {
    int newRecordPos;
    if ((newRecordPos = getPosOfNewRecord(league, timeMs)) != 3) {
        if (timeMs > 16777000L) {
            timeMs = 16777000L;  // 3 int8_ts, not five, wtf?
        }

        addNewRecord(league, newRecordPos);
        recordTimeMs[league][newRecordPos] = timeMs;

        for (int i = 0; i < 3; ++i) {
            recordName[league][newRecordPos][i] = values[i];
        }
    }
}

void RecordManager::addNewRecord(int gameLevel, int position) {
    for (int i = 2; i > position; --i) {
        recordTimeMs[gameLevel][i] = recordTimeMs[gameLevel][i - 1];
        for (int j = 0; j < 3; ++j) {
            recordName[gameLevel][i][j] = recordName[gameLevel][i - 1][j];
        }
    }
}

void RecordManager::deleteRecordStores() {
    std::vector<std::string> names = RecordStore::listRecordStores();

    for (std::size_t i = 0; i < names.size(); ++i) {
        if (names[i] != "GDTRStates") {
            try {
                // RecordStore *var10000 = recordStore;
                RecordStore::deleteRecordStore(names[i]);
            } catch (RecordStoreException &var3) {
            }
        }
    }
}

void RecordManager::closeRecordStore() {
    if (recordStore != nullptr) {
        try {
            recordStore->closeRecordStore();
            return;
        } catch (RecordStoreException &var1) {
        }
    }
}
