#pragma once

#include <fstream>
#include <algorithm>
#include <filesystem>

class FileStream : public std::fstream {
public:
    FileStream()
        : std::fstream()
    {
    }

    FileStream(const std::filesystem::path& file, std::ios::openmode mode)
        : std::fstream(file, mode)
    {
    }

    ~FileStream()
    {
        std::fstream::close();
    }

    template <class T>
    void readVariable(T* p, bool swapEndianness = false, std::size_t size = 0)
    {
        char* pChar = reinterpret_cast<char*>(p);
        if (!size) {
            size = sizeof(T);
        }
        read_impl(pChar, size);
        if (swapEndianness) {
            std::reverse(pChar, pChar + size);
        }
    }

    template <class T>
    void writeVariable(T* p, std::size_t size = 0)
    {
        char* pChar = reinterpret_cast<char*>(p);
        if (!size) {
            size = sizeof(T);
        }
        write_impl(pChar, size);
    }

    virtual bool isOpen()
    {
        return std::fstream::is_open();
    }

    virtual void setPos(std::streampos pos)
    {
        std::fstream::seekg(pos);
    }

private:
    virtual void read_impl(char* s, std::streamsize n)
    {
        std::fstream::read(s, n);
    }

    virtual void write_impl(char* s, std::streamsize n)
    {
        std::fstream::write(s, n);
    }
};
