// SPDX-License-Identifier: GPL-3.0-or-later
#pragma once
#include <cstddef>
#include <cstdint>
#include <span>
#include <vector>
namespace radiomodem {
struct OfdmProfile { const char* name; std::uint32_t sample_rate_hz; std::size_t fft_size; std::size_t cyclic_prefix; std::size_t data_carriers; double center_frequency_hz; double occupied_bandwidth_hz; };
inline constexpr OfdmProfile kOfdm500Hz{"HF-500",12000,1024,128,40,1500.0,468.75};
inline constexpr OfdmProfile kOfdm2300Hz{"HF-2300",12000,256,32,48,1450.0,2250.0};
struct OfdmConfig { OfdmProfile profile{kOfdm500Hz}; double output_gain{0.25}; };
class OfdmTransmitter {
public:
 explicit OfdmTransmitter(OfdmConfig config={});
 [[nodiscard]] std::vector<float> modulate(std::span<const std::uint8_t> bits) const;
 [[nodiscard]] const OfdmConfig& config() const noexcept { return config_; }
private: OfdmConfig config_;
};
}
