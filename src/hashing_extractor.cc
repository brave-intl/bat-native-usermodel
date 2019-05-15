/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hashing_extractor.h"
#include <cstring>
#include <codecvt>
#include <sstream>
#include <iostream>
#include <algorithm>
#include "CRC.h"
#include "static_values.h"

namespace usermodel {

HashVectorizer::~HashVectorizer() = default;

HashVectorizer::HashVectorizer(){
  for (int i = 1; i<=6;i++)
    substring_sizes.push_back(i); 
  // frequencies_ = {};
  num_buckets = 10000;
}
int HashVectorizer::get_hash(std::wstring& substring){
  // ----> following lines worked for english but c++ and utf-8 are strange bedfellows
  // auto cstr = substring.data();
  //auto rtn = CRC::Calculate(cstr, strlen(cstr), CRC::CRC_32()) % num_buckets; 
    
    
    std::wstring_convert<std::codecvt_utf8<wchar_t>> conv1;
    std::string u8str = conv1.to_bytes(substring);
    auto rtn = CRC::Calculate(u8str.data(), u8str.size(), CRC::CRC_32()) % num_buckets; 
  // std::cout << "fragment: "<<cstr<< " hash: "<<rtn<<"\n";
  return rtn;
}

std::map<unsigned, float> HashVectorizer::GetFrequencies(const std::wstring& html) {
  std::wstring data = html;
  std::map<unsigned, float> frequencies;
  frequencies = {};
  // TODO: Review maximum length 
  if (data.length() > kMaximumHtmlLengthToClassify) {
    data = data.substr(0, kMaximumHtmlLengthToClassify - 1);
  }
  // get hashes of substrings for each of the substring lengths defined:
  for(auto const& substring_size : substring_sizes) {
    if (substring_size <= data.length()){
      for (unsigned long i = 0; i <= ( data.length()-substring_size) ; i++){
        auto ss = data.substr(i, substring_size);
        auto idx = get_hash(ss);
        ++frequencies[idx];
      }
    }
  }
  return frequencies;
}

}  // namespace usermodel
