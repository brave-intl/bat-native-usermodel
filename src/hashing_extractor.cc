/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hashing_extractor.h"

#include <cstring>
#include <sstream>
#include <iostream>
#include <algorithm>
#include "CRC.h"
#include "static_values.h"

namespace usermodel {

HashVectorizer::~HashVectorizer() = default;

HashVectorizer::HashVectorizer(){
  for (int i = 2; i<=5;i++)
    substring_sizes.push_back(i); 
  frequencies_ = {};
  to_lower_ = true;
  num_buckets = 10000;
}
std::string HashVectorizer::get_hash(std::string& substring){
  auto cstr = substring.c_str();
  auto rtn = CRC::Calculate(cstr, sizeof(cstr), CRC::CRC_32()) % num_buckets; 
  return std::to_string(rtn);
}

std::map<std::string, double> HashVectorizer::GetFrequencies() {
  return frequencies_;
}

bool HashVectorizer::Process(const std::string& html) {
  std::string data = html;
// TODO: Review maximum length 
  if (data.length() > kMaximumHtmlLengthToClassify) {
    data = data.substr(0, kMaximumHtmlLengthToClassify - 1);
  }
  // get hashes of substrings for each of the substring lengths defined:
  for(auto const& substring_size : substring_sizes) {
    for (auto i = 0; i< ( data.length()-substring_size) ; i++){
      auto ss = data.substr(i, substring_size);
      if (to_lower_){
        std::transform(ss.begin(), ss.end(), ss.begin(), ::tolower);
      }
      auto idx = get_hash(ss);
      ++frequencies_[idx];
    }
  }
  return true;
}

}  // namespace usermodel
