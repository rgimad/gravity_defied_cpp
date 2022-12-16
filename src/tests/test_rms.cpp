#include "test_rms.h"

#include <memory>
#include <vector>
#include <string>
#include <numeric>

#include "../rms/RecordStore.h"
#include "../utils/String.h"

void test_rms() {
    std::vector<std::string> records = RecordStore::listRecordStores();

    RecordStore *rs = RecordStore::openRecordStore("GDTRStat", true);

    const bool create = false; // to test the rms you have to toggle this variable manually

    if (create) {
        std::vector<int8_t> v1 = {12, 54, 25, -23};
        std::vector<int8_t> v2 = {1, 17, 45, -23, -100};
        rs->addRecord(v1, 0, v1.size());
        rs->addRecord(v2, 0, v2.size());
    } else {
        RecordEnumeration* re = rs->enumerateRecords(nullptr, nullptr, false);
        re->reset();

        for (int i = 0; i < re->numRecords(); ++i) {
            int recordId = re->nextRecordId();
            std::vector<int8_t> record = re->nextRecord();
            std::cout << "Record " << std::to_string(recordId) << ": {" << String::join(record, ", ") << "}" <<  std::endl;
        }
    }
}