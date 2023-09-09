#include "EmbedFileStream.h"

#include <cstring>
#include <exception>

CMRC_DECLARE(assets);

EmbedFileStream::EmbedFileStream(const std::string& embedFileName)
    : FileStream()
{
    auto embedFs = cmrc::assets::get_filesystem();
    fileData = embedFs.open(embedFileName);
    buffPos = 0;
}

void EmbedFileStream::setPos(std::streampos pos)
{
    buffPos = pos;
}

void EmbedFileStream::read_impl(char* s, std::streamsize n)
{
    std::memcpy(s, fileData.begin() + buffPos, n);
    buffPos += n;
}

void EmbedFileStream::write_impl([[maybe_unused]] char* s, [[maybe_unused]] std::streamsize n)
{
    throw std::runtime_error("Write to buffer no supported!");
}

bool EmbedFileStream::isOpen()
{
    return true;
}
