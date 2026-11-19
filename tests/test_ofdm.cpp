// SPDX-License-Identifier: GPL-3.0-or-later
#include "radiomodem/ofdm.hpp"
#include <cassert>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <vector>
int main(){using namespace radiomodem;const std::vector<std::uint8_t> bits{1,0,1,1,0,0,1};OfdmTransmitter tx500;const auto a=tx500.modulate(bits);assert(a.size()==kOfdm500Hz.fft_size+kOfdm500Hz.cyclic_prefix);for(float x:a){assert(std::isfinite(x));assert(std::abs(x)<=1.0F);}OfdmTransmitter tx2300(OfdmConfig{kOfdm2300Hz,0.2});const auto b=tx2300.modulate(bits);assert(b.size()==kOfdm2300Hz.fft_size+kOfdm2300Hz.cyclic_prefix);assert(tx500.modulate({}).empty());std::cout<<"OFDM transmitter tests passed\n";}
