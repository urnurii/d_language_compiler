#include "jvm/internal/utils.h"
#include <ostream>
#include <cstring>

namespace jvm::internal
{
    void Utils::writeBigEndian(std::ostream& os, uint8_t val)
    {
        os.put(static_cast<char>(val));
    }

    void Utils::writeBigEndian(std::ostream& os, uint16_t val)
    {
        os.put(static_cast<char>((val >> 8) & 0xFF));
        os.put(static_cast<char>(val & 0xFF));
    }

    void Utils::writeBigEndian(std::ostream& os, uint32_t val)
    {
        os.put(static_cast<char>((val >> 24) & 0xFF));
        os.put(static_cast<char>((val >> 16) & 0xFF));
        os.put(static_cast<char>((val >> 8) & 0xFF));
        os.put(static_cast<char>(val & 0xFF));
    }

    void Utils::writeBigEndian(std::ostream& os, uint64_t val)
    {
        for (int i = 7; i >= 0; --i)
            os.put(static_cast<char>((val >> (i * 8)) & 0xFF));
    }

    void Utils::writeBigEndian(std::ostream& os, int8_t val)
    {
        os.put(static_cast<char>(val));
    }

    void Utils::writeBigEndian(std::ostream& os, int16_t val)
    {
        writeBigEndian(os, static_cast<uint16_t>(val));
    }

    void Utils::writeBigEndian(std::ostream& os, int32_t val)
    {
        writeBigEndian(os, static_cast<uint32_t>(val));
    }

    void Utils::writeBigEndian(std::ostream& os, int64_t val)
    {
        writeBigEndian(os, static_cast<uint64_t>(val));
    }

    void Utils::writeBigEndian(std::ostream& os, float val)
    {
        uint32_t bits = 0;
        static_assert(sizeof(bits) == sizeof(val));
        std::memcpy(&bits, &val, sizeof(bits));
        writeBigEndian(os, bits);
    }

    void Utils::writeBigEndian(std::ostream& os, double val)
    {
        uint64_t bits = 0;
        static_assert(sizeof(bits) == sizeof(val));
        std::memcpy(&bits, &val, sizeof(bits));
        writeBigEndian(os, bits);
    }
} // jvm