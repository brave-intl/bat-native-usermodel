/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */
 
#ifndef HASHING_EXTRACTOR_H
#define HASHING_EXTRACTOR_H

#include <string>
#include <map>
#include <vector>

namespace usermodel {

class HashVectorizer {
 public:
  HashVectorizer();
  HashVectorizer(const HashVectorizer& other);
  HashVectorizer(int n_buckets, std::vector<int> subgrams);
  ~HashVectorizer();


  std::map<unsigned,float> GetFrequencies( const std::string& html );
  int get_buckets();
 private:
  int get_hash(std::string& substring);
  std::vector<unsigned long> substring_sizes;//not ints for type comparison issues
  int num_buckets; // number of buckets to use for hashing
};

}  // namespace usermodel

#endif  
