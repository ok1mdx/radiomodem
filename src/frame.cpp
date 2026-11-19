// SPDX-License-Identifier: GPL-3.0-or-later
#include "radiomodem/frame.hpp"
#include "radiomodem/crc32c.hpp"
#include <array>
#include <stdexcept>
namespace radiomodem {
namespace {
constexpr std::array<std::uint8_t,4> magic{'R','D','M','1'};
constexpr std::uint8_t version=1;
void put16(std::vector<std::uint8_t>& o,std::uint16_t v){o.push_back(static_cast<std::uint8_t>(v>>8U));o.push_back(static_cast<std::uint8_t>(v));}
void put32(std::vector<std::uint8_t>& o,std::uint32_t v){o.push_back(static_cast<std::uint8_t>(v>>24U));o.push_back(static_cast<std::uint8_t>(v>>16U));o.push_back(static_cast<std::uint8_t>(v>>8U));o.push_back(static_cast<std::uint8_t>(v));}
std::uint16_t get16(std::span<const std::uint8_t>b,std::size_t p){return static_cast<std::uint16_t>((static_cast<std::uint16_t>(b[p])<<8U)|b[p+1]);}
std::uint32_t get32(std::span<const std::uint8_t>b,std::size_t p){return (static_cast<std::uint32_t>(b[p])<<24U)|(static_cast<std::uint32_t>(b[p+1])<<16U)|(static_cast<std::uint32_t>(b[p+2])<<8U)|static_cast<std::uint32_t>(b[p+3]);}
}
std::vector<std::uint8_t> encode_frame(const Frame& f){
 if(f.payload.size()>kMaxPayload) throw std::length_error("payload exceeds protocol maximum");
 std::vector<std::uint8_t> o;o.reserve(kHeaderSize+f.payload.size()+kTrailerSize);o.insert(o.end(),magic.begin(),magic.end());o.push_back(version);o.push_back(static_cast<std::uint8_t>(f.type));o.push_back(f.flags);o.push_back(0);put32(o,f.sequence);put16(o,static_cast<std::uint16_t>(f.payload.size()));o.insert(o.end(),f.payload.begin(),f.payload.end());put32(o,crc32c(o));return o;
}
DecodeResult decode_frame(std::span<const std::uint8_t>b){
 DecodeResult r;if(b.size()<kHeaderSize+kTrailerSize){r.error=FrameError::TooShort;return r;}
 for(std::size_t i=0;i<magic.size();++i)if(b[i]!=magic[i]){r.error=FrameError::BadMagic;return r;}
 if(b[4]!=version||b[7]!=0){r.error=FrameError::UnsupportedVersion;return r;}
 if(b[5]<1||b[5]>4){r.error=FrameError::InvalidType;return r;}
 const auto n=get16(b,12);if(n>kMaxPayload){r.error=FrameError::PayloadTooLarge;return r;}
 if(b.size()!=kHeaderSize+n+kTrailerSize){r.error=FrameError::LengthMismatch;return r;}
 if(crc32c(b.first(b.size()-kTrailerSize))!=get32(b,b.size()-kTrailerSize)){r.error=FrameError::BadCrc;return r;}
 r.frame.type=static_cast<FrameType>(b[5]);r.frame.flags=b[6];r.frame.sequence=get32(b,8);r.frame.payload.assign(b.begin()+static_cast<std::ptrdiff_t>(kHeaderSize),b.end()-static_cast<std::ptrdiff_t>(kTrailerSize));return r;
}
std::string to_string(FrameError e){switch(e){case FrameError::None:return "none";case FrameError::TooShort:return "frame too short";case FrameError::BadMagic:return "bad magic";case FrameError::UnsupportedVersion:return "unsupported version or nonzero reserved field";case FrameError::InvalidType:return "invalid frame type";case FrameError::LengthMismatch:return "length mismatch";case FrameError::BadCrc:return "CRC32C mismatch";case FrameError::PayloadTooLarge:return "payload too large";}return "unknown";}
}
