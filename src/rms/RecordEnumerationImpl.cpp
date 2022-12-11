#include "RecordEnumerationImpl.h"

#include "../Helpers.h"
#include "RecordStoreException.h"

RecordEnumerationImpl::RecordEnumerationImpl(std::vector<std::vector<int8_t>> data) {
    this->data = data;
}

RecordEnumerationImpl::RecordEnumerationImpl() {
}

RecordEnumerationImpl::~RecordEnumerationImpl() {
    data.clear();
}

int RecordEnumerationImpl::numRecords() {
    return data.size();
}

std::vector<int8_t> RecordEnumerationImpl::nextRecord() {
    return data[currentPos++];
}

int RecordEnumerationImpl::addRecord(std::vector<int8_t> bytes) {
    data.push_back(bytes);
    return data.size() - 1;
}

void RecordEnumerationImpl::setRecord(int index, std::vector<int8_t> bytes) {
    if (static_cast<int>(data.size()) <= index) {
        throw RecordStoreException();
    }
    data[index] = bytes;
}

void RecordEnumerationImpl::reset() {
    currentPos = 0;
}

int RecordEnumerationImpl::nextRecordId() {
    if (currentPos >= static_cast<int>(data.size())) {
        throw RecordStoreException();
    }
    return currentPos;
}

void RecordEnumerationImpl::destroy() {
    data.clear();
}

void RecordEnumerationImpl::serialize(std::ostream& os) {
    size_t temp = data.size();
    Helpers::writeVariableToStream(&currentPos, os);
    Helpers::writeVariableToStream(&(temp = data.size()), os);

    for (auto i = data.cbegin(); i != data.cend(); i++) {
        Helpers::writeVariableToStream(&(temp = i->size()), os);
        for (auto j = i->cbegin(); j != i->cend(); j++) {
            int8_t buffer;
            Helpers::writeVariableToStream(&(buffer = *j), os);
        }
    }
}

void RecordEnumerationImpl::deserialize(std::istream& is) {
    size_t temp;
    Helpers::readVariableFromStream(&currentPos, is);
    Helpers::readVariableFromStream(&temp, is);
    data.resize(temp);

    for (size_t i = 0; i < data.size(); ++i) {
        Helpers::readVariableFromStream(&temp, is);
        data[i].resize(temp);
        for (size_t j = 0; j < data[i].size(); ++j) {
            Helpers::readVariableFromStream(&data[i][j], is);
        }
    }
}