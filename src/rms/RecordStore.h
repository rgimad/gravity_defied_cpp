#pragma once

#include <string>
#include <filesystem>
#include <memory>
#include <unordered_map>

#include "RecordEnumerationImpl.h"

class RecordFilter;
class RecordComparator;

class RecordStore {
private:
    RecordStore(std::filesystem::path filePath, RecordEnumerationImpl *records);
    void save();
    static RecordEnumerationImpl* load(std::filesystem::path filePath);
    static std::unique_ptr<RecordStore> createRecordStore(std::string name, bool createIfNecessary);
    static void log(std::string s);

    inline static const std::string recordStoreDir = "recordStore";
    inline static std::unordered_map<std::string, std::unique_ptr<RecordStore>> opened;
    std::filesystem::path filePath;
    std::unique_ptr<RecordEnumerationImpl> records;

public:
    static RecordStore* openRecordStore(std::string name, bool createIfNecessary);
    void closeRecordStore();
    static void deleteRecordStore(std::string name);
    static std::vector<std::string> listRecordStores();
    RecordEnumeration* enumerateRecords(RecordFilter *filter, RecordComparator *comparator, bool keepUpdated);
    int addRecord(std::vector<int8_t> arr, int offset, int numBytes);
    void setRecord(int recordId, std::vector<int8_t> arr, int offset, int numBytes);
};