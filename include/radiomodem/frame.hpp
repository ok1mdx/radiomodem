// SPDX-License-Identifier: GPL-3.0-or-later
#pragma once
#include <cstdint>
#include <span>
#include <string>
#include <vector>
namespace radiomodem {
enum class FrameType : std::uint8_t { Data=0x01, Ack=0x02, Nak=0x03, Ping=0x04 };
struct Frame { FrameType type{FrameType::Data}; std::uint8_t flags{0}; std::uint32_t sequence{0}; std::vector<std::uint8_t> payload; };
enum class FrameError { None, TooShort, BadMagic, UnsupportedVersion, InvalidType, LengthMismatch, BadCrc, PayloadTooLarge };
struct DecodeResult { Frame frame; FrameError error{FrameError::None}; explicit operator bool() const noexcept { return error==FrameError::None; } };
inline constexpr std::size_t kHeaderSize=14, kTrailerSize=4, kMaxPayload=1024;
std::vector<std::uint8_t> encode_frame(const Frame& frame);
DecodeResult decode_frame(std::span<const std::uint8_t> bytes);
std::string to_string(FrameError error);
}
