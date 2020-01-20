/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements. See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership. The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License. You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied. See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

#ifndef CUEHTTP_ENDIAN_HPP_
#define CUEHTTP_ENDIAN_HPP_

#include <type_traits>
#if defined(_MSC_VER)
#include <stdlib.h>
#define cue_bswap_16 _byteswap_ushort
#define cue_bswap_32 _byteswap_ulong
#define cue_bswap_64 _byteswap_uint64
#define bswap_be(bits) cue_bswap_##bits
#define bswap_le(bits)
#elif defined(__APPLE__)
#include <libkern/OSByteOrder.h>
#define cue_bswap_16 OSSwapInt16
#define cue_bswap_32 OSSwapInt32
#define cue_bswap_64 OSSwapInt64
#else
#define cue_bswap_16 __builtin_bswap16
#define cue_bswap_32 __builtin_bswap32
#define cue_bswap_64 __builtin_bswap64
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#define bswap_be(bits) cue_bswap_##bits
#define bswap_le(bits)
#else
#define bswap_be(bits)
#define bswap_le(bits) cue_bswap_##bits
#endif
#endif

namespace cue {
namespace http {
namespace detail {

template <std::size_t Bits>
struct swapper;

template <>
struct swapper<1> {
    inline static std::uint8_t swap_be(std::uint8_t value) {
        return value;
    }

    inline static std::uint8_t swap_le(std::uint8_t value) {
        return value;
    }
};

template <>
struct swapper<2> {
    inline static std::uint16_t swap_be(std::uint16_t value) {
        return bswap_be(16)(value);
    }

    inline static std::uint16_t swap_le(std::uint16_t value) {
        return bswap_le(16)(value);
    }
};

template <>
struct swapper<4> {
    inline static std::uint32_t swap_be(std::uint32_t value) {
        return bswap_be(32)(value);
    }

    inline static std::uint32_t swap_le(std::uint32_t value) {
        return bswap_le(32)(value);
    }
};

template <>
struct swapper<8> {
    inline static std::uint64_t swap_be(std::uint64_t value) {
        return bswap_be(64)(value);
    }

    inline static std::uint64_t swap_le(std::uint64_t value) {
        return bswap_le(64)(value);
    }
};

template <typename T>
inline T from_be(T t) {
    const auto value = static_cast<std::make_unsigned_t<T>>(t);
    return static_cast<T>(swapper<sizeof(value)>::swap_be(t));
}

template <typename T>
inline T to_be(T t) {
    const auto value = static_cast<std::make_unsigned_t<T>>(t);
    return static_cast<T>(swapper<sizeof(value)>::swap_be(t));
}

template <typename T>
inline T from_le(T t) {
    const auto value = static_cast<std::make_unsigned_t<T>>(t);
    return static_cast<T>(swapper<sizeof(value)>::swap_le(t));
}

template <typename T>
inline T to_le(T t) {
    const auto value = static_cast<std::make_unsigned_t<T>>(t);
    return static_cast<T>(swapper<sizeof(value)>::swap_le(t));
}

} // namespace detail
} // namespace http
} // namespace cue

#endif // CUEHTTP_ENDIAN_HPP_
