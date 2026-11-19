// SPDX-License-Identifier: GPL-3.0-or-later
#include "radiomodem/crc32c.hpp"
namespace radiomodem {
std::uint32_t crc32c(std::span<const std::uint8_t> bytes) noexcept {
 std::uint32_t crc=0xFFFFFFFFU;
 for(const auto byte:bytes) { crc^=byte; for(int bit=0;bit<8;++bit) { const std::uint32_t mask=0U-(crc&1U); crc=(crc>>1U)^(0x82F63B78U&mask); } }
 return ~crc;
}
}
