// SPDX-License-Identifier: GPL-3.0-or-later
#include "radiomodem/frame.hpp"
#include "radiomodem/crc32c.hpp"
#include <cassert>
#include <cstdint>
#include <iostream>
#include <string>
int main(){
 const std::string s="123456789";const auto crc=radiomodem::crc32c(std::span<const std::uint8_t>(reinterpret_cast<const std::uint8_t*>(s.data()),s.size()));assert(crc==0xE3069283U);
 radiomodem::Frame f;f.type=radiomodem::FrameType::Data;f.flags=2;f.sequence=0x12345678U;f.payload={0,0x41,0x42,0xFF};auto wire=radiomodem::encode_frame(f);auto d=radiomodem::decode_frame(wire);assert(d);assert(d.frame.sequence==f.sequence);assert(d.frame.type==f.type);assert(d.frame.flags==f.flags);assert(d.frame.payload==f.payload);
 wire[15]^=1U;auto damaged=radiomodem::decode_frame(wire);assert(!damaged);assert(damaged.error==radiomodem::FrameError::BadCrc);std::cout<<"All RadioModem tests passed.\n";
}
