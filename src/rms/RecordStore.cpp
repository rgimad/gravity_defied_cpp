#include "RecordStore.h"

#include <cassert>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <numeric>

#include "RecordStoreException.h"
#include "../utils/String.h"

RecordStore::RecordStore(std::filesystem::path filePath, RecordEnumerationImpl *records) {
    // assert(std::filesystem::exists(filePath));
    this->filePath = filePath;
    this->records.reset(records);
}

RecordEnumeration* RecordStore::enumerateRecords(RecordFilter *filter, RecordComparator *comparator, bool keepUpdated) {
    assert(filter == nullptr);
    assert(comparator == nullptr);
    assert(!keepUpdated);
    log("enumerateRecords()");
    return records.get();
}

void RecordStore::closeRecordStore() {
    // nothing
}

int RecordStore::addRecord(std::vector<int8_t> arr, int offset, int numBytes) {
    log("addRecord()");
    assert(static_cast<int>(arr.size()) == numBytes);
    assert(offset == 0);
    int id = records->addRecord(arr);
    save();
    return id;
}

void RecordStore::setRecord(int recordId, std::vector<int8_t> arr, int offset, int numBytes) {
    (void)offset; (void)numBytes;
    records->setRecord(recordId, arr);
    save();
}

void RecordStore::save() {
    std::ofstream ofstream(filePath, std::ios::binary);
    records->serialize(ofstream);
    ofstream.close();
}

RecordEnumerationImpl* RecordStore::load(std::filesystem::path filePath) {
    RecordEnumerationImpl *temp = new RecordEnumerationImpl();
    std::ifstream ifstream(filePath, std::ios::binary);
    temp->deserialize(ifstream);
    ifstream.close();

    return temp;
}

RecordStore* RecordStore::openRecordStore(std::string name, bool createIfNecessary) {
    if (opened.find(name) == opened.end()) {
        opened[name] = createRecordStore(name, createIfNecessary);
    }

    return opened[name].get();
}

std::unique_ptr<RecordStore> RecordStore::createRecordStore(std::string name, bool createIfNecessary) {
    log("createRecordStore(" + name + ", " + std::to_string(createIfNecessary) + ")");
    std::filesystem::path filePath = std::filesystem::path(recordStoreDir) / std::filesystem::path(name);

    if (std::filesystem::exists(filePath)) {
        return std::unique_ptr<RecordStore>(new RecordStore(filePath, load(filePath)));
    }

    if (createIfNecessary) {
        std::filesystem::create_directories(filePath.parent_path());

        std::unique_ptr<RecordStore> rs(new RecordStore(filePath, new RecordEnumerationImpl()));
        rs->save();
        return rs;
    } else {
        throw RecordStoreException();
    }
}

std::vector<std::string> RecordStore::listRecordStores() {
    std::vector<std::string> result;

    for (const auto & entry : std::filesystem::directory_iterator(recordStoreDir))
        result.push_back(entry.path().filename().string());

    log("listRecordStores() = {" + String::join(result, ", ") + "}");

    return result;
}

void RecordStore::deleteRecordStore(std::string name) {
    log("deleteRecordStore(" + name + ")");
    throw std::runtime_error("deleteRecordStore is not implemented");
}

void RecordStore::log(std::string s) {
    std::cout << s << std::endl;
}