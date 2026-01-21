#ifndef JVM__UTILS_H
#define JVM__UTILS_H

#include <cstdint>
#include <iosfwd>
#include <ostream>

namespace jvm::internal
{
    class Utils
    {
    public:
        static void writeBigEndian(std::ostream& os, uint8_t val);
        static void writeBigEndian(std::ostream& os, uint16_t val);
        static void writeBigEndian(std::ostream& os, uint32_t val);
        static void writeBigEndian(std::ostream& os, uint64_t val);
        static void writeBigEndian(std::ostream& os, int8_t val);
        static void writeBigEndian(std::ostream& os, int16_t val);
        static void writeBigEndian(std::ostream& os, int32_t val);
        static void writeBigEndian(std::ostream& os, int64_t val);
        static void writeBigEndian(std::ostream& os, float val);
        static void writeBigEndian(std::ostream& os, double val);
        static constexpr bool hasFlag(uint16_t allFlags, uint16_t flag)
        {
            return (allFlags & flag) != 0;
        }
    };
} // jvm


#endif //JVM__UTILS_H