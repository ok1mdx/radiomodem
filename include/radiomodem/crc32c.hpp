// SPDX-License-Identifier: GPL-3.0-or-later
#pragma once
#include <cstddef>
#include <cstdint>
#include <span>
namespace radiomodem {
std::uint32_t crc32c(std::span<const std::uint8_t> bytes) noexcept;
}
