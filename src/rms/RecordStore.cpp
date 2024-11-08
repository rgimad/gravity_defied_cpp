#include "RecordStore.h"

#include <cassert>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <numeric>
#include <cstring>

#ifdef WIN32
#include <libgen.h>
#else
#include <unistd.h>
#include <pwd.h>
#endif

#include "RecordStoreException.h"
#include "../config.h"
#include "../utils/FileStream.h"
#include "../utils/String.h"

RecordStore::RecordStore(std::filesystem::path filePath, RecordEnumerationImpl* records)
{
    this->filePath = filePath;
    this->records.reset(records);
}

RecordEnumeration* RecordStore::enumerateRecords(RecordFilter* filter, RecordComparator* comparator, bool keepUpdated)
{
    assert(filter == nullptr);
    assert(comparator == nullptr);
    assert(!keepUpdated);
    log("enumerateRecords()");
    return records.get();
}

void RecordStore::closeRecordStore()
{
    // nothing
}

int RecordStore::addOrUpdateRecord(int recordId, std::vector<int8_t> arr, int offset, int numBytes)
{
    if (recordId == -1) {
        return this->addRecord(arr, offset, numBytes);
    }

    this->setRecord(recordId, arr, offset, numBytes);
    return recordId;
}

int RecordStore::addRecord(std::vector<int8_t> arr, int offset, int numBytes)
{
    log("addRecord(" + std::to_string(arr.size()) + "," + std::to_string(offset) + "," + std::to_string(numBytes) + ")");
    assert(static_cast<int>(arr.size()) == numBytes);
    assert(offset == 0);
    int id = records->addRecord(arr);
    log("record id = " + std::to_string(id));
    save();
    return id;
}

void RecordStore::setRecord(int recordId, std::vector<int8_t> arr, int offset, int numBytes)
{
    (void)offset;
    (void)numBytes;
    records->setRecord(recordId, arr);
    save();
}

void RecordStore::save()
{
    FileStream outStream(filePath, std::ios::out | std::ios::binary);
    records->serialize(&outStream);
}

RecordEnumerationImpl* RecordStore::load(std::filesystem::path filePath)
{
    RecordEnumerationImpl* temp = new RecordEnumerationImpl();
    FileStream inStream(filePath, std::ios::in | std::ios::binary);
    temp->deserialize(&inStream);
    return temp;
}

RecordStore* RecordStore::openRecordStore(std::string name, bool createIfNecessary)
{
    if (name.empty()) {
        log("empty name");
        return nullptr;
    }

    name = GlobalSetting::SavesPrefix + "_" + name;
    log("openRecordStore(" + name + ", " + std::to_string(createIfNecessary) + ")");

    if (opened.find(name) == opened.end()) {
        opened[name] = createRecordStore(name, createIfNecessary);
    }

    return opened[name].get();
}

std::unique_ptr<RecordStore> RecordStore::createRecordStore(std::string name, bool createIfNecessary)
{
    log("createRecordStore(" + name + ", " + std::to_string(createIfNecessary) + ")");
    std::filesystem::path filePath = recordStoreDir / std::filesystem::path(name);

    if (std::filesystem::exists(filePath)) {
        return std::unique_ptr<RecordStore>(new RecordStore(filePath, load(filePath)));
    }

    if (createIfNecessary) {
        std::filesystem::create_directories(filePath.parent_path());

        std::unique_ptr<RecordStore> rs(new RecordStore(filePath, new RecordEnumerationImpl()));
        rs->save();
        return rs;
    } else {
        log("can't create record, file is missing");
        throw RecordStoreException();
    }
}

std::vector<std::string> RecordStore::listRecordStores()
{
    std::vector<std::string> result;

    for (const auto& entry : recordStoreDir)
        result.push_back(entry.filename().string());

    log("listRecordStores() = {" + String::join(result, ", ") + "}");

    return result;
}

void RecordStore::deleteRecordStore(std::string name)
{
    log("deleteRecordStore(" + name + ")");
    throw std::runtime_error("deleteRecordStore is not implemented");
}

void RecordStore::log(std::string s)
{
    std::cout << s << std::endl;
}

void RecordStore::setRecordStoreDir([[maybe_unused]] const char* progName)
{
#ifdef WIN32
    const char* base = dirname(strdup(progName));
    recordStoreDir = std::filesystem::path(base) / "recordStore";
#else
    // const char* homeDir = getenv("HOME");
    // if (!homeDir)
    //     homeDir = getpwuid(getuid())->pw_dir;

    // if (!homeDir)
    //     throw std::system_error(errno, std::system_category(), "Error getting home directory");

    recordStoreDir = std::filesystem::path("./saves");
#endif
}
