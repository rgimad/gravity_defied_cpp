#pragma once

#include "FileStream.h"

#include <cmrc/cmrc.hpp>

class EmbedFileStream: public FileStream
{
public:
    EmbedFileStream(std::string embedFilePath);
    virtual void setPos(std::streampos pos) override;
    virtual bool isOpen() override;

private:
    virtual void read_impl(char* s, std::streamsize n) override;
    virtual void write_impl(char* s, std::streamsize n) override;

    std::streampos buffPos = 0;
    cmrc::file fileData;
};
