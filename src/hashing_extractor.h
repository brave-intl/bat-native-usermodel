/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef HASHING_EXTRACTOR_H
#define HASHING_EXTRACTOR_H
// #ifndef DEPS_BAT_USERMODEL_SRC_BAG_OF_WORDS_EXTRACTOR_H_
// #define DEPS_BAT_USERMODEL_SRC_BAG_OF_WORDS_EXTRACTOR_H_

#include <string>
#include <map>
#include <vector>

namespace usermodel {

class HashVectorizer {
 public:
  HashVectorizer();
  HashVectorizer(const HashVectorizer& other);
  ~HashVectorizer();

  // bool Process(const std::wstring& html);
  std::map<unsigned,float> GetFrequencies( const std::wstring& html );
  int get_buckets();
 private:
  int get_hash(std::wstring& substring);
  std::vector<unsigned long> substring_sizes;//not ints for type comparison issues
  // std::map<unsigned, float> frequencies_;
  int num_buckets; // number of buckets to use for hashing
};

}  // namespace usermodel

#endif  // DEPS_BAT_USERMODEL_SRC_BAG_OF_WORDS_EXTRACTOR_H_
