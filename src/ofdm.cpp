// SPDX-License-Identifier: GPL-3.0-or-later
#include "radiomodem/ofdm.hpp"
#include <cmath>
#include <complex>
#include <numbers>
#include <stdexcept>
namespace radiomodem {
namespace {
using C=std::complex<double>;
void fft(std::vector<C>& a,bool inverse){
 const auto n=a.size();if(n==0||(n&(n-1U))!=0U)throw std::invalid_argument("FFT size must be a non-zero power of two");
 for(std::size_t i=1,j=0;i<n;++i){std::size_t bit=n>>1U;for(;j&bit;bit>>=1U)j^=bit;j^=bit;if(i<j)std::swap(a[i],a[j]);}
 for(std::size_t len=2;len<=n;len<<=1U){const double angle=(inverse?2.0:-2.0)*std::numbers::pi/static_cast<double>(len);const C wl(std::cos(angle),std::sin(angle));for(std::size_t i=0;i<n;i+=len){C w(1,0);for(std::size_t j=0;j<len/2U;++j){const C u=a[i+j],v=a[i+j+len/2U]*w;a[i+j]=u+v;a[i+j+len/2U]=u-v;w*=wl;}}}
 if(inverse)for(auto& v:a)v/=static_cast<double>(n);
}
void validate(const OfdmConfig& c){const auto&p=c.profile;if(p.sample_rate_hz==0||p.fft_size==0||(p.fft_size&(p.fft_size-1U))!=0U||p.cyclic_prefix>=p.fft_size||p.data_carriers==0||p.data_carriers>=p.fft_size||!std::isfinite(p.center_frequency_hz)||p.center_frequency_hz<=0||p.center_frequency_hz>=static_cast<double>(p.sample_rate_hz)/2||!std::isfinite(c.output_gain)||c.output_gain<0||c.output_gain>1)throw std::invalid_argument("Invalid OFDM configuration");}
}
OfdmTransmitter::OfdmTransmitter(OfdmConfig c):config_(c){validate(config_);if((config_.profile.data_carriers&1U)!=0U)throw std::invalid_argument("This prototype requires an even carrier count");}
std::vector<float> OfdmTransmitter::modulate(std::span<const std::uint8_t> bits)const{
 const auto&p=config_.profile;const std::size_t per=p.data_carriers,symbols=(bits.size()+per-1U)/per;std::vector<float> out;out.reserve(symbols*(p.fft_size+p.cyclic_prefix));std::size_t bi=0,sample=0;const auto half=per/2U;
 for(std::size_t s=0;s<symbols;++s){std::vector<C> bins(p.fft_size,C(0,0));for(std::size_t c=0;c<half;++c){const double x=bi<bits.size()&&bits[bi++]?-1.0:1.0;const auto pos=c+1U,neg=p.fft_size-pos;const double y=bi<bits.size()&&bits[bi++]?-1.0:1.0;bins[pos]=C(x,0);bins[neg]=C(y,0);}fft(bins,true);
 for(std::size_t n=p.fft_size-p.cyclic_prefix;n<p.fft_size;++n,++sample){const double ph=2*std::numbers::pi*p.center_frequency_hz*static_cast<double>(sample)/p.sample_rate_hz;out.push_back(static_cast<float>(config_.output_gain*std::real(bins[n]*C(std::cos(ph),std::sin(ph)))));}
 for(std::size_t n=0;n<p.fft_size;++n,++sample){const double ph=2*std::numbers::pi*p.center_frequency_hz*static_cast<double>(sample)/p.sample_rate_hz;out.push_back(static_cast<float>(config_.output_gain*std::real(bins[n]*C(std::cos(ph),std::sin(ph)))));}
 }return out;
}
}
