/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hashing_extractor.h"
#include "static_values.h"
#include <cstring>
#include <codecvt>
#include <sstream>
#include <iostream>
#include <algorithm>
#include "CRC.h"
#include "static_values.h"
#include <vector>

namespace usermodel {

HashVectorizer::~HashVectorizer() = default;
HashVectorizer::HashVectorizer(){
  for (int i = 1; i<=kMaxSubLen;i++)
    substring_sizes.push_back(i); 
  num_buckets = kNumBuckets;
}
HashVectorizer::HashVectorizer(int n_buckets, std::vector<int> subgrams){
  for (size_t i = 0; i<subgrams.size();i++)
    substring_sizes.push_back(subgrams[i]); 
  num_buckets = n_buckets;
}
HashVectorizer::HashVectorizer(const HashVectorizer& other){
  substring_sizes=other.substring_sizes;
  num_buckets=other.num_buckets;
}
int HashVectorizer::get_buckets(){
  return num_buckets;
}
int HashVectorizer::get_hash(std::string& substring){    
    auto* u8str = substring.c_str();
    auto rtn = CRC::Calculate(u8str, strlen(u8str), CRC::CRC_32()) % num_buckets; 
  return rtn;
}

std::map<unsigned, float> HashVectorizer::GetFrequencies(const std::string& html) {
  std::string data = html;
  std::map<unsigned, float> frequencies;
  if (data.length() > kMaximumHtmlLengthToClassify) {
    data = data.substr(0, kMaximumHtmlLengthToClassify - 1);
  }
  // get hashes of substrings for each of the substring lengths defined:
  for(auto const& substring_size : substring_sizes) {
    if (substring_size <= data.length()){
      for (size_t i = 0; i <= ( data.length()-substring_size) ; i++){
        auto ss = data.substr(i, substring_size);
        auto idx = get_hash(ss);
        ++frequencies[idx];
      }
    }
  }
  return frequencies;
}

}  // namespace usermodel
