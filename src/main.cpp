// SPDX-License-Identifier: GPL-3.0-or-later
#include "radiomodem/frame.hpp"
#include <iostream>
#include <string>
int main(){
 const std::string message="CQ TEST DE RADIOMODEM";radiomodem::Frame tx;tx.type=radiomodem::FrameType::Data;tx.sequence=1;tx.payload.assign(message.begin(),message.end());
 const auto wire=radiomodem::encode_frame(tx);const auto rx=radiomodem::decode_frame(wire);if(!rx){std::cerr<<"Decode failed: "<<radiomodem::to_string(rx.error)<<'\n';return 1;}
 std::cout<<"RadioModem protocol frame demo\nEncoded bytes: "<<wire.size()<<"\nSequence: "<<rx.frame.sequence<<"\nPayload: "<<std::string(rx.frame.payload.begin(),rx.frame.payload.end())<<"\nCRC32C: OK\n";
}
